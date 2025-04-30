#ifndef WPAD_H
#define WPAD_H

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WPADCallback)(s32, s32);
typedef void* (*WPADAlloc)(u32);
typedef u8 (*WPADFree)(void*);

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

typedef enum {
    WPAD_RESULT_ERR_3 = -3,
    WPAD_RESULT_ERR_2,
    WPAD_RESULT_ERR_1,

    WPAD_RESULT_SUCCESS = 0,
} WPADResult;

void WPADRegisterAllocator(WPADAlloc, WPADFree);
u32 WPADGetWorkMemorySize(void);

BOOL WPADIsSpeakerEnabled(s32);
s32 WPADControlSpeaker(s32, u32, WPADCallback);
u8 WPADGetSpeakerVolume(void);

u8 WPADGetSensorBarPosition(void);

void WPADDisconnect(s32);

s32 WPADGetInfoAsync(s32, WPADInfo *, WPADCallback);

void WPADSetAutoSleepTime(u8);
BOOL WPADCanSendStreamData(s32);
s32 WPADSendStreamData(s32, void *, u16);

void WPADControlMotor(s32, u32);

#ifdef __cplusplus
}
#endif

#endif // WPAD_H
