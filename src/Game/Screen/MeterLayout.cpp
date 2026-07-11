#include "Game/Screen/MeterLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvMeterLayout {
    NEW_NERVE(MeterLayoutNrvHide, MeterLayout, Hide);
    NEW_NERVE(MeterLayoutNrvAppear, MeterLayout, Appear);
    NEW_NERVE(MeterLayoutNrvWait, MeterLayout, Wait);
    NEW_NERVE(MeterLayoutNrvWaitStart, MeterLayout, WaitStart);
    NEW_NERVE(MeterLayoutNrvPowerUp, MeterLayout, PowerUp);
    NEW_NERVE(MeterLayoutNrvMeterMove, MeterLayout, MeterMove);
    NEW_NERVE(MeterLayoutNrvCounterMove, MeterLayout, CounterMove);
    NEW_NERVE(MeterLayoutNrvRecover, MeterLayout, Recover);
    NEW_NERVE(MeterLayoutNrvDamage, MeterLayout, Damage);
    NEW_NERVE(MeterLayoutNrvBreakMeter, MeterLayout, BreakMeter);
    NEW_NERVE(MeterLayoutNrvZeroMeter, MeterLayout, ZeroMeter);
    NEW_NERVE(MeterLayoutNrvZeroMeterBreak, MeterLayout, ZeroMeterBreak);
}

namespace {
    f32 clamp(f32 v, f32 min, f32 max) {
        if (v < min) {
            return min;
        }

        if (v > max) {
            return max;
        }

        return v;
    }
}

MeterLayout::MeterLayout(const char* pName, const char* pLayoutName) : LayoutActor(pName, true), mCountUpPaneRumbler(nullptr), mFollowPos(0.0f, 0.0f), mFollowPosW(0.0f, 0.0f), _38(0.0f, 0.0f), mCount(3), _48(), _4C(1.0f) {
    MR::connectToSceneLayout(this);
    initLayoutManager(pLayoutName, 3);
}

void MeterLayout::init(const JMapInfoIter& rIter) {
    MR::createAndAddPaneCtrl(this, "HitPointMeter", 2);
    MR::createAndAddPaneCtrl(this, "HitPointNumber", 1);
    MR::createAndAddPaneCtrl(this, "Font", 2);
    MR::createAndAddPaneCtrl(this, "HPMeter", 1);
    MR::setFollowPos(&mFollowPos, this, "HPMeter");
    MR::setFollowTypeAdd(this, "HPMeter");
    MR::createAndAddPaneCtrl(this, "HitPointW", 1);
    MR::setFollowPos(&mFollowPosW, this, "HitPointW");
    MR::setFollowTypeAdd(this, "HitPointW");
    MR::startAnim(this, "FontPosition2to1", 1);
    MR::startAnim(this, "ShowHide", 2);
    MR::setAnimFrameAndStop(this, _48 * _4C * 20.0f, 2);
    setCountAnimFrame();
    mCountUpPaneRumbler = new CountUpPaneRumbler(this, "HitPointNumber");
    initNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance);
    initEffectKeeper(0, nullptr, nullptr);
    appear();
}

// ! regswap - https://decomp.me/scratch/aeADT
void MeterLayout::control() {
    if (isNerve(&NrvMeterLayout::MeterLayoutNrvAppear::sInstance)) {
        _48 = 1.0f;
    }
    else if (MR::isStarPointerPointingPaneForMeterLayout(this, "PicBase", 0, false, nullptr)) {
        _48 -= 0.050000001f;
    }
    else {
        _48 += 0.050000001f;
    }

    _48 = clamp(_48, 0.30000001f, 1.2f);

    f32 v1 = MR::normalize(_48, 0.0f, 1.0f);
    MR::setAnimFrameAndStop(this, _4C * v1 * 20.0f, 2);
    mCountUpPaneRumbler->update();
}

void MeterLayout::requestActive() {
    if (MR::isDead(this)) {
        mCountUpPaneRumbler->reset();

        if (mCount == 3) {
            _4C = 0.0f;
            mFollowPos.x = MR::getEaseInOutValue(0.0f, 1.0f, 0.0f, 1.0f) * 50.0f;
        }
        else {
            _4C = 1.0f;
            mFollowPos.y = 0.0f;
            mFollowPos.x = 0.0f;
        }

        f32 tmp = _4C;
        MR::setAnimFrameAndStop(this, tmp * 20.0f, 2);
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        appear();
        setNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance);
    }
}

