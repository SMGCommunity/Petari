#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

HitSensorInfo::HitSensorInfo(const char *pName, HitSensor *pSensor, const TVec3f *a3, MtxPtr mtx, const register TVec3f &a5, bool a6) {
    mName = pName;
    mHashCode = MR::getHashCode(pName);
    mSensor = pSensor;

    _C.setInlinePS(a5);

    _18 = a3;
    _1C = mtx;
    _20 = a6;

}

// HitSensorInfo::update()

void HitSensorInfo::doObjCol() {
    for (s32 i = 0; i < mSensor->mSensorCount; i++) {
        if (!MR::isDead(mSensor->mSensors[i]->mActor)) {
            mSensor->mActor->attackSensor(mSensor, mSensor->mSensors[i]);
        }
    }
}
