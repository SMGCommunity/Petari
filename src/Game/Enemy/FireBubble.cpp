#include "Game/Enemy/FireBubble.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvFireBubble {
    NEW_NERVE(FireBubbleNrvAppear, FireBubble, Appear);
    NEW_NERVE(FireBubbleNrvWait, FireBubble, Wait);
    NEW_NERVE(FireBubbleNrvChase, FireBubble, Chase);
    NEW_NERVE(FireBubbleNrvReflect, FireBubble, Reflect);
    NEW_NERVE(FireBubbleNrvReflectLand, FireBubble, ReflectLand);
    NEW_NERVE(FireBubbleNrvReflectWait, FireBubble, ReflectWait);
    NEW_NERVE(FireBubbleNrvDown, FireBubble, Down);
    NEW_NERVE(FireBubbleNrvWaitToAppear, FireBubble, WaitToAppear);
}  // namespace NrvFireBubble

namespace {
    const Vec cAppearVelocity = {0.0f, 35.0f, 6.0f};
    const Vec cReflectVelocity = {0.0f, 40.0f, -15.0f};

}  // namespace

FireBubble::FireBubble(const char* pName)
    : LiveActor(pName), _8C(false), _90(gZeroVec), _9C(0.0f, 0.0f, 1.0f), _A8(cAppearVelocity), _B4(0.0f, 0.0f, 1.0f), _C0(0), _C4(0) {
}

void FireBubble::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        initMapToolInfo(rIter);
    }

    initModelManagerWithAnm("FireBubble", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    MR::addHitSensorAtJointEnemy(this, "body", "Body", 8, 30.f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(40.0f, 40.0f, 0);
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTargetAtJoint(this, "Body", 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(4, false);
    MR::initShadowVolumeCylinder(this, 40.0f);
    MR::invalidateClipping(this);
    MR::declareCoin(this, 2);
    initNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance);

    if (_8C) {
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
    if (!_8C) {
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
    _8C = true;

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

            MR::turnVecToVecCos(&_B4, copy, vec2, cos, vec3, 0.02f);
        }
    }
}

void FireBubble::updateChaseFrontVecAndVelocity(f32 flt) {
    updateChaseFrontVec(flt);
    mVelocity.set(_B4);
    mVelocity.mult(4.0f);
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
    _C4++;

    if (_C0 < _C4) {
        MR::onCalcGravity(this);
        setNerve(&NrvFireBubble::FireBubbleNrvDown::sInstance);
        return true;
    }

    return false;
}

bool FireBubble::tryReflect() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        TVec2f starPointerScreenVelocity(*MR::getStarPointerScreenVelocity(*MR::getStarPointerLastPointedPort(this)));
        if (10.0f < starPointerScreenVelocity.length()) {
            MR::onCalcGravity(this);
            setNerve(&NrvFireBubble::FireBubbleNrvReflect::sInstance);
            return true;
        }
    }

    return false;
}

void FireBubble::killOrWaitToApppear() {
    if (_8C) {
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
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_LAUNCH", -1, -1);
        MR::showModel(this);
        MR::onCalcGravity(this);
        MR::calcGravity(this);
        MR::offBind(this);
        MR::validateHitSensors(this);

        calcAndSetBaseMtx();
        TPos3f baseMtx;
        baseMtx.set(getBaseMtx());
        baseMtx.mult33(_A8, mVelocity);

        _C4 = 0;
    }

    if (!MR::isNoBind(this) && MR::isOnGround(this)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP", -1, -1);
        setNerve(&NrvFireBubble::FireBubbleNrvWait::sInstance);
    } else {
        mVelocity.addInline(mGravity.scaleInline(0.75f));

        if (MR::isNoBind(this) && !isMovingDown()) {
            MR::onBind(this);
        }
    }
}

void FireBubble::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        mVelocity.zero();
        _C0 = MR::getRandom((s32)0, 120) + 600;
    }

    if (MR::isNearPlayer(this, 2000.0f) && tryReflect()) {
        return;
    }

    if (MR::isGreaterStep(this, 30) && MR::isNearPlayer(this, 2000.0f)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP", -1, -1);
        _C0 = MR::getRandom((s32)0, 120) + 300;
        setNerve(&NrvFireBubble::FireBubbleNrvChase::sInstance);
    } else if (MR::isStep(this, _C0)) {
        setNerve(&NrvFireBubble::FireBubbleNrvDown::sInstance);
    }
}

void FireBubble::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::emitEffect(this, "Wait");
    }

    updateChaseFrontVecAndVelocity(1.0f);
    mVelocity.addInline(mGravity.scaleInline(2.0f));

    if (MR::checkPassBckFrame(this, 0.0f)) {
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_JUMP", -1, -1);
    }

    if (!tryChaseEnd() && !tryReflect()) {
        //return needed to generate comparison with unused result
        return;
    }
}

void FireBubble::exeReflect() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DPDHit", nullptr);
        calcReflectVelocity(&mVelocity);
        MR::deleteEffect(this, "Wait");
        MR::emitEffect(this, "WaitS");
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_REFLECT", -1, -1);
        MR::start2PAttackAssistSound();
    }

    updateChaseFrontVec(1.0f);
    _C4++;

    if (MR::isOnGround(this)) {
        setNerve(&NrvFireBubble::FireBubbleNrvReflectLand::sInstance);
    } else {
        mVelocity.addInline(mGravity.scaleInline(1.5f));
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

    if (!tryChaseEnd() && !tryReflect() && MR::isStep(this, 60)) {
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
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_DIE", -1, -1);
        mVelocity.zero();
        MR::invalidateHitSensors(this);
    }
    updateChaseFrontVecAndVelocity(1.0f);

    mVelocity.addInline(mGravity.scaleInline(2.0f));
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
    if (MR::isStep(this, 120)) {
        mPosition.set(_90);
        _B4.set(_9C);
        _A8.set(cAppearVelocity);
        MR::calcGravity(this);
        setNerve(&NrvFireBubble::FireBubbleNrvAppear::sInstance);
    }
}
