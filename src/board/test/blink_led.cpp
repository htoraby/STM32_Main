#include "blink_led.h"

BlinkLed::BlinkLed()
  : numLed(0)
{

}

void BlinkLed::prvSetupHardware()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER1);
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLINK_GPIOx(BLINK_PORT_NUMBER1), &GPIO_InitStructure);

  GPIO_InitStructure.Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER2);
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLINK_GPIOx(BLINK_PORT_NUMBER2), &GPIO_InitStructure);

  GPIO_InitStructure.Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER3);
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLINK_GPIOx(BLINK_PORT_NUMBER3), &GPIO_InitStructure);
}

void BlinkLed::prvLedBlink(void *pdTASK_CODE)
{
  (void)(pdTASK_CODE);
  while(1) {
    turnOff();
    HAL_Delay(500);
    turnOn();
    HAL_Delay(500);
  }
}
