#include "Game/MapObj/MagicBell.h"

MagicBell::MagicBell(const char *pName) : LiveActor(pName) {
    _8C = nullptr;
    mSurface1Mtx = nullptr;
    mSurface2Mtx = nullptr;
    _94 = nullptr;
    mSurface1Mtx = nullptr;
    _9C.x = 0.0f;
    _9C.y = 0.0f;
    _9C.z = 0.0f;
}

void MagicBell::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Bell", nullptr, false);
    mSurface2Mtx = MR::getJointMtx(this, "polySurface2");
    mSurface1Mtx = MR::getJointMtx(this, "polySurface1");
    MR::connectToSceneMapObjNoCalcAnim(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 60.0f, TVec3f(0.0f, -80.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "StarWandHitMark");
    MR::setEffectHostSRT(this, "StarWandHitMark", &_9C, nullptr, nullptr);
    initSound(4, false);
    initNerve(&NrvMagicBell::MagicBellNrvWait::sInstance);
    MR::initShadowVolumeSphere(this, 100.0f);
    MR::useStageSwitchWriteA(this, rIter);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, -80.0f, 0.0f));
    TMtx34f mtx;
    mtx.identity();
    MR::makeMtxTR(mtx.toMtxPtr(), this);
    MR::calcGravity(this);
    _8C = new Swinger(&mPosition, mtx.toMtxPtr(), 100.0f, 0.60000002f, 0.99000001f, &mGravity);
    _94 = new Swinger(&mPosition, mtx.toMtxPtr(), 50.0f, 0.30000001f, 0.94999999f, &mGravity);
    makeActorAppeared();
}

void MagicBell::exeWait() {
    MR::isFirstStep(this);

    if (!tryRing()) {
        PSMTXCopy(_8C->_60.toMtxPtr(), mSurface2Mtx);
        PSMTXCopy(_94->_60.toMtxPtr(), mSurface1Mtx);
        TVec3f v3(0.0f, 0.0f, 0.0f);
        MR::setMtxTrans(mSurface2Mtx, v3.x, v3.y, v3.z);
        TVec3f v2(0.0f, 0.0f, 0.0f);
        MR::setMtxTrans(mSurface1Mtx, v2.x, v2.y, v2.z);
        PSMTXScaleApply(mSurface2Mtx, mSurface2Mtx, mScale.x, mScale.y, mScale.z);
        PSMTXScaleApply(mSurface1Mtx, mSurface1Mtx, mScale.x, mScale.y, mScale.z);
        MR::setMtxTrans(mSurface2Mtx, mPosition.x, mPosition.y, mPosition.z);
        MR::setMtxTrans(mSurface1Mtx, mPosition.x, mPosition.y, mPosition.z);
    }
}

/*
void MagicBell::exeRing() {
    if (MR::isFirstStep(this)) {
        if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
            MR::onSwitchA(this);
            MR::startSound(this, "SE_OJ_HAPPY_BELL_SWITCH_ON", -1, -1);
        }

        MR::startSound(this, "SE_OJ_HAPPY_BELL_RING", -1, -1);
    }

    _8C->update();
    _94->update();

    if (_8C->_20.dot(_94->_20) < 0.94999999f) {
        TVec3f v17(_8C->_20);
        v17 = v17 - _94->_20;
        f32 dot = _94->_20.dot(v17);
        v17.x = v17.x - (dot * _94->_20.x);
        v17.y = v17.y - (dot * _94->_20.y);
        v17.z = v17.z - (dot * _94->_20.z);
        MR::normalizeOrZero(&v17);

    }
}
*/

void MagicBell::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

// MagicBell::receiveMsgPlayerAttack
// MagicBell::tryRing
// MagicBell::startRing

MagicBell::~MagicBell() {

}

namespace NrvMagicBell {
    INIT_NERVE(MagicBellNrvWait);
    INIT_NERVE(MagicBellNrvRing);
};

MtxPtr MagicBell::getBaseMtx() const {
    return mSurface2Mtx;
}