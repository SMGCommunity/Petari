#ifndef GXDISPLAYLIST_H
#define GXDISPLAYLIST_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void *, u32);
u32 GXEndDisplayList();
void GXCallDisplayList(const void *, u32);

#ifdef __cplusplus
}
#endif

#endif // GXDISPLAYLIST_H