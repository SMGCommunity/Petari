#include "Game/Player/MarioFpView.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static bool sIsDisabled;
};  // namespace

bool Mario::isDisableStayHere() const {
    if (getMovementStates().jumping) {
        return true;
    }

    if (!getMovementStates()._1) {
        return true;
    }

    if (_3CE < 5) {
        return true;
    }

    if (getMovementStates()._A) {
        return true;
    }

    if (isDamaging()) {
        return true;
    }

    if (getMovementStates()._1B) {
        return true;
    }

    if (MR::getKarikariClingNum() != 0) {
        return true;
    }

    if (getPlayer()->getMovementStates()._23) {
        return true;
    }

    if (_278 > 0.01f) {
        return true;
    }

    if (getStickP() != 0.0f) {
        return true;
    }

    if (_10.debugMode) {
        return true;
    }

    if (_960 == 27) {
        return true;
    }

    if (_960 == 28) {
        return true;
    }

    if (getDrawStates()._B) {
        return true;
    }

    if (getMovementStates()._18) {
        return true;
    }

    if (!MR::isNearZero(_928)) {
        return true;
    }

    if (!MR::isNearZero(_350)) {
        return true;
    }

    if (!MR::isNearZero(_35C)) {
        return true;
    }

    if (!MR::isNearZero(_184)) {
        return true;
    }

    if (mActor->mAlphaEnable) {
        return true;
    }

    if (getMovementStates()._37) {
        return true;
    }

    if (getMovementStates()._3A) {
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

bool Mario::isDisableFpViewMode() const {
    if (_97C) {
        return true;
    }

    return isDisableStayHere();
}

MarioFpView::MarioFpView(MarioActor* pActor) : MarioState(pActor, MarioStatus_FpView), mDelay(), mIsActive(), mBlurOffset() {
}

bool MarioFpView::start() {
    MR::startSubjectiveCamera(-1);
    changeAnimation("見る", reinterpret_cast< char* >(nullptr));

    sIsDisabled = true;

    mDelay = 0;
    mIsActive = true;
    mBlurOffset = 30.0f;

    return true;
}

bool MarioFpView::update() {
    if (sIsDisabled) {
        sIsDisabled = false;
        return true;
    }

    mBlurOffset -= 0.5f;
    if (mBlurOffset < 0.0f) {
        mBlurOffset = 0.0f;
    }

    if (getPlayer()->isDisableStayHere()) {
        return false;
    }

    if (MR::testFpViewOutTrigger() && mDelay == 0) {
        MR::startSystemSE("SE_SY_CAMERA_RESET");

        MR::endSubjectiveCamera(15);

        mDelay = 20;
        mIsActive = false;
    }

    if (mDelay != 0 && --mDelay == 0) {
        return false;
    }

    return true;
}

bool MarioFpView::close() {
    if (mIsActive) {
        MR::endSubjectiveCamera(15);
    }

    stopAnimation("見る", reinterpret_cast< char* >(nullptr));

    // mActor->showBeeFur();

    return true;
}

void MarioFpView::forceClose() {
    MR::endSubjectiveCamera(1);

    mIsActive = false;
}

void Mario::tryFpViewMode() {
    if (isStatusActive(MarioStatus_FpView)) {
        return;
    }

    stopWalk();

    changeStatus(mFpView);

    MR::startSystemSE("SE_SY_CAMERA_FP_VIEW");
}

f32 MarioFpView::getBlurOffset() const {
    return mBlurOffset;
}
