#include <cstdlib>

int __aborting = 0;

void abort(void) {
    raise(1);
    __aborting = 1;
    exit(1);
}