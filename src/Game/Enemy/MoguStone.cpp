#include "Game/Enemy/MoguStone.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

namespace {
    static f32 hAddHeight = 200.0f;
};  // namespace

namespace NrvMoguStone {
    NEW_NERVE(MoguStoneNrvTaken, MoguStone, Taken);
    NEW_NERVE(MoguStoneNrvThrow, MoguStone, Throw);
    NEW_NERVE(MoguStoneNrvFall, MoguStone, Fall);
};  // namespace NrvMoguStone

// Initializer list doesn't match
MoguStone::MoguStone(const char* pName, const char* pModelName) : ModelObj(pName, pModelName, nullptr, -2, -2, -2, false) {
    // Crashes ?
    //_90.set(0, 0, 0, 1);
    _A0.set(0, 0, 0, 1);
    _B0.set(0.0f, 0.0f, 1.0f);
    _BC = 0.0f;
    _C0 = true;

    TVec3f offset(0.0f, 0.0f, 0.0f);
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

bool MoguStone::isTaken() {
    return isNerve(&NrvMoguStone::MoguStoneNrvTaken::sInstance);
}

void MoguStone::exeTaken() {
}
