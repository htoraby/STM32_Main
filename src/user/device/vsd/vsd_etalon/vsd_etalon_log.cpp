#include "vsd_etalon_log.h"

VsdEtalonLog::VsdEtalonLog()
  : VsdLog(VSD_LOG_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{
  createThread("VsdEtalonLog");
}

VsdEtalonLog::~VsdEtalonLog()
{

}

