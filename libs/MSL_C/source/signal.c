#include "signal.h"
#include "ansi_params.h"

int exit(int);

__signal_func_ptr signal_funcs[7];

int raise(int sig) {
    __signal_func_ptr signal_func;

    if (sig < 1 || sig > 7) {
        return -1;
    }

    signal_func = signal_funcs[sig - 1];

    if (signal_func != ((__std(__signal_func_ptr)) 1)) {
       signal_funcs[sig - 1] = ((__std(__signal_func_ptr)) 0);
    }

    if (signal_func == ((__std(__signal_func_ptr)) 1) || (signal_func == ((__std(__signal_func_ptr)) 0) && sig == 1)) {
        return 0;
    }

    if (signal_func == ((__std(__signal_func_ptr)) 0)) {
        exit(0);
    }

    (*signal_func)(sig);
    return 0;
}