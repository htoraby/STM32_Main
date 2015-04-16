#include "shell.h"
#include "cmds.h"

#include <string.h>
#include <stdio.h>

void shell_parse_request(char *buf)
{
  int argc = 0;
  char *argv[20];
  char *str;
  str = strtok(buf, " ");
  while (str != NULL) {
    argv[argc++] = str;
    str = strtok(NULL, " ");
  }
  shell_main(argc, argv);
}

int shell_main(int argc, char *argv[])
{
  for(int i = 0; i < argc; i++) {
    if (++i >= argc) {
      return 1;
    }

    if (!strcmp(argv[i], "task")) {
      return shell_task(argc, argv);
    }

    if (!strcmp(argv[i], "memuse")) {
      return shell_memuse(argc, argv);
    }
  }

  return 1;
}
