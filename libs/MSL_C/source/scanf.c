#include "stdarg.h"
#include "stdio_api.h"
#include "va_list.h"
#include <cctype>

enum argument_options {
	normal_argument,
	char_argument,
	short_argument,
	long_argument,
	intmax_argument,
	size_t_argument,
	ptrdiff_argument,
	long_long_argument,
	double_argument,
	long_double_argument,
	wchar_argument
};

typedef unsigned char char_map[32];

typedef struct {
	unsigned char	suppress_assignment;
	unsigned char 	field_width_specified;
	unsigned char 	argument_options;
	unsigned char	conversion_char;
	int				field_width;
    char_map char_set;
} scan_format;

#define set_char_map(map, ch) map[(unsigned char)ch >> 3] |= (1 << (ch & 7))
#define tst_char_map(map, ch) (map[(unsigned char)ch >> 3] &  (1 << (ch & 7)))

  const char* parse_format(const char* format_string, scan_format *format) {
    const char* s = format_string;
    int c;
    int flag_found, invert;
    scan_format f = { 0, 0, normal_argument, 0, 2147483647, { 0 } };

    if (((c = *++s) == '%')) {
        f.conversion_char = c;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '*') {
        f.suppress_assignment = 1;
        c = *++s;
    }

    if (isdigit(c)) {
        f.field_width = 0;

        do {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        } while (isdigit(c));

        if (f.field_width == 0) {
            f.conversion_char = 0xFF;
            *format = f;
            return ((const char*)s + 1);
        }

        f.field_width_specified = 1;
    }

    flag_found = 1;

    switch (c) { 
        case 'h':
            f.argument_options = short_argument;

            if (s[1] == 'h') {
                f.argument_options = char_argument;
                c = *++s;
            }

            break;
        case 'l':
            f.argument_options = long_argument;

            if (s[1] == 'l') {
                f.argument_options = long_long_argument;
                c = *++s;
            }

            break;
        case 'L':
            f.argument_options = long_double_argument;
            break;

        case 'j':
            f.argument_options = intmax_argument;
            break;
        case 'z':
            f.argument_options = size_t_argument;
            break;
        case 't':
            f.argument_options = ptrdiff_argument;
            break;
        default:
            flag_found = 0;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;
    
    switch (c) {
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
            if (f.argument_options == long_double_argument) {
                f.conversion_char = 0xFF;
                break;
            }

            break;

        case 'a':
        case 'A':
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (f.argument_options == char_argument || f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
                f.conversion_char = 0xFF;
                break;
            }

            if (f.argument_options == long_argument) {
                f.argument_options = double_argument;
            }

            break;

        case 'p':
            f.argument_options = long_argument;
            f.conversion_char = 'x';
            break;

        case 'c':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            break;

        case 's':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i) {
                    *p++ = 0xFF;
                }

                f.char_set[1] = 0xC1;
                f.char_set[4] = 0xFE;
            }

            break;

        case 'n':
            break;

        case '[':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            c = *++s;
            invert = 0;

            if (c == '^') {
                invert = 1;
                c = *++s;
            }

            if (c == ']') {
                set_char_map(f.char_set, ']');
                c = *++s;
            }

            while (c && c != ']') {
                int d;
                set_char_map(f.char_set, c);

                if (*(s + 1) == '-' && (d = *(s + 2)) != 0 && d != ']') {
                    while (++c <= d) {
                        set_char_map(f.char_set, c);
                    }

                    c = *(s += 3);
                }
                else {
                    c = *++s;
                }
            }

            if (!c) {
                f.conversion_char = 0xFF;
                break;
            }

            if (invert) {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i, ++p) {
                    *p = ~*p;
                }

                break;
            }

        default:
            f.conversion_char = 0xFF;
            break;
    }

    *format = f;
    return ((const char*)s + 1);
}


int __StringRead(void *pPtr, int ch, int act) {
    char ret;
    __InStrCtrl* Iscp = (__InStrCtrl*)pPtr;

    switch (act) {
        case __GetAChar:
            ret = *(Iscp->NextChar);
            
            if (ret == '\0') {
                Iscp->NullCharDetected = 1;
                return -1;
            }
            else {
                Iscp->NextChar++;
                return (unsigned char)ret;
            }

        case __UngetAChar:
            if (Iscp->NullCharDetected == 0) {
                Iscp->NextChar--;
            }
            else {
                Iscp->NullCharDetected = 0;
            }

            return ch;

        case __TestForError:
            return Iscp->NullCharDetected;
    }

    return 0;
}

extern int __sformatter(int (*ReadProc)(void *, int, int), void *ReadProcArg, const char *pFormat, va_list arg, int is_secure);

inline int vsscanf(const char *s, const char *format, va_list arg) {
    __InStrCtrl isc;
    isc.NextChar = (char*)s;

    if ((s == 0) || (*isc.NextChar == '\0')) {
        return -1;
    }

    isc.NullCharDetected = 0;
    return __sformatter(&__StringRead, (void*)&isc, format, arg, 0);
}

int sscanf(const char *s, const char *pFormat, ...) {
    int ret;
    va_list args;
    va_start(args, pFormat);
    return vsscanf(s, pFormat, args);
}