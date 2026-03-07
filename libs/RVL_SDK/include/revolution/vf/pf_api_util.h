#ifndef PF_API_UTIL_H
#define PF_API_UTIL_H

#include <revolution.h>

u32 VFiPFAPI_ParseOpenModeString(const char*);
int VFiPFAPI_convertError(int);
int VFiPFAPI_convertReturnValue(int);
void* VFiPFAPI_convertReturnValue2NULL(int, void*);
int VFiPFAPI_convertReturnValue4unmount(int);

#endif  // PF_API_UTIL_H
