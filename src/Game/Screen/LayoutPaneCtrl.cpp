#include "Game/Screen/LayoutPaneCtrl.hpp"
#include "Game/Animation/LayoutAnmPlayer.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include <nw4r/lyt/pane.h>

LayoutPaneCtrl::LayoutPaneCtrl(LayoutManager* pHost, const char* pPaneName, u32 animLayerNum)
    : mHost(pHost), mPane(), mPaneIndex(-1), mAnmPlayerArray(animLayerNum), mFollowType(), mFollowPos() {
    LayoutManager* pManager = mHost;
    mPane = pPaneName == nullptr ? pManager->mLayout->mpRootPane : pManager->mLayout->mpRootPane->FindPaneByName(pPaneName, true);

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
        } else {
            mHost->unbindPaneCtrlAnim(this, pAnmPlayer->mAnimTransform);
        }
    }

    pAnmPlayer->start(pAnimName);

    if (mHost->_61) {
        mPane->UnbindAnimation(pAnmPlayer->mAnimTransform, true);
        mPane->BindAnimation(pAnmPlayer->mAnimTransform, true);
    } else {
        mHost->bindPaneCtrlAnim(this, pAnmPlayer->mAnimTransform);
    }
}

void LayoutPaneCtrl::stop(u32 layer) {
    mAnmPlayerArray[layer]->stop();
}

bool LayoutPaneCtrl::isAnimStopped(u32 layer) const {
    return mAnmPlayerArray[layer]->isStop();
}

void LayoutPaneCtrl::reflectFollowPos() {
    if (mFollowPos == nullptr) {
        return;
    }

    nw4r::math::MTX34 global;
    nw4r::math::MTX34Copy(&global, &mPane->mGlbMtx);

    switch (mFollowType) {
    case 0: {
        TVec2f position;
        MR::convertScreenPosToLayoutPos(&position, *mFollowPos);
        global._03 = position.x;
        global._13 = position.y;
        break;
    }
    case 1: {
        TVec2f screenOrigin;
        MR::convertLayoutPosToScreenPos(&screenOrigin, TVec2f(0.0f, 0.0f));
        TVec2f offset(screenOrigin + *mFollowPos);
        MR::convertScreenPosToLayoutPos(&offset, offset);
        global._03 += offset.x;
        global._13 += offset.y;
        break;
    }
    case 2: {
        nw4r::math::MTX34 local = mPane->mMtx;
        nw4r::math::MTX34 inverse;
        PSMTXInverse(local, inverse);
        nw4r::math::MTX34Mult(&global, &global, &inverse);

        nw4r::math::MTX34 replacement;
        nw4r::math::MTX34Copy(&replacement, &local);
        replacement._03 = mFollowPos->x;
        replacement._13 = mFollowPos->y;
        MtxPtr pGlobal = global;
        PSMTXConcat(pGlobal, replacement, pGlobal);
        break;
    }
    case 3: {
        nw4r::math::VEC3 offset(mFollowPos->x, mFollowPos->y, 0.0f);
        nw4r::math::VEC3TransformNormal(&offset, &mPane->mMtx, &offset);
        global._03 += offset.x;
        global._13 += offset.y;
        break;
    }
    }

    mPane->mGlbMtx = global;
    recalcChildGlobalMtx(mPane);
}

J3DFrameCtrl* LayoutPaneCtrl::getFrameCtrl(u32 layer) const {
    return &mAnmPlayerArray[layer]->mFrameCtrl;
}

void LayoutPaneCtrl::recalcChildGlobalMtx(nw4r::lyt::Pane* pPane) {
    nw4r::lyt::PaneList& rChildren = pPane->GetChildList();
    for (nw4r::lyt::PaneList::Iterator iter = rChildren.GetBeginIter(); iter != rChildren.GetEndIter(); iter++) {
        nw4r::lyt::Pane& rChild = *iter;
        nw4r::math::MTX34 global;
        nw4r::math::MTX34Mult(&global, &rChild.GetParent()->mGlbMtx, &rChild.mMtx);
        rChild.mGlbMtx = global;
        recalcChildGlobalMtx(&rChild);
    }
}
