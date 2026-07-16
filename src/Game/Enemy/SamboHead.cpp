#include "Game/Enemy/SamboHead.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

namespace NrvSamboHead {
    NEW_NERVE(HostTypeNrvWaitUnderGround, SamboHead, WaitUnderGround);
    NEW_NERVE(HostTypeNrvHide, SamboHead, Hide);
    NEW_NERVE(HostTypeNrvAppear, SamboHead, Appear);
    NEW_NERVE(HostTypeNrvAppearImmediately, SamboHead, Appear);
    NEW_NERVE(HostTypeNrvWait, SamboHead, Wait);
    NEW_NERVE(HostTypeNrvPrePursue, SamboHead, PrePursue);
    NEW_NERVE(HostTypeNrvPursue, SamboHead, Pursue);
    NEW_NERVE(HostTypeNrvPursueEnd, SamboHead, PursueEnd);
    NEW_NERVE(HostTypeNrvHitToPlayer, SamboHead, HitToPlayer);
    NEW_NERVE(HostTypeNrvStarPieceHit, SamboHead, StarPieceHit);
    NEW_NERVE_ONEND(HostTypeNrvStampFall, SamboHead, StampFall, StampFall);
    NEW_NERVE_ONEND(HostTypeNrvStampDeath, SamboHead, StampDeath, StampDeath);
    NEW_NERVE_ONEND(HostTypeNrvHitBlow, SamboHead, HitBlow, HitBlow);
    NEW_NERVE(HostTypeNrvBuryDeath, SamboHead, BuryDeath);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, SamboHead, BindStarPointer, BindStarPointer);
    NEW_NERVE(HostTypeNrvBindStarPointerEnd, SamboHead, BindStarPointerEnd);
};  // namespace NrvSamboHead

SamboHead::SamboHead(const char* pName) : LiveActor(pName), mSpinCtrl(), mScaleController(), mBindStarPointer(), _98(0, 0, 1), _A4(0, 0, 0) {
    _B0 = 0;
}

void SamboHead::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
        }

        MR::useStageSwitchReadA(this, rIter);
        s32 arg0 = -1;
        MR::getJMapInfoArg0NoInit(rIter, &arg0);

        if (arg0 == -1) {
            _B0 = false;
        } else {
            _B0 = true;
        }
    }

    _A4.set< f32 >(mPosition);
    initModelManagerWithAnm("SamboHead", nullptr, false);
    TMtx34f mtx;
    mtx.setInline(getBaseMtx());
    _98.set< f32 >(mtx(0, 1), mtx(0, 2), mtx(0, 3));
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    f32 scale_y = mScale.y;
    initBinder((100.0f * mScale.y), (100.0f * mScale.y), 0);
    TVec3f v17;
    v17.x = 0.0f;
    v17.y = 50.0f;
    v17.z = 0.0f;
    MR::initStarPointerTarget(this, 100.0f, v17);
    initHitSensor(1);
    TVec3f v16;
    v16.x = 0.0f;
    v16.y = 100.0f * scale_y;
    v16.z = 0.0f;
    MR::addHitSensorEnemy(this, "body", 0x20, v16.y, v16);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    initNerve(GET_NERVE(SamboHead, HostTypeNrvWaitUnderGround));
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, (60.0f * mScale.y));
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    mScaleController = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);

    if (MR::isValidSwitchAppear(this)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }

    mSpinCtrl = new SpinHitController(this, 3, false);
}

void SamboHead::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE(SamboHead, HostTypeNrvAppearImmediately));
}

void SamboHead::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void SamboHead::control() {
    mScaleController->updateNerve();

    if (!isNerve(GET_NERVE(SamboHead, HostTypeNrvStampFall)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvStampDeath)) &&
        !isNerve(GET_NERVE(SamboHead, HostTypeNrvHitBlow)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvBuryDeath)) &&
        MR::isBindedGroundSinkDeath(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvBuryDeath));
        return;
    }

    if (!isNerve(GET_NERVE(SamboHead, HostTypeNrvWait)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvPrePursue)) &&
        !isNerve(GET_NERVE(SamboHead, HostTypeNrvPursue)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvPursueEnd)) &&
        !isNerve(GET_NERVE(SamboHead, HostTypeNrvHitToPlayer))) {
        return;
    }

    if (mBindStarPointer->tryStartPointBind()) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvBindStarPointer));
    }
}

