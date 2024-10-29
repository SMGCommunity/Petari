#include <cstdlib>
#pragma exceptions on

int raise(int);
int exit(int);

void (*__stdio_exit)(void) = 0;
int __aborting = 0;

void abort(void) {
    raise(1);
    __aborting = 1;
    exit(1);
}
