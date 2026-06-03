#ifndef D_VF_H
#define D_VF_H

#include "revolution.h"

typedef void* VFFile;

typedef enum {
    VF_OK = 0x0000,
    VF_ERROR_0002 = 0x0002,
    VF_ERROR_0005 = 0x0005,
    VF_ERROR_B001 = 0xB001,
} VFError;

s32 VFIsAvailable();

#endif  // D_VF_H
