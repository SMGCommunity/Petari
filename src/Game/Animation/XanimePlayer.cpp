#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"

XanimePlayer::XanimePlayer(J3DModel* pModel, XanimeResourceTable* pRessource) {
    init();
    mModel = pModel;
    mModelData = pModel->mModelData;
    u8 flags = pModel->mModelData->mFlags & 0xF;
    mResourceTable = pRessource;
    _0C = 0.0f;
    mDefaultAnimation = nullptr;
    mCurrentAnimation = nullptr;
    _68 = nullptr;
    mCore = new XanimeCore(mResourceTable->mMaxGroupInfoTableSize, mModelData->mJointTree.mJointNum, flags);

    _10[0] = 0.0f;
    for (u32 i = 1; i < mResourceTable->mMaxGroupInfoTableSize; i++) {
        _10[i] = 0.0f;
    }
    _7C = 0;
    _7D = 0;
    _7F = 0;
    _7E = 1;

    _24[0].init(0);
    _24[1].init(0);
    _54 = 0;
    _55 = 0;
    _20 = _24;

    mCore->initT(mModelData);
}

void XanimePlayer::init() {
    mModelData = nullptr;
    mCurrentBckName = nullptr;
    _68 = nullptr;
    mPrevAnimation = nullptr;
    mCurrentAnimation = nullptr;
    mCurrentAnimation = nullptr;
    mDefaultAnimation = nullptr;
    mCore = nullptr;
    mResourceTable = nullptr;
    _20 = nullptr;
    _55 = 0;
    _54 = 0;

    _78 = 0;
    _7F = 0;
    _7E = 0;
    _7D = 0;
    _7C = 0;
    _88 = 0;
    _80 = 0;

    for (int i = 0; i < 4; i++) {
        _10[i] = 0.0f;
    }
    _74 = 0;
    _84 = -2.0f;
}

void XanimePlayer::setDefaultAnimation(const char* pAnimationName) {
    const XanimeGroupInfo* pDefaultAnimation = mResourceTable->getGroupInfo(pAnimationName);

    if (mCurrentAnimation == mDefaultAnimation) {
        changeAnimation(pDefaultAnimation);
    }

    mDefaultAnimation = pDefaultAnimation;
    _7D = true;
}

void XanimePlayer::changeCurrentAnimation(const XanimeGroupInfo* pAnimation) {
    if (mCurrentAnimation == pAnimation) {
        return;
    }

    mPrevAnimation = mCurrentAnimation;
    mCurrentAnimation = pAnimation;

    updateAfterMovement();
}

void XanimePlayer::stopAnimation() {
    runDefaultAnimation();
}

void XanimePlayer::stopAnimation(const char* pName) {
    if (mResourceTable->getGroupInfo(pName) == mCurrentAnimation) {
        stopAnimation();
    }
}
