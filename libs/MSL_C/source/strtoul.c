#include "stdio_api.h"
#include <cctype>

#define final_state(scan_state)	(scan_state & (0x20 | 0x40))
#define success(scan_state) (scan_state & (0x4 | 0x10 | 0x20))
#define fetch() (count++, (*ReadProc)(ReadProcArg, 0, __GetAChar))
#define unfetch(c) (*ReadProc)(ReadProcArg, c, __UngetAChar)	

unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow) {
    int scan_state = 1;
    int count = 0;
    int spaces = 0;
    unsigned long value = 0;
    unsigned long value_max = 0;
    int c;

    *negative = *overflow = 0;

    if (base < 0 || base == 1 || base > 36 || max_width < 1) {
        scan_state = 0x40;
    }
    else {
        c = fetch();
    }

    if (base != 0) {
        value_max = 0xFFFFFFFF / base;
    }

    while (count <= max_width && c != -1 && !final_state(scan_state)) {
        switch (scan_state) {
            case 1:
                if (isspace(c)) {
                    c = fetch();
                    count--;
                    spaces++;
                    break;
                }

                if (c == '+') {
                    c = fetch();
                }
                else if (c == '-') {
                    c = fetch();
                    *negative = 1;
                }

                scan_state = 2;
                break;
            
            case 2:
                if (base == 0 || base == 16) {
                    if (c == '0') {
                        scan_state = 4;
                        c = fetch();
                        break;
                    }
                }

                scan_state = 8;
                break;
            
            case 4:
                if (c == 'X' || c == 'x') {
                    base = 16;
                    scan_state = 8;
                    c = fetch();
                    break;
                }

                if (base == 0) {
                    base = 8;
                }

                scan_state = 16;
                break;

            case 8:
            case 16:
                if (base == 0) {
                    base = 10;
                }

                if (!value_max) {
                    value_max = 0xFFFFFFFF / base;
                }

                if (isdigit(c)) {
                    if ((c -= '0') >= base) {
                        if (scan_state == 16) {
                            scan_state = 32;
                        }
                        else {
                            scan_state = 64;
                        }

                        c += '0';
                        break;
                    }
                }
                else if (!isalpha(c) || (toupper(c) - 'A' + 10) >= base) {
                    if (scan_state == 16) {
                        scan_state = 32;
                    }
                    else {
                        scan_state = 64;
                    }

                    break;
                }
                else {
                    c = toupper(c) - 'A' + 10;
                }

                if (value > value_max) {
                    *overflow = 1;
                }

                value *= base;

                if (c > (0xFFFFFFFF - value)) {
                    *overflow = 1;
                }

                value += c;
                scan_state = 16;
                c = fetch();
                break;
        }
    }

    if (!success(scan_state)) {
        count = 0;
        value = 0;
        *chars_scanned = 0;
    }
    else {
        count--;
        *chars_scanned = count + spaces;
    }

    unfetch(c);
    return value;
}

unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow) {
    int scan_state = 1;
    int count = 0;
    int spaces = 0;
    unsigned long long value = 0;
    unsigned long long value_max = 0;
    unsigned long long ullmax = 0xFFFFFFFFFFFFFFFF;
    int c;

    *negative = *overflow = 0;

    if (base < 0 || base == 1 || base > 36 || max_width < 1) {
        scan_state = 64;
    }
    else {
        c = fetch();
    }

    if (base != 0) {
        value_max = 0xFFFFFFFFFFFFFFFF / base;
    }

    while (count <= max_width && c != -1 && !final_state(scan_state)) {
        switch (scan_state) {
            case 1:
                if (isspace(c)) {
                    c = fetch();
                    count--;
                    spaces++;
                    break;
                }

                if (c == '+') {
                    c = fetch();
                }
                else if (c == '-') {
                    c = fetch();
                    *negative = 1;
                }

                scan_state = 2;
                break;
            
            case 2:
                if (base == 0 || base == 16) {
                    if (c == '0') {
                        scan_state = 4;
                        c = fetch();
                        break;
                    }
                }

                scan_state = 8;
                break;

            case 4:
                if (c == 'X' || c == 'x') {
                    base = 16;
                    scan_state = 8;
                    c = fetch();
                    break;
                }

                if (base == 0) {
                    base = 8;
                }

                scan_state = 16;
                break;

            case 8:
            case 16:
                if (base == 0) {
                    base = 10;
                }

                if (!value_max) {
                    value_max = ullmax / base;
                }

                if (isdigit(c)) {
                    if ((c -= '0') >= base) {
                        if (scan_state == 16) {
                            scan_state = 32;
                        }
                        else {
                            scan_state = 64;
                        }

                        c += '0';
                        break;
                    }
                }
                else if (!isalpha(c) || (toupper(c) - 'A' + 10) >= base) {
                    if (scan_state == 16) {
                        scan_state = 32;
                    }
                    else {
                        scan_state = 64;
                    }
                    
                    break;
                }
                else {
                    c = toupper(c) - 'A' + 10;
                }

                if (value > value_max) {
                    *overflow = 1;
                }

                value *= base;

                if (c > (ullmax - value)) {
                    *overflow = 1;
                }

                value += c;
                scan_state = 16;
                c = fetch();
                break;
        }
    }

    if (!success(scan_state)) {
        count = 0;
        value = 0;
        *chars_scanned = 0;
    }
    else {
        count--;
        *chars_scanned = count + spaces;
    }

    unfetch(c);
    return value;
}

/*
unsigned long strtoul(const char *str, char **end, int base) {
    unsigned long value;
    int count, negative, overflow;

    __InStrCtrl isc;
    isc.NextChar = (char*)str;
    isc.NullCharDetected = 0;

    value = __strtoul(base, 0xFFFFFFFF, )
}
*/