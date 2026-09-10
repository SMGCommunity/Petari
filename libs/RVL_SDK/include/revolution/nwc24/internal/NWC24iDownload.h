#ifndef RVL_SDK_NWC24_INTERNAL_DOWNLOAD_H
#define RVL_SDK_NWC24_INTERNAL_DOWNLOAD_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nand.h>
#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_DL_TASK_MAX 120
#define NWC24i_DL_SUBTASK_MAX 32

typedef struct NWC24iDlEntry {
    u32 app;
    u32 nextTime;
    u32 lastAccess;
    u8 flags;
    char UNK_0xD[0x10 - 0xD];
} NWC24iDlEntry;

typedef struct NWC24iDlHeader {
    u32 magic;
    u32 version;
    char UNK_0x8[0x10 - 0x8];
    u16 maxSubTasks;
    u16 privateTasks;
    u16 maxTasks;
    char UNK_0x16[0x80 - 0x16];
    NWC24iDlEntry entries[NWC24i_DL_TASK_MAX];
} __attribute__((packed)) NWC24iDlHeader;

typedef struct NWC24iDlTask {
    u16 id;
    u8 type;
    u8 priority;
    u32 flags;
    u32 appId;
    u32 titleIdHi;
    u32 titleIdLo;
    u16 groupId;
    char UNK_0x16[0x2];
    s16 count;
    s16 errorCount;
    u16 interval;
    u16 margin;
    u32 lastError;
    u8 subTaskCounter;
    u8 subTaskType;
    u8 subTaskFlags;
    char UNK_0x27[0x1];
    u32 subTaskMask;
    u32 serverInterval;
    u32 lastUpdate;
    u32 lastUpdateSubTask[NWC24i_DL_SUBTASK_MAX];
    char url[236];
    char fileName[NAND_MAX_PATH];
    char UNK_0x1E0[0x1F8 - 0x1E0];
    u32 userParam;
    u8 optFlags;
    u8 rootCaId;
    char UNK_0x1FE[0x200 - 0x1FE];
} __attribute__((packed)) NWC24iDlTask;

NWC24Err NWC24iOpenDlTaskList(void);
NWC24Err NWC24iCloseDlTaskList(void);
NWC24iDlHeader* NWC24iGetCachedDlHeader(void);
NWC24Err NWC24iCheckDlHeaderConsistency(NWC24iDlHeader* pHeader, BOOL clear) NO_INLINE;
NWC24Err NWC24iLoadDlHeader(void);

#ifdef __cplusplus
}
#endif
#endif
