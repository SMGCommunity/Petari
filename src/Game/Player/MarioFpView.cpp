#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Enemy/KariKariDirector.hpp"

namespace {
    bool lbl_806B62B0;
}

MarioFpView::MarioFpView(MarioActor *pActor) : MarioState(pActor, 0x12), _12(0) {
    _14 = false;
    mBlurOffset = 0.0f;
}

bool MarioFpView::start() {
    MR::startSubjectiveCamera(-1);
    changeAnimation("見る", (char*)nullptr);
    ::lbl_806B62B0 = true;
    _12 = 0;
    _14 = true;
    mBlurOffset = 30.0f;
    return true;
}

bool MarioFpView::update() {
    if (::lbl_806B62B0) {
        ::lbl_806B62B0 = false;
        return true;
    }
    mBlurOffset -= 0.5f;
    if (mBlurOffset < 0.0f) {
        mBlurOffset = 0.0f;
    }
    if (getPlayer()->isDisableStayHere()) {
        return false;
    }
    if (MR::testFpViewOutTrigger() && _12 == 0) {
        MR::startSystemSE("SE_SY_CAMERA_RESET", -1, -1);
        MR::endSubjectiveCamera(0xf);
        _12 = 0x14;
        _14 = false;
    }
    if (_12 != 0) {
        _12--;
        if (_12 == 0) {
            return false;
        }
    }
    return true;
}

bool MarioFpView::close() {
    if (_14) {
        MR::endSubjectiveCamera(0xf);
    }
    stopAnimation("見る", (char*)nullptr);
    mActor->showBeeFur();
    return true;
}

void MarioFpView::forceClose() {
    MR::endSubjectiveCamera(1);
    _14 = false;
}

f32 MarioFpView::getBlurOffset() const {
    return mBlurOffset;
}

bool Mario::isDisableStayHere() const {
    if (mMovementStates._0) {
        return true;
    }
    if (!mMovementStates._1) {
        return true;
    }
    if (_3CE < 5) {
        return true;
    }
    if (mMovementStates._A) {
        return true;
    }
    if (isDamaging()) {
        return true;
    }
    if (mMovementStates._1B) {
        return true;
    }
    if (MR::getKarikariClingNum() != 0) {
        return true;
    }
    if (getPlayer()->mMovementStates._23) {
        return true;
    }
    if (_278 > 0.01f) {
        return true;
    }
    if (getStickP() != 0.0f) {
        return true;
    }
    if (_10._16) {
        return true;
    }
    if (_960 == 0x1b) {
        return true;
    }
    if (_960 == 0x1c) {
        return true;
    }
    if (mDrawStates._B) {
        return true;
    }
    if (mMovementStates._18) {
        return true;
    }
    if (!MR::isNearZero(_928, 0.001f)) {
        return true;
    }
    if (!MR::isNearZero(_350, 0.001f)) {
        return true;
    }
    if (!MR::isNearZero(_35C, 0.001f)) {
        return true;
    }
    if (!MR::isNearZero(_184, 0.001f)) {
        return true;
    }
    if (mActor->mAlphaEnable) {
        return true;
    }
    if (mMovementStates._37) {
        return true;
    }
    if (mMovementStates._3A) {
        return true;
    }
    if (_10._15) {
        return true;
    }
    if (_735) {
        return true;
    }
    if (mActor->_EA4) {
        return true;
    }
    return mActor->_3C0;
}

void Mario::tryFpViewMode() {
    if (!isStatusActive(0x12)) {
        stopWalk();
        changeStatus(mFpView);
        MR::startSystemSE("SE_SY_CAMERA_FP_VIEW", -1, -1);
    }
}

bool Mario::isDisableFpViewMode() const {
    if (_97C) {
        return true;
    }
    return isDisableStayHere();
}
