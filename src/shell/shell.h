#ifndef SHELL_H
#define SHELL_H

#include <string.h>
#include <stdio.h>

void shellInit();
void shellParseRequest(char *buf);
int shellMain(int argc, char *argv[]);

#endif // SHELL_H
