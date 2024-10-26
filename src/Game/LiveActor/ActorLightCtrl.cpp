#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Util.hpp"

ActorLightCtrl::ActorLightCtrl(const LiveActor *pActor) 
    : mActor(pActor), _4(-1), _8(0), _C(0), mAreaLightInf(0), mLightID() {
    _1C = 0;
    mInterpolate = -1;
    _54 = -1;
}

#ifdef NON_MATCHING
// initActorLightInfo call is getting inlined
void ActorLightCtrl::init(int interpolate, bool /* unused */) {
    if (interpolate >= 0) {
        _4 = interpolate;
        _C = 1;
    }

    initActorLightInfo();
    tryFindNewAreaLight(false);
    mAreaLightInf = LightFunction::getAreaLightInfo(mLightID);
    mLightInfo = *getTargetActorLight(mAreaLightInf);
}
#endif

void ActorLightCtrl::update(bool direct) {
    if (!MR::isHiddenModel(mActor)) {
        tryFindNewAreaLight(direct);
        updateLightBlend();
    }
}

void ActorLightCtrl::loadLight() const {
    if (mAreaLightInf) {
        if (_1C) {
            LightFunction::loadActorLightInfo(&mLightInfo);
        }
        else {
            LightFunction::loadActorLightInfo(getTargetActorLight(mAreaLightInf));
        }
    }
}

bool ActorLightCtrl::isSameLight(const ActorLightCtrl *pLight) const {
    if (_1C) {
        return false;
    }

    return mAreaLightInf == pLight->mAreaLightInf;
}

const ActorLightInfo* ActorLightCtrl::getActorLight() const {
    if (_1C) {
        return &mLightInfo;
    }

    return getTargetActorLight(mAreaLightInf);
}

void ActorLightCtrl::initActorLightInfo() {
    if (_C) {
        return;
    }
    
    MR::findActorLightInfo(mActor);
    return;
}

void ActorLightCtrl::resetCurrentLightInfo() {
    mAreaLightInf = LightFunction::getAreaLightInfo(mLightID);
    mInterpolate = mAreaLightInf->mInterpolate;
    mLightInfo.mInfo0.mIsFollowCamera = getTargetActorLight(mAreaLightInf)->mInfo0.mIsFollowCamera;
    mLightInfo.mInfo1.mIsFollowCamera = getTargetActorLight(mAreaLightInf)->mInfo1.mIsFollowCamera;
    _54 = 0;
}

const ActorLightInfo* ActorLightCtrl::getTargetActorLight(const AreaLightInfo *pInfo) const {
    s32 type = _4;

    if (type == 0) {
        return &pInfo->mPlayerLight;
    }
    else if (type == 1) {
        return &pInfo->mStrongLight;
    }
    else if (type == 2) {
        return &pInfo->mWeakLight;
    }
    else if (type == 3) {
        return &pInfo->mPlanetLight;
    }
    
    return 0;
}
