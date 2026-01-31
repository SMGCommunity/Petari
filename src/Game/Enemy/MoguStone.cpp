#include "Game/Enemy/MoguStone.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

namespace {
    const static f32 hAddHeight = 200.0f;
};  // namespace

namespace NrvMoguStone {
    NEW_NERVE(MoguStoneNrvTaken, MoguStone, Taken);
    NEW_NERVE(MoguStoneNrvThrow, MoguStone, Throw);
    NEW_NERVE(MoguStoneNrvFall, MoguStone, Fall);
};  // namespace NrvMoguStone

// Initializer list doesn't match?
MoguStone::MoguStone(const char* pName, const char* pModelName) : ModelObj(pName, pModelName, nullptr, -2, -2, -2, false) {
    // Crashes?
    //_90.set< int >(0, 0, 0, 1);
    _A0.set< int >(0, 0, 0, 1);
    _B0.set(0.0f, 0.0f, 1.0f);
    _BC = 0.0f;
    _C0 = true;
}

void MoguStone::init(const JMapInfoIter& rIter) {
    initBinder(40.0f * mScale.y, 0.0f, 0);
    ModelObj::init(rIter);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 32, 80.0f * mScale.y, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvMoguStone::MoguStoneNrvTaken::sInstance);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 50.0f * mScale.y);
    makeActorDead();
}

void MoguStone::appear() {
    LiveActor::appear();
    setNerve(&NrvMoguStone::MoguStoneNrvTaken::sInstance);
}

void MoguStone::emit(bool arg1, const TVec3f& pArg2, const TVec3f& pArg3, f32 arg4) {
    _BC = arg4;

    mPosition.set(pArg2);

    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;

    MR::calcGravity(this);

    TVec3f v1(pArg3);
    v1 -= pArg2;

    if (arg1) {
        MR::onCalcGravity(this);

        MR::vecKillElement(v1, mGravity, &_B0);
        _C0 = true;
    } else {
        MR::offCalcGravity(this);
        _B0.set(v1);
        _C0 = false;
    }

    MR::normalizeOrZero(&_B0);
    setNerve(&NrvMoguStone::MoguStoneNrvThrow::sInstance);
}

void MoguStone::doBehavior() {
    if (MR::isFirstStep(this)) {
        _A0.set(0, 0, 0, 1);
    }

    if (_C0) {
        MR::vecKillElement(_B0, mGravity, &_B0);
        MR::normalizeOrZero(&_B0);
    }

    f32 scale = _BC;
    TVec3f v1(_B0);
    v1.x *= scale;
    v1.y *= scale;
    v1.z *= scale;
    mVelocity.set(v1);

    TVec3f up(-mGravity);
    MR::makeQuatUpFront(&_90, up, _B0);

    if (MR::isGreaterStep(this, 100)) {
        MR::emitEffect(this, "Break");
        startBreakSound();
        kill();
    }
}

void MoguStone::startBreakSound() {
    MR::startSound(this, "SE_EM_MOGU_SPANA_BREAK", -1, -1);
}

void MoguStone::startThrowLevelSound() {
    MR::startLevelSound(this, "SE_EM_LV_MOGU_SPANA_FLY", -1, -1, -1);
}

void MoguStone::exeThrow() {
    startThrowLevelSound();
    doBehavior();
    PSQUATMultiply(&_A0, &_90, &_90);
    _90.normalize();

    if (MR::isBinded(this)) {
        MR::emitEffect(this, "Break");
        startBreakSound();
        kill();
        return;
    }

    if (MR::isGreaterStep(this, 100)) {
        setNerve(&NrvMoguStone::MoguStoneNrvFall::sInstance);
    }
}

void MoguStone::exeFall() {
    PSQUATMultiply(&_A0, &_90, &_90);
    _90.normalize();
    MR::applyVelocityDampAndGravity(this, 2.0f, 0.8f, 0.98f, 0.98f, 1.0f);

    if (MR::isBinded(this) || MR::isGreaterStep(this, 60)) {
        MR::emitEffect(this, "Break");
        startBreakSound();
        kill();
    }
}

