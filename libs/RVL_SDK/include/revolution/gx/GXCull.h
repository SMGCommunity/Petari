#ifndef GXCULL_H
#define GXCULL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

void GXSetCullMode(GXCullMode);
void GXSetCoPlanar(GXBool);

#ifdef __cplusplus
}
#endif

#endif // GXCULL_H