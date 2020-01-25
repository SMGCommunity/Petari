#include "revolution/os/OSCache.h"

void DCEnable()
{
    __asm
    {
        sync
        mfspr r3, hid0
        ori r3, r3, 0x4000
        mtspr hid0, r3
        blr
    }
}