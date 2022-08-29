#ifndef WPAD_H
#define WPAD_H

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WPADCallback)(s32, s32);

u32 WPADGetWorkMemorySize(void);

BOOL WPADIsSpeakerEnabled(s32);
s32 WPADControlSpeaker(s32, u32, WPADCallback);
u8 WPADGetSpeakerVolume(void);

#ifdef __cplusplus
}
#endif

#endif // WPAD_H