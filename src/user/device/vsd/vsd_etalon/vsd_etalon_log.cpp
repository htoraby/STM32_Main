#include "vsd_etalon_log.h"

VsdEtalonLog::VsdEtalonLog()
  : VsdLog(VSD_LOG_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{

}

VsdEtalonLog::~VsdEtalonLog()
{

}

void VsdEtalonLog::readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                                uint16_t *ud)
{
  uint16_t t = 0;
  VsdLog::readLog(0x9fff, &t, 1);

  VsdLog::readLog(0x8030, ia, 2000);
  VsdLog::readLog(0x8830, ib, 2000);
  VsdLog::readLog(0x9030, ic, 2000);
  VsdLog::readLog(0x9800, ud, 200);
}

void VsdEtalonLog::readRunningLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                                  uint16_t *ud, uint16_t *cos)
{
  VsdLog::readLog(0xA000, ia, 400);
  VsdLog::readLog(0xA190, ib, 400);
  VsdLog::readLog(0xA320, ic, 400);
  VsdLog::readLog(0xA4B0, ud, 400);
  VsdLog::readLog(0xA640, cos, 400);
}
