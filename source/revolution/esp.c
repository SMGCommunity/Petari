#include "revolution/esp.h"
#include "revolution/ipc/ipcclt.h"

s32 ESP_InitLib()
{
    s32 ret = 0;

    if (__esFd < 0)
    {
        s32 fd = IOS_Open("/dev/es", 0);
        __esFd = fd;

        // IOS_Open returns negative on error
        if (fd < 0)
        {
            ret = fd;
        }
    }
    
    return ret;
}

s32 ESP_CloseLib()
{
    s32 ret = 0;

    if (__esFd >= 0)
    {
        s32 val = IOS_Close(__esFd);
        ret = val;

        // IOS_Close returns nonzero on failure
        if (val != 0)
        {
            __esFd = -1;
        }
    }

    return ret;
}