#include "Game/Enemy/JumpSpider.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

void JumpSpider_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    // hCommonGravity
    // hNoPowerGravity
    static const f32 hGroundDamp = 0.8f;
    static const f32 hAirDampH = 0.98f;
    static const f32 hAirDampV = 0.98f;
    // hOnGroundTime
    // hJumpDistance
    static const f32 hJumpVelV = 1.0f;
    // hStampDeathFallVel
    static const s32 hStampDeathTime = 20;
    // hSearchTurnLimitRadian
    // hJumpTurnLimitRadian
    // hToPreJumpRestRadian
    // hWaitTime
    // hHideTime
    // hWaitUnderGroundTime
    static const s32 hNoPowerWaitTime = 240;
};  // namespace

namespace NrvJumpSpider {
    NEW_NERVE(HostTypeNrvWait, JumpSpider, Wait);
    NEW_NERVE(HostTypeNrvPreJump, JumpSpider, PreJump);
    NEW_NERVE(HostTypeNrvJump, JumpSpider, Jump);
    NEW_NERVE(HostTypeNrvJumpEnd, JumpSpider, JumpEnd);
    NEW_NERVE(HostTypeNrvHitToPlayer, JumpSpider, HitToPlayer);
    NEW_NERVE_ONEND(HostTypeNrvStampFall, JumpSpider, StampFall, StampFall);
    NEW_NERVE_ONEND(HostTypeNrvStampDeath, JumpSpider, StampDeath, StampDeath);
    NEW_NERVE_ONEND(HostTypeNrvHitBlow, JumpSpider, HitBlow, HitBlow);
    NEW_NERVE_ONEND(HostTypeNrvDpdPointed, JumpSpider, DpdPointed, DpdPointed);
    NEW_NERVE(HostTypeNrvDpdPointedEnd, JumpSpider, NoPowerStart);
    NEW_NERVE(HostTypeNrvStarPieceHit, JumpSpider, NoPowerStart);
    NEW_NERVE(HostTypeNrvDpdPointedLand, JumpSpider, NoPowerLand);
    NEW_NERVE(HostTypeNrvStarPieceHitLand, JumpSpider, NoPowerLand);
    NEW_NERVE(HostTypeNrvStarPieceHitWait, JumpSpider, NoPowerWait);
    NEW_NERVE(HostTypeNrvNoPowerEnd, JumpSpider, NoPowerEnd);
};  // namespace NrvJumpSpider

JumpSpider::JumpSpider(const char* pName) : LiveActor(pName), mSpinHitController(nullptr), mScaleController(nullptr), _94(0, 0, 1), _A0(0, 0, 0) {
}

JumpSpider::~JumpSpider() {
}

void JumpSpider::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
    }

    initModelManagerWithAnm("JumpSpider", nullptr, false);
    MR::initFur(this);
    TPos3f pos;
    pos.setInline(getBaseMtx());
    pos.getZDir(_94);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    f32 scaleY = mScale.y;
    initBinder(scaleY * 100.0f, scaleY * 100.0f, 0);
    initHitSensor(1);
    TVec3f offset1;
    offset1.set(0.0f, 60.0f * scaleY, 0.0f);
    MR::addHitSensorEnemy(this, "body", 32, scaleY * 100.0f, offset1);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(GET_NERVE(JumpSpider, HostTypeNrvWait));
    TVec3f offset2;
    offset2.x = 0.0f;
    offset2.y = 80.0f;
    offset2.z = 0.0f;
    MR::initStarPointerTarget(this, 110.0f, offset2);
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    mSpinHitController = new SpinHitController(this, 3, false);
    mScaleController = new AnimScaleController(nullptr);
    MR::offCalcGravity(this);
    MR::initShadowFromCSV(this, "Shadow");
    MR::onCalcShadowAll(this);
    MR::emitEffect(this, "EyeLight");
}

