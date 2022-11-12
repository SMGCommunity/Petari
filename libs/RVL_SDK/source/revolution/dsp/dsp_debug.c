#include <revolution/dsp/dsp_debug.h>
#include <va_list.h>

void __DSP_debug_printf(char *pFormat, ...) {
    // oops. code was probably #ifdef'd out at some point
    va_list list;
}
