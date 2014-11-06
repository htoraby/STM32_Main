#include "gpio.h"

GPIO_TypeDef* portLeds[LED_N] = {
  STOP_LED_PORT, WAIT_LED_PORT, WORK_LED_PORT, FAN_LED_PORT
};
const uint16_t pinLeds[LED_N] = {
  STOP_LED_PIN, WAIT_LED_PIN, WORK_LED_PIN, FAN_LED_PIN
};

void initGpio()
{
  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOI_CLK_ENABLE();

  initLed(StopLed);
  initLed(WaitLed);
  initLed(WorkLed);
  initLed(FanLed);

  initPinInput(POWER_BUTTON_PORT, POWER_BUTTON_PIN);
  initPinInput(SYS_RESET_BUTTON_PORT, SYS_RESET_BUTTON_PIN);
}

void initLed(LedType led, GPIO_PinState value)
{
  initPinOut(portLeds[led], pinLeds[led], value);
}

void onLed(LedType led)
{
  HAL_GPIO_WritePin(portLeds[led], pinLeds[led], GPIO_PIN_SET);
}

void offLed(LedType led)
{
  HAL_GPIO_WritePin(portLeds[led], pinLeds[led], GPIO_PIN_RESET);
}

void toggleLed(LedType led)
{
  HAL_GPIO_TogglePin(portLeds[led], pinLeds[led]);
}

void initPinInput(GPIO_TypeDef* port, const uint16_t pin)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(port, &GPIO_InitStruct);
}

GPIO_PinState getPinInput(GPIO_TypeDef* port, const uint16_t pin)
{
  return HAL_GPIO_ReadPin(port, pin);
}

void initPinOut(GPIO_TypeDef* port, const uint16_t pin, GPIO_PinState value)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(port, pin, value);
}

void setPinOut(GPIO_TypeDef* port, const uint16_t pin)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void clrPinOut(GPIO_TypeDef* port, const uint16_t pin)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}
