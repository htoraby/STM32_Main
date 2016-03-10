#include "log_main.h"
#include "usb_host.h"
#include "user_main.h"
#include "update.h"

#include "minilzo.h"

#define LOG_DIR USB_DISK ":ksu_log"
#define FILE_MASK "М-е%d куст%d скв%d(%d).log"

#define IN_BUF_SIZE 4096
#define OUT_BUF_SIZE (IN_BUF_SIZE + IN_BUF_SIZE / 16 + 64 + 3 + 8)
#define PARAMETERS_SIZE 21504*4

#pragma pack(1)

typedef struct {
  unsigned int size;
  unsigned short codeProduction;
  unsigned char codeEquip;
  unsigned char subCodeEquip;
  unsigned short version;
  unsigned int date;
  unsigned int mainLogSize;
  unsigned int debugLogSize;
  unsigned int parametersSize;
} LOG_FILE_HEADER;

#pragma pack()

#define HEAP_ALLOC(var,size) \
  lzo_align_t __LZO_MMODEL var [((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t)]

#if USE_EXT_MEM
static uint8_t __LZO_MMODEL inBufData[IN_BUF_SIZE] __attribute__((section(".extmem")));
static uint8_t __LZO_MMODEL outBufData[OUT_BUF_SIZE] __attribute__((section(".extmem")));
static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS) __attribute__((section(".extmem")));
#else
static uint8_t inBufData[IN_BUF_SIZE];
static uint8_t outBufData[OUT_BUF_SIZE];
static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
#endif

LogEvent logEvent;
LogData logData;
LogRunning logRunning;
LogAlarm logAlarm;
LogTms logTms;

LogDebug logDebug;

static osThreadId logSaveThreadId;
static osSemaphoreId saveSemaphoreId_;
static osSemaphoreId deleteSemaphoreId_;
static EventType eventType;

static void logSaveTask(void *argument);

void logInit()
{
  framReadData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);
  framReadData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logEvent.init();
  logRunning.init();
  logAlarm.init();
  logData.init();
  logTms.init();

  logDebug.init();

  saveSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(saveSemaphoreId_, 0);
  deleteSemaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(deleteSemaphoreId_, 0);

  osThreadDef(LogSaveUsb, logSaveTask, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
  logSaveThreadId = osThreadCreate(osThread(LogSaveUsb), NULL);
}

void logStartSave(EventType type)
{
  parameters.startSave();

  eventType = type;
  parameters.set(CCS_PROGRESS_VALUE, 0);
  parameters.set(CCS_PROGRESS_MAX, 0);
  parameters.set(CCS_PROGRESS_MAX, (EndAddrDebugLog + EndAddrTmsLog + PARAMETERS_SIZE)/1024);
  osSemaphoreRelease(saveSemaphoreId_);
}

void logStartDelete(EventType type)
{
  eventType = type;
  parameters.set(CCS_PROGRESS_VALUE, 0);
  parameters.set(CCS_PROGRESS_MAX, 0);
  if (parameters.get(CCS_CMD_LOG_DELETE))
    parameters.set(CCS_PROGRESS_MAX, EndAddrTmsLog/1024);
  else
    parameters.set(CCS_PROGRESS_MAX, EndAddrDebugLog/1024);
  osSemaphoreRelease(deleteSemaphoreId_);
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

static void getFilePath(char *path)
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
  char buf[_MAX_LFN + 1];
  char *fileName = buf;

  int cdng = parameters.get(CCS_NUMBER_CDNG);
  int bush = parameters.get(CCS_NUMBER_BUSH);
  int well = parameters.get(CCS_NUMBER_WELL);
  int count = 0;
  sprintf(fileName, FILE_MASK, cdng, bush, well, count);
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
        sprintf(fileName, FILE_MASK, cdng, bush, well, count);
        convert_utf8_to_windows1251(fileName, fileName, _MAX_LFN + 1);
      }
    }
  }
  f_closedir(&dir);

  strcat(path, "\\");
  strcat(path, fileName);
}

