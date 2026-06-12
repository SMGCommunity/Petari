#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    ClipAreaHolder* getClipAreaHolder() {
        return MR::getSceneObj< ClipAreaHolder >(SceneObj_ClipAreaHolder);
    }
};  // namespace

ClipAreaHolder::ClipAreaHolder(const char* pName) : DeriveActorGroup< ClipArea >(pName, 64), mIsActive(true) {
    MR::registerPreDrawFunction(MR::Functor_Inline(&MR::setupShadowVolumeDraw), MR::DrawType_ClipArea);
}

void ClipAreaHolder::init(const JMapInfoIter& rIter) {
}

bool ClipAreaHolder::isInArea(const TVec3f& rParam1, f32 param2) const {
    if (!mIsActive) {
        return false;
    }
    s32 count = getObjectCount();
    for (s32 i = 0; i < count; i++) {
        if (MR::isDead(getActor(i))) {
            continue;
        }
        ClipArea* area = static_cast< ClipArea* >(getActor(i));
        if (!area->isInArea(rParam1, param2)) {
            continue;
        }
        return true;
    }
    return false;
}

ClipAreaCollisionFilter::ClipAreaCollisionFilter(const TVec3f* pParam1, f32 param2) : _04(pParam1), _08(param2) {
}

bool ClipAreaCollisionFilter::isInvalidParts(const CollisionParts* pParts) const {
    if (!MR::isExistSceneObj(SceneObj_ClipAreaHolder)) {
        return false;
    }
    if (pParts->mHitSensor->isType(ATYPE_CLIP_FIELD_MAP_PARTS)) {
        if (!MR::isInClipArea(*_04, _08)) {
            return true;
        }
    }

    return false;
}

namespace MR {
    void createClipAreaHolder() {
        createSceneObj(SceneObj_ClipAreaHolder);
    }

    bool isExistClipAreaHolder() {
        return isExistSceneObj(SceneObj_ClipAreaHolder);
    }

    void addClipArea(ClipArea* pClipArea) {
        ::getClipAreaHolder()->registerActor(pClipArea);
    }

    bool isInClipArea(const TVec3f& rParam1, f32 param2) {
        return ::getClipAreaHolder()->isInArea(rParam1, param2);
    }

    bool isActiveClipArea() {
        return ::getClipAreaHolder()->mIsActive != false;
    }

    void activateClipArea() {
        ::getClipAreaHolder()->mIsActive = true;
    }

    void deactivateClipArea() {
        ::getClipAreaHolder()->mIsActive = false;
    }

    void setBinderExceptSensorType(LiveActor* actor, const TVec3f* pParam1, f32 param2) {
        ClipAreaCollisionFilter* filter = new ClipAreaCollisionFilter(pParam1, param2);
        setBinderCollisionPartsFilter(actor, filter);
    }
};  // namespace MR
