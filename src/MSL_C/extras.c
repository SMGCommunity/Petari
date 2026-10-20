#include "cctype"
#include "size_t.h"

int stricmp(const char *s1, const char *s2) {
    char c1, c2;

    while (1) {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);

        if (c1 < c2) {
            return -1;
        }

        if (c1 > c2) {
            return 1;
        }

        if (c1 == 0) {
            return 0;
        }
    }
}

int strncasecmp(const char *s1, const char *s2, size_t n) {
    return __msl_strnicmp(s1, s2, n);
}

int strcasecmp(const char *s1, const char *s2) {
    return stricmp(s1, s2);
}
