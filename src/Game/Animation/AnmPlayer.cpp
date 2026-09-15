#include "Game/Animation/AnmPlayer.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

void AnmPlayer_FORCE_EMIT(TQuat4f& rQuat, const TQuat4f& rOther, TPos3f& rMtx, const TPos3f& rSrc, TVec3f& rVec, f32 value) {
    1.0f;
    0.0f;
    3.814697265625e-6f;
    -3.814697265625e-6f;
    0.5f;
    3.0f;
    -1.0f;
    3.1415927f;
    1023.5f;
    1.5707964f;
    2.0f;
    -1.5707964f;

    rQuat.setEuler(value, value, value);
    rQuat.setRotate(rVec, rVec);
    rQuat.slerp(rOther, value);
    rQuat.normalize(rOther);
    rQuat.getEuler(rVec);

    delete[] new TPos3f[1];
    rMtx.identity();
    rMtx.mult(rVec, rVec);
    rMtx.multTranspose(rVec, rVec);
    rMtx.invert(rSrc);
    rMtx.concat(rSrc, rSrc);
    rMtx.concat(rSrc);

    TVec3f vectors[2] = {value, value};
    rVec.set< f32 >(value, value, value);
    JGeometry::TUtil< f32 >::epsilonEquals(value, value, value);
    JGeometry::TUtil< f32 >::sqrt(value);
    rMtx.set(value, value, value, value, value, value, value, value, value, value, value, value);
    rMtx.getQuat(rQuat);
    rQuat.set< f32 >(value, value, value, value);
    (rQuat.*&TQuat4f::operator=)(rOther);
}

AnmPlayerBase::AnmPlayerBase(const ResTable* pResTable) : mResTable(pResTable), mAnmRes(), mFrameCtrl(0) {
}

void AnmPlayerBase::update() {
    if (mAnmRes != nullptr) {
        mFrameCtrl.update();
    }
}

void AnmPlayerBase::reflectFrame() {
    if (mAnmRes != nullptr) {
        mAnmRes->mFrame = mFrameCtrl.mFrame;
    }
}

void AnmPlayerBase::start(const char* pResName) {
    J3DAnmBase* pAnmRes = reinterpret_cast< J3DAnmBase* >(mResTable->getRes(pResName));

    if (pAnmRes != mAnmRes) {
        changeAnimation(pAnmRes);
        mAnmRes = pAnmRes;
    }

    mFrameCtrl.init(mAnmRes->mFrameMax);
    mFrameCtrl.setAttribute(mAnmRes->mAttribute);
    mFrameCtrl.setFrame(0.0f);
    mFrameCtrl.setRate(1.0f);
}

void AnmPlayerBase::stop() {
    stopAnimation();
    mFrameCtrl.setRate(0.0f);
}

bool AnmPlayerBase::isPlaying(const char* pAnimName) const {
    if (mAnmRes != nullptr) {
        if (MR::isEqualStringCase(pAnimName, mResTable->getResName(mAnmRes))) {
            return true;
        }
    }

    return false;
}

MaterialAnmPlayerBase::MaterialAnmPlayerBase(const ResTable* pResTable, J3DModelData* pModelData) : AnmPlayerBase(pResTable), mModelData(pModelData) {
}

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

void AnmPlayerBase::changeAnimation(J3DAnmBase*) {
}

void AnmPlayerBase::stopAnimation() {
}
