#ifndef VIFUNCS_H
#define VIFUNCS_H

#include <revolution.h>

#ifdef __cplusplus
extern "C" {
#endif

void VIInit(void);

u32 VIGetTVFormat(void);

#ifdef __cplusplus
}
#endif

#endif // VIFUNCS_H