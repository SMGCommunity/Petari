#ifndef DVD_H
#define DVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/os.h>
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

typedef struct DVDDriveInfo DVDDriveInfo;

void DVDInit(void);
BOOL DVDOpen(const char *, DVDFileInfo *);
BOOL DVDFastOpen(s32, DVDFileInfo *);

s32 DVDCancel(DVDCommandBlock *);
BOOL DVDClose(DVDFileInfo *);

BOOL DVDGetCurrentDir(char *, u32 maxlen);

BOOL DVDReadAbsAsyncPrio(DVDCommandBlock *, void *, s32, u32, DVDCBCallback, s32);

s32 DVDConvertPathToEntrynum(const char *);

BOOL DVDInquiryAsync(DVDCommandBlock *, DVDDriveInfo* , DVDCBCallback);

/* internal funcs */
BOOL __DVDCheckDevice(void);

/* dvdqueue */
void __DVDClearWaitingQueue(void);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(DVDCommandBlock *);

/* dvd_broadway */
bool DVDLowClearCoverInterrupt(DVDLowCallback);

u32 DVDLowGetImmBufferReg(void);
bool DVDLowRequestError(DVDLowCallback);
bool DVDLowUnencryptedRead(void*, u32, u32, DVDLowCallback);
bool DVDLowReportKey(DVDVideoReportKey *, u32, u32, DVDLowCallback);
bool DVDLowPrepareStatusRegister(DVDLowCallback);

#ifdef __cplusplus
}
#endif

#endif // DVD_H