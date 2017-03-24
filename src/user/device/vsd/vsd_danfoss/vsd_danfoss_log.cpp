#include "vsd_danfoss_log.h"
#include "user_main.h"
#include "adc_ext.h"

VsdDanfossLog::VsdDanfossLog()
  : VsdLog(VSD_LOG_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{

}

VsdDanfossLog::~VsdDanfossLog()
{

}

bool VsdDanfossLog::checkAlarm()
{
  static bool oldStateNoAlarm = true;
  static bool oldStateNoStop = false;
  bool isAlarm = false;
  if ((!parameters.get(CCS_DI_11_VALUE) && parameters.isValidity(CCS_DI_11_VALUE) && oldStateNoAlarm ) ||
      (ksu.isStopMotor() && ksu.isAlarmStop() && oldStateNoStop )) {
    isAlarm = true;
  }
  oldStateNoAlarm = parameters.get(CCS_DI_11_VALUE);
  oldStateNoStop = !ksu.isStopMotor();
  return isAlarm;
}

bool VsdDanfossLog::checkReady()
{
  //osDelay(10);
  return true;
}

void VsdDanfossLog::resetReady()
{
  return;
}

void VsdDanfossLog::readAlarmLog(uint16_t *ia, uint16_t */*ib*/, uint16_t */*ic*/,
                                 uint16_t */*ud*/)
{
  copyAdcDataAI6(ia);
}

void VsdDanfossLog::readRunningLog(uint16_t *ia, uint16_t */*ib*/, uint16_t */*ic*/, uint16_t */*ud*/, uint16_t */*cos*/)
{
  copyAdcDataAI6(ia);
}

