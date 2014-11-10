#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
enum LedType
{
  StopLed,
  WaitLed,
  WorkLed,
  FanLed
};

/* Exported constants --------------------------------------------------------*/
#define LED_N 4
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

/* Exported functions ------------------------------------------------------- */
/*!
 \brief Настройка выводов

*/
void gpioInit();

/*!
 \brief Configures LED GPIO

 \param led: Specifies the Led to be configured
 \param value: 0 - off; 1- on
*/
void initLed(LedType led, GPIO_PinState value = GPIO_PIN_RESET);

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

 \param port
 \param pin
*/
void initPinInput(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Получение значения на входе

 \param port
 \param pin
*/
GPIO_PinState getPinInput(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Настройка вывода на выход

 \param port
 \param pin
 \param value: значение на выходе 0 или 1. По умолчанию 0
*/
void initPinOut(GPIO_TypeDef* port, const uint16_t pin, GPIO_PinState value = GPIO_PIN_RESET);

/*!
 \brief Установка значения на выходе в 1

 \param port
 \param pin
*/
void setPinOut(GPIO_TypeDef* port, const uint16_t pin);

/*!
 \brief Установка значения на выходе в 0

 \param port
 \param pin
*/
void clrPinOut(GPIO_TypeDef* port, const uint16_t pin);


#endif // GPIO_H
