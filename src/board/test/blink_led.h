#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "board.h"

// ----- LED definitions ------------------------------------------------------
// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...

#define BLINK_PORT_NUMBER1              (1)
#define BLINK_PIN_NUMBER1               (6)
#define BLINK_PORT_NUMBER2              (1)
#define BLINK_PIN_NUMBER2               (7)
#define BLINK_PORT_NUMBER3              (1)
#define BLINK_PIN_NUMBER3               (8)

#define BLINK_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)              (1 << (_N))
#define BLINK_RCC_MASKx(_N)             (RCC_AHB1Periph_GPIOA << (_N))

// ----------------------------------------------------------------------------

class BlinkLed
{
public:
  BlinkLed();

  void prvSetupHardware();
  void prvLedBlink(void *pvParameters);

  inline void __attribute__((always_inline)) turnOn()
  {
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER1),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER1), GPIO_PIN_SET);
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER2),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER2), GPIO_PIN_SET);
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER3),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER3), GPIO_PIN_SET);
  }

  inline void __attribute__((always_inline)) turnOff()
  {
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER1),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER1), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER2),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER2), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER3),
                      BLINK_PIN_MASK(BLINK_PIN_NUMBER3), GPIO_PIN_RESET);
  }

  inline void __attribute__((always_inline)) toggle()
  {
    switch (numLed) {
      case 0:
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER1),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER1), GPIO_PIN_SET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER2),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER2), GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER3),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER3), GPIO_PIN_RESET);
        break;
      case 1:
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER1),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER1), GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER2),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER2), GPIO_PIN_SET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER3),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER3), GPIO_PIN_RESET);
        break;
      case 2:
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER1),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER1), GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER2),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER2), GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER3),
                          BLINK_PIN_MASK(BLINK_PIN_NUMBER3), GPIO_PIN_SET);
        break;
      default:
        break;
    }
    if (numLed >= 2)
      numLed = 0;
    else
      numLed++;
  }

private:
  int numLed;

};

#endif // BLINKLED_H_
