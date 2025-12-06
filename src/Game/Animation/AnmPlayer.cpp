#include "Game/Animation/AnmPlayer.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

AnmPlayerBase::AnmPlayerBase(const ResTable* pResTable) : mResTable(pResTable), mAnmRes(nullptr), mFrameCtrl(0) {}

void AnmPlayerBase::update() {
    if (mAnmRes != nullptr) {
        mFrameCtrl.update();
    }
}

void AnmPlayerBase::reflectFrame() {
    if (mAnmRes != nullptr) {
        mAnmRes->mCurrentFrame = mFrameCtrl.mCurrentFrame;
    }
}

void AnmPlayerBase::start(const char* pResName) {
    J3DAnmBase* pAnmRes = reinterpret_cast< J3DAnmBase* >(mResTable->getRes(pResName));

    if (pAnmRes != mAnmRes) {
        changeAnimation(pAnmRes);
        mAnmRes = pAnmRes;
    }

    mFrameCtrl.init(mAnmRes->mLoopDuration);
    mFrameCtrl.mLoopMode = mAnmRes->mLoopMode;
    mFrameCtrl.mCurrentFrame = 0.0f;
    mFrameCtrl.mSpeed = 1.0f;
}

void AnmPlayerBase::stop() {
    stopAnimation();
    mFrameCtrl.mSpeed = 0.0f;
}

bool AnmPlayerBase::isPlaying(const char* pAnimName) const {
    if (mAnmRes != nullptr) {
        if (MR::isEqualStringCase(pAnimName, mResTable->getResName(mAnmRes))) {
            return true;
        }
    }

    return false;
}

MaterialAnmPlayerBase::MaterialAnmPlayerBase(const ResTable* pResTable, J3DModelData* pModelData)
    : AnmPlayerBase(pResTable), mModelData(pModelData) {}

void MaterialAnmPlayerBase::beginDiff() {
    if (mAnmRes != nullptr) {
        reflectFrame();
        attach(mAnmRes, mModelData);
    }
}

void MaterialAnmPlayerBase::endDiff() {
    if (mAnmRes != nullptr) {
        detach(mAnmRes, mModelData);
    }
}

void AnmPlayerBase::changeAnimation(J3DAnmBase*) {}

void AnmPlayerBase::stopAnimation() {}
