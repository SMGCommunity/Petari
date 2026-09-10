#ifndef RVL_SDK_NWC24_DOWNLOAD_H
#define RVL_SDK_NWC24_DOWNLOAD_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/fs.h>
#include <revolution/nwc24/NWC24Types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_DL_TASK_MAX 120
#define NWC24i_DL_SUBTASK_MAX 32

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

#define NWC24_DL_TASK_PUBLIC_SIZE 0x200
typedef struct NWC24DlTask {
    u8 data[NWC24_DL_TASK_PUBLIC_SIZE];
} NWC24DlTask;

NWC24Err NWC24CheckDlTask(const NWC24DlTask* pTask, BOOL wantWrite);
NWC24Err NWC24DeleteDlTaskForced(NWC24DlTask* pTask);
NWC24Err NWC24GetDlTask(NWC24DlTask* pTask, u16 id);

#ifdef __cplusplus
}
#endif
#endif
