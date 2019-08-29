#ifndef VEC_H
#define VEC_H

#include "types.h"
#include "JGeometry/TVec3.h"

typedef JGeometry::TVec3<f32> Vec;

#ifdef __cplusplus
extern "C" {
#endif

f32 C_VECMag(const Vec *);

#ifdef __cplusplus
}
#endif

#endif // VEC_H