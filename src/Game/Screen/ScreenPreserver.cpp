#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/Util/ScreenUtil.hpp"

ScreenPreserver::ScreenPreserver() : NameObj("") {
    mIsActive = false;

    for (s32 i = 0; i < 3; i++) {
        _D[i] = 0;
    }
}

void ScreenPreserver::captureIfAllow() {
    MR::captureScreenIfAllow("GameScreen");
}

// WAY too many dependencies
// ScreenPreserver::draw

void ScreenPreserver::activate() {
    if (!mIsActive) {
        mIsActive = true;
        MR::startToCaptureScreen("GameScreen");
    }
}

void ScreenPreserver::deactivate() {
    if (mIsActive) {
        mIsActive = false;
        MR::endToCaptureScreen("GameScreen");
    }
}

ScreenPreserver::~ScreenPreserver() {

}
