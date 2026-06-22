#include "Game/Animation/LayoutAnmPlayer.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include <nw4r/lyt/animation.h>

LayoutAnmPlayer::LayoutAnmPlayer(const LayoutManager* pManager) : mManager(pManager), mAnimName(nullptr), mAnimTransform(nullptr), mFrameCtrl(0) {
}

void LayoutAnmPlayer::movement() {
    if (mAnimTransform != nullptr) {
        mFrameCtrl.update();
    }
}

void LayoutAnmPlayer::reflectFrame() {
    if (mAnimTransform != nullptr) {
        // TODO: Should be AnimTransform::SetFrame
        mAnimTransform->mFrame = mFrameCtrl.getFrame();
    }
}

void LayoutAnmPlayer::start(const char* pAnimName) {
    nw4r::lyt::AnimTransform* pAnimTransform = mManager->getAnimTransform(pAnimName);

    if (pAnimTransform != mAnimTransform) {
        mAnimTransform = pAnimTransform;
    }

    u32 frameSize = mAnimTransform->GetFrameSize();

    mFrameCtrl.init(static_cast< s16 >(frameSize));

    if (mAnimTransform->IsLoopData()) {
        mFrameCtrl.setAttribute(2);
    } else {
        mFrameCtrl.setAttribute(0);
    }

    mFrameCtrl.setFrame(0.0f);
    mFrameCtrl.setRate(1.0f);
    mAnimName = pAnimName;
}

void LayoutAnmPlayer::stop() {
    mFrameCtrl.setRate(0.0f);
}

// LayoutAnmPlayer::isStop
