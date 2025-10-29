#include "Game/Animation/LayoutAnmPlayer.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/LayoutPaneCtrl.hpp"
#include <nw4r/lyt/pane.h>

LayoutPaneCtrl::LayoutPaneCtrl(LayoutManager* pHost, const char* pPaneName, u32 animLayerNum) :
    mHost(pHost),
    mPane(nullptr),
    mPaneIndex(-1),
    mAnmPlayerArray(animLayerNum),
    _14(0),
    _18(0)
{
    mPane = mHost->getPane(pPaneName);

    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i] = new LayoutAnmPlayer(pHost);
    }
}

void LayoutPaneCtrl::movement() {
    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i]->movement();
    }
}

void LayoutPaneCtrl::calcAnim() {
    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i]->reflectFrame();
    }
}

void LayoutPaneCtrl::start(const char* pAnimName, u32 layer) {
    LayoutAnmPlayer* pAnmPlayer = mAnmPlayerArray[layer];

    if (pAnmPlayer->mAnimTransform != nullptr) {
        if (mHost->_61) {
            mPane->UnbindAnimation(pAnmPlayer->mAnimTransform, true);
        }
        else {
            mHost->unbindPaneCtrlAnim(this, pAnmPlayer->mAnimTransform);
        }
    }

    pAnmPlayer->start(pAnimName);

    if (mHost->_61) {
        mPane->UnbindAnimation(pAnmPlayer->mAnimTransform, true);
        mPane->BindAnimation(pAnmPlayer->mAnimTransform, true);
    }
    else {
        mHost->bindPaneCtrlAnim(this, pAnmPlayer->mAnimTransform);
    }
}

void LayoutPaneCtrl::stop(u32 layer) {
    mAnmPlayerArray[layer]->stop();
}

bool LayoutPaneCtrl::isAnimStopped(u32 layer) const {
    return mAnmPlayerArray[layer]->isStop();
}

// LayoutPaneCtrl::reflectFollowPos

J3DFrameCtrl* LayoutPaneCtrl::getFrameCtrl(u32 layer) const {
    return &mAnmPlayerArray[layer]->mFrameCtrl;
}

// LayoutPaneCtrl::recalcChildGlobalMtx
