#ifndef WUD_H
#define WUD_H

#include "revolution.h"
#include "revolution/bte.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 BTD_NAME[64];
typedef u8 BTD_ADDR[6];
typedef u8 LINKKEY[16];

typedef struct WUDDeviceInfo {
    BTD_NAME bd_name;
    BTD_ADDR bd_addr;
    LINKKEY link_key;
    u8 handle;
    u8 sub_class;
    u8 app_id;
    u8 status;
    u8 overwrite;
    u8 sync_type;
    u8 reg;
    u16 attr_mask;
} WUDDeviceInfo;

typedef void* (*WUDAlloc)(u32);
typedef u8 (*WUDFree)(void*);
typedef void (*WUDSyncDeviceCallback)(s32, s32);
typedef void (*WUDClearDeviceCallback)(s32);
typedef void (*WUDHidRecvCallback)(u8, u8*, u16);
typedef void (*WUDHidConnCallback)(WUDDeviceInfo*, u8);
typedef void (*WUDStoredCallback)(void*);

typedef struct DeviceList {
    WUDDeviceInfo* info;
    void* prev;
    void* next;
} DeviceList;

typedef struct WUDControlBlock {
    WUDSyncDeviceCallback syncCallback;
    WUDSyncDeviceCallback syncSmpCallback;
    WUDClearDeviceCallback clearCallback;
    u8 syncStatus;
    u8 delStatus;
    u8 keyStatus;
    u8 profStatus;
    u8 initStatus;
    u8 shutStatus;
    u8 devNums;
    u8 devSmpNums;
    DeviceList* smpListHead;
    DeviceList* smpListTail;
    DeviceList smpList[6];
    DeviceList* stdListHead;
    DeviceList* stdListTail;
    DeviceList stdList[10];
    WUDDeviceInfo devInfo[10];
    WUDDeviceInfo devSmpInfo[6];
    u8 connNums;
    u8 linkNums;
    u8 pairNums;
    u8 syncFast;
    s8 syncLoop;
    u8 syncMode;
    u8 connectable;
    u8 discoverable;
    WUDHidRecvCallback hidRecvCallback;
    WUDHidConnCallback hidConnCallback;
    WUDAlloc alloc;
    WUDFree free;
    BTD_ADDR lastAddr;
    BTD_ADDR hostAddr;
    s8 status;
    u8 siStatus;
    u8 pmId;
    s8 rssi;
    OSAlarm evtAlarm;
    u32 profileMask;
    u16 aclAvailBufSize;
    u16 aclMaxBufSize;
    s16 delayCnt;
    s16 incomeCnt;
} WUDControlBlock;

void WUD_DEBUGPrint(const char*, ...);
void WUDiMoveTopSmpDevInfoPtr(WUDDeviceInfo*);
void WUDiMoveTopStdDevInfoPtr(WUDDeviceInfo*);
void WUDSetSniffMode(BTD_ADDR, s32);
void WUDiRemoveDevice(BTD_ADDR);
void WUDiMoveBottomSmpDevInfoPtr(WUDDeviceInfo*);
void WUDiMoveTopSmpDevInfoPtr(WUDDeviceInfo*);
void WUDiMoveBottomStdDevInfoPtr(WUDDeviceInfo*);
void WUDiMoveTopOfDisconnectedSmpDevice(WUDDeviceInfo*);
void WUDiMoveTopOfDisconnectedStdDevice(WUDDeviceInfo*);
WUDDeviceInfo* WUDiGetDevInfo(BTD_ADDR);

BOOL WUDCancelSyncDevice(void);
BOOL WUDStopSyncSimple(void);
s32 WUDGetStatus(void);
void WUDShutdown(BOOL);
void WUDSetVisibility(u8, u8);
BOOL WUDIsBusy(void);
WUDHidRecvCallback WUDSetHidRecvCallback(WUDHidRecvCallback);
WUDHidConnCallback WUDSetHidConnCallback(WUDHidConnCallback);
void WUDRegisterAllocator(WUDAlloc, WUDFree);
u8* _WUDGetDevAddr(u8);
BOOL WUDSetDisableChannel(s8);

extern WUDControlBlock _wcb;
extern WUDDeviceInfo _work;
extern u8* _dev_handle_to_bda[16];
extern u16 _dev_handle_queue_size[16];
extern u16 _dev_handle_notack_num[16];
extern SCBtDeviceInfoArray _scArray;

#ifdef __cplusplus
}
#endif

#endif  // WUD_H
