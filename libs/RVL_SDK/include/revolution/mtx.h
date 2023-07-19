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

typedef f32 Mtx23[2][3];

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];

/* C Matrix */
void C_MTXIdentity(Mtx);
void C_MTXCopy(const Mtx, Mtx);

/* Paired Single Matrix */
void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXMultVecSR(const Mtx, const Vec *, Vec *);

u32 PSMTXInverse(const Mtx, Mtx);

void PSMTXMultVec(const Mtx, const Vec *, Vec *);

void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);

void PSMTXRotAxisRad(Mtx dst, const Vec *, f32);

/* C Vector */

f32 C_VECMag(const Vec *);

/* Paired Single Vector */
void PSVECAdd(const Vec *, const Vec *, Vec *);
void PSVECSubtract(const Vec *, const Vec *, Vec *);
void PSVECScale(const Vec *, const Vec *, f32);
void PSVECNormalize(const Vec *, Vec *);

f32 PSVECMag(const Vec *);
f32 PSVECDotProduct(const Vec *, const Vec *);
void PSVECCrossProduct(const Vec *, const Vec *, Vec *);
f32 PSVECDistance(const Vec *, const Vec *);

/* SMG1 uses the paired single versions of matrix / vector operations */
#define VECNormalize    PSVECNormalize
#define VECMag          PSVECMag


#ifdef __cplusplus
}
#endif

#endif // MTX_H