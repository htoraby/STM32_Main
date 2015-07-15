#include "log_main.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "usb_host.h"
#include "user_main.h"

#define LOG_DIR USB_DISK ":ksu_log"
#define FILE_MASK "М-е%d куст%d скв%d(%d).%s"

#if USE_EXT_MEM
static uint8_t bufData[4096] __attribute__((section(".extmem")));
#else
static uint8_t bufData[4096];
#endif
static osSemaphoreId semaphoreId_;

LogEvent logEvent;
LogData logData;
LogRunning logRunning;
LogAlarm logAlarm;
LogTms logTms;

LogDebug logDebug;

static void logSaveTask(void *argument);

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);
  framReadData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logEvent.init();
  logRunning.init();
  logAlarm.init();

#ifndef DEBUG
  logData.init();
  logTms.init();
#endif

  logDebug.init();

  // Запись в журнал "Включение питания"
  logEvent.add(PowerCode, AutoType, PowerOnId);

  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);
  osThreadDef(LogSaveUsb, logSaveTask, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(LogSaveUsb), NULL);
}

void logErase()
{
  Log::id_ = 0;
  framWriteData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);
  Log::idDebug_ = 0;
  framWriteData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logEvent.deInit();
  logData.deInit();
  logRunning.deInit();
  logTms.deInit();
  logDebug.deInit();

  flashExtChipErase(FlashSpi5);
  flashExtChipErase(FlashSpi1);

  logInit();
}

StatusType logRead(uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = flashExtRead(FlashSpi5, address, data, size);
  return status;
}

StatusType logDebugRead(uint32_t address, uint8_t *data, uint32_t size)
{
  StatusType status = flashExtRead(FlashSpi1, address, data, size);
  return status;
}

void logStartSave()
{
  parameters.set(CCS_PROGRESS_VALUE, 0);
  parameters.set(CCS_PROGRESS_MAX, 0);
  parameters.set(CCS_PROGRESS_MAX, (EndAddrDebugLog + EndAddrTmsLog)/1024);
  osSemaphoreRelease(semaphoreId_);
}

static void getFilePath(char *path, const char *suffix)
{
  FRESULT result;
  FILINFO fileInfo;
  DIR dir;
  char *fn;
#if _USE_LFN
  static char lfn[_MAX_LFN + 1];
  fileInfo.lfname = lfn;
  fileInfo.lfsize = sizeof(lfn);
#endif
  char *fileName = new char[_MAX_LFN + 1];

  int cdng = parameters.get(CCS_NUMBER_CDNG);
  int bush = parameters.get(CCS_NUMBER_BUSH);
  int well = parameters.get(CCS_NUMBER_WELL);
  int count = 0;
  sprintf(fileName, FILE_MASK, cdng, bush, well, count, suffix);
  convert_utf8_to_windows1251(fileName, fileName, _MAX_LFN + 1);

  result = f_opendir(&dir, path);
  if (result == FR_OK) {
    while (1) {
      result = f_readdir(&dir, &fileInfo);
      if (result != FR_OK || fileInfo.fname[0] == 0)
        break;
#if _USE_LFN
      fn = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
      fileName = fileInfo.fname;
#endif
      if (!strcmp(fn, fileName)) {
        count++;
        sprintf(fileName, FILE_MASK, cdng, bush, well, count, suffix);
        convert_utf8_to_windows1251(fileName, fileName, _MAX_LFN + 1);
      }
    }
  }

  strcat(path, "\\");
  strcat(path, fileName);

  delete[] fileName;
}

void logSaveTask(void *argument)
{
  (void)argument;
  FATFS fatfs;
  FRESULT result;
  FIL file;
  UINT bytesWritten;

  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    bool error = false;

    uint32_t time = 0;
    while(disk_status(0) != RES_OK) {
      osDelay(10);
      time += 10;
      if (time > 5000) {
        error = true;
        break;
      }
    }
    if (error) {
      parameters.set(CCS_CMD_LOG_COPY, 0);
      continue;
    }
    time = 0;
    while (f_mount(&fatfs, USB_DISK, 1) != FR_OK) {
      osDelay(10);
      time += 10;
      if (time > 5000) {
        error = true;
        break;
      }
    }
    if (error) {
      parameters.set(CCS_CMD_LOG_COPY, 0);
      continue;
    }

    result = f_mkdir(LOG_DIR);
    if ((result == FR_OK) || (result == FR_EXIST)) {
      time = HAL_GetTick();

      char *logPath = new char[_MAX_LFN + 1];

      strcpy(logPath, LOG_DIR);
      getFilePath(logPath, "dlog");

      if (f_open(&file, logPath, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
        uint32_t addr = 0;
        uint32_t size = 4096;
        uint32_t count = 0;

        while (1) {
          StatusType status = logDebugRead(addr, &bufData[0], size);
          if (status == StatusError)
            asm("nop");

          result = f_write(&file, &bufData[0], size, &bytesWritten);
          if ((result != FR_OK) || (size != bytesWritten))
            asm("nop");

          addr = addr + size;
          count++;
          if (count >= 20) {
            count = 0;
            parameters.set(CCS_PROGRESS_VALUE, (float)addr/1024);
          }
          if (addr >= EndAddrDebugLog)
            break;
        }

        result = f_close(&file);
        if (result != FR_OK)
          asm("nop");
      }

      strcpy(logPath, LOG_DIR);
      getFilePath(logPath, "log");

      int t = f_open(&file, logPath, FA_CREATE_ALWAYS | FA_WRITE);
      if (t == FR_OK) {
        uint32_t addr = 0;
        uint32_t size = 4096;
        bytesWritten = 0;
        uint32_t count = 0;

        while (1) {
          StatusType status = logRead(addr, &bufData[0], size);
          if (status == StatusError)
            asm("nop");

          result = f_write(&file, &bufData[0], size, &bytesWritten);
          if ((result != FR_OK) || (size != bytesWritten))
            asm("nop");

          addr = addr + size;
          count++;
          if (count >= 20) {
            count = 0;
            parameters.set(CCS_PROGRESS_VALUE, (float)(addr + EndAddrDebugLog)/1024);
          }
          if (addr >= EndAddrTmsLog)
            break;
        }

        result = f_close(&file);
        if (result != FR_OK)
          asm("nop");
      }

      delete[] logPath;

      time = HAL_GetTick() - time;
      asm("nop");
    }

    while (f_mount(&fatfs, USB_DISK, 0) != FR_OK) {
      osDelay(10);
      time += 10;
      if (time > 5000) {
        error = true;
        break;
      }
    }
    if (error) {
      parameters.set(CCS_CMD_LOG_COPY, 0);
      continue;
    }

    parameters.set(CCS_CMD_LOG_COPY, 0);
  }
}
