#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#include "ansi_params.h"
#include <cstdio>

int __read_console(__std(unsigned long), unsigned char *, __std(size_t) *, __std(void *));
int __write_console(__std(unsigned long), unsigned char *, __std(size_t) *, __std(void *));
int __close_console(__std(unsigned long));

#endif // CONSOLE_IO_H