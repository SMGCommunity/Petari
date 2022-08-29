#ifndef GXVERT_H
#define GXVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/base/PPCWGPipe.h>

volatile PPCWGPipe GXWGFifo : 0xCC008000;

#ifdef __cplusplus
}
#endif

#endif // GXVERT_H