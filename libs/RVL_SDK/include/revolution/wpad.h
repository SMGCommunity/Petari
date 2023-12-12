#ifndef WPAD_H
#define WPAD_H

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct WPADInfo {
    BOOL dpd;
    BOOL speaker;
    BOOL attach;
    BOOL lowBat;
    BOOL nearempty;
    u8 battery;
    u8 led;
    u8 protocol;
    u8 firmware;
} WPADInfo;


typedef void (*WPADCallback)(s32, s32);

u32 WPADGetWorkMemorySize(void);

BOOL WPADIsSpeakerEnabled(s32);
s32 WPADControlSpeaker(s32, u32, WPADCallback);
u8 WPADGetSpeakerVolume(void);

void WPADDisconnect(s32);

s32 WPADGetInfoAsync(s32, WPADInfo *, WPADCallback);

#ifdef __cplusplus
}
#endif

#endif // WPAD_H