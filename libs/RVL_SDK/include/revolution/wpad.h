#ifndef WPAD_H
#define WPAD_H

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_CHAN0 0
#define WPAD_CHAN1 1
#define WPAD_CHAN2 2
#define WPAD_CHAN3 3

#define WPAD_MAX_CONTROLLERS 4

#define WPAD_MOTOR_STOP 0
#define WPAD_MOTOR_RUMBLE 1

#define WPAD_BUTTON_LEFT 0x0001
#define WPAD_BUTTON_RIGHT 0x0002
#define WPAD_BUTTON_DOWN 0x0004
#define WPAD_BUTTON_UP 0x0008
#define WPAD_BUTTON_PLUS 0x0010
#define WPAD_BUTTON_2 0x0100
#define WPAD_BUTTON_1 0x0200
#define WPAD_BUTTON_B 0x0400
#define WPAD_BUTTON_A 0x0800
#define WPAD_BUTTON_MINUS 0x1000
#define WPAD_BUTTON_Z 0x2000
#define WPAD_BUTTON_C 0x4000
#define WPAD_BUTTON_HOME 0x8000

#define WPAD_DEV_CORE 0
#define WPAD_DEV_FREESTYLE 1
#define WPAD_DEV_CLASSIC 2
#define WPAD_DEV_MPLS 5
#define WPAD_DEV_MPLS_FREESTYLE 6
#define WPAD_DEV_MPLS_CLASSIC 7
#define WPAD_DEV_MPLS_FUTURE 250
#define WPAD_DEV_FUTURE 251
#define WPAD_DEV_NOT_SUPPORTED 252
#define WPAD_DEV_NOT_FOUND 253
#define WPAD_DEV_UNKNOWN 255

#define WPAD_SENSOR_BAR_POS_BOTTOM 0
#define WPAD_SENSOR_BAR_POS_TOP 1

#define WPAD_ERR_NONE 0
#define WPAD_ERR_NO_CONTROLLER -1
#define WPAD_ERR_BUSY -2
#define WPAD_ERR_TRANSFER -3
#define WPAD_ERR_INVALID -4
#define WPAD_ERR_NOPERM -5
#define WPAD_ERR_BROKEN -6
#define WPAD_ERR_CORRUPTED -7

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

typedef void (*WPADExtensionCallback)(s32 chan, s32 result);
typedef void (*WPADConnectCallback)(s32 chan, s32 reason);
typedef void (*WPADCallback)(s32 chan, s32 result);
typedef void* (*WPADAlloc)(u32 size);
typedef u8 (*WPADFree)(void *ptr);

void WPADRegisterAllocator(WPADAlloc, WPADFree);
u32 WPADGetWorkMemorySize(void);

void WPADDisconnect(s32);
s32 WPADGetInfoAsync(s32, WPADInfo *, WPADCallback);

BOOL WPADIsSpeakerEnabled(s32);
s32 WPADControlSpeaker(s32, u32, WPADCallback);
u8 WPADGetSpeakerVolume(void);
s32 WPADSendStreamData(s32, void *, u16);
BOOL WPADCanSendStreamData(s32);

void WPADControlMotor(s32, u32);

void WPADSetAutoSleepTime(u8);
u8 WPADGetSensorBarPosition(void);

WPADExtensionCallback WPADSetExtensionCallback(s32, WPADExtensionCallback);
WPADConnectCallback WPADSetConnectCallback(s32, WPADConnectCallback callback);

#ifdef __cplusplus
}
#endif

#endif // WPAD_H
