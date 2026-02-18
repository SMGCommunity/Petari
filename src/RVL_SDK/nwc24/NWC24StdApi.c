#include "revolution/nwc24/NWC24StdApi.h"
#include "revolution/net.h"
#include "revolution/nwc24.h"

char* Mail_strcpy(char* dst, const char* src) {
    char* backup = dst;

    while (*src != '\0') {
        *dst++ = *src++;
    };

    *dst = '\0';
    return backup;
}

size_t Mail_strlen(const char* str) {
    size_t len = 0;

    while (str[len] != '\0') {
        len++;
    }

    return len;
}

size_t STD_strnlen(const char* str, size_t n) {
    size_t len = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (str[len] == '\0') {
            break;
        }

        len++;
    }

    return len;
}

void* Mail_memcpy(void* dst, const void* src, size_t n) {
    NETMemCpy(dst, src, n);
}

void* Mail_memset(void* dst, int ch, size_t n) {
    NETMemSet(dst, ch, n);
}

char* Mail_strcat(char* dst, const char* src) {
    Mail_strcpy(dst + Mail_strlen(dst), src);
    return dst;
}

char* Mail_strncat(char* dst, const char* src, size_t n) {
    const size_t len = Mail_strlen(dst);
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dst[i + len] = src[i];
    };

    dst[(int)len + (int)i] = '\0';
    return dst;
}

inline static int Mail_strlen_inline(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

inline static void set_to_head(char* str, char c) {
    int len = Mail_strlen_inline(str);
    int i;
    for (i = len; i >= 0; i--) {
        str[i + 1] = str[i];
    }
    *str = c;
}

inline static void set_to_tail(char* str, char c) {
    int len;
    len = Mail_strlen(str);
    str[len++] = c;
    str[len] = '\0';
}

int convNum(char* dst, int number, int numberBase, char charBase, int signedFlag, int width, char specifierChar, char justifyChar) {
    unsigned int workingValue;
    int charsWritten;
    int digitcharsWritten;
    char remainder;
    char digitCharBase;
    char finalPadChar;
    BOOL isNegative;

    if ((signedFlag != 0) && (number & 0x80000000)) {
        isNegative = 1;
        workingValue = -number;
    } else {
        isNegative = 0;
        workingValue = number;
    }

    digitcharsWritten = 0;
    charsWritten = 0;

    while (workingValue != 0) {
        remainder = workingValue % numberBase;

        workingValue /= numberBase;
        digitCharBase = charBase;
        set_to_head(dst, remainder + ((char)((remainder <= 9) ? (0x30) : (digitCharBase))));
        digitcharsWritten++;
        charsWritten++;
    }

    if ((*dst) == '\0') {
        set_to_head(dst, '0');
        digitcharsWritten++;
        charsWritten++;
    }

    finalPadChar = specifierChar;
    if (finalPadChar != '0') {
        finalPadChar = ' ';
    }

    digitcharsWritten += isNegative;

    while (digitcharsWritten < width) {
        if (justifyChar == 'L') {
            set_to_tail(dst, ' ');
            charsWritten++;
        } else {
            set_to_head(dst, finalPadChar);
            charsWritten++;
        }
        digitcharsWritten++;
    }

    if (isNegative) {
        set_to_head(dst, '-');
    }

    return charsWritten;
}

int Mail_sprintf(char* buffer, const char* format, ...) {
    va_list args;
    int ret;

    va_start(args, format);
    ret = Mail_vsprintf(buffer, format, args);
    va_end(args);

    return ret;
}

int Mail_vsprintf(char* str, char* format, va_list arg) {
    s32 charsWritten;
    BOOL isNumberFormat;
    BOOL signedFlag;
    s32 stringLength;
    s32 numberBase;
    s32 width;
    char formatChar;
    char justifyChar;
    char specifierChar;
    char charBase;
    char* stringArg;
    char longFlag;
    u32 number;

    *str = 0;
    charsWritten = 0;

    while ((formatChar = *format++) != 0) {
        while (*str) {
            str++;
        }

        if (formatChar == '%') {
            (formatChar = *format++);
            if (formatChar == '%') {
                set_to_tail(str++, formatChar);
                charsWritten++;
            } else {
                justifyChar = formatChar;
                if (formatChar == '-') {
                    formatChar = *format++;
                }

                specifierChar = formatChar;
                signedFlag = FALSE;
                numberBase = 10;
                charBase = '0';
                isNumberFormat = FALSE;

                if (formatChar == '*') {
                    width = va_arg(arg, int);
                    formatChar = *format++;
                } else {
                    width = 0;
                    while ((formatChar >= '0') && (formatChar <= '9')) {
                        width = width * 10 + formatChar - '0';
                        formatChar = *format++;
                    }
                }

                longFlag = formatChar & 0xDF;
                if (longFlag == 'L') {
                    formatChar = *format++;
                }

                switch (formatChar) {
                case 'd':
                    isNumberFormat = TRUE;
                    signedFlag = TRUE;
                    break;
                case 'o':
                    isNumberFormat = TRUE;
                    numberBase = 8;
                    break;
                case 'u':
                    isNumberFormat = TRUE;
                    break;
                case 'x':
                    isNumberFormat = TRUE;
                    numberBase = 16;
                    charBase = 'a' - 10;
                    break;
                case 'X':
                    isNumberFormat = TRUE;
                    numberBase = 16;
                    charBase = 'A' - 10;
                    break;
                case 'c':
                    formatChar = va_arg(arg, s32);
                    set_to_tail(str++, formatChar);
                    charsWritten++;
                    break;
                case 's':
                    stringArg = va_arg(arg, char*);
                    if (stringArg) {
                        stringLength = Mail_strlen(stringArg);
                        Mail_strcat(str, stringArg);
                    } else {
                        stringLength = 0;
                    }
                    charsWritten += stringLength;
                    while (stringLength < width) {
                        charsWritten++;
                        if (justifyChar == '-') {
                            set_to_tail(str, ' ');
                        } else {
                            set_to_head(str, ' ');
                        }
                        stringLength++;
                    }
                    break;
                }

                if (isNumberFormat) {
                    if (longFlag == 'L') {
                        number = va_arg(arg, u32);
                    } else if (signedFlag) {
                        number = va_arg(arg, s32);
                    } else {
                        number = va_arg(arg, u32);
                    }
                    charsWritten += convNum(str, number, numberBase, charBase, signedFlag, width, specifierChar, justifyChar);
                }
            }
        } else {
            set_to_tail(str++, formatChar);
            charsWritten++;
        }
    }
    return charsWritten;
}
