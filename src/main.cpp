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

#include "ff_gen_drv.h"
#include "board.h"
#include "rcause.h"
#include "user_main.h"
#include "test.h"

static void systemClockConfig();
static void mainThread(void *argument);

int main()
{
  // Сброс всей переферии, инициализация интерфейса Flash и системного таймера (1мс)
  HAL_Init();

  // Инициализация системы тактирования
  systemClockConfig();

  boardInit();

  // Создание основной задачи
  osThreadDef(MainThread, mainThread, osPriorityNormal, 0, 6*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(MainThread), NULL);

  // Запуск диспетчера задач
  osKernelStart();

  // Сюда не должны попасть, т.к. мы запустили диспетчер задач
  while (1) { }
}

/*!
 * \brief Инициализация системы тактирования
*/
static void systemClockConfig()
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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

  __DMA1_CLK_ENABLE();
  __DMA2_CLK_ENABLE();
}

/*!
 * \brief Основная задача
*/
static void mainThread(void *argument)
{
  (void)argument;

  // Подсчет счетчиков перезапуска СPU
  resetCauseCheck();

  usbInit();

#if (USE_TEST == 1)
  testInit();
#endif

  userInit();

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
