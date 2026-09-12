#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

char* Mail_strcpy(char* pDst, const char* pSrc) {
    char* pOrigin = pDst;

    while (*pSrc != '\0') {
        *pDst++ = *pSrc++;
    }

    *pDst = '\0';
    return pOrigin;
}

int Mail_strlen(const char* pStr) {
    int len = 0;

    while (pStr[len] != '\0') {
        len++;
    }

    return len;
}

int STD_strnlen(const char* pStr, size_t maxlen) {
    size_t len = 0;

    for (; len < maxlen; len++) {
        if (pStr[len] == '\0') {
            break;
        }
    }

    return len;
}

void* Mail_memcpy(void* dst, const void* src, size_t n) {
    NETMemCpy(dst, src, n);
}

void* Mail_memset(void* dst, int ch, size_t n) {
    NETMemSet(dst, ch, n);
}

char* Mail_strcat(char* pDst, const char* pSrc) {
    Mail_strcpy(pDst + Mail_strlen(pDst), pSrc);
    return pDst;
}

char* Mail_strncat(char* pDst, const char* pSrc, size_t maxlen) {
    int len = Mail_strlen(pDst);
    int i;

    for (i = 0; i < maxlen && pSrc[i] != '\0'; i++) {
        pDst[len + i] = pSrc[i];
    }

    pDst[len + i] = '\0';
    return pDst;
}

static void set_to_head(char* pStr, char ch) {
    int i;

    for (i = Mail_strlen(pStr); i >= 0; i--) {
        pStr[i + 1] = pStr[i];
    }

    *pStr = ch;
}

static void set_to_tail(char* pStr, char ch) {
    int i = Mail_strlen(pStr);

    pStr[i++] = ch;
    pStr[i] = '\0';
}

static s32 convNum(char* pDst, u32 number, s32 numberBase, char charBase, BOOL isSigned, s32 width, char specifierChar, char justifyChar) {
    s32 charsWritten;
    s32 digitsWritten;
    char remainder;

    if (isSigned && (number & 0x80000000)) {
        isSigned = TRUE;
        number = -number;
    } else {
        isSigned = FALSE;
        number = number;
    }

    charsWritten = digitsWritten = 0;

    while (number != 0) {
        remainder = number % numberBase;
        number /= numberBase;

        set_to_head(pDst, remainder + (char)(remainder > 9 ? charBase : '0'));
        digitsWritten++;
        charsWritten++;
    }

    if (*pDst == '\0') {
        set_to_head(pDst, '0');
        digitsWritten++;
        charsWritten++;
    }

    if (specifierChar != '0') {
        specifierChar = ' ';
    }

    digitsWritten += isSigned;

    while (digitsWritten < width) {
        if (justifyChar == 'L') {
            set_to_tail(pDst, ' ');
            charsWritten++;
        } else {
            set_to_head(pDst, specifierChar);
            charsWritten++;
        }

        digitsWritten++;
    }

    if (isSigned) {
        set_to_head(pDst, '-');
    }

    return charsWritten;
}

int Mail_sprintf(char* pDst, char* pFmt, ...) {
    va_list argv;
    int ret;

    va_start(argv, pFmt);
    ret = Mail_vsprintf(pDst, pFmt, argv);
    va_end(argv);

    return ret;
}

int Mail_vsprintf(char* pDst, char* pFmt, va_list argv) {
    int charsWritten;
    BOOL isNumberFormat;
    BOOL isSigned;
    s32 stringLength;
    s32 numberBase;
    s32 width;
    char formatChar;
    char justifyChar;
    char specifierChar;
    char charBase;
    char* pStrArg;
    char longFlag;
    u32 number;

    *pDst = 0;
    charsWritten = 0;

    while ((formatChar = *pFmt++) != '\0') {
        while (*pDst != '\0') {
            pDst++;
        }

        if (formatChar == '%') {
            formatChar = *pFmt++;

            if (formatChar == '%') {
                set_to_tail(pDst++, formatChar);
                charsWritten++;
            } else {
                justifyChar = formatChar;
                if (formatChar == '-') {
                    formatChar = *pFmt++;
                }

                specifierChar = formatChar;
                isSigned = FALSE;
                numberBase = 10;
                charBase = '0';
                isNumberFormat = FALSE;

                if (formatChar == '*') {
                    width = va_arg(argv, s32);
                    formatChar = *pFmt++;
                } else {
                    width = 0;

                    while (formatChar >= '0' && formatChar <= '9') {
                        width = width * 10 + formatChar - '0';
                        formatChar = *pFmt++;
                    }
                }

                if ((longFlag = formatChar & (u8)~0x20) == 'L') {
                    formatChar = *pFmt++;
                }

                switch (formatChar) {
                case 'd': {
                    isNumberFormat = TRUE;
                    isSigned = TRUE;
                    break;
                }

                case 'o': {
                    isNumberFormat = TRUE;
                    numberBase = 8;
                    break;
                }

                case 'u': {
                    isNumberFormat = TRUE;
                    break;
                }

                case 'x': {
                    isNumberFormat = TRUE;
                    numberBase = 16;
                    charBase = 'a' - 10;
                    break;
                }

                case 'X': {
                    isNumberFormat = TRUE;
                    numberBase = 16;
                    charBase = 'A' - 10;
                    break;
                }

                case 'c': {
                    formatChar = va_arg(argv, s32);
                    set_to_tail(pDst++, formatChar);
                    charsWritten++;
                    break;
                }

                case 's': {
                    pStrArg = va_arg(argv, char*);

                    if (pStrArg) {
                        stringLength = Mail_strlen(pStrArg);
                        Mail_strcat(pDst, pStrArg);
                    } else {
                        stringLength = 0;
                    }

                    charsWritten += stringLength;

                    while (stringLength < width) {
                        charsWritten++;

                        if (justifyChar == '-') {
                            set_to_tail(pDst, ' ');
                        } else {
                            set_to_head(pDst, ' ');
                        }

                        stringLength++;
                    }
                    break;
                }
                }

                if (isNumberFormat) {
                    if (longFlag == 'L') {
                        number = va_arg(argv, u32);
                    } else if (isSigned) {
                        number = va_arg(argv, s32);
                    } else {
                        number = va_arg(argv, u32);
                    }

                    charsWritten += convNum(pDst, number, numberBase, charBase, isSigned, width, specifierChar, justifyChar);
                }
            }

        } else {
            set_to_tail(pDst++, formatChar);
            charsWritten++;
        }
    }

    return charsWritten;
}
