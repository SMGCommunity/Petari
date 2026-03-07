#include "revolution/nwc24.h"

void NWC24InitBase64Table(char* codes) {
    int i;
    for (i = 0; i < 256; i++)
        codes[i] = (char)0xFF;
    for (i = 'A'; i <= 'Z'; i++)
        codes[i] = (char)(i - 'A');
    for (i = 'a'; i <= 'z'; i++)
        codes[i] = (char)(26 + i - 'a');
    for (i = '0'; i <= '9'; i++)
        codes[i] = (char)(52 + i - '0');
    codes['+'] = 62;
    codes['/'] = 63;

    return;
}
