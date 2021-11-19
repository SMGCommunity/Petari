#include "Game/LiveActor/HitSensorInfo.h"
#include "Game/LiveActor/HitSensor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

HitSensorInfo::HitSensorInfo(const char *pName, HitSensor *pSensor, const TVec3f *a3, MtxPtr mtx, const register TVec3f &a5, bool a6) {
    mName = pName;
    mHashCode = MR::getHashCode(pName);
    mSensor = pSensor;

    register HitSensorInfo* inf = this;
    __asm {
        psq_l f0, 0(a5), 0, 0
        lfs f1, 8(a5)
        psq_st f0, 0xC(inf), 0, 0
        stfs f1, 0x14(inf)
    };
    //_C.x = a5.x;
    //_C.y = a5.y;
    //_C.z = a5.z;
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