void SamboHead::exeWaitUnderGround() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "AppearMini");
        mPosition.set< f32 >(_A4);
        MR::invalidateShadow(this, nullptr);
        getSensor("body")->invalidate();
    }

    if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
        mVelocity.zero();
        f32 dist = MR::calcDistanceToPlayer(this);

        if (MR::isGreaterStep(this, 120)) {
            if (dist < 800.0f) {
                getSensor("body")->validate();
                setNerve(GET_NERVE(SamboHead, HostTypeNrvAppear));
            }
        }
    }
}

void SamboHead::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        mVelocity.zero();
    }

    mVelocity.zero();

    if (MR::isStep(this, 30)) {
        MR::invalidateShadow(this, nullptr);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvWaitUnderGround));
    }
}

void SamboHead::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");

        if (isNerve(GET_NERVE(SamboHead, HostTypeNrvAppearImmediately))) {
            MR::setBckFrame(this, 45.0f);
        }
    }

    mVelocity.zero();

    if (MR::isStep(this, 63)) {
        MR::validateShadow(this, nullptr);
    }

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvPrePursue));
    }
}

void SamboHead::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);
    f32 dist = MR::calcDistanceToPlayer(this);

    if (800.0f < dist && !_B0) {
        if (MR::isGreaterStep(this, 120)) {
            setNerve(GET_NERVE(SamboHead, HostTypeNrvHide));
        }
    } else {
        if (dist < 500.0f || _B0) {
            TVec3f v4;
            MR::calcVecToPlayerH(&v4, this, nullptr);

            if (MR::turnVecToVecRadian(&_98, _98, v4, 0.05f, mGravity) < 0.015f) {
                if (_B0) {
                    setNerve(GET_NERVE(SamboHead, HostTypeNrvPursue));
                } else {
                    setNerve(GET_NERVE(SamboHead, HostTypeNrvPrePursue));
                }
            }
        } else {
            TVec3f v3;
            MR::calcVecToPlayerH(&v3, this, nullptr);
            MR::turnVecToVecRadian(&_98, _98, v3, 0.05f, mGravity);
        }
    }
}

void SamboHead::exePrePursue() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Find");
    }

    TVec3f v2;
    MR::calcVecToPlayerH(&v2, this, nullptr);
    MR::turnVecToVecRadian(&_98, _98, v2, 0.05f, mGravity);

    if (MR::isActionEnd(this) && MR::isOnGround(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvPursue));
        MR::startSound(this, "SE_EM_SAMBOHEAD_HOP_LAND");
    } else {
        MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
        MR::killVelocityOnGroundH(this);
    }
}

void SamboHead::exePursue() {
    TVec3f v3;
    MR::calcVecToPlayerH(&v3, this, nullptr);

    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Run");
        mVelocity.set< f32 >(_98 * 7.0f);
        MR::addVelocityJump(this, 25.0f);
    }

    MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);
    MR::turnVecToVecRadian(&_98, _98, v3, 0.05f, mGravity);

    if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvPursueEnd));
        MR::startSound(this, "SE_EM_SAMBOHEAD_HOP_LAND");
    }
}

void SamboHead::exePursueEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Land");
    }

    mVelocity.zero();

    if (MR::isActionEnd(this)) {
        if (MR::calcDistanceToPlayer(this) > 1000.0f) {
            setNerve(GET_NERVE(SamboHead, HostTypeNrvWait));
        } else {
            setNerve(GET_NERVE(SamboHead, HostTypeNrvPursue));
        }
    }
}

void SamboHead::exeHitToPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hit");
        MR::startSound(this, "SE_EM_SAMBOHEAD_ATTACK");
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);

    if (MR::isGreaterStep(this, 58)) {
        TVec3f v2;
        MR::calcVecToPlayerH(&v2, this, nullptr);
        MR::turnVecToVecRadian(&_98, _98, v2, 0.05f, mGravity);
    }

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvWait));
    }
}