static bool logSave()
{
  FRESULT result;
  FIL file;
  UINT bytesWritten;
  LOG_FILE_HEADER header;
  char buf[_MAX_LFN + 1];

  uint32_t timeReady = 0;
  while(usbState != USB_READY) {
    osDelay(10);
    timeReady += 10;
    if (timeReady > 5000) {
      ksu.setError(NoConnectionUsbErr);
      return false;
    }
  }

  result = f_mkdir(LOG_DIR);
  if ((result == FR_OK) || (result == FR_EXIST)) {
    char *logPath = buf;
    strcpy(logPath, LOG_DIR);
    getFilePath(logPath);

    // Коды изготовителя и оборудования
    header.codeProduction = CODE_PRODUCTION;
    header.codeEquip = CODE_EQUIP;
    header.subCodeEquip = SUBCODE_EQUIP;
    header.version = FIRMWARE_VERSION;

    time_t time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);
    if (dateTime.tm_year > 100)
      dateTime.tm_year = dateTime.tm_year - 100;
    else
      dateTime.tm_year = 0;
    header.date = (toBcd(dateTime.tm_year) << 24) |
        (toBcd(dateTime.tm_mon + 1) << 16) |
        (toBcd(dateTime.tm_mday) << 8) |
        (toBcd(dateTime.tm_hour) & 0xFF);

    header.mainLogSize = EndAddrTmsLog;
    header.debugLogSize = EndAddrDebugLog;
    header.parametersSize = PARAMETERS_SIZE;

    if (f_open(&file, logPath, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
      uint16_t calcCrc = 0xFFFF;
      uint32_t addr = 0;
      uint32_t inLen = IN_BUF_SIZE;
      lzo_uint outLen = 0;
      uint32_t size = 0;
      uint32_t count = 0;
      int resCompress = 0;

      if (lzo_init() != LZO_E_OK) {
        printf("lzo_init() failed!\n");
        ksu.setError(MiniLzoInitUsbErr);
        return false;
      }

      header.size = header.mainLogSize + header.debugLogSize +
          header.parametersSize + sizeof(header) + 2;

      result = f_write(&file, (uint8_t*)&header, sizeof(header), &bytesWritten);
      if ((result != FR_OK) || (sizeof(header) != bytesWritten)) {
        f_close(&file);
        ksu.setError(WriteFileUsbErr);
        return false;
      }
      calcCrc = crc16_ibm((uint8_t*)&header, bytesWritten, calcCrc);

      while (usbState == USB_READY) {
        StatusType status = logRead(addr, inBufData, inLen);
        if (status == StatusError)
          asm("nop");
        addr = addr + inLen;
        calcCrc = crc16_ibm(inBufData, inLen, calcCrc);

        resCompress = lzo1x_1_compress(inBufData, inLen, &outBufData[8], &outLen, wrkmem);
        if (resCompress != LZO_E_OK) {
          f_close(&file);
          ksu.setError(MiniLzoUsbErr);
          return false;
        }
        *(uint32_t*)(outBufData) = inLen;
        if (outLen < inLen) {
          *(uint32_t*)(outBufData+4) = outLen;
          size = outLen + 8;
          result = f_write(&file, outBufData, size, &bytesWritten);
        } else {
          *(uint32_t*)(outBufData+4) = inLen;
          result = f_write(&file, outBufData, 8, &bytesWritten);
          size = inLen;
          result = f_write(&file, inBufData, inLen, &bytesWritten);
        }
        if ((result != FR_OK) || (bytesWritten != size)) {
          f_close(&file);
          ksu.setError(WriteFileUsbErr);
          return false;
        }

        count++;
        if (count >= 60) {
          count = 0;
          parameters.set(CCS_PROGRESS_VALUE, (float)(addr)/1024);
        }
        if (addr >= EndAddrTmsLog)
          break;
      }

      addr = 0;
      inLen = IN_BUF_SIZE;
      count = 0;
      while (usbState == USB_READY) {
        StatusType status = logDebugRead(addr, inBufData, inLen);
        if (status == StatusError)
          asm("nop");
        addr = addr + inLen;
        calcCrc = crc16_ibm(inBufData, inLen, calcCrc);

        resCompress = lzo1x_1_compress(inBufData, inLen, &outBufData[8], &outLen, wrkmem);
        if (resCompress != LZO_E_OK) {
          f_close(&file);
          ksu.setError(MiniLzoUsbErr);
          return false;
        }
        *(uint32_t*)(outBufData) = inLen;
        if (outLen < inLen) {
          *(uint32_t*)(outBufData+4) = outLen;
          size = outLen + 8;
          result = f_write(&file, outBufData, size, &bytesWritten);
        } else {
          *(uint32_t*)(outBufData+4) = inLen;
          result = f_write(&file, outBufData, 8, &bytesWritten);
          size = inLen;
          result = f_write(&file, inBufData, inLen, &bytesWritten);
        }
        if ((result != FR_OK) || (bytesWritten != size)) {
          f_close(&file);
          ksu.setError(WriteFileUsbErr);
          return false;
        }

        count++;
        if (count >= 60) {
          count = 0;
          parameters.set(CCS_PROGRESS_VALUE, (float)(addr + EndAddrTmsLog)/1024);
        }
        if (addr >= EndAddrDebugLog)
          break;
      }

      addr = 0;
      inLen = IN_BUF_SIZE;
      count = 0;
      while (usbState == USB_READY) {
        StatusType status = framReadData(addr, inBufData, inLen);
        if (status == StatusError)
          asm("nop");
        addr = addr + inLen;
        calcCrc = crc16_ibm(inBufData, inLen, calcCrc);

        resCompress = lzo1x_1_compress(inBufData, inLen, &outBufData[8], &outLen, wrkmem);
        if (resCompress != LZO_E_OK) {
          f_close(&file);
          ksu.setError(MiniLzoUsbErr);
          return false;
        }
        *(uint32_t*)(outBufData) = inLen;
        if (outLen < inLen) {
          *(uint32_t*)(outBufData+4) = outLen;
          size = outLen + 8;
          result = f_write(&file, outBufData, size, &bytesWritten);
        } else {
          *(uint32_t*)(outBufData+4) = inLen;
          result = f_write(&file, outBufData, 8, &bytesWritten);
          size = inLen;
          result = f_write(&file, inBufData, inLen, &bytesWritten);
        }
        if ((result != FR_OK) || (bytesWritten != size)) {
          f_close(&file);
          ksu.setError(WriteFileUsbErr);
          return false;
        }

        count++;
        if (count >= 60) {
          count = 0;
          parameters.set(CCS_PROGRESS_VALUE, (float)(addr + EndAddrTmsLog + EndAddrDebugLog)/1024);
        }
        if (addr >= PARAMETERS_SIZE)
          break;
      }

      result = f_write(&file, (uint8_t*)&calcCrc, sizeof(calcCrc), &bytesWritten);
      if ((result != FR_OK) || (sizeof(calcCrc) != bytesWritten)) {
        f_close(&file);
        ksu.setError(WriteFileUsbErr);
        return false;
      }

      result = f_close(&file);
      if (result != FR_OK) {
        ksu.setError(CloseFileUsbErr);
        return false;
      }
    } else {
      ksu.setError(OpenFileUsbErr);
      return false;
    }
  } else {
    ksu.setError(MkDirUsbErr);
    return false;
  }

  return true;
}

