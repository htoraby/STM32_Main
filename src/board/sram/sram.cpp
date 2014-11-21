#include "sram.h"
#include "gpio.h"
#include "test_sram.h"

SRAM_HandleTypeDef hsram;

void sramInit()
{
  FMC_NORSRAM_TimingTypeDef Timing;
  FMC_NORSRAM_TimingTypeDef ExtTiming;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram.Instance = FMC_NORSRAM_DEVICE;
  hsram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  hsram.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
  hsram.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  /* Timing */
  Timing.AddressSetupTime = 3;
  Timing.AddressHoldTime = 0;
  Timing.DataSetupTime = 15;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 1;
  Timing.DataLatency = 0;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 3;
  ExtTiming.AddressHoldTime = 0;
  ExtTiming.DataSetupTime = 6;
  ExtTiming.BusTurnAroundDuration = 0;
  ExtTiming.CLKDivision = 1;
  ExtTiming.DataLatency = 0;
  ExtTiming.AccessMode = FMC_ACCESS_MODE_A;

  HAL_SRAM_Init(&hsram, &Timing, &ExtTiming);

  // Тест памяти
  int res = xmemTestAll();
  if (res) {
    while (1) {
      toggleLed(StopLed);
      HAL_Delay(300);
    }
  }
}

static int FMC_Initialized = 0;

static void HAL_FMC_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;
  /* Peripheral clock enable */
  __FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF12  ------> FMC_A6
  PF13  ------> FMC_A7
  PF14  ------> FMC_A8
  PF15  ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10  ------> FMC_D7
  PE11  ------> FMC_D8
  PE12  ------> FMC_D9
  PE13  ------> FMC_D10
  PE14  ------> FMC_D11
  PE15  ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10  ------> FMC_D15
  PD11  ------> FMC_A16
  PD12  ------> FMC_A17
  PD13  ------> FMC_A18
  PD14  ------> FMC_D0
  PD15  ------> FMC_D1
  PG2   ------> FMC_A12
  PG3   ------> FMC_A13
  PG4   ------> FMC_A14
  PG5   ------> FMC_A15
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
      |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13
      |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
      |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
      |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
      |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
      |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
      |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
      |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
  (void)hsram;
  HAL_FMC_MspInit();
}

static int FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void)
{
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;

  __FMC_CLK_DISABLE();

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13
                  |GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                  |GPIO_PIN_4|GPIO_PIN_5);

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                  |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                  |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                  |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                  |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                  |GPIO_PIN_7);
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram)
{
  HAL_FMC_MspDeInit();
}

StatusType sramWriteData(uint32_t address, uint32_t *data,
                         uint32_t size, NumberBits bits)
{
  switch (bits) {
    case bit8:
      if(HAL_SRAM_Write_8b(&hsram, (uint32_t *)address, (uint8_t *)data, size) == HAL_OK)
        return StatusOk;
      break;
    case bit16:
      if(HAL_SRAM_Write_16b(&hsram, (uint32_t *)address, (uint16_t *)data, size) == HAL_OK)
        return StatusOk;
      break;
    case bit32:
      if(HAL_SRAM_Write_32b(&hsram, (uint32_t *)address, data, size) == HAL_OK)
        return StatusOk;
      break;
  }
  return StatusError;
}

StatusType sramReadData(uint32_t address, uint32_t *data,
                        uint32_t size, NumberBits bits)
{
  switch (bits) {
    case bit8:
      if(HAL_SRAM_Read_8b(&hsram, (uint32_t *)address, (uint8_t *)data, size) == HAL_OK)
        return StatusOk;
      break;
    case bit16:
      if(HAL_SRAM_Read_16b(&hsram, (uint32_t *)address, (uint16_t *)data, size) == HAL_OK)
        return StatusOk;
      break;
    case bit32:
      if(HAL_SRAM_Read_32b(&hsram, (uint32_t *)address, data, size) == HAL_OK)
        return StatusOk;
      break;
  }
  return StatusError;
}
