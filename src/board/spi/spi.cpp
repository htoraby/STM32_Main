#include "spi.h"

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
    __SPI1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
  }
  else if(hspi->Instance == SPI2) {
    __SPI2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  }
  else if(hspi->Instance == SPI3) {
    __SPI3_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
  }
  else if(hspi->Instance == SPI5) {
    __SPI5_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_8|GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_6);
  }
}
