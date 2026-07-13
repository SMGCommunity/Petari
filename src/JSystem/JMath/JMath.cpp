#include "JSystem/JMath/JMath.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

// this is a different type but it works for now
f32 sLookupTable[1024];

// some small issues
f32 JMAAcosRadian(f32 val) {
    if (val >= 1.0f) {
        return 0.0f;
    }

    if (val <= -1.0f) {
        return 3.1415927f;
    }

    if (val >= 0.0f) {
        u32 idx = 1023.5f * -val;
        return 1.5707964f + sLookupTable[idx];
    }

    u32 idx = 1023.5f * val;
    return sLookupTable[idx] - 1.5707964f;
}

void JMAEulerToQuat(s16 x, s16 y, s16 z, Quaternion* quat) {
    f32 cosX = JMASCos(x / 2);
    f32 cosY = JMASCos(y / 2);
    f32 cosZ = JMASCos(z / 2);
    f32 sinX = JMASSin(x / 2);
    f32 sinY = JMASSin(y / 2);
    f32 sinZ = JMASSin(z / 2);

    f32 cyz = cosY * cosZ;
    f32 syz = sinY * sinZ;
    quat->w = cosX * (cyz) + sinX * (syz);
    quat->x = sinX * (cyz)-cosX * (syz);
    quat->y = cosZ * (cosX * sinY) + sinZ * (sinX * cosY);
    quat->z = sinZ * (cosX * cosY) - cosZ * (sinX * sinY);
}

void JMAQuatLerp(__REGISTER const Quaternion* p, __REGISTER const Quaternion* q, f32 t, Quaternion* dst) {
    __REGISTER f32 pxy, pzw, qxy, qzw;
    __REGISTER f32 dp;

#ifdef __MWERKS__  // clang-format off
    // compute dot product
    asm {
        psq_l       pxy, 0(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_mul      dp, pxy, qxy
        
        psq_l       pzw, 8(p), 0, 0
        psq_l       qzw, 8(q), 0, 0
        ps_madd     dp, pzw, qzw, dp
        
        ps_sum0     dp, dp, dp, dp
    }
#endif  // clang-format on
    f32 local_78 = dp;
    if (local_78 < 0.0) {
        int unused;
        dst->x = -t * (p->x + q->x) + p->x;
        dst->y = -t * (p->y + q->y) + p->y;
        dst->z = -t * (p->z + q->z) + p->z;
        dst->w = -t * (p->w + q->w) + p->w;
    } else {
        dst->x = -t * (p->x - q->x) + p->x;
        dst->y = -t * (p->y - q->y) + p->y;
        dst->z = -t * (p->z - q->z) + p->z;
        dst->w = -t * (p->w - q->w) + p->w;
    }
}

void JMAVECScaleAdd(__REGISTER const Vec* vec1, __REGISTER const Vec* vec2, __REGISTER Vec* dst, __REGISTER f32 scale) {
    __REGISTER f32 v1xy;
    __REGISTER f32 v2xy;
    __REGISTER f32 rxy, v1z, v2z, rz;
#ifdef __MWERKS__  // clang-format off
	asm {
        psq_l v1xy, 0(vec1), 0, 0
        psq_l v2xy, 0(vec2), 0, 0
        ps_madds0 rxy, v1xy, scale, v2xy
        psq_st rxy, 0(dst), 0, 0

        psq_l v1z, 8(vec1), 1, 0
        psq_l v2z, 8(vec2), 1, 0
        ps_madds0 rz, v1z,  scale, v2z
        psq_st rz, 8(dst), 1, 0
	}
#endif  // clang-format on
}
