#include "Game/Enemy/FireBubble.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvFireBubble {
    NEW_NERVE(FireBubbleNrvAppear, FireBubble, Appear);
    NEW_NERVE(FireBubbleNrvWait, FireBubble, Wait);
    NEW_NERVE(FireBubbleNrvChase, FireBubble, Chase);
    NEW_NERVE(FireBubbleNrvReflect, FireBubble, Reflect);
    NEW_NERVE(FireBubbleNrvReflectLand, FireBubble, ReflectLand);
    NEW_NERVE(FireBubbleNrvReflectWait, FireBubble, ReflectWait);
    NEW_NERVE(FireBubbleNrvDown, FireBubble, Down);
    NEW_NERVE(FireBubbleNrvWaitToAppear, FireBubble, WaitToAppear);
};  // namespace NrvFireBubble

namespace {
    const f32 cSensorRadius = 30.0f;
    const f32 cBinderRadius = 40.0f;
    const s32 cCoinMaxNum = 2;
    const s32 cRandomActFrame = 120;
    const f32 cStarWandRadius = 150.0f;
    const f32 cAppearGravity = 0.75f;
    const s32 cChaseInvalidFrame = 300;
    const s32 cForceKillFrame = 600;
    const f32 cChaseStartDistance = 2000.0f;
    const f32 cChaseSpeed = 4.0f;
    const f32 cChaseGravity = 2.0f;
    const f32 cChaseRotateSpeed = 0.02f;
    const s32 cChaseFrame = 30;
    const f32 cReflectGravity = 1.5f;
    const s32 cReflectWaitFrame = 60;
    const f32 cReflectCursorSpeed = 10.0f;
    const s32 cAppearInterval = 120;
    const Vec cAppearVelocity = {0.0f, 35.0f, 6.0f};
    const Vec cReflectVelocity = {0.0f, 40.0f, -15.0f};
};  // namespace

FireBubble::FireBubble(const char* pName)
    : LiveActor(pName), mIsValidInfo(false), _90(gZeroVec), _9C(0.0f, 0.0f, 1.0f), _A8(cAppearVelocity), _B4(0.0f, 0.0f, 1.0f), mActFrame(0),
      mChaseCounter(0) {
}

void FireBubble::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        initMapToolInfo(rIter);
    }

    initModelManagerWithAnm("FireBubble", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    MR::addHitSensorAtJointEnemy(this, "body", "Body", 8, cSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(cBinderRadius, 40.0f, 0);
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTargetAtJoint(this, "Body", cStarWandRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(4, false);
    MR::initShadowVolumeCylinder(this, 40.0f);
    MR::invalidateClipping(this);
    MR::declareCoin(this, cCoinMaxNum);
    initNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance);

    if (mIsValidInfo) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorDead();
    }
}

void FireBubble::appear() {
    mPosition.set(_90);
    _B4.set(_9C);
    MR::calcGravity(this);
    LiveActor::appear();
    setNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance);
}

void FireBubble::appear(const TVec3f& vec1, const TVec3f& vec2, const TVec3f& vec3) {
    _90.set(vec1);
    _9C.set(vec2);
    _A8.set(vec3);
    appear();
}

void FireBubble::kill() {
    LiveActor::kill();
    if (!mIsValidInfo) {
        MR::emitEffect(this, "Down");
    }
}

void FireBubble::control() {
}

void FireBubble::calcAndSetBaseMtx() {
    TVec3f minusGravity;
    TPos3f baseMtx;
    minusGravity.negate(mGravity);

    if (MR::isSameDirection(_B4, minusGravity, 0.01f)) {
        baseMtx.set(getBaseMtx());
        baseMtx.setTransInline(mPosition);
    } else {
        MR::makeMtxUpFrontPos(&baseMtx, minusGravity, _B4, mPosition);
    }

    MR::setBaseTRMtx(this, baseMtx);
}