void JumpSpider::initAfterPlacement() {
    MR::calcGravity(this);
    TVec3f v15(mPosition - mGravity * 10.0f);
    TVec3f v1, v2;

    if (MR::getFirstPolyNormalOnLineToMap(&v1, v15, mGravity * 300.0f, &v2, nullptr)) {
        _A0.set(v2);
        mPosition.set(v2);
        mGravity.set(-v1);
    } else {
        kill();
        return;
    }

    makeActorAppeared();
}

void JumpSpider::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void JumpSpider::startClipped() {
    LiveActor::startClipped();
    ;
    MR::deleteEffect(this, "EyeLight");
}

void JumpSpider::endClipped() {
    LiveActor::endClipped();
    MR::emitEffect(this, "EyeLight");
}

void JumpSpider::control() {
    mScaleController->updateNerve();
    if (!isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampFall)) && !isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampDeath)) &&
        !isNerve(GET_NERVE(JumpSpider, HostTypeNrvHitBlow)) && !isNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointed)) &&
        !isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit)) && !isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitLand)) &&
        !isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitWait)) && MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointed));
    }
}

void JumpSpider::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    if (MR::calcDistanceToPlayer(this) < 1000.0f) {
        MR::startSound(this, "SE_EV_JUMPSPIDER_FIND");
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvPreJump));
    }
}

void JumpSpider::exePreJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "JumpStart");
        mVelocity.zero();
    }

    TVec3f heightVec;
    MR::calcVecToPlayerH(&heightVec, this, nullptr);
    MR::turnVecToVecRadian(&_94, _94, heightVec, 0.05f, mGravity);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvJump));
    }
}

void JumpSpider::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Jump");
        MR::startSound(this, "SE_EM_JUMPSPIDER_JUMP");
        mVelocity.set(-mGravity * 22.0f);
    }

    MR::applyVelocityDampAndGravity(this, 0.3f, ::hGroundDamp, ::hAirDampH, ::hAirDampV, ::hJumpVelV);
    TVec3f heightVec;
    MR::calcVecToPlayerH(&heightVec, this, nullptr);
    MR::turnVecToVecRadian(&_94, _94, heightVec, 0.02f, mGravity);

    if (MR::isOnGround(this)) {
        MR::startSound(this, "SE_EM_JUMPSPIDER_LAND");
        mPosition.set(_A0);
        mVelocity.zero();
        MR::startBck(this, "JumpEnd");
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvJumpEnd));
    }
}

void JumpSpider::exeJumpEnd() {
    if (MR::isFirstStep(this)) {
    }

    mPosition.set(_A0);

    if (MR::isBckStopped(this)) {
        if (1000.0f < MR::calcDistanceToPlayer(this)) {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvWait));
        } else {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvPreJump));
        }
    }
}

void JumpSpider::exeHitToPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit");
        mVelocity.zero();
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, 0.3f, 0.8f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);

    if (MR::isGreaterStep(this, 58)) {
        TVec3f heightVec;
        MR::calcVecToPlayerH(&heightVec, this, nullptr);
        MR::turnVecToVecRadian(&_94, _94, heightVec, 0.05f, mGravity);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvJump));
    }
}

void JumpSpider::exeStampFall() {
    if (MR::isFirstStep(this)) {
        mVelocity.set(mGravity * 30.0f);
        MR::invalidateClipping(this);
    }

    if (MR::isOnGround(this)) {
        mVelocity.zero();
        mPosition.set(_A0);
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvStampDeath));
    }
}

void JumpSpider::endStampFall() {
    MR::validateClipping(this);
}

void JumpSpider::exeStampDeath() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Press");
        MR::invalidateClipping(this);
        mVelocity.zero();
    }

    mPosition.set(_A0);

    if (MR::isGreaterStep(this, ::hStampDeathTime)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        MR::appearCoinPop(this, mPosition, 1);
        kill();
    }
}

void JumpSpider::endStampDeath() {
    MR::validateClipping(this);
}

