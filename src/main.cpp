/**
  ******************************************************************************
  * @file   main.c
  * @author Shilyaev E.V.
  * @date   16.09.2014
  * @brief  Main program body
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "usb_host.h"
#include "test.h"
#include "gpio.h"
#include "rtc.h"
#include "sram.h"
#include "fram.h"
#include "flash_ext.h"
#include "adc.h"

/* Private function prototypes -----------------------------------------------*/
static void systemClockConfig();
static void mainThread(const void *argument);

uint8_t flagMcuInit = false;

int main()
{
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  systemClockConfig();

  /* Initialize all configured peripherals */
  gpioInit();
  sramInit();
  rtcInit();
  adcInit(adc1);
  framInit();
  flashExtInit(FlashSpi1);
  flashExtInit(FlashSpi5);

  flagMcuInit = true;

  /* Code generated for FreeRTOS */
  /* Create Start thread */
  osThreadDef(Main_Thread, mainThread, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Main_Thread), NULL);

  /* Start scheduler */
  osKernelStart(NULL, NULL);

  /* We should never get here as control is now taken by the scheduler */
  while (1) { }
}

/** System Clock Configuration
*/
static void systemClockConfig()
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
      |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

static void mainThread(void const * argument)
{
  (void)argument;

  /* FatFS: Link the USBH disk I/O driver */
  USBH_DriverNum = FATFS_LinkDriver(&USBH_Driver, USBH_Path);

  /* Init code for USB_HOST */
  MX_USB_HOST_Init();

#if (USE_TEST == 1)
  // Init tests
  testInit();
#endif

  while(1) {
    osDelay(100);
  }
}

#ifdef USE_FULL_ASSERT
/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("Wrong parameters value: file %s on line %d\r\n", file, (int)line);
}
#endif
