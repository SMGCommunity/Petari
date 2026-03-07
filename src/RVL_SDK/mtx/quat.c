#include "revolution/mtx.h"
#include <cmath>

// clang-format off

void PSQUATMultiply
(
    const register Quaternion *p,
    const register Quaternion *q,
          register Quaternion *pq
)
{
    register f32    pxy, pzw, qxy, qzw;
    register f32    pnxy, pnzw, pnxny, pnznw;
    register f32    rxy, rzw, sxy, szw;

    asm
    {
        psq_l       pxy, 0(p), 0, 0
        psq_l       pzw, 8(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_neg      pnxny, pxy
        psq_l       qzw, 8(q), 0, 0
        ps_neg      pnznw, pzw
        ps_merge01  pnxy, pnxny, pxy
        ps_muls0    rxy, pzw, qxy
        ps_muls0    rzw, pnxny, qxy
        ps_merge01  pnzw, pnznw, pzw
        ps_muls1    szw, pnxy, qxy
        ps_madds0   rxy, pnxy, qzw, rxy
        ps_muls1    sxy, pnzw, qxy
        ps_madds0   rzw, pnzw, qzw, rzw
        ps_madds1   szw, pnznw, qzw, szw
        ps_merge10  rxy, rxy, rxy
        ps_madds1   sxy, pxy, qzw, sxy
        ps_merge10  rzw, rzw, rzw
        ps_add      rxy, rxy, sxy
        psq_st      rxy, 0(pq), 0, 0
        ps_sub      rzw, rzw, szw
        psq_st      rzw, 8(pq), 0, 0
    }
}

f32 PSQUATDotProduct( const register Quaternion *p, const register Quaternion *q ) 
{
    register f32    pxy, pzw, qxy, qzw, dp;

    asm
    {
        psq_l       pxy, 0(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_mul      dp, pxy, qxy
        
        psq_l       pzw, 8(p), 0, 0
        psq_l       qzw, 8(q), 0, 0
        ps_madd     dp, pzw, qzw, dp
        
        ps_sum0     dp, dp, dp, dp
    }

    return dp;
}

// clang-format on

void C_QUATMtx(Quaternion* r, const Mtx m) {
    f32 tr, s;
    s32 i, j, k;
    s32 nxt[3] = {1, 2, 0};
    f32 q[3];

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) {
        s = (f32)sqrt(tr + 1.0f);
        r->w = s * 0.5f;
        s = 0.5f / s;
        r->x = (m[2][1] - m[1][2]) * s;
        r->y = (m[0][2] - m[2][0]) * s;
        r->z = (m[1][0] - m[0][1]) * s;
    } else {
        i = 0;
        if (m[1][1] > m[0][0])
            i = 1;
        if (m[2][2] > m[i][i])
            i = 2;
        j = nxt[i];
        k = nxt[j];
        s = (f32)sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5f;

        if (s != 0.0f)
            s = 0.5f / s;

        r->w = (m[k][j] - m[j][k]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        r->x = q[0];
        r->y = q[1];
        r->z = q[2];
    }
}

void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t) {
    f32 theta, sin_th, cos_th, tp, tq, sin_1mtth, sin_tth;

    cos_th = p->x * q->x + p->y * q->y + p->z * q->z + p->w * q->w;
    tq = 1.0F;

    if (cos_th < 0.0F) {
        cos_th = -cos_th;
        tq = -tq;
    }

    if (cos_th <= 1.0F - 0.00001f) {
        theta = acos(cos_th);
        sin_th = sin(theta);
        sin_1mtth = sin((1.0F - t) * theta);
        tp = sin_1mtth / sin_th;
        sin_tth = sin(t * theta);
        tq *= sin_tth/sin_th;
    } else {
        tp = 1.0F - t;
        tq = tq * t;
    }

    r->x = tp * p->x + tq * q->x;
    r->y = tp * p->y + tq * q->y;
    r->z = tp * p->z + tq * q->z;
    r->w = tp * p->w + tq * q->w;
}
