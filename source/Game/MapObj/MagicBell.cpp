#include "Game/MapObj/MagicBell.hpp"

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

// Minor mismatch: "squaredInline" near the bottom gets its registers swapped
void MagicBell::exeRing()
{
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
        v17.subtract(_94->_20);
        f32 v3 = _94->_20.dot(v17);
        v17.x -= v3 * _94->_20.x;
        v17.y -= v3 * _94->_20.y;
        v17.z -= v3 * _94->_20.z;
        MR::normalizeOrZero(&v17);
        f32 v4 = _8C->mAcceleration.dot(v17);
        f32 v5 = _94->mAcceleration.dot(v17);

        f32 v6 = v5 >= 0.0f ? 0.0f : v5;

        if (v4 - v6 > 0.0f) {
            f32 v7 = v5 >= 0.0f ? 0.0f : v5;

            v17.scale(v4 - v7);
            v17.subtract(_94->mAcceleration);
            _94->accel(v17);
            _9C.set(_94->_8);
        }
        PSMTXCopy(_8C->_60.toMtxPtr(), mSurface2Mtx);
        PSMTXCopy(_94->_60.toMtxPtr(), mSurface1Mtx);
        TVec3f v16(0.0f, 0.0f, 0.0f);
        MR::setMtxTrans(mSurface2Mtx, v16.x, v16.y, v16.z);
        TVec3f v15(0.0f, 0.0f, 0.0f);
        MR::setMtxTrans(mSurface1Mtx, v15.x, v15.y, v15.z);
        PSMTXScaleApply(mSurface2Mtx, mSurface2Mtx, mScale.x, mScale.y, mScale.z);
        PSMTXScaleApply(mSurface1Mtx, mSurface1Mtx, mScale.x, mScale.y, mScale.z);
        MR::setMtxTrans(mSurface2Mtx, mPosition.x, mPosition.y, mPosition.z);
        MR::setMtxTrans(mSurface1Mtx, mPosition.x, mPosition.y, mPosition.z);

        if ((!MR::isGreaterStep(this, 10) || !tryRing())) {
            if (_8C->_20.y < -0.99f) {
                if (_8C->mAcceleration.squaredInline() < 0.01f) {
                    MR::deleteEffect(this, "Ring");
                    setNerve(&NrvMagicBell::MagicBellNrvWait::sInstance);
                }
            }
        }
    }
}

void MagicBell::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

bool MagicBell::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver)
{
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return false;
    }

    if (!isNerve(&NrvMagicBell::MagicBellNrvWait::sInstance) && (isNerve(&NrvMagicBell::MagicBellNrvRing::sInstance) && MR::isGreaterStep(this, 10))) {

        TVec3f v15(mPosition);
        v15.subtract(*MR::getPlayerPos());
        v15.y += 100.0f;
        MR::normalizeOrZero(&v15);
        TVec3f v14(v15);
        v14.scale(-200.0f);
        v14.addInline2(mPosition);
        startRing(v15, v14);
        return true;
    }
    return false;
}

bool MagicBell::tryRing()
{
    if (!MR::isExecScenarioStarter() && MR::isStarPointerPointing(this, 0, 0, "Žã")) {
        TVec2f *v4(MR::getStarPointerScreenVelocity(0));
        if (((v4->x * v4->x) + (v4->y * v4->y)) > 64.0f) {
            TVec3f v12;
            MR::getStarPointerWorldVelocityDirection(&v12, 0);
            if (MR::isNearZero(v12, 0.001f)) {
                v12.set(MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f));
                if (MR::isNearZero(v12, 0.001f)) {
                    v12.set(1.0f, 0.0f, 0.0f);
                }
                else {
                    MR::normalize(&v12);
                }
            }
            TVec3f v11;
            MR::calcStarPointerWorldPointingPos(&v11, mPosition, 0);
            startRing(v12, v11);
            return true;
        }
    }
    return false;
}

void MagicBell::startRing(const TVec3f &a1, const TVec3f &a2)
{
    f32 v10 = PSVECMag(_8C->mAcceleration.toCVec());
    TVec3f v13(_8C->mAcceleration);
    v13.scale(-1.0f);
    _8C->accel(v13);
    TVec3f v12(a1);
    v12.scale(5.0f + v10);
    _8C->accel(v12);
    _9C.set(a2);
    MR::emitEffect(this, "StarWandHitMark");
    MR::emitEffect(this, "Ring");
    setNerve(&NrvMagicBell::MagicBellNrvRing::sInstance);
}

MagicBell::~MagicBell() {

}

namespace NrvMagicBell {
    INIT_NERVE(MagicBellNrvWait);
    INIT_NERVE(MagicBellNrvRing);
};

MtxPtr MagicBell::getBaseMtx() const {
    return mSurface2Mtx;
}