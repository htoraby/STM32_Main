#include "log_main.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "usb_host.h"

#define LOG_DIR USB_DISK ":ksu_log"
#define DEBUG_LOG_FILE LOG_DIR "\\debug.log"
#define MAIN_LOG_FILE LOG_DIR "\\main.log"

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


  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);
  osThreadDef(LogSaveUsbTask, logSaveTask, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(LogSaveUsbTask), NULL);
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
  return flashExtRead(FlashSpi5, address, data, size);
}

StatusType logDebugRead(uint32_t address, uint8_t *data, uint32_t size)
{
  return flashExtRead(FlashSpi1, address, data, size);
}

void logStartSave()
{
  osSemaphoreRelease(semaphoreId_);
}

void logSaveTask(void *argument)
{
  (void)argument;
  FATFS fatfs;
  FIL file;
  UINT bytesWritten;

  while (1) {
    osSemaphoreWait(semaphoreId_, osWaitForever);

    while(disk_status(0) != RES_OK)
      osDelay(10);
    while (f_mount(&fatfs, USB_DISK, 1) != FR_OK)
      osDelay(10);

    f_unlink(DEBUG_LOG_FILE);
    f_unlink(MAIN_LOG_FILE);

    FRESULT result = f_mkdir(LOG_DIR);
    if ((result == FR_OK) || (result == FR_EXIST)) {
      uint32_t time = HAL_GetTick();
      if (f_open(&file, DEBUG_LOG_FILE, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
        uint32_t addr = 0;
        uint32_t size = 4096;
        bytesWritten = 0;
        while (1) {
          logDebugRead(addr, bufData, size);
          f_write(&file, bufData, size, &bytesWritten);

          addr = addr + size;
          if (addr >= flashExts[FlashSpi1].size)
            break;
        }
        f_close(&file);
      }

      if (f_open(&file, MAIN_LOG_FILE, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
        uint32_t addr = 0;
        uint32_t size = 4096;
        bytesWritten = 0;
        while (1) {
          logRead(addr, bufData, size);
          f_write(&file, bufData, size, &bytesWritten);

          addr = addr + size;
          if (addr >= flashExts[FlashSpi5].size)
            break;
        }
        f_close(&file);
      }
      time = HAL_GetTick() - time;
      asm("nop");
    }
  }
}
