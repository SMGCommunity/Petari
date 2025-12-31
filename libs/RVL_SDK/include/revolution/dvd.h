#ifndef DVD_H
#define DVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/os.h>
#include <revolution/esp.h>
#include <stdbool.h>

typedef struct DVDDiskID DVDDiskID;

struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[14];
    u32 rvlMagic;
    u32 gcMagic;
};

typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);

struct DVDCommandBlock {
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
    u32 command;
    s32 state;
    u32 offset;
    u32 length;
    void* addr;
    u32 currTransferSize;
    u32 transferredSize;
    DVDDiskID* id;
    DVDCBCallback callback;
    void* userData;
};

typedef struct DVDFileInfo  DVDFileInfo;
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);
typedef void (*DVDLowCallback)(u32 intType);
typedef void (*DVDOptionalCommandChecker)(DVDCommandBlock* block, void (*cb)(u32));

struct DVDFileInfo {
    DVDCommandBlock cb;
    u32 startAddr;
    u32 length;
    DVDCallback callback;
};

typedef struct {
    u32 entryNum;
    u32 location;
    u32 next;
} DVDDir;

typedef struct {
    u32 entryNum;
    BOOL isDir;
    char* name;
} DVDDirEntry;

typedef struct DVDBB2 DVDBB2;

struct DVDBB2 {
    u32 bootFilePosition;
    u32 FSTPosition;
    u32 FSTLength;
    u32 FSTMaxLength;
    void* FSTAddress;
    u32 userPosition;
    u32 userLength;
    u32 padding0;
};

struct DVDDriveInfo {
    u16 revisionLevel;
    u16 deviceCode;
    u32 releaseDate;
    u8 pad[24];
};

typedef struct diRegVals {
    u32 ImmRegVal;
    u32 CoverRegVal;
    u32 pad[6];
} diRegVals_t;

typedef struct diCommand {
    u8 theCommand;
    u8 pad1[3];
    u32 arg[5];
    u32 pad2[2];
} diCommand_t;

typedef struct DVDVideoReportKey {
    u8 data[32];
} DVDVideoReportKey;

typedef struct DVDCommandInfo DVDCommandInfo;
struct DVDCommandInfo {
    u32 command;
    u32 offset;
    u32 length;
    u32 intType;
    u32 tick;
};

typedef struct DVDErrorInfo DVDErrorInfo;
struct DVDErrorInfo {
    char gameName[4];
    u8 diskNumber;
    u8 gameVersion;
    u8 reserved0[2];
    u32 error;
    u32 dateTime;
    u32 status;
    u8 reserved1[4];
    u32 nextOffset;
    DVDCommandInfo lastCommand[5];
};

typedef struct DVDGamePartition {
    ESTicket ticket;
    u32 tmdSize;
    ESTitleMeta* tmd;
    u32 certBlobSize;
    void* certBlob;      
    u8* h3Hashes;
    u8* encryptedArea;
} DVDGamePartition;

typedef struct DVDPartitionInfo {
    DVDGamePartition* gamePartition;
    u32 type;
} DVDPartitionInfo;

typedef struct DVDGameTOC {
    u32 numGamePartitions;
    DVDPartitionInfo* partitionInfos;
} DVDGameTOC;

typedef struct DVDPartitionParams  DVDPartitionParams;

struct DVDPartitionParams {
    ESTicket ticket;
    u8 padding0[OSRoundUp32B(sizeof(ESTicket)) - sizeof(ESTicket)];
    ESTicketView ticketView;
    u8 padding1[OSRoundUp32B(sizeof(ESTicketView)) - sizeof(ESTicketView)];
    u32 numTmdBytes;
    u8 padding2[28];
    ESTitleMeta tmd;
    u8 padding3[OSRoundUp32B(sizeof(ESTitleMeta)) - sizeof(ESTitleMeta)];
    u32 numCertBytes;
    u8 padding4[28];
    u8 certificates[4096];
    u32 dataWordOffset;
    u8 padding5[28];
    u8 h3Hash[98304];
};

typedef struct DVDDriveInfo DVDDriveInfo;

#define DVD_STATE_FATAL_ERROR     -1
#define DVD_STATE_END             0
#define DVD_STATE_BUSY            1
#define DVD_STATE_WAITING         2
#define DVD_STATE_COVER_CLOSED    3
#define DVD_STATE_NO_DISK         4
#define DVD_STATE_COVER_OPEN      5
#define DVD_STATE_WRONG_DISK      6
#define DVD_STATE_MOTOR_STOPPED   7
#define DVD_STATE_PAUSING         8
#define DVD_STATE_IGNORED         9
#define DVD_STATE_CANCELED        10
#define DVD_STATE_RETRY           11
#define DVD_STATE_NO_INPUT        12

