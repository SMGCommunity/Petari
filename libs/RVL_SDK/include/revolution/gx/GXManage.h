#ifndef GXMANAGE_H
#define GXMANAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>
#include <revolution/gx/GXFifo.h>

typedef void (*GXDrawSyncCallback)(u16 token);
typedef void (*GXDrawDoneCallback)(void);

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback);

void GXFlush(void);
void GXPixModeSync(void);
void GXDrawDone(void);

#ifdef __cplusplus
}
#endif

#endif // GXMANAGE_H