void logDeleted()
{
  Log::id_ = 0;
  framWriteData(IdLogAddrFram, (uint8_t*)&Log::id_, 4);

  logEvent.deInit();
  logRunning.deInit();
  logAlarm.deInit();
  logData.deInit();
  logTms.deInit();

  uint32_t addr = 0;
  while (1) {
    StatusType status = flashExtEraseBlock(FlashSpi5, addr);
    if (status == StatusError)
      asm("nop");

    addr = addr + flashExts[FlashSpi5].blockSize;
    parameters.set(CCS_PROGRESS_VALUE, (float)addr/1024);
    if (addr >= EndAddrTmsLog)
      break;
  }

  logEvent.init();
  logRunning.init();
  logAlarm.init();
  logData.init();
  logTms.init();

  logEvent.add(DelLogCode, eventType, DelLogId);
  parameters.set(CCS_CMD_LOG_DELETE, 0);
}

void logDebugDeleted()
{
  Log::idDebug_ = 0;
  framWriteData(IdDebugLogAddrFram, (uint8_t*)&Log::idDebug_, 4);

  logDebug.deInit();

  uint32_t addr = 0;
  while (1) {
    StatusType status = flashExtEraseBlock(FlashSpi1, addr);
    if (status == StatusError)
      asm("nop");

    addr = addr + flashExts[FlashSpi1].blockSize;
    parameters.set(CCS_PROGRESS_VALUE, (float)addr/1024);
    if (addr >= EndAddrDebugLog)
      break;
  }

  logDebug.init();

  logEvent.add(DelLogCode, eventType, DelDebugLogId);
  parameters.set(CCS_CMD_SERVICE_LOG_DELETE, 0);
}

void logSaveTask(void *argument)
{
  (void)argument;

  while (1) {
    if (osSemaphoreWait(saveSemaphoreId_, 10) == osOK) {
      osDelay(100);

      if (logSave()) {
        logEvent.add(CopyLogCode, eventType, CopyLogId);
        parameters.set(CCS_CMD_LOG_COPY, 2);
      } else {
        parameters.set(CCS_CMD_LOG_COPY, 0);
      }
    }

    if (osSemaphoreWait(deleteSemaphoreId_, 10) == osOK) {
      if (parameters.get(CCS_CMD_LOG_DELETE))
        logDeleted();
      else
        logDebugDeleted();
    }
  }
}
