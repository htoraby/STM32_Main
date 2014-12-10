#ifndef GPIO_H
#define GPIO_H

#include "board.h"

/*!
 \brief Значения вывода

*/
enum PinState {
  PinReset,
  PinSet,
};

/*!
 \brief Список LED

*/
enum LedType {
  StopLed,
  WaitLed,
  WorkLed,
  FanLed,
  LedMax,
};

/*!
 \brief Список кнопок

*/
enum ButtonType {
  PowerButton,
  SysResetButton,
  ButtonMax,
};

/*!
 \brief Список цифровых входов

*/
enum DiNum {
  DI1, DI2, DI3, DI4, DI5, DI6, DI7, DI8, DI9, DI10, DI11, DI12, DI13, DI14,
  DigitalInputMax
};

/*!
 \brief Список цифровых выходов

*/
enum DoNum {
  DO1, DO2, DO3, DO4,
  DigitalOutputMax
};

#define STOP_LED_PIN                    GPIO_PIN_6
#define STOP_LED_PORT                   GPIOB
#define WAIT_LED_PIN                    GPIO_PIN_7
#define WAIT_LED_PORT                   GPIOB
#define WORK_LED_PIN                    GPIO_PIN_8
#define WORK_LED_PORT                   GPIOB
#define FAN_LED_PIN                     GPIO_PIN_3
#define FAN_LED_PORT                    GPIOH

#define POWER_BUTTON_PIN                GPIO_PIN_9
#define POWER_BUTTON_PORT               GPIOI
#define SYS_RESET_BUTTON_PIN            GPIO_PIN_13
#define SYS_RESET_BUTTON_PORT           GPIOC

/*!
 \brief Инициализация выводов, кнопок, LED, DI, DO

*/
void gpioInit();

/*!
 \brief Turns selected LED On

 \param led: Specifies the Led to be set on
    This parameter can be one of following parameters:
      \arg LED_R
      \arg LED_O
      \arg LED_G
 \retval None
*/
void onLed(LedType led);

/*!
 \brief Turns selected LED Off

 \param led: Specifies the Led to be set off
    This parameter can be one of following parameters:
      \arg LED_R
      \arg LED_O
      \arg LED_G
*/
void offLed(LedType led);

/*!
 \brief Toggles the selected LED

 \param led: Specifies the Led to be toggled
    This parameter can be one of following parameters:
      \arg LED_R
      \arg LED_O
      \arg LED_G
*/
void toggleLed(LedType led);

/*!
 \brief Настройка вывода на вход

 \param port @ref GPIO_TypeDef
 \param pin @ref GPIO_pins_define
*/
void initPinInput(GPIO_TypeDef* port, const uint16_t pin);


/*!
 \brief Получение значения на входе

 \param port @ref GPIO_TypeDef
 \param pin @ref GPIO_pins_define
 \return PinState - значение на входе
*/
PinState getPinInput(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Настройка вывода на выход

 \param port @ref GPIO_TypeDef
 \param pin @ref GPIO_pins_define
 \param value: значение на выходе 0 или 1. По умолчанию 0
*/
void initPinOut(GPIO_TypeDef* port, const uint16_t pin, PinState value = PinReset);

/*!
 \brief Установка значения на выходе в 1

 \param port @ref GPIO_TypeDef
 \param pin @ref GPIO_pins_define
*/
void setPinOut(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Установка значения на выходе в 0

 \param port @ref GPIO_TypeDef
 \param pin @ref GPIO_pins_define
*/
void clrPinOut(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Получение значения на цифровом входе

 \param num: номер входа
 \return PinState - значение на входе
*/
PinState getDigitalInput(const uint16_t num);

/*!
 \brief Установка значения на цифровом выходе

 \param num: номер выхода
 \param value: значение на выходе 0 или 1. По умолчанию 0
*/
void setDigitalOutput(const uint16_t num, PinState value = PinReset);

#endif // GPIO_H
