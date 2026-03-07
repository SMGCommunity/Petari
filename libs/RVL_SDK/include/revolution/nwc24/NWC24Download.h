#ifndef NWC24DOWNLOAD_H
#define NWC24DOWNLOAD_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NWC24_DL_TASK_MAX 120
#define NWC24_DL_SUBTASK_MAX 32

typedef enum { NWC24_DLTYPE_MULTIPART_V1, NWC24_DLTYPE_OCTETSTREAM_V1, NWC24_DLTYPE_MULTIPART_V2, NWC24_DLTYPE_OCTETSTREAM_V2 } NWC24DlType;

typedef enum {
    NWC24_DL_STTYPE_NONE,
    NWC24_DL_STTYPE_INCREMENT,
    NWC24_DL_STTYPE_TIME_HOUR,
    NWC24_DL_STTYPE_TIME_DAYOFWEEK,
    NWC24_DL_STTYPE_TIME_DAY
} NWC24DlSubTaskType;

typedef enum {
    NWC24_DL_STFLAG_TRAILING_FILENAME = (1 << 0),
    NWC24_DL_STFLAG_TRAILING_URL = (1 << 1),
    NWC24_DL_STFLAG_INTELLIGENT_UPDATE = (1 << 8),
    NWC24_DL_STFLAG_RETICENT_UPDATE = (1 << 9)
} NWC24DlSubTaskFlags;

typedef struct NWC24DlEntry {
    u32 app;         // at 0x0
    u32 nextTime;    // at 0x4
    u32 lastAccess;  // at 0x8
    u8 flags;        // at 0xC
    char UNK_0xD[0x10 - 0xD];
} NWC24DlEntry;

#pragma pack(push, 1)
typedef struct NWC24DlHeader {
    u32 magic;    // at 0x0
    u32 version;  // at 0x4
    char UNK_0x8[0x10 - 0x8];
    u16 maxSubTasks;   // at 0x10
    u16 privateTasks;  // at 0x12
    u16 maxTasks;      // at 0x14
    char UNK_0x16[0x80 - 0x16];
    NWC24DlEntry entries[NWC24_DL_TASK_MAX];  // at 0x80
} NWC24DlHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct NWC24DlTask {
    u16 id;         // at 0x0
    u8 type;        // at 0x2
    u8 priority;    // at 0x3
    u32 flags;      // at 0x4
    u32 appId;      // at 0x8
    u32 titleIdHi;  // at 0xC
    u32 titleIdLo;  // at 0x10
    u16 groupId;    // at 0x14
    char UNK_0x16[0x2];
    s16 count;          // at 0x18
    s16 errorCount;     // at 0x1A
    u16 interval;       // at 0x1C
    u16 margin;         // at 0x1E
    u32 lastError;      // at 0x20
    u8 subTaskCounter;  // at 0x24
    u8 subTaskType;     // at 0x25
    u8 subTaskFlags;    // at 0x26
    char UNK_0x27[0x1];
    u32 subTaskMask;                              // at 0x28
    u32 serverInterval;                           // at 0x2C
    u32 lastUpdate;                               // at 0x30
    u32 lastUpdateSubTask[NWC24_DL_SUBTASK_MAX];  // at 0x34
    char url[236];                                // at 0xB4
    char fileName[64];                            // at 0x1A0
    char UNK_0x1E0[0x1F8 - 0x1E0];
    u32 userParam;  // at 0x1F8
    u8 optFlags;    // at 0x1FC
    u8 rootCaId;    // at 0x1FD
    char UNK_0x1FE[0x200 - 0x1FE];
} NWC24DlTask;
#pragma pack(pop)

NWC24Err NWC24CheckDlTask(NWC24DlTask* task);
NWC24Err NWC24DeleteDlTaskForced(NWC24DlTask* task);
NWC24Err NWC24GetDlTask(NWC24DlTask* task, u16 i);
NWC24Err NWC24iOpenDlTaskList(void);
NWC24Err NWC24iCloseDlTaskList(void);
NWC24DlHeader* NWC24iGetCachedDlHeader(void);
NWC24Err NWC24iCheckHeaderConsistency(NWC24DlHeader* header, BOOL clear) NO_INLINE;
NWC24Err NWC24iLoadDlHeader(void);

#ifdef __cplusplus
}
#endif

#endif  // NWC24DOWNLOAD_H
