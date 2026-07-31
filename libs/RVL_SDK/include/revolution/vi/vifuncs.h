#ifndef VIFUNCS_H
#define VIFUNCS_H

#include <revolution/types.h>
#include <revolution/vi/vitypes.h>
#include <revolution/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _VITimeToDIM { VI_DM_DEFAULT = 0, VI_DM_10M, VI_DM_15M } VITimeToDIM;

void VIInit(void);
void VIFlush();
void VIWaitForRetrace();
void VIConfigure(const GXRenderModeObj*);
void VIConfigurePan(u16, u16, u16, u16);
void VISetNextFrameBuffer(void*);
void* VIGetNextFrameBuffer(void);
void* VIGetCurrentFrameBuffer(void);
VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback);
void VISetBlack(BOOL);
u32 VIGetRetraceCount(void);
u32 VIGetCurrentLine(void);
u32 VIGetTvFormat(void);
u32 VIGetScanMode(void);
u32 VIGetDTVStatus(void);
BOOL VIEnableDimming(BOOL);
u32 VIGetDimmingCount(void);
BOOL VIResetDimmingCount();

#ifdef __cplusplus
}
#endif

#endif // VIFUNCS_H
