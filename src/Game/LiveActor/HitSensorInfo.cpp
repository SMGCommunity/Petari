#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

HitSensorInfo::HitSensorInfo(const char* pName, HitSensor* pSensor, const TVec3f* pPosition, MtxPtr pMtx, const TVec3f& rOffset,
                             bool useHostCallback) {
    mName = pName;
    mHashCode = MR::getHashCode(pName);
    mSensor = pSensor;

    _C = rOffset;

    _18 = pPosition;
    _1C = pMtx;
    _20 = useHostCallback;
}

void HitSensorInfo::update() {
    if (_20) {
        mSensor->mHost->updateHitSensor(mSensor);
        return;
    }

    TVec3f position;
    if (_1C) {
        position.set< f32 >(_1C[0][3], _1C[1][3], _1C[2][3]);
        position.x += _1C[0][0] * _C.x + _1C[0][1] * _C.y + _1C[0][2] * _C.z;
        position.y += _1C[1][0] * _C.x + _1C[1][1] * _C.y + _1C[1][2] * _C.z;
        position.z += _1C[2][0] * _C.x + _1C[2][1] * _C.y + _1C[2][2] * _C.z;
    } else {
        if (_18) {
            position.set< f32 >(_18->x, _18->y, _18->z);
        } else {
            position.set(mSensor->mHost->mPosition);
        }

        MtxPtr baseMtx = mSensor->mHost->getBaseMtx();
        if (baseMtx) {
            position.x += baseMtx[0][0] * _C.x + baseMtx[0][1] * _C.y + baseMtx[0][2] * _C.z;
            position.y += baseMtx[1][0] * _C.x + baseMtx[1][1] * _C.y + baseMtx[1][2] * _C.z;
            position.z += baseMtx[2][0] * _C.x + baseMtx[2][1] * _C.y + baseMtx[2][2] * _C.z;
        } else {
            position.add(_C);
        }
    }

    mSensor->mPosition.set(position);
}

void HitSensorInfo::doObjCol() {
    for (s32 i = 0; i < mSensor->mSensorCount; i++) {
        if (!MR::isDead(mSensor->mSensors[i]->mHost)) {
            mSensor->mHost->attackSensor(mSensor, mSensor->mSensors[i]);
        }
    }
}