void FireBubble::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance) && !isNerve(&NrvFireBubble::FireBubbleNrvDown::sInstance) &&
        !isNerve(&NrvFireBubble::FireBubbleNrvWaitToAppear::sInstance)) {
        if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            killOrWaitToApppear();
        }
    }
}

bool FireBubble::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::appearCoinPop(this, mPosition, 1);
        killOrWaitToApppear();
        return true;
    }

    return false;
}

void FireBubble::initMapToolInfo(const JMapInfoIter& rIter) {
    mIsValidInfo = true;

    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchReadAppear(this, rIter);
    _90.set(mPosition);
    MR::calcGravity(this);

    TPos3f Mtx;
    TVec3f minusGravity;
    TVec3f rotateZDir;
    minusGravity.negate(mGravity);

    MR::makeMtxRotate(Mtx, mRotation);
    Mtx.getZDir(rotateZDir);

    MR::makeMtxUpFront(&Mtx, minusGravity, rotateZDir);
    Mtx.getZDir(_9C);
}

void FireBubble::updateChaseFrontVec(f32 flt) {
    TVec3f vec2;
    TVec3f vec3;
    TVec3f playerUpVec;

    MR::getPlayerUpVec(&playerUpVec);
    MR::normalize(&playerUpVec);
    playerUpVec.scale(40.0f);

    vec2.sub(MR::getPlayerPos()->addOtherInline(playerUpVec), mPosition);

    if (!MR::normalizeOrZero(&vec2)) {
        if (MR::isBindedGround(this)) {
            vec3.negate(*MR::getGroundNormal(this));
        } else {
            vec3.set(mGravity);
        }

        if (!MR::isSameDirection(vec2, vec3, 0.01f)) {
            MR::vecKillElement(vec2, vec3, &vec2);
            MR::normalize(&vec2);

            TVec3f copy(_B4);
            f32 cos = JMACosRadian(flt);

            MR::turnVecToVecCos(&_B4, copy, vec2, cos, vec3, cChaseRotateSpeed);
        }
    }
}

void FireBubble::updateChaseFrontVecAndVelocity(f32 flt) {
    updateChaseFrontVec(flt);
    mVelocity.set(_B4);
    mVelocity.mult(cChaseSpeed);
}

void FireBubble::updateGravity(f32 strength) {
    mVelocity.add(mGravity.scaleInline(strength));
}

void FireBubble::calcReflectVelocity(TVec3f* pOut) const {
    TVec3f toPlayer;

    toPlayer.sub(*MR::getPlayerPos(), mPosition);

    if (MR::normalizeOrZero(&toPlayer)) {
        MR::calcFrontVec(&toPlayer, this);
    }

    TVec3f upVec;
    TPos3f rotateMatrix;
    MR::calcUpVec(&upVec, this);

    MR::makeMtxUpFront(&rotateMatrix, upVec, toPlayer);

    rotateMatrix.mult33(TVec3f(cReflectVelocity), *pOut);
}

bool FireBubble::tryChaseEnd() {
    mChaseCounter++;

    if (mActFrame < mChaseCounter) {
        MR::onCalcGravity(this);
        setNerve(&NrvFireBubble::FireBubbleNrvDown::sInstance);
        return true;
    }

    return false;
}

bool FireBubble::tryReflect() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        TVec2f starPointerScreenVelocity(*MR::getStarPointerScreenVelocity(*MR::getStarPointerLastPointedPort(this)));
        if (cReflectCursorSpeed < starPointerScreenVelocity.length()) {
            MR::onCalcGravity(this);
            setNerve(&NrvFireBubble::FireBubbleNrvReflect::sInstance);
            return true;
        }
    }

    return false;
}

void FireBubble::killOrWaitToApppear() {
    if (mIsValidInfo) {
        MR::emitEffect(this, "Down");
        setNerve(&NrvFireBubble::FireBubbleNrvWaitToAppear::sInstance);
    } else {
        kill();
    }
}

bool FireBubble::isMovingDown() {
    TVec3f normalisedVel;
    return !MR::normalizeOrZero(mVelocity, &normalisedVel) && normalisedVel.dot(mGravity) < 0.0f;
}