void DVDInit(void);
BOOL DVDOpen(const char *, DVDFileInfo *);
BOOL DVDFastOpen(s32, DVDFileInfo *);

BOOL DVDOpenDir(const char *, DVDDir *);

s32 DVDCancel(DVDCommandBlock *);
BOOL DVDClose(DVDFileInfo *);

BOOL DVDReadDir(DVDDir *, DVDDirEntry *);
BOOL DVDGetCurrentDir(char *, u32 maxlen);
BOOL DVDCloseDir(DVDDir *);

BOOL DVDReadAbsAsyncPrio(DVDCommandBlock *, void *, s32, u32, DVDCBCallback, s32);

s32 DVDConvertPathToEntrynum(const char *);

BOOL DVDInquiryAsync(DVDCommandBlock *, DVDDriveInfo* , DVDCBCallback);

/* internal funcs */
BOOL __DVDCheckDevice(void);

BOOL DVDCompareDiskID(const DVDDiskID *, const DVDDiskID *);

void __DVDPrepareReset(void);
u32 __DVDGetCoverStatus(void);

/* dvdqueue */
void __DVDClearWaitingQueue(void);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(DVDCommandBlock *);
BOOL __DVDCheckWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32, DVDCommandBlock *);
DVDCommandBlock* __DVDGetNextWaitingQueue();

/* dvd_broadway */
bool DVDLowClearCoverInterrupt(DVDLowCallback);

u32 DVDLowGetImmBufferReg(void);
bool DVDLowRequestError(DVDLowCallback);
bool DVDLowUnencryptedRead(void*, u32, u32, DVDLowCallback);
bool DVDLowReportKey(DVDVideoReportKey *, u32, u32, DVDLowCallback);
bool DVDLowPrepareStatusRegister(DVDLowCallback);
bool DVDLowMaskCoverInterrupt(void);
bool DVDLowInit(void);
bool DVDLowUnmaskStatusInterrupts(void);
bool DVDLowRead(void *, u32, u32, DVDLowCallback);
bool DVDLowStopMotor(bool, bool, DVDLowCallback);
bool DVDLowSetSpinupFlag(u32);
bool DVDLowReset(DVDLowCallback);
bool DVDLowSeek(u32, DVDLowCallback);
bool DVDLowOpenPartitionWithTmdAndTicketView(const u32, const ESTicketView* const, const u32, const ESTitleMeta* const, const u32, const u8* const, DVDLowCallback);
bool DVDLowOpenPartition(const u32, const ESTicket* const, const u32, const u8* const, ESTitleMeta *, DVDLowCallback);
bool DVDLowPrepareCoverRegister(DVDLowCallback);
u32 DVDLowGetCoverRegister(void);
bool DVDLowReadDiskID(DVDDiskID *, DVDLowCallback);
bool DVDLowRequestAudioStatus(u32, DVDLowCallback);
bool DVDLowAudioStream(u32, u32, u32, DVDLowCallback);
bool DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);
bool DVDLowInquiry(DVDDriveInfo *, DVDLowCallback);
bool DVDLowClosePartition(DVDLowCallback);
bool DVDLowSetMaximumRotation(u32, DVDLowCallback);
bool DVDLowGetNoDiscBufferSizes(const u32, u32 *, u32 *, DVDLowCallback);
bool DVDLowGetNoDiscOpenPartitionParams(const u32, ESTicket *, u32 *, ESTitleMeta *, u32 *, u8 *, u32 *, u8 *, DVDLowCallback);

s32 DVDReadPrio(DVDFileInfo *, void *, s32, s32, s32);
BOOL DVDReadAsyncPrio(DVDFileInfo *, void *, s32, s32, DVDCallback, s32);

/* dvdFatal */
BOOL DVDSetAutoFatalMessaging(BOOL);
void __DVDPrintFatalMessage(void);
BOOL __DVDGetAutoFatalMessaging(void);

/* dvderror */
 void __DVDStoreErrorCode(u32, DVDCBCallback);

/* dvdfs */
void __DVDFSInit(void);

DVDDiskID *DVDGetCurrentDiskID(void);

BOOL DVDCheckDiskAsync(DVDCommandBlock *, DVDCBCallback);

void __DVDResetWithNoSpinup(void);

s32 DVDGetDriveStatus(void);

#define DVD_RESETCOVER_TIMELAG_TICKS2  OSMillisecondsToTicks(100)

#ifdef __cplusplus
}
#endif

#endif // DVD_H
