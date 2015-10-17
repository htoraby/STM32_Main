#include "vsd_novomet_log.h"

VsdNovometLog::VsdNovometLog()
  : VsdLog(VSD_LOG_UART, 115200, 8, UART_STOPBITS_1, UART_PARITY_NONE, 1)
{

}

VsdNovometLog::~VsdNovometLog()
{

}

