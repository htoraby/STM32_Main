#include "cmds.h"
#include "shell.h"
#include "user_main.h"

#if USE_EXT_MEM
static uint8_t bufferTx[4096] __attribute__((section(".extmem")));
#else
static uint8_t bufferTx[4096];
#endif

int shell_task(int argc, char *argv[])
{
  (void)argv;

  if (argc > 1)
    return 1;

  osThreadList(bufferTx);

  uartWriteData(HOST_UART, bufferTx, strlen((char*)bufferTx));

  return 0;
}

int shell_memuse(int argc, char *argv[])
{
  (void)argv;

  if (argc > 1)
    return 1;

  int size = xPortGetFreeHeapSize();
  sprintf((char*)bufferTx, "\nHeap size: %d\n", size);
  uartWriteData(HOST_UART, bufferTx, strlen((char*)bufferTx));

  return 0;
}
