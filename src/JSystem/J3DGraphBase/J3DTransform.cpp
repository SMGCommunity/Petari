#include "JSystem/J3DGraphBase/J3DTransform.hpp"
#include "JSystem/JMath.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include <revolution/mtx.h>

void J3DCalcYBBoardMtx(Mtx mtx) {
    f32 x = (mtx[0][0] * mtx[0][0]) + (mtx[1][0] * mtx[1][0]) + (mtx[2][0] * mtx[2][0]);
    f32 z = (mtx[0][2] * mtx[0][2]) + (mtx[1][2] * mtx[1][2]) + (mtx[2][2] * mtx[2][2]);

    if (x > 0.0f) {
        x = JMath::fastSqrt(x);
    }
    if (z > 0.0f) {
        z = JMath::fastSqrt(z);
    }

    Vec vec = {0.0f, -mtx[2][1], mtx[1][1]};
    PSVECNormalize(&vec, &vec);

    mtx[0][0] = x;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;

    mtx[1][2] = vec.y * z;
    mtx[2][0] = 0.0f;
    mtx[2][2] = vec.z * z;
}

asm void J3DPSCalcInverseTranspose(__REGISTER Mtx src, __REGISTER Mtx33 dst) {
#ifdef __MWERKS__  // clang-format off
	psq_l    f0, 0(src), 1, 0
	psq_l    f1, 4(src), 0, 0
	psq_l    f2, 16(src), 1, 0
	ps_merge10 f6, f1, f0
	psq_l    f3, 20(src), 0, 0
	psq_l    f4, 32(src), 1, 0
	ps_merge10 f7, f3, f2
	psq_l    f5, 36(src), 0, 0
	ps_mul   f11, f3, f6
	ps_merge10 f8, f5, f4
	ps_mul   f13, f5, f7
	ps_msub  f11, f1, f7, f11
	ps_mul   f12, f1, f8
	ps_msub  f13, f3, f8, f13
	ps_msub  f12, f5, f6, f12
	ps_mul   f10, f3, f4
	ps_mul   f9, f0, f5
	ps_mul   f8, f1, f2
	ps_msub  f10, f2, f5, f10
	ps_msub  f9, f1, f4, f9
	ps_msub  f8, f0, f3, f8
	ps_mul   f7, f0, f13
	ps_sub   f1, f1, f1
	ps_madd  f7, f2, f12, f7
	ps_madd  f7, f4, f11, f7
	ps_cmpo0 cr0, f7, f1
	bne      lbl_8005F118
	li       r3, 0
	blr

lbl_8005F118:
	fres     f0, f7
	ps_add   f6, f0, f0
	ps_mul   f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	ps_add   f6, f0, f0
	ps_mul   f5, f0, f0
	ps_nmsub f0, f7, f5, f6
	ps_muls0 f13, f13, f0
	ps_muls0 f12, f12, f0
	psq_st   f13, 0(dst), 0, 0
	ps_muls0 f11, f11, f0
	psq_st   f12, 12(dst), 0, 0
	ps_muls0 f10, f10, f0
	psq_st   f11, 24(dst), 0, 0
	ps_muls0 f9, f9, f0
	psq_st   f10, 8(dst), 1, 0
	ps_muls0 f8, f8, f0
	psq_st   f9, 20(r4), 1, 0
	li       r3, 1
	psq_st   f8, 32(r4), 1, 0
	blr
#endif  // clang-format on
}

void J3DGetTranslateRotateMtx(const J3DTransformInfo& tx, Mtx dst) {
    f32 cxsz;
    f32 sxcz;

    f32 sx = JMASSin(tx.mRotation.x), cx = JMASCos(tx.mRotation.x);
    f32 sy = JMASSin(tx.mRotation.y), cy = JMASCos(tx.mRotation.y);
    f32 sz = JMASSin(tx.mRotation.z), cz = JMASCos(tx.mRotation.z);

    dst[2][0] = -sy;
    dst[0][0] = cz * cy;
    dst[1][0] = sz * cy;
    dst[2][1] = cy * sx;
    dst[2][2] = cy * cx;

    cxsz = cx * sz;
    sxcz = sx * cz;
    dst[0][1] = sxcz * sy - cxsz;
    dst[1][2] = cxsz * sy - sxcz;

    cxsz = sx * sz;
    sxcz = cx * cz;
    dst[0][2] = sxcz * sy + cxsz;
    dst[1][1] = cxsz * sy + sxcz;

    dst[0][3] = tx.mTranslate.x;
    dst[1][3] = tx.mTranslate.y;
    dst[2][3] = tx.mTranslate.z;
}

void J3DGetTranslateRotateMtx(s16 rx, s16 ry, s16 rz, f32 tx, f32 ty, f32 tz, Mtx dst) {
    f32 cxsz;
    f32 sxcz;

    f32 sx = JMASSin(rx), cx = JMASCos(rx);
    f32 sy = JMASSin(ry), cy = JMASCos(ry);
    f32 sz = JMASSin(rz), cz = JMASCos(rz);

    dst[2][0] = -sy;
    dst[0][0] = cz * cy;
    dst[1][0] = sz * cy;
    dst[2][1] = cy * sx;
    dst[2][2] = cy * cx;

    cxsz = cx * sz;
    sxcz = sx * cz;
    dst[0][1] = sxcz * sy - cxsz;
    dst[1][2] = cxsz * sy - sxcz;

    cxsz = sx * sz;
    sxcz = cx * cz;
    dst[0][2] = sxcz * sy + cxsz;
    dst[1][1] = cxsz * sy + sxcz;

    dst[0][3] = tx;
    dst[1][3] = ty;
    dst[2][3] = tz;
}
