#include "Game/Enemy/Balloonfish.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    static const s32 hWaitTime = 120;
    static const s32 hAttackBeginTime = 82;
    static const f32 hTargetPlayerVelocityMult = 10.0f;
    static const f32 hWaitMaxScale = 1.5f;
    static const s32 hDashTime = 180;
    static const f32 hDashVel = 10.0f;
    static const f32 hDashScale = 0.5f;
    static const s32 hDashEndTime = 95;
    // static const ??? hRotateAngle = ???;
};  // namespace

namespace NrvBalloonfish {
    NEW_NERVE(HostTypeNrvWait, Balloonfish, Wait);
    NEW_NERVE(HostTypeNrvDash, Balloonfish, Dash);
    NEW_NERVE(HostTypeNrvDashEnd, Balloonfish, Dash);
    NEW_NERVE_ONEND(HostTypeNrvStarPointerBind, Balloonfish, StarPointerBind, StarPointerBind);
};  // namespace NrvBalloonfish

Balloonfish::Balloonfish(const char* pName) : LiveActor(pName), mAnimeScaleController(), mNerveBeforeBind(), mQuat(0.0f, 0.0f, 0.0f, 1.0f), _A8() {
}

void Balloonfish::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::needStageSwitchReadAppear(this, rIter);
        MR::FunctorV0M< Balloonfish*, void (Balloonfish::*)() > functor = MR::Functor_Inline(this, &Balloonfish::appear);
        MR::listenStageSwitchOnAppear(this, functor);
    }

    initModelManagerWithAnm("Balloonfish", nullptr, false);
    mQuat.setEuler(mRotation);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);

    f32 scale = mScale.y;
    initBinder(100.0f * scale, 0.0f, 0);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 32, 100.0f * scale, TVec3f(0.0f, 60.0f * scale, 0.0f));
    initEffectKeeper(0, "Balloonfish", false);
    initSound(2, false);
    initNerve(&NrvBalloonfish::HostTypeNrvWait::sInstance);
    // float regswap
    f32 offset = 80.0f;
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, offset, 0.0f));
    mAnimeScaleController = new AnimScaleController(nullptr);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 80.0f * mScale.y);
    MR::declareCoin(this, 1);
    MR::invalidateClipping(this);
    makeActorDead();
}

void Balloonfish::appear() {
    LiveActor::appear();
    MR::emitEffect(this, "AppearFromBox");
    MR::startSound(this, "SE_EM_BLNFISH_APPEAR");
}

void Balloonfish::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_BLNFISH_EXPLOSION");
}

void Balloonfish::control() {
    mAnimeScaleController->updateNerve();
}

void Balloonfish::exeWait() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        if (mNerveBeforeBind != mSpine->getCurrentNerve()) {
            mNotBoundStep = 0;
            MR::startBck(this, "appearance", nullptr);
            MR::startSound(this, "SE_EM_BLNFISH_PRE_DASH");
        }

        mNerveBeforeBind = nullptr;
    }

    TVec3f toPredictedPlayerPos(*MR::getPlayerCenterPos());
    toPredictedPlayerPos += *MR::getPlayerVelocity() * ::hTargetPlayerVelocityMult;
    toPredictedPlayerPos -= mPosition;

    f32 ratio = static_cast< f32 >(mNotBoundStep) / ::hWaitTime;
    f32 scale = (1.0f - ratio) * 1.0f + ratio * ::hWaitMaxScale;
    mScale.set(scale, scale, scale);
    MR::blendQuatFrontUp(&mQuat, -mGravity, toPredictedPlayerPos, 0.02f, 0.1f);

    if (mNotBoundStep > ::hWaitTime) {
        setNerve(&NrvBalloonfish::HostTypeNrvDash::sInstance);
        return;
    }

    // "weak"
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mNerveBeforeBind = mSpine->getCurrentNerve();
        setNerve(&NrvBalloonfish::HostTypeNrvStarPointerBind::sInstance);
        return;
    }

    mNotBoundStep++;
}

void Balloonfish::exeDash() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvBalloonfish::HostTypeNrvDash::sInstance)) {
            if (mNerveBeforeBind != mSpine->getCurrentNerve()) {
                mNotBoundStep = 0;
                MR::startBck(this, "Attack", nullptr);
                MR::emitEffect(this, "SwimBubble");
            }
        } else {
            if (mNerveBeforeBind != mSpine->getCurrentNerve()) {
                mNotBoundStep = 0;
                MR::startBck(this, "AttackEnd", nullptr);
                MR::emitEffect(this, "SwimBubble");
            }
        }

        TVec3f zDir;
        mQuat.getZDir(zDir);
        mVelocity.set(zDir * ::hDashVel);

        mNerveBeforeBind = nullptr;
    }

    MR::startLevelSound(this, "SE_EM_LV_BLNFISH_DASH");

    s32 nerveMaxDuration;
    if (isNerve(&NrvBalloonfish::HostTypeNrvDash::sInstance)) {
        nerveMaxDuration = ::hDashTime;
        f32 ratio = static_cast< f32 >(mNotBoundStep) / nerveMaxDuration;
        mScale.set(::hWaitMaxScale * (1.0f - ratio) + ::hDashScale * ratio);
    } else {
        nerveMaxDuration = ::hDashEndTime;
    }

    if (nerveMaxDuration < mNotBoundStep) {
        if (isNerve(&NrvBalloonfish::HostTypeNrvDash::sInstance)) {
            setNerve(&NrvBalloonfish::HostTypeNrvDashEnd::sInstance);
            return;
        } else {
            kill();
            return;
        }
    }

    if (!MR::isInWater(getSensor("body")->mPosition)) {
        kill();
        return;
    }

    // "weak"
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mNerveBeforeBind = mSpine->getCurrentNerve();
        setNerve(&NrvBalloonfish::HostTypeNrvStarPointerBind::sInstance);
        return;
    }

    mNotBoundStep++;
}

void Balloonfish::exeStarPointerBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::setBckRate(this, 0.0f);
        mAnimeScaleController->startDpdHitVibration();
        MR::emitEffect(this, "Touch");
    }

    MR::startDPDFreezeLevelSound(this);
    mVelocity.zero();

    // "weak"
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(mNerveBeforeBind);
    }
}

void Balloonfish::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (isNerve(&NrvBalloonfish::HostTypeNrvWait::sInstance) && MR::isLessStep(this, ::hAttackBeginTime) ||
        isNerve(&NrvBalloonfish::HostTypeNrvStarPointerBind::sInstance)) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender) != true) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Balloonfish::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        if (!isNerve(&NrvBalloonfish::HostTypeNrvStarPointerBind::sInstance)) {
            mAnimeScaleController->startHitReaction();
        }
        return true;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        kill();
        return true;
    }

    return false;
}

void Balloonfish::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.makeQuat(mQuat);

    TPos3f mtx2;
    mtx2.makeRotate(TVec3f(0.0f, 0.0f, 1.0f), _A8);
    mtx.concat(mtx, mtx2);

    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);

    TVec3f scale(mAnimeScaleController->_C * mScale);
    MR::setBaseScale(this, scale);
}

void Balloonfish::endStarPointerBind() {
    MR::setBckRate(this, 1.0f);
    mAnimeScaleController->startAnim();
    MR::deleteEffect(this, "Touch");
}
