#include "shell.h"
#include "cmds.h"
#include "cmsis_os.h"

#include <string.h>
#include <stdio.h>

#ifdef USE_RTT
#include "SEGGER_RTT.h"
#endif

static void shellThread(void *argument);

static char buf[100];

void shellInit()
{
  osThreadDef(Shell, shellThread, osPriorityNormal, 0, 3*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Shell), NULL);
}

void shellParseRequest(char *buf)
{
  int argc = 0;
  char *argv[20];
  char *str;
  str = strtok(buf, " ");
  while (str != NULL) {
    argv[argc++] = str;
    str = strtok(NULL, " ");
  }
  shellMain(argc, argv);
}

int shellMain(int argc, char *argv[])
{
  for(int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "task")) {
      return shell_task(argc, argv);
    }

    if (!strcmp(argv[i], "memuse")) {
      return shell_memuse(argc, argv);
    }
  }

  return 1;
}

static void shellThread(void *argument)
{
  (void)argument;

  int key = -1;
  int count = 0;

  memset(buf, 0, sizeof(buf));

  while(1) {
    osDelay(10);

#ifdef USE_RTT
    key = SEGGER_RTT_GetKey();
#endif

    if (key >= 0) {
      if (key == '\n') {
        shellParseRequest(buf);
        count = 0;
        memset(buf, 0, sizeof(buf));
      } else {
        buf[count++] = (char)key;
      }
    }
  }
}
