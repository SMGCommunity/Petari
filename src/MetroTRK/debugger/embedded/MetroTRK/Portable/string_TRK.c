#include "TRK_Types.h"
#include "size_t.h"

size_t TRK_strlen(const s8* str) {
    size_t len = 0xFFFFFFFF;

    u8 *p = (u8 *)(str - 1);
    do
        len++;
    while (*++p);

    return len;
}