#include "Game/MapObj/WaterPressureBullet.h"
#include "Game/LiveActor/ActorCameraInfo.h"
#include "JSystem/JMath/JMath.h"

WaterPressureBullet::WaterPressureBullet(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    _A4 = 0;
    _A8 = 0.0f;
    mHostActor = NULL;
    _B0 = false;
    _B1 = false;
    _B2 = false;
    mCameraInfo = NULL;
}

void WaterPressureBullet::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("WaterBullet", NULL, false);
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(2);
    MR::addHitSensor(this, "body", 24, 4, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "binder", 106, 4, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(100.0f, 0.0f, 0);
    initEffectKeeper(0, NULL, false);
    initSound(6, false);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::initStarPointerTarget(this, 100.0f, offs);
    MR::initShadowVolumeSphere(this, 75.0f);
    MR::setShadowDropLength(this, NULL, 1500.0f);
    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvWaterPressureBullet::WaterPressureBulletNrvFly::sInstance);
    makeActorDead();
}

void WaterPressureBullet::kill() {
    if (MR::isPlayerInRush() && mHostActor) {
        MR::startBckPlayer("GCaptureBreak", (const char*)NULL);
        MR::endBindAndPlayerJumpWithRollLanding(this, mVelocity, 0);
        mHostActor = NULL;
        endHostCamera();
    }

    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_W_PRESS_BUBBLE_BREAK", -1, -1);
    LiveActor::kill();
}

#ifdef NON_MATCHING
void WaterPressureBullet::control() {
    bool v1 = true;
    bool v2 = false;

    if (_B2 && mHostActor == NULL) {
        v2 = true;
    }

    if (!v2 && _B2) {
        v1 = false;
    }

    if (v1 && MR::isStarPointerPointing2POnTriggerButton(this, "弱", true, false)) {
        kill();
    }
    else {
        TVec3f stack_8;
        if (MR::isNearZero(mVelocity, 0.001)) {
            stack_8.set(mGravity);
        }
        else {
            stack_8.set(mVelocity);
        }
 
        f32 val = (45.511112f * MR::negFloat(2.5f));
        MR::turnVecToVecCosOnPlane(&_8C, stack_8, _98, JMASCos(val));
    } 
}
#endif

void WaterPressureBullet::calcAndSetBaseMtx() {
    TPos3f pos;
    MR::makeMtxFrontSidePos(&pos, _8C, _98, mPosition);
    MR::setBaseTRMtx(this, pos);
}

void WaterPressureBullet::shotWaterBullet(LiveActor *pActor, const TPos3f &rPos, f32 a3, bool a4, bool a5, bool a6, ActorCameraInfo **pInfo) {
    f32 z = rPos.mMtx[2][2];
    f32 y = rPos.mMtx[1][2];
    f32 x = rPos.mMtx[0][2];
    _A8 = a3;
    _A4 = pActor;
    _B0 = a4;
    _B1 = a5;
    _B2 = a6;
    mCameraInfo = pInfo; 
    _8C.set<f32>(x, y, z);
    mVelocity.scale(_A8, _8C);
    z = rPos.mMtx[2][3];
    y = rPos.mMtx[1][3];
    x = rPos.mMtx[0][3];
    mPosition.set<f32>(x, y, z);
    z = rPos.mMtx[2][0];
    y = rPos.mMtx[1][0];
    x = rPos.mMtx[0][0];
    _98.set<f32>(x, y, z);
    mRotation.zero();
    makeActorAppeared();
    MR::validateHitSensors(this);
    MR::invalidateClipping(this);
    MR::setShadowDropLength(this, NULL, 1500.0f);

    if (!_B0) {
        MR::onCalcGravity(this);
    }

    setNerve(&NrvWaterPressureBullet::WaterPressureBulletNrvFly::sInstance);
}

void WaterPressureBullet::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shot", NULL);
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Move", NULL);
    }

    if (mHostActor != NULL && MR::isBckOneTimeAndStopped(mHostActor)) {
        MR::startBckPlayer("WaterBulletWait", (const char*)NULL);
    }

    if (mHostActor != NULL) {
        MR::startLevelSound(this, "SE_OJ_LV_W_PRESS_BUBBLE_SUS", -1, -1, -1);
    }

    if (!_B0) {
        JMAVECScaleAdd(mGravity.toCVec(), mVelocity.toCVec(), mVelocity.toVec(), 0.40000001f);
    }

    if (MR::isPadSwing(0) && mHostActor != NULL && !_B2) {
        MR::startSound(mHostActor, "SE_PV_TWIST_START", -1, -1);
        MR::startSound(mHostActor, "SE_PM_SPIN_ATTACK", -1, -1);
        MR::tryRumblePadMiddle(this, 0);
        setNerve(&NrvWaterPressureBullet::WaterPressureBulletNrvSpinKill::sInstance);
        return;
    }

    bool v2 = false;

    if (MR::isBinded(this) || MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        v2 = true;
    }

    if (v2) {
        if (!_B1 || mHostActor == NULL || MR::isBindedGroundSand(this)) {
            kill();
            return;
        }

        TVec3f* vel = &mVelocity;
        TVec3f* grav = &mGravity;
        f32 dot = grav->dot(mVelocity);
        JMAVECScaleAdd(grav->toCVec(), vel->toCVec(), vel->toVec(), -dot);
    }

    if (!_B2 && MR::isGreaterEqualStep(this, 180) || _B2 && MR::isGreaterEqualStep(this, 300)) {
        kill();
    }
}

