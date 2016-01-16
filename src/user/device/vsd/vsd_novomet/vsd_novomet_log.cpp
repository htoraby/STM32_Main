#include "vsd_novomet_log.h"
#include "uart.h"
#include "string.h"
#include "user_main.h"

VsdNovometLog::VsdNovometLog()
  : VsdLog(VSD_LOG_UART, 57600, 8, UART_STOPBITS_2, UART_PARITY_NONE, 1)
{

}

VsdNovometLog::~VsdNovometLog()
{

}

bool VsdNovometLog::checkAlarm()                // Функция отсле
{
  static bool old = true;                       // Изначально низкий
  static bool alarm = false;
  bool result = false;
  bool now = parameters.get(CCS_DI_11_VALUE);   // Текущий
  if (parameters.isValidity(CCS_DI_11_VALUE)) { // Валиден
    if ((old == false) && (now == true)) {      // Переход с высокого на низкий
      alarm = true;
      setAlarm();
      result = true;                            // Авария
    }
    else if ((alarm == true) && (now == false)) {
      alarm = false;
      resetAlarm();
    }
    old = now;
  }
  return result;
}

bool VsdNovometLog::checkReady()
{
  return true;
}

void VsdNovometLog::resetReady()
{
//  setDigitalOutput(DO1, PinSet);
  return;
}

void VsdNovometLog::resetAlarm()
{
  setDigitalOutput(DO1, PinSet);
}

void VsdNovometLog::setAlarm()
{
  setDigitalOutput(DO1, PinReset);
}

void VsdNovometLog::readAlarmLog(int16_t *ia, int16_t *ib, int16_t *ic,
                                 int16_t *ud)
{
  VsdLog::readNovometLog(ia, ib, ic, ud);
}

void VsdNovometLog::readRunningLog(int16_t *ia, int16_t *ib, int16_t *ic,
                                  int16_t *ud, int16_t *cos)
{

//  memset(cos, 0x0, sizeof(cos));  TODO: Очищение массива cos в 0, т.к. не получаем данные от ПЧ
//  VsdLog::readNovometLog(ia, ib, ic, ud);
return;
}
