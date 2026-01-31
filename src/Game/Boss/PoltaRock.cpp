#include "Game/Boss/PoltaRock.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

namespace NrvPoltaRock {
    NEW_NERVE(PoltaRockNrvFloat, PoltaRock, Float);
    NEW_NERVE(PoltaRockNrvSign, PoltaRock, Sign);
    NEW_NERVE(PoltaRockNrvMove, PoltaRock, Move);
};

PoltaRock::PoltaRock(const char* pName)
    : LiveActor(pName), mOwner(nullptr), _90(0.0f, 0.0f, 0.0f, 1.0f), _A0(0.0f, 0.0f, 1.0f), _AC(0.0f, 0.0f, 0.0f), mType(0) {}

void PoltaRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PoltaRock", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "Body", 8, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(120.0f, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    initNerve(&NrvPoltaRock::PoltaRockNrvFloat::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void PoltaRock::kill() {
    MR::startSound(this, "SE_OJ_POLTA_ROCK_BREAK", -1, -1);
    LiveActor::kill();
    switch (mType) {
        case 2:
        MR::emitEffect(this, "BreakYellow");
        break;
        case 0:
        MR::emitEffect(this, "BreakWhite");
        break;
    }
    mOwner = nullptr;
}

void PoltaRock::control() {}

void PoltaRock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _90);
    MR::setBaseScale(this, mScale);
}

void PoltaRock::start(Polta* pPolta, const TVec3f& rPosition, const TVec3f& rVec2) {
    mOwner = pPolta;
    mPosition.set(rPosition);
    _AC.set(rVec2);
    setNerve(&NrvPoltaRock::PoltaRockNrvFloat::sInstance);
    appear();
    MR::offBind(this);
}

void PoltaRock::setColorWhite() {
    mType = 0;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 0.0f);
}

void PoltaRock::setColorBlack() {
    mType = 1;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 0.0f);
}

void PoltaRock::setColorYellow() {
    mType = 2;
    MR::startBva(this, "Color");
    MR::setBvaFrameAndStop(this, 2.0f);
}

void PoltaRock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isEnableAttack()) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                generateKill();
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool PoltaRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    if (MR::isMsgPlayerSpinAttack(msg) && isEnableAttack()) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    return false;
}

bool PoltaRock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && isEnableExplosion()) {
        MR::zeroVelocity(this);
        generateKill();
        return true;
    }
    return false;
}

void PoltaRock::exeFloat() {
    TVec3f v3;

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POLTA_ROCK_FLOAT_ST", -1, -1);
        MR::startAction(this, "Froat");
        MR::emitEffect(this, "Appear");
    }
    MR::startLevelSound(this, "SE_OJ_LV_POLTA_ROCK_FLOAT", -1, -1, -1);
    PSMTXMultVec(getOwner()->getBaseMtx(), &_AC, v3);
    MR::addVelocityMoveToTarget(this, v3, 0.0f, 1.2f, 0.0f, 200.0f);
    MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
    MR::attenuateVelocity(this, 0.92f);
    if (MR::isGreaterStep(this, 90)) {
        setNerve(&NrvPoltaRock::PoltaRockNrvSign::sInstance);
        MR::onBind(this);
    }
}

void PoltaRock::exeSign() {
    TVec3f v3;

    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Sign");
    }
    PSMTXMultVec(getOwner()->getBaseMtx(), &_AC, v3);
    MR::addVelocityMoveToTarget(this, v3, 0.0f, 1.2f, 0.0f, 200.0f);
    MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
    MR::attenuateVelocity(this, 0.92f);
    if (MR::isGreaterStep(this, 90)) {
        setNerve(&NrvPoltaRock::PoltaRockNrvMove::sInstance);
        MR::onBind(this);
    }
}

void PoltaRock::exeMove() {
    if (MR::isFirstStep(this)) {
        _A0.set(*MR::getPlayerCenterPos() - mPosition);
        MR::normalizeOrZero(&_A0);
        MR::startSound(this, "SE_OJ_POLTA_ROCK_FLY", -1, -1);
        MR::zeroVelocity(this);
    }

    if (MR::isLessStep(this, 30)) {
        mVelocity.addInLine(_A0.multInLine(3.0f * MR::calcNerveRate(this, 30)));
    }
    MR::addVelocityToGravity(this, -0.02f);
    MR::attenuateVelocity(this, 0.99f);
    MR::rotateQuatRollBall(&_90, mVelocity, mGravity.negateInline(), 120.0f);
    if (MR::isBinded(this)) {
        MR::sendMsgEnemyAttackToBindedSensor(this, getSensor("Body"));
        MR::tryRumbleDefaultHit(this, 0);
        generateKill();
        return;
    }
    
    if (MR::isGreaterStep(this, 300)) {
        kill();
    }
}

void PoltaRock::generateKill() {
    switch (mType) {
        case 1:
            PoltaFunction::appearBombTeresaNormal(getOwner(), mPosition, TVec3f(0.0f,0.0f,0.0f));
            break;
        case 2:
            MR::appearCoinPop(getOwner(), mPosition, 1);
            break;
    }
    kill();
}

bool PoltaRock::isEnableAttack() const {
    return isNerve(&NrvPoltaRock::PoltaRockNrvMove::sInstance);
}

bool PoltaRock::isEnableExplosion() const {
    if (isNerve(&NrvPoltaRock::PoltaRockNrvFloat::sInstance) && MR::isLessStep(this, 200)) {
        return false;
    }
    return true;
}

PoltaRock::~PoltaRock() {}
