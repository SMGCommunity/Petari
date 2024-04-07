#ifndef RVL_FACE_LIBRARY_SYSTEM_H
#define RVL_FACE_LIBRARY_SYSTEM_H
#include <RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 RFLGetWorkSize(BOOL deluxeTex);
RFLErrcode RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize,
                           BOOL deluxeTex);
RFLErrcode RFLInitRes(void* workBuffer, void* resBuffer, u32 resSize,
                      BOOL deluxeTex);
void RFLExit(void);
BOOL RFLAvailable(void);
RFLErrcode RFLGetAsyncStatus(void);
s32 RFLGetLastReason(void);
RFLErrcode RFLWaitAsync(void);

#ifdef __cplusplus
}
#endif
#endif
