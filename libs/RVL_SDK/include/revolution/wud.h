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

typedef enum {
    WUD_STATE_SYNC_START = 0,
    WUD_STATE_SYNC_PREPARE_SEARCH = 1,
    WUD_STATE_SYNC_START_SEARCH = 2,
    WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT = 3,
    WUD_STATE_SYNC_CHECK_SEARCH_RESULT = 4,
    WUD_STATE_SYNC_IS_EXISTED_DEVICE = 5,
    WUD_STATE_SYNC_6 = 6,
    WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM = 7,
    WUD_STATE_SYNC_WAIT_FOR_STORING = 8,
    WUD_STATE_SYNC_WAIT_FOR_READING = 10,
    WUD_STATE_SYNC_11 = 11,
    WUD_STATE_SYNC_12 = 12,
    WUD_STATE_SYNC_13 = 13,
    WUD_STATE_SYNC_DONE = 14,
    WUD_STATE_SYNC_TRY_CONNECT = 15,
    WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE = 16,
    WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE = 17,
    WUD_STATE_SYNC_REGISTER_DEVICE = 18,
    WUD_STATE_SYNC_VIRGIN_SIMPLE = 19,
    WUD_STATE_SYNC_VIRGIN_STANDARD = 20,
    WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD = 21,
    WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND = 22,
    WUD_STATE_SYNC_COMPLETE = 23,
    WUD_STATE_SYNC_WAIT_FOR_INCOMING = 24,
    WUD_STATE_SYNC_SC_FLUSH = 25,
    WUD_STATE_SYNC_CANCEL_SEARCH = 26,
    WUD_STATE_SYNC_WAIT_FOR_START_SEARCH = 29,
    WUD_STATE_SYNC_ERROR = 255,
} WUDSyncState;

typedef enum {
    WUD_STATE_DELETE_START = 0,
    WUD_STATE_DELETE_DISALLOW_INCOMING = 1,
    WUD_STATE_DELETE_DISCONNECT_ALL = 2,
    WUD_STATE_DELETE_CLEANUP_DATABASE = 3,
    WUD_STATE_DELETE_CLEANUP_SETTING = 5,
    WUD_STATE_DELETE_6 = 6,
    WUD_STATE_DELETE_7 = 7,
    WUD_STATE_DELETE_DONE = 8
} WUDDeleteState;

typedef enum {
    WUD_RESULT_DELETE_BUSY = -1,
    WUD_RESULT_DELETE_WAITING,
    WUD_RESULT_DELETE_COMPLETE,
} WUDDeleteResult;

typedef enum {
    WUD_STATE_LINK_KEY_START = 0,
    WUD_STATE_LINK_KEY_READING = 1,
    WUD_STATE_LINK_KEY_WRITING = 2,
    WUD_STATE_LINK_KEY_DELETING = 3,
    WUD_STATE_LINK_KEY_ERROR = 255,
} WUDLinkKeyState;

typedef enum {
    WUD_STATE_STACK_START = 0,
    WUD_STATE_STACK_GET_STORED_LINK_KEY = 1,
    WUD_STATE_STACK_CHECK_DEVICE_INFO = 2,
    WUD_STATE_STACK_DONE = 3,
    WUD_STATE_STACK_INITIALIZED = 4,
    WUD_STATE_STACK_ERROR = 255,
} WUDStackState;

typedef enum {
    WUD_STATE_INIT_START = 0,
    WUD_STATE_INIT_WAIT_FOR_INITIALIZATION = 1,
    WUD_STATE_INIT_GET_DEV_INFO = 2,
    WUD_STATE_INIT_DONE = 3,
    WUD_STATE_INIT_INITIALIZED = 4,
    WUD_STATE_INIT_ERROR = 255,
} WUDInitState;

typedef enum {
    WUD_STATE_SHUTDOWN_START = 0,
    WUD_STATE_SHUTDOWN_STORE_SETTINGS = 1,
    WUD_STATE_SHUTDOWN_FLUSH_SETTINGS = 2,
    WUD_STATE_SHUTDOWN_DONE = 3,
    WUD_STATE_SHUTDOWN_ERROR = 255,
} WUDShutdownState;

typedef enum {
    WUD_VSE_INITIATE_PAIRING = 8,
    WUD_VSE_DELETE_ALL_KEYS,
    WUD_VSE_SI_PORT_STATUS,
    WUD_VSE_WATCH_DOG_RESET_HW = 16,
} WUDVendorSpecificEvent;

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

BOOL WUDInit();
BOOL WUDStartFastSyncSimple(void);
u32 WUDGetAllocatedMemSize(void);
WUDSyncDeviceCallback WUDSetSyncSimpleCallback(WUDSyncDeviceCallback);
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
u8 WUDGetBufferStatus(void);
u8 _WUDGetLinkNumber(void);
u16 _WUDGetQueuedSize(s8);
u16 _WUDGetNotAckedSize(s8);

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
