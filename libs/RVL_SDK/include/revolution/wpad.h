#ifndef WPAD_H
#define WPAD_H

#include <revolution/os.h>
#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_CHAN0 0
#define WPAD_CHAN1 1
#define WPAD_CHAN2 2
#define WPAD_CHAN3 3

#define WPAD_STATE_DISABLED 0
#define WPAD_STATE_ENABLING 1
#define WPAD_STATE_ENABLED 2
#define WPAD_STATE_SETUP 3
#define WPAD_STATE_DISABLING 4

#define WPAD_MAX_CONTROLLERS 4
#define WPAD_DPD_MAX_OBJECTS 4

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

#define WPAD_FMT_CORE 0
#define WPAD_FMT_CORE_ACC 1
#define WPAD_FMT_CORE_ACC_DPD 2
#define WPAD_FMT_FREESTYLE 3
#define WPAD_FMT_FREESTYLE_ACC 4
#define WPAD_FMT_FREESTYLE_ACC_DPD 5
#define WPAD_FMT_CLASSIC 6
#define WPAD_FMT_CLASSIC_ACC 7
#define WPAD_FMT_CLASSIC_ACC_DPD 8
#define WPAD_FMT_CORE_ACC_DPD_FULL 9

#define WPAD_DPD_IMG_RESO_WX 1024
#define WPAD_DPD_IMG_RESO_WY 768

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

typedef struct DPDObject {
    s16 x;
    s16 y;
    u16 size;
    u8 traceId;
} DPDObject;

typedef struct DPDObjEx {
    s16 range_x1;
    s16 range_y1;
    s16 range_x2;
    s16 range_y2;
    u16 pixel;
    s8 radius;
} DPDObjEx;

typedef struct WPADStatus {
    u16 button;
    s16 accX;
    s16 accY;
    s16 accZ;
    DPDObject obj[4];
    u8 dev;
    s8 err;
} WPADStatus;

typedef struct WPADFSStatus {
    u16 button;
    s16 accX;
    s16 accY;
    s16 accZ;
    DPDObject obj[4];
    u8 dev;
    s8 err;
    s16 fsAccX;
    s16 fsAccY;
    s16 fsAccZ;
    s8 fsStickX;
    s8 fsStickY;
} WPADFSStatus;

typedef struct WPADCLStatus {
    u16 button;
    s16 accX;
    s16 accY;
    s16 accZ;
    DPDObject obj[4];
    u8 dev;
    s8 err;
    u16 clButton;
    s16 clLStickX;
    s16 clLStickY;
    s16 clRStickX;
    s16 clRStickY;
    u8 clTriggerL;
    u8 clTriggerR;
} WPADCLStatus;

typedef struct WPADAcc {
    s16 x;
    s16 y;
    s16 z;
} WPADAcc;

typedef void (*WPADExtensionCallback)(s32 chan, s32 result);
typedef void (*WPADConnectCallback)(s32 chan, s32 reason);
typedef void (*WPADCallback)(s32 chan, s32 result);
typedef void* (*WPADAlloc)(u32 size);
typedef u8 (*WPADFree)(void* ptr);
typedef void (*WPADSamplingCallback)(s32 chan);

typedef u32 WPADDeviceType;
typedef s32 WPADChannel;

void WPADRegisterAllocator(WPADAlloc, WPADFree);
u32 WPADGetWorkMemorySize(void);

void WPADDisconnect(s32);
s32 WPADGetInfoAsync(s32, WPADInfo*, WPADCallback);

BOOL WPADIsSpeakerEnabled(s32);
s32 WPADControlSpeaker(s32, u32, WPADCallback);
u8 WPADGetSpeakerVolume(void);
s32 WPADSendStreamData(s32, void*, u16);
BOOL WPADCanSendStreamData(s32);
void WPADGetAccGravityUnit(s32 chan, u32 type, WPADAcc* acc);
void WPADControlMotor(s32, u32);

BOOL WPADIsDpdEnabled(s32 chan);
s32 WPADControlDpd(s32 chan, u32 command, WPADCallback callback);
u32 WPADGetDataFormat(s32 chan);
s32 WPADSetDataFormat(s32 chan, u32 fmt);
void WPADRead(s32 chan, void* status);
void WPADSetAutoSleepTime(u8);
u8 WPADGetSensorBarPosition(void);
s32 WPADProbe(s32 chan, u32* type);
s32 WPADGetStatus(void);

void WPADInit(void);
u8 WPADGetDpdSensitivity(void);

WPADExtensionCallback WPADSetExtensionCallback(s32, WPADExtensionCallback);
WPADConnectCallback WPADSetConnectCallback(s32, WPADConnectCallback callback);
WPADSamplingCallback WPADSetSamplingCallback(s32 chan, WPADSamplingCallback callback);

#define WPADStartMotor(chan) WPADControlMotor((chan), WPAD_MOTOR_RUMBLE)
#define WPADStopMotor(chan) WPADControlMotor((chan), WPAD_MOTOR_STOP)

#ifdef __cplusplus
}
#endif

#endif  // WPAD_H
