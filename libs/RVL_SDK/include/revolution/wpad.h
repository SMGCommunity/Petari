#ifndef WPAD_H
#define WPAD_H

#include <revolution/os.h>
#include <revolution/sc.h>
#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WPADExtensionCallback)(s32 chan, s32 result);
typedef void (*WPADConnectCallback)(s32 chan, s32 reason);
typedef void (*WPADCallback)(s32 chan, s32 result);
typedef void* (*WPADAlloc)(u32 size);
typedef int (*WPADFree)(void* ptr);
typedef void (*WPADSyncDeviceCallback)(s32 result, s32 num);
typedef void (*WPADSamplingCallback)(s32 chan);
typedef void (*WPADFlushCallback)(SCStatus result);

typedef u32 WPADDeviceType;
typedef s32 WPADChannel;

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

#define WPAD_CL_BUTTON_UP 0x0001
#define WPAD_CL_BUTTON_LEFT 0x0002
#define WPAD_CL_TRIGGER_ZR 0x0004
#define WPAD_CL_BUTTON_X 0x0008
#define WPAD_CL_BUTTON_A 0x0010
#define WPAD_CL_BUTTON_Y 0x0020
#define WPAD_CL_BUTTON_B 0x0040
#define WPAD_CL_TRIGGER_ZL 0x0080
#define WPAD_CL_RESERVED 0x0100
#define WPAD_CL_TRIGGER_R 0x0200
#define WPAD_CL_BUTTON_PLUS 0x0400
#define WPAD_CL_BUTTON_HOME 0x0800
#define WPAD_CL_BUTTON_MINUS 0x1000
#define WPAD_CL_TRIGGER_L 0x2000
#define WPAD_CL_BUTTON_DOWN 0x4000
#define WPAD_CL_BUTTON_RIGHT 0x8000

#define WPAD_DPD_MAX_OBJECTS 4
#define WPAD_DPD_IMG_RESO_WX 1024
#define WPAD_DPD_IMG_RESO_WY 768

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

#define WPAD_STATE_DISABLED 0
#define WPAD_STATE_ENABLING 1
#define WPAD_STATE_ENABLED 2
#define WPAD_STATE_SETUP 3
#define WPAD_STATE_DISABLING 4

#define WPAD_BATTERY_LEVEL_CRITICAL 0
#define WPAD_BATTERY_LEVEL_LOW 1
#define WPAD_BATTERY_LEVEL_MEDIUM 2
#define WPAD_BATTERY_LEVEL_HIGH 3
#define WPAD_BATTERY_LEVEL_MAX 4

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

#define WPAD_RX_DATASIZE 96
#define WPAD_COMMAND_CMD_MAX_LEN 24
#define WPAD_COMMAND_EXT_MAX_LEN 12
#define WPAD_COMMAND_BUF_LEN 22

#define WPAD_HIDREP_BASE 0x10
#define WPAD_HIDREP_VIBRATOR 0x10
#define WPAD_HIDREP_PORT 0x11
#define WPAD_HIDREP_REPTYPE 0x12
#define WPAD_HIDREP_ENDPD 0x13
#define WPAD_HIDREP_ENSPK 0x14
#define WPAD_HIDREP_GETSTAT 0x15
#define WPAD_HIDREP_WRDATA 0x16
#define WPAD_HIDREP_RDDATA 0x17
#define WPAD_HIDREP_STRM 0x18
#define WPAD_HIDREP_MUTE 0x19
#define WPAD_HIDREP_DPDCSB 0x1A
#define WPAD_HIDREP_WAIT 0xFF

#define WPAD_I2CDEV_SEL_MASK (u32)(0xFF000000)
#define WPAD_I2CDEV_DADR_MASK (u32)(0x00FF0000)
#define WPAD_I2CDEV_UADR_MASK (u32)(0x0000FF00)
#define WPAD_I2CDEV_LADR_MASK (u32)(0x000000FF)
#define WPAD_WRITE_LEN_MASK (u16)(0x001F)
#define WPAD_READ_ULEN_MASK (u16)(0xFF00)
#define WPAD_READ_LLEN_MASK (u16)(0x00FF)

