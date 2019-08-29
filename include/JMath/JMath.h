#ifndef JMATH_H
#define JMATH_H

#include "OS/vec.h"

#ifdef __cplusplus
extern "C" {
#endif

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);

#ifdef __cplusplus
}
#endif

#endif // JMATH_H