#ifndef VIFUNCS_H
#define VIFUNCS_H

#include <revolution.h>
#include <revolution/vi/vitypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void VIInit(void);

u32 VIGetTVFormat(void);

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback );

#ifdef __cplusplus
}
#endif

#endif // VIFUNCS_H