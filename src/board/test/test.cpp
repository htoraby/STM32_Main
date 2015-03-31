#include "test.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "uart.h"
#include "gpio.h"
#include "rtc.h"
#include "fram.h"
#include "flash_ext.h"
#include "adc.h"
#include "temp_sensor.h"
#include "adc_ext.h"
#include "host.h"
#include "user_main.h"
#include "test_protection.h"

static void testThread(void *argument);

#if (TEST_UART == 1)
static void testUartThread(void *argument);
#endif

#if (TEST_HOST_UART == 1)
static void testHostUartRxThread(void *argument);
static void testHostUartTxThread(void *argument);
#endif

static void testRtc();
static void testAdc();
static void testFram();
static void testFlash1();
static void testFlash2();
static void testTempSensor();
static void testDI();
static void testDO();
static void testAdcExt();
static void testHostSpi();
static void testLog();

#if USE_EXT_MEM
uint8_t bufferTx[4096] __attribute__((section(".extmem")));
uint8_t bufferRx[4096] __attribute__((section(".extmem")));
#else
uint8_t bufferTx[4096];
uint8_t bufferRx[4096];
#endif

#define UPLOAD_FILENAME "0:test.tmp"

void testInit()
{
  /* Create Test thread */
  osThreadDef(Test, testThread, osPriorityNormal, 0, 10 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Test), NULL);

#if (TEST_UART == 1)
  osThreadDef(TestUart, testUartThread, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(TestUart), NULL);
#endif

#if (TEST_HOST_UART == 1)
  osThreadDef(TestHostUartRx, testHostUartRxThread, osPriorityNormal, 0, 2 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(TestHostUartRx), NULL);
  osThreadDef(TestHostUartTx, testHostUartTxThread, osPriorityNormal, 0, 3 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(TestHostUartTx), NULL);
#endif

#if (TEST_PROTECTIONS == 1)
  testProtectionInit();
#endif
}

#if (TEST_HOST_UART == 1)
static int testCmd(int argc, char *argv[])
{
  for(int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "stm32")) {
      if (++i >= argc) {
        return 1;
      }
      if (!strcmp(argv[i], "tasklist")) {
        osThreadList((uint8_t*)bufferTx);

        static uint8_t buffer[UART_BUF_SIZE*2];
        int size = xPortGetFreeHeapSize();
        sprintf((char*)buffer, "\n%d\n", size);
        uartWriteData(HOST_TEST_UART, buffer, strlen((char*)buffer));

        uartWriteData(HOST_TEST_UART, bufferTx, strlen((char*)bufferTx));

        return 0;
      }
    }
  }

  return 1;
}
#endif

