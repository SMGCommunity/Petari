#include "Game/Screen/SuddenDeathMeter.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvSuddenDeathMeter {
    NEW_NERVE(SuddenDeathMeterNrvAppear, SuddenDeathMeter, Appear);
    NEW_NERVE(SuddenDeathMeterNrvWait, SuddenDeathMeter, Wait);
    NEW_NERVE(SuddenDeathMeterNrvPowerUp, SuddenDeathMeter, PowerUp);
    NEW_NERVE(SuddenDeathMeterNrvMeterMove, SuddenDeathMeter, MeterMove);
    NEW_NERVE(SuddenDeathMeterNrvCounterMove, SuddenDeathMeter, CounterMove);
    NEW_NERVE(SuddenDeathMeterNrvBreakMeter, SuddenDeathMeter, BreakMeter);
    NEW_NERVE(SuddenDeathMeterNrvZeroMeter, SuddenDeathMeter, ZeroMeter);
    NEW_NERVE(SuddenDeathMeterNrvZeroMeterBreak, SuddenDeathMeter, ZeroMeterBreak);
}    // namespace NrvSuddenDeathMeter

SuddenDeathMeter::SuddenDeathMeter(const char *a1, const char *a2) :
    LayoutActor(a1, true),
    mCountUpPaneRumbler(nullptr),
    mFollowPosW(0.0f, 0.0f),
    _2C(0.0f, 0.0f),
    mCount(1),
    _38(0.0f),
    _3C(1.0f)
{
    MR::connectToSceneLayout(this);
    initLayoutManager(a2, 3);
}

void SuddenDeathMeter::init(const JMapInfoIter &rIter) {
    MR::createAndAddPaneCtrl(this, "HitPointMeter", 2);
    MR::createAndAddPaneCtrl(this, "HitPointNumber", 1);
    MR::createAndAddPaneCtrl(this, "Font", 2);
    MR::createAndAddPaneCtrl(this, "HitPointW", 1);
    MR::setFollowPos(&mFollowPosW, this, "HitPointW");
    MR::setFollowTypeAdd(this, "HitPointW");
    MR::startAnim(this, "FontPosition2to1", 1);
    MR::startAnim(this, "ShowHide", 2);
    MR::setAnimFrameAndStop(this, 20.0f, 2);
    setCountAnimFrame();
    mCountUpPaneRumbler = new CountUpPaneRumbler(this, "HitPointNumber");
    initNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvWait::sInstance);
    initEffectKeeper(0, nullptr, nullptr);
    appear();
}

inline float clamp(float x, float min, float max) {
    return x < min ? min : (x > max ? max : x);
}

void SuddenDeathMeter::control() {
    if (isNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvAppear::sInstance)) {
        _38 = 1.0f;
    }
    _38 = clamp(_38, 0.3f, 1.2f);
    MR::setAnimFrameAndStop(this, MR::normalize(_38, 0.0f, 1.0f) * _3C * 20.0f, 2);
    mCountUpPaneRumbler->update();
}

void SuddenDeathMeter::requestActive() {
    if (MR::isDead(this)) {
        appear();
        mCountUpPaneRumbler->reset();
        MR::showLayout(this);
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvWait::sInstance);
    }
}

void SuddenDeathMeter::requestDeactivate() {
    if (!MR::isDead(this)) {
        kill();
    }
}

void SuddenDeathMeter::requestPowerUp() {
    setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvPowerUp::sInstance);
}

void SuddenDeathMeter::requestForceAppear() {
    requestActive();
    setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvAppear::sInstance);
}

void SuddenDeathMeter::requestPlayerMoving() {}

void SuddenDeathMeter::exeAppear() {
    if (MR::isFirstStep(this)) {
        mFollowPosW.x = mFollowPosW.y = 0.0f;
        setCountAnimFrame();
    }
    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvWait::sInstance);
    }
}

void SuddenDeathMeter::exeWait() {
    if (MR::isFirstStep(this)) {
        setCountAnimFrame();
        mFollowPosW.x = mFollowPosW.y = 0.0f;
    }
}

void SuddenDeathMeter::exePowerUp() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::setAnimFrameAndStop(this, 0.0f, 1);
        setRecoveryCountAnimFrame();
    }
    TVec2f position;
    calcMarioHeadPosition(&position);
    setPowerUpMeterPosition(position);
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvMeterMove::sInstance);
    }
}

// void SuddenDeathMeter::exeMeterMove()

void SuddenDeathMeter::exeCounterMove() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "FontPosition1to2", 1);
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", 2);
    }
    if (MR::isAnimStopped(this, 1)) {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvWait::sInstance);
    }
}

void SuddenDeathMeter::exeBreakMeter() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(this, "HitPointMeter", "Break", 0);
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
        MR::startAnim(this, "Damage2", 1);
        mFollowPosW.x = mFollowPosW.y = 0.0f;
    }
    if (MR::isStep(this, 40)) {
        MR::startSystemSE("SE_SY_LIFE_PLATE_DEC", -1, -1);
        MR::startAnim(this, "FontPosition2to1", 1);
        setCountAnimFrame();
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvWait::sInstance);
    }
}

void SuddenDeathMeter::exeZeroMeter() {
    if (MR::isFirstStep(this)) {
        mFollowPosW.x = mFollowPosW.y = 0.0f;
        setCountAnimFrame();
    }
    if (MR::isAnimStopped(this, 1)) {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvZeroMeterBreak::sInstance);
    }
}

void SuddenDeathMeter::exeZeroMeterBreak() {
    if (MR::isFirstStep(this)) {
        mFollowPosW.x = mFollowPosW.y = 0.0f;
    }
    if (MR::isStep(this, 40)) {
        MR::hideLayout(this);
    }
}

void SuddenDeathMeter::initCount(s32 count) {
    mCount = count;
}

void SuddenDeathMeter::setCount(s32 count) {
    s32 prevCount = mCount;
    mCount = count;
    if (count >= prevCount) return;
    if (count == 0) {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvZeroMeter::sInstance);
    }
    else {
        setNerve(&NrvSuddenDeathMeter::SuddenDeathMeterNrvBreakMeter::sInstance);
    }
}

// void SuddenDeathMeter::calcMarioHeadPosition(TVec2f *pos) const;

void SuddenDeathMeter::calcPowerUpMeterBasePosition(TVec2f *pos) const {
    pos->x = MR::getPaneTransX(this, "MoveMeterW");
    pos->y = MR::getPaneTransY(this, "MoveMeterW");
}

// void SuddenDeathMeter::setPowerUpMeterPosition(const TVec2f &pos);

void SuddenDeathMeter::setCountAnimFrame() {
    MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
    MR::startPaneAnim(this, "HitPointMeter", "Count", 0);
    MR::setPaneAnimFrameAndStop(this, "HitPointMeter", mCount, 0);
}

void SuddenDeathMeter::setRecoveryCountAnimFrame() {
    MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
    MR::startPaneAnim(this, "HitPointMeter", "RecoveryCount", 0);
    MR::setPaneAnimFrameAndStop(this, "HitPointMeter", mCount, 0);
}
