#include <revolution/dsp/dsp_debug.h>
#include <cstdarg.h>
#include <cstdio.h>

void __DSP_debug_printf(char *pFormat, ...) {
    // oops. code was probably #ifdef'd out at some point
    va_list list;
}