#define WPAD_BTN_HCURSOR_MASK (WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT)
#define WPAD_BTN_VCURSOR_MASK (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN)
#define WPAD_CLB_HCURSOR_MASK (WPAD_CL_BUTTON_LEFT | WPAD_CL_BUTTON_RIGHT)
#define WPAD_CLB_VCURSOR_MASK (WPAD_CL_BUTTON_UP | WPAD_CL_BUTTON_DOWN)

#define SPK_CLK_REG 0x04a20009
#define SPK_CLK_ON 1
#define SPK_CLK_OFF 0
#define SPK_RST_REG 0x04a20001
#define SPK_DERESET 0x01
#define SPK_RESET 0x80
#define SPK_CTRL_REG 0x04a20008
#define SPK_CTRL_PLAY 1

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

typedef struct WPADStatusEx {
    u16 button;
    s16 accX;
    s16 accY;
    s16 accZ;
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];
    u8 dev;
    s8 err;
    DPDObjEx exp[WPAD_DPD_MAX_OBJECTS];

} WPADStatusEx;

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

typedef struct WPADMEMGameInfo {
    OSTime calendarTimeTick;
    u16 gameTitle[17];
    char gameId[4];
    u8 gameType;
    u8 checkSum;
    u8 reserved[8];
} WPADMEMGameInfo;

typedef struct WPADCommand {
    u32 command;
    u8 data[22];
    u16 len;
    void* readBuf;
    u16 readLen;
    u32 readAddr;
    WPADInfo* info;
    WPADCallback callback;
} WPADCommand;

typedef struct WPADCmdQueue {
    s8 head;
    s8 tail;
    WPADCommand* cmd;
    u32 cmdlen;
} WPADCmdQueue;

typedef struct WPADConfig {
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];
    WPADAcc acc_0g;
    WPADAcc acc_1g;
    u8 motor;
    u8 volume;
} WPADConfig;

typedef struct WPADStick {
    s16 x;
    s16 x_min;
    s16 x_max;
    s16 y;
    s16 y_min;
    s16 y_max;
} WPADStick;

typedef struct WPADFsConfig {
    WPADStick stick;
    WPADAcc acc_0g;
    WPADAcc acc_1g;
} WPADFsConfig;

typedef struct WPADClConfig {
    WPADStick lstk;
    WPADStick rstk;
    u8 triggerL;
    u8 triggerR;
} WPADClConfig;

typedef struct WPADExtConfig {
    union {
        WPADFsConfig fs;
        WPADClConfig cl;
    };
} WPADExtConfig;

typedef struct WPADControlBlock {
    WPADMEMGameInfo gameInfo;
    s32 gameInfoErr[2];
    u8 prBuf[WPAD_RX_DATASIZE] ATTRIBUTE_ALIGN(32);
    u8 rxBuf[WPAD_RX_DATASIZE * 2] ATTRIBUTE_ALIGN(32);
    WPADCmdQueue cmdq;
    WPADCommand cmds[WPAD_COMMAND_CMD_MAX_LEN];
    WPADCmdQueue extq;
    WPADCommand exts[WPAD_COMMAND_EXT_MAX_LEN];
    WPADInfo infos;
    WPADInfo* infoBuf;
    WPADConfig conf;
    WPADExtConfig extConf;
    WPADCallback resultCallback;
    WPADExtensionCallback extensionCallback;
    WPADConnectCallback connectCallback;
    WPADSamplingCallback sampleCallback;
    void* samplingBufs_ptr;
    u32 bufLatestIndex;
    u32 bufLength;
    u32 dataFormat;
    s32 status;
    u8 getStatFlag;
    u8 devType;
    u8 devMode;
    s8 dev_handle;
    WPADCallback reqVolCb;
    u8 rxBufIndex;
    u8 reqVolume;
    u8 dpdDummyObjSize;
    u8 dpdFormat;
    u8 dpdCommand;
    u8 audioStop;
    u8 audioStopCnt;
    u8 audioFrames;
    BOOL rumble;
    BOOL motor;
    BOOL used;
    BOOL setup;
    BOOL oldFw;
    OSThreadQueue threadq;
    OSTime lastUpdateTime;
    u16 diffCountDpd;
    u16 hystCountDpd;
    u16 diffCountAcc;
    u16 hystCountAcc;
    u16 diffCountfsAcc;
    u16 hystCountfsAcc;
    OSTime cmdTimer;
    u8 cmdTimeoutAction;
    u8 isSetStickOrigin;
    u16 recalibrateCount;
    u8 key[16];
    u8 ft[8];
    u8 sb[8];
    u8 readBuf[64];
    void* readBufPtr;
    u32 readBaseAddr;
    s32 readError;
    u16 readLength;
    u8 keyIdx;
    u8 radioSense;
    u16 packetCnt;
    u8 disconnect;
    u8 cmdId;
    WPADCallback infoCallback;
    u8 infoLocked;
    u8 extEnc;
    u8 pwrSave;
    u8 padding2;
} WPADControlBlock;

