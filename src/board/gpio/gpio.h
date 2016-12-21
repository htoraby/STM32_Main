#ifndef GPIO_H
#define GPIO_H

#include "board.h"

/*!
 * \brief Значения вывода
*/
enum PinState {
  PinReset,
  PinSet,
};

/*!
 * \brief Список LED
*/
enum LedType {
  StopLed,
  WaitLed,
  WorkLed,
  TestLed,
  LedMax,
};

/*!
 * \brief Список цифровых входов
*/
enum DiNum {
  DI1, DI2, DI3, DI4, DI5, DI6, DI7, DI8, DI9, DI10, DI11, DI12, DI13, DI14,
  DigitalInputMax
};

/*!
 * \brief Список цифровых выходов
*/
enum DoNum {
  DO1, DO2, DO3,
  DigitalOutputMax
};

/*!
 * \brief Список релейных выходов
*/
enum RoNum {
  RO1, RO2, RO3, RO4, RO5,
  RelayOutputMax
};

#define STOP_LED_PIN                    GPIO_PIN_6
#define STOP_LED_PORT                   GPIOB
#define WAIT_LED_PIN                    GPIO_PIN_7
#define WAIT_LED_PORT                   GPIOB
#define WORK_LED_PIN                    GPIO_PIN_8
#define WORK_LED_PORT                   GPIOB
#define FAN_LED_PIN                     GPIO_PIN_3
#define FAN_LED_PORT                    GPIOH

#define POWER_BUTTON_PIN                GPIOI, GPIO_PIN_9
#define SYS_RESET_BUTTON_PIN            GPIOC, GPIO_PIN_13

#define MASTER_GPIO_PIN                 GPIOI, GPIO_PIN_10

//! Вывод детектора основного питания
#define POWER_WARNING_PIN  GPIOI, GPIO_PIN_5
//! Вывод детектора ИБП
#define POWER_ALARM_PIN  GPIOC, GPIO_PIN_0
//! Вывод переключения питания на ИБП
#define OFF_REL_PIN  GPIOI, GPIO_PIN_6
//! Вывод включения подсветки LCD
#define LCD_ON_PIN  GPIOD, GPIO_PIN_3
//! Вывод переключения USB между контроллерами
#define USB_ON_PIN  GPIOG, GPIO_PIN_9

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
 * \brief Отключение всех LED
 */
void offAllLeds();

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

/*!
 \brief Установка значения на релейном выходе

 \param num: номер выхода
 \param value: значение на выходе 0 или 1. По умолчанию 0
*/
void setRelayOutput(const uint16_t num, PinState value = PinReset);

/*!
 * \brief Получение состояния питания платы от детектора
 * \return bool: true - норма, false - падение напряжения ниже порога детектора
*/
bool isPowerGood();

/*!
 * \brief Получение состояния ИБП от детектора
 * \return
 */
bool isUpsGood();

/*!
 * \brief Функция вкл/выкл питания от ИБП
 * \param on - вкл/выкл
 */
void turnPowerBattery(bool on);

/*!
 * \brief Включение LCD
*/
void onLcd();

/*!
 * \brief Выключение LCD
*/
void offLcd();

/*!
 * \brief Включение модуля AM335x
*/
void onPowerAm335x();

/*!
 * \brief Выключение модуля AM335x
*/
void offPowerAm335x();

/*!
 * \brief Сброс модуля AM335x
 */
void resetAm335x();

#endif // GPIO_H
