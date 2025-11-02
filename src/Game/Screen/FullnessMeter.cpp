#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/FullnessMeter.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvFullnessMeter {
    NEW_NERVE(FullnessMeterNrvAppear, FullnessMeter, Appear);
    NEW_NERVE(FullnessMeterNrvWait, FullnessMeter, Wait);
    NEW_NERVE(FullnessMeterNrvEnd, FullnessMeter, End);
};

FullnessMeter::FullnessMeter(LiveActor *pHost, s32 a2, s32 a3) :
    LayoutActor("満腹計", true),
    _20(a3),
    _24(a2),
    mHost(pHost)
{
    MR::connectToSceneTalkLayout(this);
    initLayoutManager("StarPieceTargetMeter", 1);
    MR::createAndAddPaneCtrl(this, "TargetMeter", 1);
    MR::createAndAddPaneCtrl(this, "PicFrameShine", 1);
    mTargetCounter = MR::createSimpleLayoutTalkParts("満腹カウンター", "StarPieceTargetCounter", 1);
    MR::setTextBoxNumberRecursive(mTargetCounter, "NumberMax", _24);
    setNumber(a3);
    initNerve(&NrvFullnessMeter::FullnessMeterNrvAppear::sInstance);
}

void FullnessMeter::init(const JMapInfoIter &rIter) {
    MR::startPaneAnim(this, "TargetMeter", "Count", 0);
    MR::setPaneAnimFrameAndStop(this, "TargetMeter", 0.0f, 0);
}

void FullnessMeter::control() {
    //const TVec2f& pos = ;
    setTrans(MR::getStarPointerScreenPositionOrEdge(0));
}

void FullnessMeter::setNumber(s32 num) {
    _20 = num;
    s32 frame = 128 - (u16)(128.0f * (1.0f - ((f32)num / (f32)_24)));

    MR::startPaneAnim(this, "TargetMeter", "Count", 0);
    MR::setPaneAnimFrameAndStop(this, "TargetMeter", frame, 0);
    MR::startPaneAnim(this, "PicFrameShine", "Shine", 0);

    if (MR::isAnimStopped(mTargetCounter, 0)) {
        MR::startAnim(mTargetCounter, "Flash", 0);
    }

    MR::setTextBoxNumberRecursive(mTargetCounter, "Number", _20);
}

void FullnessMeter::requestAppear() {
    if (MR::isDead(this)) {
        appear();
        mTargetCounter->appear();
        MR::showScreen(this);
        setNerve(&NrvFullnessMeter::FullnessMeterNrvAppear::sInstance);
        MR::startStarPointerModeStarPieceTarget(this);
        MR::startSystemSE("SE_SY_TICOFAT_POINT", -1, -1);
    }
}

void FullnessMeter::requestDisappear() {
    if (MR::isDead(this) || isNerve(&NrvFullnessMeter::FullnessMeterNrvEnd::sInstance)) {
        return;
    }
    else {
        setNerve(&NrvFullnessMeter::FullnessMeterNrvEnd::sInstance);
        MR::hideScreen(this);
        MR::startAnim(mTargetCounter, "End", 0);
        MR::endStarPointerMode(this);
    }
}

void FullnessMeter::pauseOff() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mTargetCounter);
}

void FullnessMeter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mTargetCounter, "Appear", 0);
    }

    if (MR::isAnimStopped(mTargetCounter, 0)) {
        setNerve(&NrvFullnessMeter::FullnessMeterNrvWait::sInstance);
    }
}

void FullnessMeter::exeWait() {
    if (MR::isFirstStep(this)) {
        
    }
}

void FullnessMeter::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mTargetCounter, "End", 0);
    }

    if (MR::isAnimStopped(mTargetCounter, 0)) {
        kill();
        mTargetCounter->kill();
    }
}