void MeterLayout::requestDeactivate() {
    kill();
}

void MeterLayout::requestPowerUp() {
    _4C = 1.0f;
    MR::setAnimFrameAndStop(this, 20.0f * _4C, 2);
    mFollowPos.y = 0.0f;
    mFollowPos.x = 0.0f;
    mFollowPosW.y = 0.0f;
    mFollowPosW.x = 0.0f;
    appear();
    setNerve(&NrvMeterLayout::MeterLayoutNrvPowerUp::sInstance);
}

void MeterLayout::requestForceAppear() {
    if (mCount == 1) {
        requestActive();
        setNerve(&NrvMeterLayout::MeterLayoutNrvAppear::sInstance);
    }
}

void MeterLayout::requestPlayerMoving() {
    if (isNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance)) {
        if (mCount == 3) {
            _4C -= 0.050000001f;
            if (_4C < 0.0f) {
                _4C = 0.0f;
            }
        }
        else {
            _4C = 1.0f;
        }
    }
}

void MeterLayout::requestPlayerStopped() {
    if (isNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance)) {
        if (mCount == 3) {
            _4C += 0.050000001f;
            if (_4C > 1.0f) {
                _4C = 1.0f;
            }
        }
        else {
            _4C = 1.0f;
        }
    }
}

void MeterLayout::initCount(s32 count) {
    mCount = count;
}

void MeterLayout::setCount(s32 count) {
    s32 prevCount = mCount;
    mCount = count;

    if (count < prevCount) {
        if (count == 0) {
            setNerve(&NrvMeterLayout::MeterLayoutNrvZeroMeter::sInstance);
        }
        else if (prevCount >= 4 && count < 4) {
            setNerve(&NrvMeterLayout::MeterLayoutNrvBreakMeter::sInstance);
        }
        else {
            setNerve(&NrvMeterLayout::MeterLayoutNrvDamage::sInstance);
        }
    }
    else if (prevCount < count) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvRecover::sInstance);
    }
}

void MeterLayout::exeHide() {

}

void MeterLayout::exeAppear() {
    if (MR::isFirstStep(this)) {
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        setAnimBase();
        _4C = 1.0f;
    }

    mFollowPos.y = 0.0f;
    mFollowPos.x = 0.0f;

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance);
    }
}

void MeterLayout::exeWaitStart() {
    if (MR::isFirstStep(this)) {
        setAnimBase();
        _4C = 1.0f;
    }

    mFollowPos.y = 0.0f;
    mFollowPos.x = 0.0f;
    mFollowPosW.y = 0.0f;
    mFollowPosW.x = 0.0f;

    if (MR::isGreaterStep(this, 240)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvWait::sInstance);
    }   
}

void MeterLayout::exeWait() {
    if (MR::isFirstStep(this)) {
        setAnimBase();
        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
    }
    
    mFollowPos.x = MR::getEaseInOutValue(_4C, 1.0f, 0.0f, 1.0f) * 50.0f;
    mFollowPosW.y = 0.0f;
    mFollowPosW.x = 0.0f;
}

// ! minus operator gets inlined - https://decomp.me/scratch/wWkjw
void MeterLayout::exePowerUp() {
    TVec2f headPos;
    TVec2f basePos;

    mFollowPos.y = 0.0f;
    mFollowPos.x = 0.0f;

    calcMarioHeadPosition(&headPos);
    calcPowerUpMeterBasePosition(&basePos);
    mFollowPosW.set(headPos - basePos);

    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Danger1", 1);
        MR::setAnimFrameAndStop(this, 0.0f, 1);
        setRecoveryCountAnimFrame();
        _4C = 1.0f;
    }

    if (mCount < 6 && MR::isGreaterStep(this, 0) && MR::isIntervalStep(this, 5)) {
        mCount++;
        setRecoveryCountAnimFrame();

        if (mCount >= 4) {
            MR::setTextBoxNumberRecursive(this, "HitPointNumber", 3);
        }
        else {
            mCountUpPaneRumbler->start();
            MR::startPaneAnim(this, "Font", "Flash", 0);
        }
    }

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvMeterMove::sInstance);
    }
}

void MeterLayout::exeMeterMove() {
    if (MR::isFirstStep(this)) {
        _38 = mFollowPosW;
        _4C = 1.0f;
    }

    f32 easeIn = MR::calcNerveEaseInRate(this, 25.0f);
    mFollowPosW = _38 * (1.0f - easeIn);

    if (MR::isGreaterStep(this, 25)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvCounterMove::sInstance);
    }
}

