#include "Game/LiveActor/MirrorActor.hpp"
#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

void MirrorActor_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

MirrorActor::~MirrorActor() {
}

MirrorActor::MirrorActor(LiveActor* pActor, const char* pName, const char* pArcName) : LiveActor(pName), mHost(pActor), mIsDisconnected() {
    initModelManagerWithAnm(pArcName, nullptr, false);
}

void MirrorActor::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMirrorMapObj(this);
    MR::copyJointAnimation(this, mHost);
    MR::syncMaterialAnimation(this, mHost);
    MR::invalidateClipping(this);

    TBox3f box;
    MR::calcModelBoundingBox(&box, mHost);

    TVec3f center;
    center.lerp(box.f, box.i, 0.5f);
    mRadius = 0.5f * (box.f - box.i).length();
    mCenter = center;

    makeActorAppeared();
}

void MirrorActor::movement() {
    if (MR::isDead(this) || MR::isDead(mHost) || MR::isClipped(mHost) || MR::isHiddenModel(mHost) || isHostInTheOtherSideOfMirror()) {
        if (!mIsDisconnected) {
            mIsDisconnected = true;

            if (!MR::isHiddenModel(this)) {
                MR::disconnectToDrawTemporarily(this);
            }
        }
    } else {
        if (mIsDisconnected) {
            mIsDisconnected = false;

            if (!MR::isHiddenModel(this)) {
                MR::connectToDrawTemporarily(this);
            }
        }

        LiveActor::movement();
    }
}

void MirrorActor::calcAnim() {
    MR::copyJointAnimation(this, mHost);
    MR::updateMaterial(this);
}

void MirrorActor::calcViewAndEntry() {
    mModelManager->calcView();
}

bool MirrorActor::isHostInTheOtherSideOfMirror() const {
    if (MR::isExistMirrorCamera()) {
        return MR::getDistanceToMirror(getHostCenterPos()) < 0.0f;
    }

    return false;
}

TVec3f MirrorActor::getHostCenterPos() const {
    return mCenter + mHost->mPosition;
}
