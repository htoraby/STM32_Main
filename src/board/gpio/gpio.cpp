#include "gpio.h"

GPIO_TypeDef* portLeds[LedMax] = {
  STOP_LED_PORT, WAIT_LED_PORT, WORK_LED_PORT, FAN_LED_PORT
};
const uint16_t pinLeds[LedMax] = {
  STOP_LED_PIN, WAIT_LED_PIN, WORK_LED_PIN, FAN_LED_PIN
};

GPIO_TypeDef* portButtons[ButtonMax] = {
  POWER_BUTTON_PORT, SYS_RESET_BUTTON_PORT,
};
const uint16_t pinButtons[ButtonMax] = {
  POWER_BUTTON_PIN, SYS_RESET_BUTTON_PIN,
};

GPIO_TypeDef* portDI[DigitalInputMax] = {
  GPIOF, GPIOB, GPIOB, GPIOC, GPIOC, GPIOG,
  GPIOB, GPIOB, GPIOB, GPIOH, GPIOD, GPIOB,
  GPIOC, GPIOB,
};
const uint16_t pinDI[DigitalInputMax] = {
  GPIO_PIN_11, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_15,
  GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_15,
  GPIO_PIN_8, GPIO_PIN_13,
};

GPIO_TypeDef* portDO[DigitalOutputMax] = {
  GPIOB, GPIOH, GPIOH, GPIOH,
};
const uint16_t pinDO[DigitalOutputMax] = {
  GPIO_PIN_12, GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_10,
};

static void initLed(LedType led, PinState value = PinReset);
static void initButton(ButtonType button);

void gpioInit()
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

  initButton(PowerButton);
  initButton(SysResetButton);

  initPinInput(WATCH_PIN);

  for (int i = 0; i < DigitalInputMax; ++i) {
    initPinInput(portDI[i], pinDI[i]);
  }
  for (int i = 0; i < DigitalOutputMax; ++i) {
    initPinOut(portDO[i], pinDO[i]);
  }
}

/*!
 \brief Инициализация LED

 \param led: номер из списка @ref LedType
 \param value: 0 - off; 1- on
*/
static void initLed(LedType led, PinState value)
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

/*!
 \brief Инициализация кнопки

 \param button: номер из списка @ref ButtonType
*/
static void initButton(ButtonType button)
{
  initPinInput(portButtons[button], pinButtons[button]);
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

PinState getPinInput(GPIO_TypeDef* port, const uint16_t pin)
{
  return (PinState)HAL_GPIO_ReadPin(port, pin);
}

void initPinOut(GPIO_TypeDef* port, const uint16_t pin, PinState value)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)value);
}

void setPinOut(GPIO_TypeDef* port, const uint16_t pin)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void clrPinOut(GPIO_TypeDef* port, const uint16_t pin)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

PinState getDigitalInput(const uint16_t num)
{
  return (PinState)HAL_GPIO_ReadPin(portDI[num], pinDI[num]);
}

void setDigitalOutput(const uint16_t num, PinState value)
{
  HAL_GPIO_WritePin(portDO[num], pinDO[num], (GPIO_PinState)value);
}

bool isPowerGood()
{
  return getPinInput(WATCH_PIN);
}