void MeterLayout::exeCounterMove() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "FontPosition1to2", 1);
        _4C = 1.0f;
    }

    if (MR::isStep(this, 0)) {
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", 4);
    }

    if (MR::isStep(this, 5)) {
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", 5);
    }

    if (MR::isStep(this, 10)) {
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", 6);
    }

    if (MR::isAnimStopped(this, 1)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvWaitStart::sInstance);
    }
}

void MeterLayout::exeRecover() {
    if (MR::isFirstStep(this)) {
        mCountUpPaneRumbler->start();
        MR::startPaneAnim(this, "Font", "Flash", 0);
        setCountAnimFrame();
        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        _4C = 1.0f;
    }

    if (MR::isPaneAnimStopped(this, "Font", 0)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvWaitStart::sInstance);
    }
}

void MeterLayout::exeDamage() {
    if (MR::isFirstStep(this)) {
        if (mCount < 3) {
            MR::startAnim(this, "Damage2", 1);
        }
        else {
            MR::startAnim(this, "Damage1", 1);
        }

        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        setCountAnimFrame();
        _4C = 1.0f;
    }

    if (MR::isAnimStopped(this, 1)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvWaitStart::sInstance);
    }
}

void MeterLayout::exeBreakMeter() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(this, "HitPointMeter", "Break", 0);
        MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
        MR::startAnim(this, "Damage2", 1);
        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        _4C = 1.0f;
    }

    if (MR::isStep(this, 40)) {
        MR::emitEffect(this, "Break");
        MR::startSystemSE("SE_SY_LIFE_PLATE_DEC");
        MR::startAnim(this, "FontPosition2to1", 1);
        setCountAnimFrame();
        setNerve(&NrvMeterLayout::MeterLayoutNrvWaitStart::sInstance);
    }
}

void MeterLayout::exeZeroMeter() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Damage1", 1);
        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
        setCountAnimFrame();
        _4C = 1.0f;
    }

    if (MR::isAnimStopped(this, 1)) {
        setNerve(&NrvMeterLayout::MeterLayoutNrvZeroMeterBreak::sInstance);
    }
}

void MeterLayout::exeZeroMeterBreak() {
    if (MR::isFirstStep(this)) {
        mFollowPos.y = 0.0f;
        mFollowPos.x = 0.0f;
        mFollowPosW.y = 0.0f;
        mFollowPosW.x = 0.0f;
    }

    if (MR::isStep(this, 40)) {
        MR::emitEffect(this, "Break123");
        MR::hideLayout(this);
    }
}

// ! regswap - https://decomp.me/scratch/LdV5o
void MeterLayout::calcMarioHeadPosition(TVec2f* pHeadPosition) const {
    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, *MR::getPlayerCenterPos());
    f32 radius = MR::calcPointRadius2D(*MR::getPlayerCenterPos(), 120.0f);

    screenPos.x += 32.0f;
    screenPos.y = screenPos.y - radius + (-38.0f);

    pHeadPosition->x = screenPos.x;
    pHeadPosition->y = screenPos.y;
}

void MeterLayout::calcPowerUpMeterBasePosition(TVec2f* pBasePosition) const {
    pBasePosition->x = MR::getPaneTransX(this, "MoveMeterW");
    pBasePosition->y = MR::getPaneTransY(this, "MoveMeterW");
}

void MeterLayout::setAnimBase() {
    setCountAnimFrame();

    if (mCount < 3) {
        MR::startAnim(this, "Danger1", 1);
    }
    else {
        MR::startAnim(this, "Danger2", 1);
    }

    if (mCount <= 1) {
        MR::setAnimRate(this, 4.0f, 1);
    }
    else if (mCount <= 2) {
        MR::setAnimRate(this, 2.0f, 1);
    }
    else {
        MR::setAnimFrameAndStop(this, 0.0f, 1);
    }
}

void MeterLayout::setCountAnimFrame() {
    MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
    MR::startPaneAnim(this, "HitPointMeter", "Count", 0);
    MR::setPaneAnimFrameAndStop(this, "HitPointMeter", mCount, 0);
}

void MeterLayout::setRecoveryCountAnimFrame() {
    MR::setTextBoxNumberRecursive(this, "HitPointNumber", mCount);
    MR::startPaneAnim(this, "HitPointMeter", "RecoveryCount", 0);
    MR::setPaneAnimFrameAndStop(this, "HitPointMeter", mCount, 0);
}
