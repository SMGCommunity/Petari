#include "portable/msghndlr.h"

extern void OSReport(const char *);

void usr_put_initialize() {
    
}

int usr_puts_serial(const char *s) {
    int result = 0;
    char c;
    char buf[2];

    while ((result == 0) && (c = *s++) != 0) {
        int conn = GetTRKConnected();

        buf[0] = c;
        buf[1] = '\0';

        SetTRKConnected(0);
        OSReport(buf);
        SetTRKConnected(conn);
        result = 0;
    }

    return result;
}