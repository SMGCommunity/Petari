#include "Game/MapObj/Fountain.h"

Fountain::Fountain(const char *pName) : LiveActor(pName) {
    mFountainName = NULL;
    _90.x = 0.0f;
    _90.y = 1.0f;
    _90.z = 0.0f;
}

#ifdef NON_MATCHING
void Fountain::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    TMtx34f mtx;
    mtx.identity();
    TVec3f v30 = mRotation * 0.017453292f;
    f32 v10 = v30.z;
    f32 v11 = v30.y;
    f32 v12 = v30.x;
    f32 v13 = cos(v30.z);
    f32 v14 = cos(v11);
    f32 v15 = cos(v12);
    f32 v16 = sin(v10);
    f32 v17 = sin(v11);
    f32 v18 = sin(v12);
    f32 v19 = v18;
    f32 v20 = (v18 * v14);
    mtx.mMtx[0][0] = v14 * v13;
    mtx.mMtx[2][1] = v18 * v14;
    mtx.mMtx[1][0] = v14 * v16;
    f32 v21 = ((v15 * v13) + ((v18 * v17) * v16));
    f32 v22 = (((v18 * v17) * v13) - (v15 * v16));
    mtx.mMtx[1][1] = v21;
    mtx.mMtx[0][1] = v22;
    mtx.mMtx[0][2] = ((v15 * v13) * v17) + (v19 * v16);
    mtx.mMtx[2][0] = -v17;
    mtx.mMtx[2][2] = v15 * v14;
    mtx.mMtx[1][2] = ((v15 * v16) * v17) - (v19 * v13);

    _90.set<f32>(v22, v21, v20);
    MR::normalize(&_90);
    MR::getObjectName(&mFountainName, rIter);
    initEffectKeeper(0, mFountainName, false);
    initSound(4, false);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 300.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 0x10, 300.0f, offs);
    MR::setClippingTypeSphere(this, 3500.0f);
    MR::useStageSwitchReadA(this, rIter);
    initNerve(&NrvFountain::HostTypeMove::sInstance);
    MR::connectToSceneMapObjMovement(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        appear();
    }
}
#endif

void Fountain::appear() {
    LiveActor::appear();

    if (MR::isValidSwitchA(this)) {
        setNerve(&NrvFountain::HostTypeWait::sInstance);
    }
    else {
        setNerve(&NrvFountain::HostTypeMove::sInstance);
    }
}

void Fountain::startClipped() {
    LiveActor::startClipped();

    if (!isNerve(&NrvFountain::HostTypeWait::sInstance)) {
        MR::deleteEffectAll(this);
    }
}


void Fountain::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(&NrvFountain::HostTypeWait::sInstance)) {
        MR::emitEffect(this, mFountainName);
    }
}

// Fountain::attackSensor

void Fountain::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
    }

    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvFountain::HostTypeMove::sInstance);
    }
}

void Fountain::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::emitEffect(this, mFountainName);
    }

    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN", -1, -1, -1);
}

Fountain::~Fountain() {

}

namespace NrvFountain {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMove);
};