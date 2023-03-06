#include "size_t.h"
#include <revolution.h>

/*
int __write_console(unsigned long handle, unsigned char* pBuf, size_t *pCount, void *pCon) {
    size_t limit;
}
*/

typedef void (* __idle_proc)  (void);

int __read_console(unsigned long handle, unsigned char *buffer, size_t *count, __idle_proc) {
    return 0;
}

int __write_console(unsigned long, unsigned char *buffer, size_t *count, void *ref) {
    return 0;
}

int __close_console(unsigned long handle) {
    return 0;
}