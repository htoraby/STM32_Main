#ifndef SHELL_H
#define SHELL_H

#include <string.h>
#include <stdio.h>

void shell_parse_request(char *buf);
int shell_main(int argc, char *argv[]);

#endif // SHELL_H
