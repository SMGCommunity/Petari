#include "size_t.h"
#include "UART.h"
#include <revolution.h>
#pragma exceptions on

int __TRK_write_console(unsigned long, unsigned char *, size_t *, void *);

typedef void (* __idle_proc)  (void);

static UARTError __init_uart_console(void) {
    UARTError err = kUARTNoError;
    static int initialized = 0;

    if (initialized == 0) {
        err = InitializeUART(kBaud57600);

        if (err == kUARTNoError) {
            initialized = 1;
        }
    }

    return err;

}

int __write_console(unsigned long handle, unsigned char *buffer, size_t *count, void *ref) {
    if (!(OSGetConsoleType() & 0x20000000)) {
        if (__init_uart_console() != kUARTNoError) {
            return 1;
        }

        if (WriteUARTN(buffer, *count) != kUARTNoError) {
            *count = 0;
            return 1;
        }
    }

    __TRK_write_console(handle, buffer, count, ref);
    return 0;
}

int __close_console(unsigned long handle) {
    return 0;
}