void SamboHead::exeStarPieceHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StarPieceHit");
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S");
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvWait));
    }
}

void SamboHead::exeStampFall() {
    if (MR::isFirstStep(this)) {
        mVelocity.set< f32 >(mGravity * 30.0f);
        MR::invalidateClipping(this);
    }

    if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvStampDeath));
    }
}

void SamboHead::exeStampDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Press");
        MR::invalidateClipping(this);
    }

    MR::killVelocityOnGroundH(this);

    if (MR::isGreaterStep(this, 35)) {
        kill();
        MR::appearCoinPop(this, mPosition, 1);
    }
}

void SamboHead::exeHitBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Blow");
        MR::startBlowHitSound(this);
        getSensor("body")->invalidate();
    }

    if (mSpinCtrl->execute(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        getSensor("body")->validate();
    }
}

void SamboHead::exeBuryDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Sink");
        mVelocity.zero();
    }

    mVelocity.zero();

    if (MR::isActionEnd(this)) {
        kill();
    }
}

void SamboHead::exeBindStarPointer() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
    }

    MR::startDPDFreezeLevelSound(this);
    if (MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(SamboHead, HostTypeNrvBindStarPointerEnd))) {
        return;
    }
}

void SamboHead::exeBindStarPointerEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, 1.5f, 0.80f, 0.98f, 0.98f, 1.0f);
    MR::killVelocityOnGroundH(this);

    if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(SamboHead, HostTypeNrvWait));
    }
}

// this was laid out in some other way
void SamboHead::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("body")) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgPush(pReceiver, pSender);
        }

        return;
    }

    if (MR::isPlayerHipDropFalling()) {
        return;
    }

    if (MR::isPlayerHipDropLand()) {
        return;
    }

    if (isNerve(GET_NERVE(SamboHead, HostTypeNrvPursue)) || isNerve(GET_NERVE(SamboHead, HostTypeNrvPrePursue)) ||
        isNerve(GET_NERVE(SamboHead, HostTypeNrvPursueEnd)) || isNerve(GET_NERVE(SamboHead, HostTypeNrvWait))) {
        HitSensor* sensor = getSensor("body");

        if (MR::isPlayerExistUp(this, sensor->mRadius, 0.25f)) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (MR::sendMsgEnemyAttack(pReceiver, pSender) == 1) {
            setNerve(GET_NERVE(SamboHead, HostTypeNrvHitToPlayer));
            return;
        }

        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (isNerve(GET_NERVE(SamboHead, HostTypeNrvAppear)) && MR::isGreaterStep(this, 45)) {
        HitSensor* sensor = getSensor("body");

        if (MR::isPlayerExistUp(this, sensor->mRadius, 0.25f)) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (MR::sendMsgEnemyAttack(pReceiver, pSender) != 1) {
            MR::sendMsgPush(pReceiver, pSender);
        }

        return;
    }

    if (!isNerve(GET_NERVE(SamboHead, HostTypeNrvWaitUnderGround)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvHide)) &&
        !isNerve(GET_NERVE(SamboHead, HostTypeNrvStampFall)) && !isNerve(GET_NERVE(SamboHead, HostTypeNrvStampDeath)) &&
        !isNerve(GET_NERVE(SamboHead, HostTypeNrvHitBlow))) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

// SamboHead::receiveMsgPlayerAttack
// SamboHead::receiveMsgPush
// SamboHead::receiveOtherMsg

void SamboHead::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _98);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale;
    scale.mul(mScaleController->_C, mScale);
    MR::setBaseScale(this, scale);
}

void SamboHead::endBindStarPointer() {
    mBindStarPointer->kill();
}

void SamboHead::endHitBlow() {
    MR::validateClipping(this);
}

void SamboHead::endStampDeath() {
    MR::validateClipping(this);
}

void SamboHead::endStampFall() {
    MR::validateClipping(this);
}

SamboHead::~SamboHead() {
}
