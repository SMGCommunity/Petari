#ifndef MTX_H
#define MTX_H

#include "types.h"

const static f32 mtx_zero = 0.0f;
const static f32 mtx_one_half = 0.5f;
const static f32 mtx_one = 1.0f;

#ifdef __cplusplus
extern "C" {
#endif

void PSMTXIdentity(Mtx matrix);
void PSMTXCopy(Mtx src, Mtx dest);
u32 PSMTXInverse(Mtx src, Mtx dest);
u32 PSMTXInvXpose(Mtx src, Mtx dest);

void PSMTXRotTrig(Mtx matrix, s8 axis, f32 sin, f32 cos);

#ifdef __cplusplus
}
#endif

#endif // MTX_H