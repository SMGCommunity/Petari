#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"
#include "JSystem/J3DGraphAnimator/J3DJoint.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DMtxCalc.hpp"

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

    mWeights[0] = 0.0f;
    for (u32 i = 1; i < mResourceTable->mMaxGroupInfoTableSize; i++) {
        mWeights[i] = 0.0f;
    }

    _7C = false;
    _7D = 0;
    _7F = 0;
    _7E = 1;

    _24[0].init(0);
    _24[1].init(0);

    _54 = 0;
    mCurrent_24 = 0;

    _20 = _24;

    mCore->initT(mModelData);
}

XanimePlayer::XanimePlayer(J3DModel* pModel, XanimeResourceTable* pRessource, XanimePlayer* pPlayer) {
    init();
    mModel = pModel;
    mModelData = pModel->mModelData;
    mResourceTable = pRessource;

    _0C = 0.0f;

    mDefaultAnimation = nullptr;
    mCurrentAnimation = nullptr;
    mPrevAnimation = nullptr;

    _68 = nullptr;

    mCore = new XanimeCore(mResourceTable->mMaxGroupInfoTableSize, pPlayer->mCore);

    mWeights[0] = 0.0f;
    for (u32 i = 1; i < mResourceTable->mMaxGroupInfoTableSize; i++) {
        mWeights[i] = 0.0f;
    }

    _7C = false;
    _7D = 0;
    _7F = 0;
    _7E = 1;

    _24[0].init(0);
    _24[1].init(0);

    _54 = 0;
    mCurrent_24 = 0;

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
    mCurrent_24 = 0;
    _54 = 0;

    _78 = 0;
    _7F = 0;
    _7E = 0;
    _7D = 0;
    _7C = false;
    _88 = 0;
    _80 = 0;

    for (int i = 0; i < 4; i++) {
        mWeights[i] = 0.0f;
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

void XanimePlayer::changeTrackAnimation(u8 arg1, const char* pName) {
    s32 index = mResourceTable->getSingleIndex(pName);
    if (index != -1) {
        changeTrackAnimation(arg1, index);
    }
}

void XanimePlayer::changeTrackAnimation(u8 arg1, u32 index) {
    if (arg1 <= mCurrentAnimation->mBckTableVariant) {
        J3DAnmTransform* resMotion = static_cast< J3DAnmTransform* >(mResourceTable->findResMotion(mResourceTable->mBckTables[index].mFileName));
        if (resMotion != nullptr) {
            mCore->setBck(arg1, resMotion);
        }
    }
}

void XanimePlayer::changeAnimationBck(const char* pName) {
    changeSimpleBck(pName);
}

bool XanimePlayer::changeSimpleBck(const char* pName) {
    J3DAnmTransform* resMotion = static_cast< J3DAnmTransform* >(mResourceTable->findResMotion(pName));
    if (resMotion == nullptr) {
        mCurrentBckName = nullptr;
        return false;
    }

    changeAnimationSimple(resMotion);
    mCurrentBckName = mResourceTable->findStringMotion(pName);

    return true;
}

void XanimePlayer::changeAnimation(const char* pName) {
    const XanimeGroupInfo* info = mResourceTable->getGroupInfo(pName);
    if (info == nullptr) {
        changeSimpleBck(pName);
        return;
    }

    changeAnimation(info);
}

void XanimePlayer::changeAnimationByHash(u32 hash) {
    const XanimeGroupInfo* info = mResourceTable->getGroupInfoFromHash(hash);
    if (info != nullptr) {
        changeAnimation(info);
    }
}

/*void XanimePlayer::swapFrameCtrl(const XanimeGroupInfo* pInfo) {
    if (pInfo == nullptr) {
        return;
    }

    if (pInfo->_20[0] == nullptr) {
        return;
    }
}*/

void XanimePlayer::changeAnimation(const XanimeGroupInfo* pInfo) {
    if (pInfo == nullptr) {
        return;
    }

    if (mCurrentAnimation == pInfo) {
        return;
    }

    swapFrameCtrl(pInfo);
    _68 = pInfo;
    changeCurrentAnimation(pInfo);
    mCurrentBckName = pInfo->mBckName;
}

void XanimePlayer::prepareAnimation(const XanimeGroupInfo* pInfo) {
    if (pInfo == nullptr) {
        return;
    }

    mCore->doFreeze();
    changeCurrentAnimation(pInfo);
    _7F = 1;
    runNextAnimation();
}

void XanimePlayer::setModel(J3DModel* pModel) {
    mModel = pModel;
    mModelData = pModel->mModelData;
    mCore->reconfigJointTransform(mModelData);
}

void XanimePlayer::runNextAnimation() {
    if (!_7F) {
        return;
    }

    _7F = false;

    const XanimeGroupInfo* curAnim = mCurrentAnimation;
    u32 i = 0;
    for (; i < curAnim->mBckTableVariant; i++) {
        mCore->setBck(i & 0xFF, static_cast< J3DAnmTransform* >(curAnim->_20[i]));
        mWeights[i] = curAnim->_30[i];
    }

    for (u32 j = i; j < mResourceTable->mMaxGroupInfoTableSize; j++) {
        mCore->setBck(j & 0xFF, nullptr);
        mWeights[j] = 0.0f;
    }

    bool cond = _7C;

    _54 = mCurrent_24;

    _08 = 0.0f;
    _0C = 1.0f;

    _78 = false;
    _7C = true;

    if (!cond) {
        _24[mCurrent_24]._14 = 1;
        _08 = 1.0f;
        updateBeforeMovement();
        calcAnm(0);
    }
}

void XanimePlayer::changeAnimationSimple(J3DAnmTransform* pAnm) {
    _68 = nullptr;

    if (_54 != mCurrent_24) {
        _54 = mCurrent_24;
    }

    mCore->doFreeze();
    changeCurrentAnimation(getSimpleGroup());
    mCore->setBck(0, pAnm);

    mWeights[0] = 1.0f;
    for (u32 i = 1; i < mResourceTable->mMaxGroupInfoTableSize; i++) {
        mCore->setBck(i & 0xFF, nullptr);
        mWeights[i] = 0.0f;
    }

    XanimeGroupInfo* info = getSimpleGroup();
    info->_20[0] = pAnm;
    J3DAnmTransform* transform = static_cast< J3DAnmTransform* >(info->_20[0]);
    _20->init(transform->mFrameMax);
    _20->mAttribute = transform->mAttribute;
    _20->mFrame = 0.0f;

    _08 = 0.0f;
    _0C = 1.0f;
    _78 = 0;
    _20->_14 = 1;
    _7C = true;
    _7F = 0;
    _68 = nullptr;
    _84 = 0.0f;
    _88 = 0;
}

void XanimePlayer::changeSpeed(f32 speed) {
    _20->mRate = speed;
}

void XanimePlayer::changeInterpoleFrame(s32 frame) {
    _20->_14 = frame;
    if (frame == 0) {
        _08 = 1.0f;
        mCore->_1C = 1.0f;
        return;
    }

    _08 = 0.0f;
    mCore->_1C = 0.0f;
    updateInterpoleRatio();
}

bool XanimePlayer::changeTrackWeight(u32 track, f32 weight) {
    if (mCurrentAnimation == nullptr) {
        return false;
    }

    if (track >= mCurrentAnimation->mBckTableVariant) {
        return false;
    }

    mWeights[track] = weight;
    return true;
}

void XanimePlayer::overWriteMtxCalc(u16 arg) {
    mModelData->mJointTree.mJointNodePointer[arg]->mMtxCalc = mCore;
}

void XanimePlayer::clearMtxCalc(u16 arg) {
    mModelData->mJointTree.mJointNodePointer[arg]->mMtxCalc = nullptr;
}

void XanimePlayer::clearAnm(u16 arg) {
    if (_7C == 0) {
        return;
    }

    mModelData->mJointTree.mJointNodePointer[arg]->mMtxCalc = nullptr;
}

void XanimePlayer::updateBeforeMovement() {
    if (_7C == 0) {
        return;
    }

    if ((_20->mState & 0x1) == 1 && _7E != 0 && (_20->mAttribute == 0 || _20->mAttribute == 3)) {
        runDefaultAnimation();
    }

    if (_78 != 0 && (_78 -= 0x1) == 0) {
        runDefaultAnimation();
    }
}

void XanimePlayer::updateAfterMovement() {
    prepareAnimation(_68);
    _68 = nullptr;

    if (_7C == 0) {
        return;
    }

    s32 cast;
    if ((_20->mState & 1) != 0) {
        cast = _20->mEnd;
        // This convertion happens but nothing is done with it, and gets optimized out.
        // TODO: make it not optimize out.
    } else if (_88 == 0) {
        f32 rate = _20->mRate;
        f32 frame = _20->mFrame;

        _20->update();
        _84 = frame;
        _88 = 1;

        if ((_20->mState & 1) != 0) {
            _20->mRate = rate;
        }
    }

    updateInterpoleRatio();

    for (u32 i = 0; i < mCurrentAnimation->mBckTableVariant; i++) {
        mCore->setWeight(i, mWeights[i]);
    }

    mCore->_1C = _08;
    mCore->_20 = _0C;
}

void XanimePlayer::updateInterpoleRatio() {
    XanimeFrameCtrl* frameCtrl = _20;
    if (frameCtrl->mRate != 0.0f || frameCtrl->mAttribute == 1) {
        if (frameCtrl->_14 != 0) {
            // TODO: instruction swap
            f32 cast = frameCtrl->_14;
            cast -= _08;
            cast = 1.0f / cast;
            _08 += cast;
            _20->_14 -= 1;
        } else {
            _08 = 1.0f;
        }
        return;
    }

    _08 = 1.0f;
    frameCtrl->_14 = 0;
}

bool XanimePlayer::isRun(const char* pName) const {
    if (mCurrentAnimation == nullptr) {
        return false;
    }

    XanimeGroupInfo* group = getSimpleGroup();
    if (mCurrentAnimation == group) {
        return mResourceTable->findResMotion(pName) == getSimpleGroup()->_20[0];
    }

    return mCurrentAnimation == mResourceTable->getGroupInfo(pName);
}

void XanimePlayer::runDefaultAnimation() {
    if (_68 != nullptr) {
        _68 = nullptr;
    }

    if (_7D == 0) {
        return;
    }

    if (mResourceTable->mAmountOfGroupInfos == 0) {
        return;
    }

    swapFrameCtrl(mDefaultAnimation);
    _68 = mDefaultAnimation;
    changeCurrentAnimation(mDefaultAnimation);
    mCurrentBckName = mDefaultAnimation->mBckName;
}

bool XanimePlayer::isAnimationRunSimple() const {
    if (mCurrentAnimation == nullptr) {
        return false;
    }

    return mCurrentAnimation == getSimpleGroup();
}

const char* XanimePlayer::getCurrentAnimationName() const {
    if (mCurrentAnimation == nullptr) {
        return nullptr;
    }

    return "";
}

const char* XanimePlayer::getDefaultAnimationName() const {
    if (mDefaultAnimation == nullptr) {
        return nullptr;
    }

    return "";
}

const char* XanimePlayer::getCurrentBckName() const {
    return mCurrentBckName;
}

const char* XanimePlayer::getNameStringPointer(const char* pName) const {
    const XanimeGroupInfo* info = mResourceTable->getGroupInfo(pName);
    if (info != nullptr) {
        return info->mParent.animationName;
    }

    return nullptr;
}

bool XanimePlayer::checkPass(f32 arg) const {
    if (_88 == 0) {
        return _20->checkPass(arg);
    }

    f32 oldFrame = _20->mFrame;
    _20->mFrame = _84;
    bool res = _20->checkPass(arg);
    _20->mFrame = oldFrame;

    return res;
}

XanimeGroupInfo* XanimePlayer::getSimpleGroup() const {
    if (_74 != nullptr) {
        return _74;
    }

    return &mResourceTable->_1C;
}

void XanimePlayer::duplicateSimpleGroup() {
    _74 = new XanimeGroupInfo();
    _74->init();
    _74->mParent.animationName = "dup-non-group";
    _74->_4 = 1.0f;
    _74->_8 = 1;
    _74->mBckTableVariant = 1;
    _74->_1D = 0;
}

XanimeFrameCtrl::XanimeFrameCtrl() : _14(0) {
}