bool MoguStone::isTaken() {
    return isNerve(&NrvMoguStone::MoguStoneNrvTaken::sInstance);
}

void MoguStone::attackSensor(HitSensor* pSensor1, HitSensor* pSensor2) {
    if (pSensor1 == getSensor("body") && MR::isSensorPlayer(pSensor2) && MR::sendMsgEnemyAttack(pSensor2, pSensor1) == true) {
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
        kill();
    }
}

void MoguStone::exeTaken() {
}

bool MoguStone::receiveMsgPlayerAttack(u32 msg, HitSensor* pSensor1, HitSensor* pSensor2) {
    if (!isNerve(&NrvMoguStone::MoguStoneNrvThrow::sInstance)) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg) || MR::isMsgStarPieceAttack(msg)) {
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
        kill();
        return true;
    }

    return false;
}

void MoguStone::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;

    mtx.setRotateQuaternionInlineAndTrans(_90, mPosition);

    MR::setBaseTRMtx(this, mtx);
}

ThrowingIce::ThrowingIce(const char* pName) : MoguStone(pName, "IceManIce"), mPath(nullptr) {
    mPath = new ParabolicPath();
    mScale.x = 1.3f;
    mScale.y = 1.3f;
    mScale.z = 1.3f;
}

void ThrowingIce::init(const JMapInfoIter& rIter) {
    initBinder(60.0f * mScale.y, 0.0f, 0);
    ModelObj::init(rIter);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 32, 80.0f * mScale.y, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvMoguStone::MoguStoneNrvTaken::sInstance);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 90.0f * mScale.y);
    makeActorDead();
}

void ThrowingIce::emitIce(const TVec3f& arg1, const TVec3f& arg2, f32 arg3, const TVec3f& arg4) {
    mPath->initFromUpVectorAddHeight(arg1, arg2, -arg4, hAddHeight);
    _BC = arg3;

    TVec3f v2(arg2);
    v2 -= arg1;
    MR::vecKillElement(v2, mGravity, &_B0);
    MR::normalizeOrZero(&_B0);
    setNerve(&NrvMoguStone::MoguStoneNrvThrow::sInstance);
    MR::emitEffect(this, "Smoke");
}

void ThrowingIce::doBehavior() {
    if (MR::isFirstStep(this)) {
        TVec3f v1;
        PSVECCrossProduct(_B0, mGravity, &v1);

        f32 one_eighth = 0.125f;

        // Matches, except v3 is actually _A0 which is a TVec4f that somehow uses TVec3f::scale(f32, TVec3f)
        TVec3f v3;
        v3.scale(sin(one_eighth), v1);
        _A0.w = cos(one_eighth);
    }

    f32 rate = MR::calcNerveRate(this, 101);
    TVec3f v2;
    mPath->calcPosition(&v2, rate);
    TVec3f v3(v2);
    v3 -= mPosition;
    mVelocity.set(v3);
}

void ThrowingIce::attackSensor(HitSensor* pSensor1, HitSensor* pSensor2) {
    if (pSensor1 == getSensor("body") && MR::isSensorPlayer(pSensor2) &&
        (MR::isPlayerElementModeIce() ? MR::sendMsgEnemyAttackStrong(pSensor2, pSensor1) : MR::sendMsgEnemyAttackFreeze(pSensor2, pSensor1))) {
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
        MR::deleteEffect(this, "Smoke");
        kill();
    }
}

bool ThrowingIce::receiveMsgPlayerAttack(u32 msg, HitSensor* pSensor1, HitSensor* pSensor2) {
    if (!isNerve(&NrvMoguStone::MoguStoneNrvThrow::sInstance)) {
        return false;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    MoguStone::receiveMsgPlayerAttack(msg, pSensor1, pSensor2);

    return false;
}

void ThrowingIce::startBreakSound() {
    MR::startSound(this, "SE_BM_ICEMERAKING_STONE_BREAK", -1, -1);
}

void ThrowingIce::startThrowLevelSound() {
}