void JumpSpider::exeHitBlow() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Blow");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EV_JUMPSPIDER_DEAD");
        MR::onCalcGravity(this);
    }

    if (mSpinHitController->execute(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
    }
}

void JumpSpider::endHitBlow() {
    MR::validateClipping(this);
}

void JumpSpider::exeDpdPointed() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::stopBck(this);
        mScaleController->startDpdHitVibration();
        MR::emitEffect(this, "Touch");
    }

    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointedEnd));
    } else {
        mVelocity.zero();
    }
}

void JumpSpider::endDpdPointed() {
    mScaleController->startAnim();
    MR::deleteEffect(this, "Touch");
}

void JumpSpider::exeNoPowerStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "NoPowerStart");
        mVelocity.zero();
    }

    MR::applyVelocityDampAndGravity(this, 0.8f, 0.8f, 0.98f, 0.98f, 1.0f);

    if (MR::isOnGround(this) && MR::isBckStopped(this)) {
        mPosition.set(_A0);
        MR::startSound(this, "SE_EM_JUMPSPIDER_LAND");

        if (isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit))) {
            mPosition.set(_A0);
            mVelocity.zero();
            MR::startBck(this, "NoPowerLand");
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitLand));
        } else {
            MR::startBck(this, "NoPowerLand");
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointedLand));
        }
    }
}

void JumpSpider::exeNoPowerLand() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }

    mPosition.set(_A0);

    if (MR::isBckStopped(this)) {
        if (isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitLand))) {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitWait));
        } else {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvNoPowerEnd));
        }
    }
}

void JumpSpider::exeNoPowerWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "NoPowerWait");
        mVelocity.zero();
    }

    mPosition.set(_A0);
    MR::startLevelSound(this, "SE_EM_LV_SWOON_S");

    if (MR::isGreaterStep(this, ::hNoPowerWaitTime)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvNoPowerEnd));
    }
}

void JumpSpider::exeNoPowerEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "NoPowerEnd");
        MR::startSound(this, "SE_EM_JUMPSPIDER_STEP");
    }

    mPosition.set(_A0);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvWait));
    }
}

void JumpSpider::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(JumpSpider, HostTypeNrvHitBlow)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampDeath)) ||
        isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampFall))) {
        return;
    }

    if (pSender != getSensor("body")) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
            return;
        }

        if (isNerve(GET_NERVE(JumpSpider, HostTypeNrvWait)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvJumpEnd)) ||
            isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitLand)) ||
            isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitWait)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointed)) ||
            isNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointedLand)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvDpdPointedEnd)) ||
            isNerve(GET_NERVE(JumpSpider, HostTypeNrvNoPowerEnd))) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f)) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (MR::sendMsgEnemyAttack(pReceiver, pSender) == true) {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvHitToPlayer));
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool JumpSpider::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(JumpSpider, HostTypeNrvHitBlow)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampDeath)) ||
        isNerve(GET_NERVE(JumpSpider, HostTypeNrvStampFall))) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (!isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit))) {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit));
        }

        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerTrample(msg)) {
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::startSound(this, "SE_EV_JUMPSPIDER_DEAD");

        if (MR::isOnGround(this)) {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvStampDeath));
        } else {
            setNerve(GET_NERVE(JumpSpider, HostTypeNrvStampFall));
        }

        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvHitBlow));
        return true;
    }

    return false;
}

bool JumpSpider::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerKick(msg) &&
        (isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHit)) || isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitLand)) ||
         isNerve(GET_NERVE(JumpSpider, HostTypeNrvStarPieceHitWait)))) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(GET_NERVE(JumpSpider, HostTypeNrvHitBlow));
        return true;
    }

    return false;
}

void JumpSpider::calcAndSetBaseMtx() {
    TPos3f pos;
    MR::calcMtxFromGravityAndZAxis(&pos, this, mGravity, _94);
    MR::setBaseTRMtx(this, pos);
    MR::setBaseScale(this, mScaleController->_C * mScale);
}