typedef struct WPADMEMControlBlock {
    BOOL used;
    u8* p_buf;
    u16 len;
    u32 addr;
    WPADCallback callback;
} WPADMEMControlBlock;

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
BOOL WPADStopSimpleSync(void);

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

void WPADRegisterAllocator(WPADAlloc, WPADFree);

WPADExtensionCallback WPADSetExtensionCallback(s32, WPADExtensionCallback);
WPADConnectCallback WPADSetConnectCallback(s32, WPADConnectCallback callback);
WPADSamplingCallback WPADSetSamplingCallback(s32 chan, WPADSamplingCallback callback);

void WPADiCreateKey(s32);
void WPADiCreateKeyFor3rd(s32);
void WPADiDecode(s32, void*, u16, u16);
s32 WPADiHIDParser(u8, u8*);

void WPADiClearQueue(WPADCmdQueue*);
BOOL WPADiSendSetReportType(WPADCmdQueue*, u32, BOOL, WPADCallback);
BOOL WPADiSendWriteDataCmd(WPADCmdQueue*, u8, u32, WPADCallback);
BOOL WPADiSendReadData(WPADCmdQueue*, void*, u16, u32, WPADCallback);
BOOL WPADiSendWriteData(WPADCmdQueue*, void*, u16, u32, WPADCallback);

void WPADiCopyOut(s32);
void WPADiExcludeButton(s32);
s32 WPADiGetStatus(s32);

void __WPADShutdown(void);
void __WPADReconnect(BOOL);

BOOL WPADStopSimpleSync();
BOOL WPADCancelSyncDevice();

void WPADiShutdown(BOOL);
void WPADiDisconnect(s32, BOOL);

s32 WPADControlLed(s32, u8, WPADCallback);
void WPADGetAddress(s32, u8*);

BOOL WPADiSendSetPort(WPADCmdQueue*, u8, WPADCallback);
BOOL WPADiSendDPDCSB(WPADCmdQueue*, BOOL, WPADCallback);
BOOL WPADiSendGetContStat(WPADCmdQueue*, WPADInfo*, WPADCallback);

s32 WPADiRetrieveChannel(u8);
s32 WPADiSendData(s32, WPADCommand);

BOOL WPADiGetCommand(WPADCmdQueue*, WPADCommand*);
BOOL WPADiPopCommand(WPADCmdQueue*);
BOOL WPADiPushCommand(WPADCmdQueue*, WPADCommand);

void WPADSetAutoSamplingBuf(s32, void*, u32);

#define WPADStartMotor(chan) WPADControlMotor((chan), WPAD_MOTOR_RUMBLE)
#define WPADStopMotor(chan) WPADControlMotor((chan), WPAD_MOTOR_STOP)

extern WPADMEMControlBlock _wmb[WPAD_MAX_CONTROLLERS];
extern WPADControlBlock* _wpdcb[WPAD_MAX_CONTROLLERS];

#ifdef __cplusplus
}
#endif

#endif  // WPAD_H