void FireBubble::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", 0);
        MR::emitEffect(this, "Wait");
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_LAUNCH");
        MR::showModel(this);
        MR::onCalcGravity(this);
        MR::calcGravity(this);
        MR::offBind(this);
        MR::validateHitSensors(this);

        calcAndSetBaseMtx();
        TPos3f baseMtx;
        baseMtx.set(getBaseMtx());
        baseMtx.mult33(_A8, mVelocity);

        mChaseCounter = 0;
    }

    if (!MR::isNoBind(this) && MR::isOnGround(this)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP");
        setNerve(&NrvFireBubble::FireBubbleNrvWait::sInstance);
    } else {
        updateGravity(cAppearGravity);

        if (MR::isNoBind(this) && !isMovingDown()) {
            MR::onBind(this);
        }
    }
}

void FireBubble::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        mVelocity.zero();
        mActFrame = MR::getRandom((s32)0, cRandomActFrame) + cForceKillFrame;
    }

    if (MR::isNearPlayer(this, cChaseStartDistance) && tryReflect()) {
        return;
    }

    if (MR::isGreaterStep(this, cChaseFrame) && MR::isNearPlayer(this, cChaseStartDistance)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP");
        mActFrame = MR::getRandom((s32)0, cRandomActFrame) + cChaseInvalidFrame;
        setNerve(&NrvFireBubble::FireBubbleNrvChase::sInstance);
    } else if (MR::isStep(this, mActFrame)) {
        setNerve(&NrvFireBubble::FireBubbleNrvDown::sInstance);
    }
}

void FireBubble::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::emitEffect(this, "Wait");
    }

    updateChaseFrontVecAndVelocity(1.0f);
    updateGravity(cChaseGravity);

    if (MR::checkPassBckFrame(this, 0.0f)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP");
    }

    if (!tryChaseEnd() && !tryReflect()) {
        // return needed to generate comparison with unused result
        return;
    }
}

void FireBubble::exeReflect() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DPDHit", nullptr);
        calcReflectVelocity(&mVelocity);
        MR::deleteEffect(this, "Wait");
        MR::emitEffect(this, "WaitS");
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_REFLECT");
        MR::start2PAttackAssistSound();
    }

    updateChaseFrontVec(1.0f);
    mChaseCounter++;

    if (MR::isOnGround(this)) {
        setNerve(&NrvFireBubble::FireBubbleNrvReflectLand::sInstance);
    } else {
        updateGravity(cReflectGravity);
    }
}

void FireBubble::exeReflectLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DPDHitDown", nullptr);
        mVelocity.zero();
        MR::offCalcGravity(this);
    }

    if (!tryChaseEnd() && !tryReflect()) {
        MR::setNerveAtBckStopped(this, &NrvFireBubble::FireBubbleNrvReflectWait::sInstance);
    }
}

void FireBubble::exeReflectWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DPDHitDownWait", nullptr);
    }

    if (!tryChaseEnd() && !tryReflect() && MR::isStep(this, cReflectWaitFrame)) {
        MR::deleteEffect(this, "WaitS");
        MR::onCalcGravity(this);
        setNerve(&NrvFireBubble::FireBubbleNrvChase::sInstance);
    }
}

void FireBubble::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", nullptr);
        MR::deleteEffect(this, "Wait");
        MR::deleteEffect(this, "WaitS");
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_DIE");
        mVelocity.zero();
        MR::invalidateHitSensors(this);
    }
    updateChaseFrontVecAndVelocity(1.0f);

    updateGravity(cChaseGravity);
    if (MR::isBckStopped(this)) {
        killOrWaitToApppear();
    }
}

void FireBubble::exeWaitToAppear() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::offCalcGravity(this);
        MR::offBind(this);
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, cAppearInterval)) {
        mPosition.set(_90);
        _B4.set(_9C);
        _A8.set(cAppearVelocity);
        MR::calcGravity(this);
        setNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance);
    }
}
