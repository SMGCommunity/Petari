#include <revolution/esp.h>
#include <revolution/ipc/ipcclt.h>
#include <private/iosrestypes.h>

static s32 __esFd = -1;

s32 ESP_InitLib(void) {
    s32 ret = 0;

    if (__esFd < 0) {
        __esFd = IOS_Open("/dev/es", 0);

        if (__esFd < 0) {
            ret = __esFd;
        }
    }

    return ret;
}

s32 ESP_CloseLib(void) {
    s32 ret = 0;

    if (__esFd >= 0) {
        ret = IOS_Close(__esFd);
        if (ret == 0) {
            __esFd = -1;
        }
    }

    return ret;
} 

s32 ESP_LaunchTitle(u64 titleID, ESTicketView *pTicketView) {
    s32 ret = 0;
    // buffer being aligned is required for codegen
    u8 buf[256] __attribute__ ((aligned(32)));
    IOSIoVector* vec = (IOSIoVector *)(buf + 208);
    u64* id = (u64*)buf;

    if (__esFd < 0) {
        return -0x3F9;
    }
    
    // check if ptr is aligned to 0x20
    if ((u32)pTicketView & 31) {
        return -0x3F9;
    }

    *id = titleID;
    // this seems to be two vector instances in an array, considering they are accessed via stack at 0xF0, 0xF4, 0xF8, and 0xFC
    vec[0].base = (u8*)id;
    vec[0].length = 8;
    vec[1].base = (u8*)pTicketView;
    vec[1].length = 0xD8;

    ret = IOS_IoctlvReboot(__esFd, 8, 2, 0, vec);
    __esFd = -1;
    return ret;
}