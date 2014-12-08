#include "temp_sensor.h"

enum {
  CMD_TEMP = 0x00,
  CMD_CONF = 0x01,
  CMD_T_HYS = 0x02,
  CMD_T_OS = 0x03,
};

#define I2C_ADDRESS 0x90

I2C_HandleTypeDef hi2c3;

/* I2C3 init function */
StatusType tempSensorInit()
{
  StatusType status = StatusError;

  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 300000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c3);

  if (HAL_I2C_IsDeviceReady(&hi2c3, I2C_ADDRESS, 5, 1000) == HAL_OK)
    status = StatusOk;

  return status;
}


void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance == I2C3) {
    /* Peripheral clock enable */
    __I2C3_CLK_ENABLE();

    /**I2C3 GPIO Configuration
    PH7     ------> I2C3_SCL
    PH8     ------> I2C3_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance == I2C3) {
    __I2C1_FORCE_RESET();
    __I2C1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_7|GPIO_PIN_8);
  }
}

float tempSensorReadData()
{
  uint8_t data[2];
  data[0] = CMD_TEMP;

  if (HAL_I2C_Master_Transmit(&hi2c3, I2C_ADDRESS, data, 1, 1000) != HAL_OK)
    return 999;
  if (HAL_I2C_Master_Receive(&hi2c3, I2C_ADDRESS, data, 2, 1000) != HAL_OK)
    return 999;

  uint16_t tempInt = data[1];
  tempInt += (data[0] & 0x7F)<<8;
  tempInt = (tempInt >> 7);
  float tempFloat = (float)tempInt/2;
  if (data[0] & 0x80)
    tempFloat = -tempFloat;

  return tempFloat;
}