static void testThread(void * argument)
{
  (void)argument;

#if (TEST_USB_FAT == 1)
  FATFS fatfs;
  FIL file;
  FIL fileR;
  uint8_t ramBuf[512] = { 0x00 };
  uint8_t startTestUsb = 1;
#endif

#if (TEST_LED == 1)
  BlinkLed blinkLed;
  blinkLed.prvSetupHardware();
#endif

#if (TEST_UART == 1)
  int sizePkt;
  static uint8_t buffer[UART_BUF_SIZE];
  bool turn = false;

  uartInit(uart4, 115200);
  osSemaphoreId semaphoreUart = uartGetSemaphoreId(uart4);
#endif

  testRtc();
  testAdc();
  testFram();
  testFlash1();
  testFlash2();
  testTempSensor();
  testDI();
  testDO();
  testAdcExt();
  testHostSpi();
  testLog();

//  int t = 0;

  while(1) {
//    toggleLed(FanLed);

    osDelay(500);
//    parameters.setValue(CCS_NOTE_1 + t, 10);
//    parameters.setValue(CCS_NOTE_1 + t + 1, 20);
//    t = t + 2;
//    if (t > 10) t = 0;


//    logRunning.start(AutoType);
//    logAlarm.start(AutoType, 0);


#if (TEST_USB_FAT == 1)
    if ((disk_status(0) == RES_OK) && (startTestUsb == 1)) {
      FRESULT result = f_mount(&fatfs, USB_DISK, 1);
      if (result == FR_OK) {
        UINT bytesWritten = 0;
        UINT BytesRead;

        ramBuf[0] = 0x55;
        ramBuf[511] = 0xAA;
        f_unlink(UPLOAD_FILENAME);
        if (f_open(&file, UPLOAD_FILENAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
          f_write(&file, ramBuf, 512, &bytesWritten);
          f_close(&file);
        }
        ramBuf[0] = 0x00;
        ramBuf[511] = 0x00;
        if (f_open(&fileR, UPLOAD_FILENAME, FA_READ) == FR_OK) {
          f_read(&fileR, ramBuf, 512, &BytesRead);
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

#if (TEST_UART == 1)
    buffer[0] = 0x55;
    buffer[1] = 0xAA;
    buffer[2] = 0x02;
    uartWriteData(uart4, buffer, UART_BUF_SIZE);

    osSemaphoreWait(semaphoreUart, osWaitForever);
    while (1) {
      if (osSemaphoreWait(semaphoreUart, 5) == osEventTimeout) {
        sizePkt = uartReadData(uart4, buffer);
        if ((buffer[0] != 0xFE) || (buffer[1] != 0x55) ||
            (buffer[2] != 0x01) || (sizePkt != UART_BUF_SIZE))
          asm("nop");

        if (turn)
          onLed(FanLed);
        else
          offLed(FanLed);
        turn = !turn;
        break;
      }
    }
#endif
  }
}

#if (TEST_UART == 1)
static void testUartThread(void * argument)
{
  (void)argument;
  int countByte = 0;
  int sizePkt;
  static uint8_t buffer[UART_BUF_SIZE];

  uartInit(uart2, 115200);
  osSemaphoreId semaphoreUart = uartGetSemaphoreId(uart2);

  while(1) {
    osSemaphoreWait(semaphoreUart, osWaitForever);
    while (1) {
      if (osSemaphoreWait(semaphoreUart, 5) == osEventTimeout) {
        buffer[0] = 0xFE;
        buffer[1] = 0x55;
        buffer[2] = 0x01;

        sizePkt = uartReadData(uart2, buffer);
        if ((buffer[0] != 0x55) || (buffer[1] != 0xAA) ||
            (buffer[2] != 0x02) || (sizePkt != UART_BUF_SIZE))
          asm("nop");

        buffer[0] = 0xFE;
        buffer[1] = 0x55;
        buffer[2] = 0x01;
        uartWriteData(uart2, buffer, UART_BUF_SIZE);

        countByte = 0;
        break;
      } else {
        countByte++;
      }
    }
  }
}
#endif

#if (TEST_HOST_UART == 1)
static void testHostUartRxThread(void * argument)
{
  (void)argument;
  int countByte = 0;
  int sizePkt;
  static uint8_t buffer[UART_BUF_SIZE];

  uartInit(HOST_UART, 115200);
  osSemaphoreId semaphoreUart = uartGetSemaphoreId(HOST_UART);

  while(1) {
    osSemaphoreWait(semaphoreUart, osWaitForever);
    while (1) {
      if (osSemaphoreWait(semaphoreUart, 5) == osEventTimeout) {
        sizePkt = uartReadData(HOST_UART, buffer);
        uartWriteData(HOST_TEST_UART, buffer, sizePkt);

        countByte = 0;
        break;
      } else {
        countByte++;
      }
    }
  }
}

static void testHostUartTxThread(void * argument)
{
  (void)argument;
  int countByte = 0;
  int sizePkt;
  static uint8_t buffer[UART_BUF_SIZE];

  uartInit(HOST_TEST_UART, 115200);
  osSemaphoreId semaphoreUart = uartGetSemaphoreId(HOST_TEST_UART);

  while(1) {
    osSemaphoreWait(semaphoreUart, osWaitForever);
    while (1) {
      if (osSemaphoreWait(semaphoreUart, 5) == osEventTimeout) {
        memset(buffer, 0, UART_BUF_SIZE);
        sizePkt = uartReadData(HOST_TEST_UART, buffer);

        memcpy(bufferRx, buffer, UART_BUF_SIZE);

        int argc = 0;
        char *argv[20];
        char *str;
        str = strtok((char*)bufferRx, " ");
        while (str != NULL) {
          argv[argc++] = str;
          str = strtok (NULL, " ");
        }
        if (testCmd(argc, argv))
          uartWriteData(HOST_UART, buffer, sizePkt);

        countByte = 0;
        break;
      } else {
        countByte++;
      }
    }
  }
}
#endif

static void testRtc()
{
#if (TEST_RTC == 1)
  tm dateTime;
  // Установка даты и времени
  dateTime.tm_mon = 11;
  dateTime.tm_mday = 6;
  dateTime.tm_year = 2014 - 1900;
  dateTime.tm_hour = 17;
  dateTime.tm_min = 21;
  dateTime.tm_sec = 10;
  time_t time = mktime(&dateTime);
  rtcSetTime(&time);

  osDelay(1005);

  time = rtcGetTime();
  dateTime = *localtime(&time);
  if ((dateTime.tm_mon != 11) || (dateTime.tm_mday != 6) ||
      (dateTime.tm_year != (2014 - 1900)) || (dateTime.tm_hour != 17) ||
      (dateTime.tm_min != 21) || (dateTime.tm_sec != 11)) {
    asm("nop");
  }
  asm("nop");
#endif
}

static void testAdc()
{
#if (TEST_ADC == 1)
  StatusType status;
  static float coreTemperature;
  static float coreVbattery;
  static float coreVref;

  status = getCoreVref(&coreVref);
  if (status == StatusOk)
    status = getCoreTemperature(&coreTemperature);
  if (status == StatusOk)
    status = getCoreVbattery(&coreVbattery);

  if (status != StatusOk)
    asm("nop");
  asm("nop");
#endif
}

static void testFram()
{
#if (TEST_FRAM == 1)
  uint32_t time = HAL_GetTick();
  bufferTx[0] = 0x21;
  bufferTx[1] = 0xAA;
  bufferTx[2] = 0x00;
  bufferTx[3] = 0x00;
  bufferTx[4] = 0x00;
  bufferTx[5] = 0x33;
  bufferTx[6] = 0x00;
  bufferTx[7] = 0x00;
  bufferTx[8] = 0x55;
  bufferTx[9] = 0x12;
  bufferTx[sizeof(bufferTx)-1] = 0x55;
  memset(bufferRx, 0, sizeof(bufferRx));
  framWriteData(0, bufferTx, sizeof(bufferTx));
  framReadData(0, bufferRx, sizeof(bufferTx));

  if (memcmp(bufferTx, bufferRx, sizeof(bufferTx))) {
    asm("nop"); // Ошибка
  }
  time = HAL_GetTick() - time;
  asm("nop");
#endif
}

static void testFlash1()
{
#if (TEST_FLASH1 == 1)
  flashEraseSector4k(FlashSpi1, 0);

  uint32_t time = HAL_GetTick();
  bufferTx[0] = 0x21;
  bufferTx[1] = 0xAA;
  bufferTx[2] = 0x01;
  bufferTx[3] = 0x12;
  bufferTx[4] = 0x03;
  bufferTx[5] = 0x33;
  bufferTx[6] = 0x01;
  bufferTx[7] = 0x20;
  bufferTx[8] = 0x55;
  bufferTx[9] = 0x12;
  bufferTx[sizeof(bufferTx)-1] = 0x55;
  memset(bufferRx, 0, sizeof(bufferRx));
  flashExtWrite(FlashSpi1, 0, &bufferTx[0], sizeof(bufferTx));
  flashExtRead(FlashSpi1, 0, &bufferRx[0], sizeof(bufferTx));

  if (memcmp(bufferTx, bufferRx, sizeof(bufferTx))) {
    asm("nop"); // Ошибка
  }
  time = HAL_GetTick() - time;
  asm("nop");
#endif
}

static void testFlash2()
{
#if (TEST_FLASH2 == 1)
  flashEraseSector4k(FlashSpi5, 0);

  uint32_t time = HAL_GetTick();
  bufferTx[0] = 0x21;
  bufferTx[1] = 0xAA;
  bufferTx[2] = 0x01;
  bufferTx[3] = 0x12;
  bufferTx[4] = 0x03;
  bufferTx[5] = 0x33;
  bufferTx[6] = 0x01;
  bufferTx[7] = 0x20;
  bufferTx[8] = 0x55;
  bufferTx[9] = 0x12;
  bufferTx[sizeof(bufferTx)-1] = 0x55;
  memset(bufferRx, 0, sizeof(bufferRx));
  flashExtWrite(FlashSpi5, 0, &bufferTx[0], sizeof(bufferTx));
  flashExtRead(FlashSpi5, 0, &bufferRx[0], sizeof(bufferTx));

  if (memcmp(bufferTx, bufferRx, sizeof(bufferTx))) {
    asm("nop"); // Ошибка
  }
  time = HAL_GetTick() - time;
  asm("nop");
#endif
}

static void testTempSensor()
{
#if TEST_T_SENSOR
  float temp = tempSensorReadData();
  if (temp == 999)
    asm("nop");
#endif
}

static void testDI()
{
#if TEST_DI
  PinState value;
  for (int i = 0; i < DigitalInputMax; ++i) {
    value = getDigitalInput(i);
    if (value)
      asm("nop");
  }
#endif
}

static void testDO()
{
#if TEST_DO
  for (int i = 0; i < DigitalOutputMax; ++i) {
    setDigitalOutput(i, PinSet);
    asm("nop");
  }
  for (int i = 0; i < DigitalOutputMax; ++i) {
    setDigitalOutput(i, PinReset);
    asm("nop");
  }
#endif
}

static void testAdcExt()
{
#if TEST_ADC_EXT
  osDelay(500);
  for (int i = 0; i < ANALOG_IN_NUM; ++i) {
    float value = getValueAnalogInExt(i);
    if (value)
      asm("nop");
  }
#endif
}

static void testHostSpi()
{
#if TEST_HOST_SPI
  osSemaphoreId semaphoreId = getHostSemaphore();

  bufferTx[0] = 0x55;
  bufferTx[1] = 0x7E;
  bufferTx[9] = 0xAA;
  bufferTx[HOST_BUF_SIZE-1] = 0x55;
  hostWriteData(bufferTx, HOST_BUF_SIZE);

  osSemaphoreWait(semaphoreId, osWaitForever);

  int sizePkt = hostReadData(bufferRx);

  if (memcmp(bufferTx, bufferRx, HOST_BUF_SIZE) ||
      (sizePkt != HOST_BUF_SIZE)) {
    asm("nop"); // Ошибка
  }
  asm("nop");
#endif
}

static void testLog()
{
#if TEST_LOG
  logErase();

  for (int i = 0; i < 4; ++i) {
    logEvent.add(0x01, 13, i, 0, 1);
  }
  for (int i = 0; i < 4; ++i) {
    logDebug.add(CriticalMsg, "Test Test Привет!");
  }

  logRead(StartAddrEventLog, bufferRx, 20*4);
  logDebugRead(StartAddrDebugLog, bufferRx, 256*4);
  asm("nop");
#endif
}
