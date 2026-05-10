#include "Game/Util/FixedPosition.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

FixedPosition::FixedPosition(const LiveActor* pActor, const char* pJointName, const TVec3f& rLocalTrans, const TVec3f& rRotAxes) {
    MtxPtr jointMtx = MR::getJointMtx(pActor, pJointName);
    init(jointMtx, rLocalTrans, rRotAxes);
}

FixedPosition::FixedPosition(const LiveActor* pActor, const TVec3f& rLocalTrans, const TVec3f& rRotAxes) {
    MtxPtr baseMtx = pActor->getBaseMtx();
    init(baseMtx, rLocalTrans, rRotAxes);
}

FixedPosition::FixedPosition(MtxPtr mtx, const TVec3f& rLocalTrans, const TVec3f& rRotAxes) {
    init(mtx, rLocalTrans, rRotAxes);
}

void FixedPosition::calc() {
    // FIXME: matrix creation inline problem
    // https://decomp.me/scratch/Q7A0B

    mMtx.identity();

    TVec3f rot = mRotDegrees.multInLine(PI_180);
    mMtx.makeMatrixFromRotAxesInline(rot.z, rot.y, rot.x);

    mMtx.setTrans(mLocalTrans);

    if (mBaseMtx != nullptr) {
        TMtx34f mtx;
        mtx.setInline((const MtxPtr)mBaseMtx);
        mMtx.concat(mtx, mMtx);
    }

    if (mNormalizeScale) {
        MR::makeMtxWithoutScale(&mMtx, mMtx);
    }
}

void FixedPosition::setBaseMtx(MtxPtr mtx) {
    mBaseMtx = (TMtx34f*)mtx;
}

void FixedPosition::setLocalTrans(const TVec3f& rLocalTrans) {
    mLocalTrans.set< f32 >(rLocalTrans);
}

void FixedPosition::init(MtxPtr mtx, const TVec3f& rLocalTrans, const TVec3f& rRotAxes) {
    setBaseMtx(mtx);
    mLocalTrans.set< f32 >(rLocalTrans);
    mRotDegrees.set< f32 >(rRotAxes);
    mMtx.identity();
    mNormalizeScale = true;
}

FixedPosition::FixedPosition(const LiveActor* pActor, const char* pBcsvName, const LiveActor* pResourceActor) {
    if (pResourceActor == nullptr) {
        pResourceActor = pActor;
    }
    ResourceHolder* resourceHolder = MR::getResourceHolder(pResourceActor);
    JMapInfo* csv = MR::tryCreateCsvParser(resourceHolder, "%s.bcsv", pBcsvName);

    const char* jointName = nullptr;
    TVec3f trans(0.0f, 0.0f, 0.0f);
    TVec3f rotate(0.0f, 0.0f, 0.0f);
    MR::getCsvDataStrOrNULL(&jointName, csv, "JointName", 0);
    MR::getCsvDataVec(&trans, csv, "Trans", 0);
    MR::getCsvDataVec(&rotate, csv, "Rotate", 0);

    if (jointName == nullptr) {
        init(pActor->getBaseMtx(), trans, rotate);
    } else {
        init(MR::getJointMtx(pActor, jointName), trans, rotate);
    }
}
