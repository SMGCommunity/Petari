#include "Game/Util.hpp"
#include "Game/LiveActor/LiveActor.hpp"

FixedPosition::FixedPosition(const LiveActor *pActor, const char *pJointName, const TVec3f &rLocalTrans, const TVec3f &a4) {
    MtxPtr jointMtx = MR::getJointMtx(pActor, pJointName);
    init(jointMtx, rLocalTrans, a4);
}

FixedPosition::FixedPosition(const LiveActor *pActor, const TVec3f &rLocalTrans, const TVec3f &a3) {
    MtxPtr baseMtx = pActor->getBaseMtx();
    init(baseMtx, rLocalTrans, a3);
}

FixedPosition::FixedPosition(MtxPtr mtx, const TVec3f &rLocalTrans, const TVec3f &a3) {
    init(mtx, rLocalTrans, a3);
}

//FixedPosition::FixedPosition::calc()

void FixedPosition::setBaseMtx(MtxPtr mtx) {
    mBaseMtx = mtx;
}

void FixedPosition::setLocalTrans(const TVec3f &rLocalTrans) {
    mLocalTrans.set<f32>(rLocalTrans);
}

void FixedPosition::init(MtxPtr mtx, const TVec3f &rLocalTrans, const TVec3f &a3) {
    mBaseMtx = mtx;
    mLocalTrans.set<f32>(rLocalTrans);
    _10.set<f32>(a3);
    _1C.identity();
    mNormalizeScale = true;
}

//FixedPosition::FixedPosition(const LiveActor *, const char *, const LiveActor *)