void WaterPressureBullet::exeSpinKill() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Spin2nd", (const char*)NULL);
        mVelocity.zero();
        MR::invalidateHitSensors(this);

        if (MR::isPlayerInRush()) {
            if (mHostActor != NULL) {
                MR::endBindAndPlayerJump(this, mVelocity, 0);
                mHostActor = NULL;
                endHostCamera();
            }
        }
    }

    kill();
}

void WaterPressureBullet::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorMapObj(a2)) {
        kill();
    }
}

bool WaterPressureBullet::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    if (MR::isMsgFireBallAttack(msg)) {
        kill();
        return true;
    }

    return false;
}

bool WaterPressureBullet::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(a2) && mHostActor == NULL) {
        if (MR::isDemoActive()) {
            kill();
            return false;
        }

        if (!inviteMario(a2)) {
            return false;
        }
        else {
            MR::startSound(this, "SE_OJ_W_PRESS_BUBBLE_IN", -1, -1);
            MR::startSound(mHostActor, "SE_PV_CATCH", -1, -1);
            return true;
        }
    }
    else if (msg == 152) {
        return true;
    }
    else if (msg == 147) {
        kill();
        return true;
    }
    else if (msg == 161 && mHostActor != NULL) {
        updateSuffererMtx();
        return true;
    }

    return false;
}

bool WaterPressureBullet::startHostCamera() const {
    if (_A4 != NULL && mCameraInfo != NULL) {
        MR::startActorCameraNoTarget(_A4, *mCameraInfo, -1);
        return true;
    }

    return false;
}

bool WaterPressureBullet::endHostCamera() const {
    if (_A4 != NULL && mCameraInfo != NULL) {
        MR::endActorCamera(_A4, *mCameraInfo, true, -1);
        return true;
    }

    return false;
}

bool WaterPressureBullet::inviteMario(HitSensor *pSensor) {
    MR::tryRumblePadMiddle(this, 0);

    if (MR::isOnGroundPlayer() && MR::isNearAngleDegree(mVelocity, mGravity, 60.0f)) {
        if (_B1) {
            TVec3f* vel = &mVelocity;
            TVec3f* grav = &mGravity;
            f32 dot = grav->dot(mVelocity);
            JMAVECScaleAdd(grav->toCVec(), vel->toCVec(), vel->toVec(), -dot);
        }
        else {
            kill();
            MR::sendArbitraryMsg(76, pSensor, getSensor("body"));
            return false;
        }
    }

    mHostActor = pSensor->mActor;
    MR::startBckWithInterpole(this, "Touch", 0);
    MR::startBckPlayer("WaterBulletStart", 2);
    startHostCamera();
    MR::setShadowDropLength(this, NULL, 2000.0f);
    return true;
}

void WaterPressureBullet::updateSuffererMtx() {
    TPos3f pos;
    pos.identity();

    if (isNerve(&NrvWaterPressureBullet::WaterPressureBulletNrvSpinKill::sInstance)) {
        TVec3f front;
        MR::calcFrontVec(&front, mHostActor);
        MR::calcMtxFromGravityAndZAxis(&pos, mHostActor, mGravity, front);
    }
    else {
        MtxPtr mtx = getBaseMtx();
        pos.setInline(mtx);
    }

    MR::setBaseTRMtx(this, pos);
}

WaterPressureBullet::~WaterPressureBullet() {

}

namespace NrvWaterPressureBullet {
    INIT_NERVE(WaterPressureBulletNrvFly);
    INIT_NERVE(WaterPressureBulletNrvSpinKill);

    void WaterPressureBulletNrvSpinKill::execute(Spine *pSpine) const {
        WaterPressureBullet* bullet = reinterpret_cast<WaterPressureBullet*>(pSpine->mExecutor);
        bullet->exeSpinKill();
    }

    void WaterPressureBulletNrvFly::execute(Spine *pSpine) const {
        WaterPressureBullet* bullet = reinterpret_cast<WaterPressureBullet*>(pSpine->mExecutor);
        bullet->exeFly();
    }
};