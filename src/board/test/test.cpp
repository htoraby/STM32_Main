#include "test.h"
#include "ff.h"
#include "ff_gen_drv.h"

static void testThread(void const * argument);

#define UPLOAD_FILENAME "0:test.tmp"

FATFS fatfs;
FIL file;
FIL fileR;
DIR dir;
FILINFO fno;
static uint8_t RAM_Buf[512] = { 0x00 };
static uint8_t startTestUsb = 1;

void testInit()
{
  /* Create Test thread */
  osThreadDef(Test_Thread, testThread, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Test_Thread), NULL);
}

static void testThread(void const * argument)
{
  (void)argument;

  BlinkLed blinkLed;
  blinkLed.prvSetupHardware();

  while(1) {
#if (TEST_USB_FAT == 1)
    if ((disk_status(0) == RES_OK) && (startTestUsb == 1)) {
      FRESULT result = f_mount(&fatfs, "0", 1);
      if (result == FR_OK) {
        UINT bytesWritten = 0;
        UINT BytesRead;

        RAM_Buf[0] = 0x55;
        RAM_Buf[511] = 0xAA;
        f_unlink(UPLOAD_FILENAME);
        if (f_open(&file, UPLOAD_FILENAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
          f_write(&file, RAM_Buf, 512, &bytesWritten);
          f_close(&file);
        }
        RAM_Buf[0] = 0x00;
        RAM_Buf[511] = 0x00;
        if (f_open(&fileR, UPLOAD_FILENAME, FA_READ) == FR_OK) {
          f_read(&fileR, RAM_Buf, 512, &BytesRead);
          f_close(&fileR);
        }
        startTestUsb = 0;
      }
    }
#endif

#if (TEST_LED == 1)
    blinkLed.toggle();
    osDelay(500);
#endif

  }
}
