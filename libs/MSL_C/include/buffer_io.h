#ifndef BUFFER_IO_H
#define BUFFER_IO_H

#include "file_struct.h"

void __prep_buffer(FILE *);
int __flush_buffer(FILE *, size_t *);

#endif // BUFFER_IO_H