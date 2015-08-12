#include "spi.h"
#include "flash_ext.h"

extern void framTxRxCpltCallback();
extern void adcExtTxRxCpltCallback();
extern void hostSpiDmaTransmitCplt(DMA_HandleTypeDef *hdma);

static DMA_HandleTypeDef hdma_spi4_tx;

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance == SPI1) {
    /* Peripheral clock enable */
    __SPI1_CLK_ENABLE();

    /**SPI1 GPIO Configuration
      PA5     ------> SPI1_SCK
      PA6     ------> SPI1_MISO
      PA7     ------> SPI1_MOSI
      */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(hspi->Instance == SPI2) {
    /* Peripheral clock enable */
    __SPI2_CLK_ENABLE();

    /**SPI2 GPIO Configuration
    PI1     ------> SPI2_SCK
    PI2     ------> SPI2_MISO
    PI3     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  }
  else if(hspi->Instance == SPI3) {
    /* Peripheral clock enable */
    __SPI3_CLK_ENABLE();

    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else if(hspi->Instance == SPI4) {
    /* Peripheral clock enable */
    __SPI4_CLK_ENABLE();

    /**SPI4 GPIO Configuration
    PE2     ------> SPI4_SCK
    PE4     ------> SPI4_NSS
    PE5     ------> SPI4_MISO
    PE6     ------> SPI4_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2/*|GPIO_PIN_4*/|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_spi4_tx.Instance = DMA2_Stream1;
    hdma_spi4_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_spi4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi4_tx.Init.Mode = DMA_NORMAL;
    hdma_spi4_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_spi4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_spi4_tx);

    hdma_spi4_tx.XferCpltCallback = hostSpiDmaTransmitCplt;

    __HAL_LINKDMA(hspi, hdmatx, hdma_spi4_tx);

    HAL_NVIC_SetPriority(SPI4_IRQn, HOST_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(SPI4_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, HOST_IRQ_PREPRIO, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  }
  else if(hspi->Instance == SPI5) {
    /* Peripheral clock enable */
    __SPI5_CLK_ENABLE();

    /**SPI5 GPIO Configuration
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    PH6     ------> SPI5_SCK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance == SPI1) {
    __SPI1_FORCE_RESET();
    __SPI1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
  }
  else if(hspi->Instance == SPI2) {
    __SPI2_FORCE_RESET();
    __SPI2_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  }
  else if(hspi->Instance == SPI3) {
    __SPI3_FORCE_RESET();
    __SPI3_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
  }
  else if(hspi->Instance == SPI4) {
    __SPI4_FORCE_RESET();
    __SPI4_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2/*|GPIO_PIN_4*/|GPIO_PIN_5|GPIO_PIN_6);

    HAL_DMA_DeInit(hspi->hdmatx);

    HAL_NVIC_DisableIRQ(SPI4_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
  }
  else if(hspi->Instance == SPI5) {
    __SPI5_FORCE_RESET();
    __SPI5_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_8|GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_6);
  }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1) {
    flashTxRxCpltCallback(FlashSpi1);
  } else if (hspi->Instance == SPI3) {
    framTxRxCpltCallback();
  } else if (hspi->Instance == SPI5) {
    flashTxRxCpltCallback(FlashSpi5);
  }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1) {
    flashTxRxCpltCallback(FlashSpi1);
  } else if (hspi->Instance == SPI3) {
    framTxRxCpltCallback();
  } else if (hspi->Instance == SPI5) {
    flashTxRxCpltCallback(FlashSpi5);
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI2) {
    adcExtTxRxCpltCallback();
  }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi)
{
  if (hspi->Instance == SPI1) {
    flashTxRxCpltCallback(FlashSpi1);
  } else if (hspi->Instance == SPI3) {
    framTxRxCpltCallback();
  } else if (hspi->Instance == SPI5) {
    flashTxRxCpltCallback(FlashSpi5);
  }
}

void spiWait(SPI_HandleTypeDef* hspi)
{
  while (HAL_SPI_GetState(hspi) != HAL_SPI_STATE_READY) {
    osDelay(1);
  }
}
