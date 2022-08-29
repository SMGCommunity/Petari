#ifndef MTX_H
#define MTX_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    f32 x, y, z;
} Vec, *VecPtr;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} S16Vec, *S16VecPtr;

typedef struct {
    f32 x, y, z, w;
} Quaternion, *QuaternionPtr;

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];

void C_MTXIdentity(Mtx);
void C_MTXCopy(const Mtx, Mtx);

void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void MSMTXConcat(const Mtx, const Mtx, Mtx);

#ifdef __cplusplus
}
#endif

#endif // MTX_H