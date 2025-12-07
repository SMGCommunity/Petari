#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"

MiniatureGalaxyHolder::MiniatureGalaxyHolder() : LiveActor("ミニチュアギャラクシー保持") {
    _8C = nullptr;
    _90 = 0;
    _94 = 0;
    _98 = -1;
    _9C = 0;
    _8C = new LiveActorGroup("ミニチュアギャラクシーグループ", 16);
}

void MiniatureGalaxyHolder::registerActor(LiveActor* mActor, const JMapInfoIter& rIter) {
    _8C->registerActor(mActor);
    if (!_90) {
        MR::tryRegisterDemoCast(this, rIter);
        MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &MiniatureGalaxyHolder::killAllMiniatureGalaxy), "飛び出す");
        _90 = true;
    }
}

void MiniatureGalaxyHolder::init(const JMapInfoIter& rIter) {
    makeActorAppeared();
}

#pragma push
#pragma ppc_iro_level 1

bool MiniatureGalaxyHolder::isRegisteredActor(const LiveActor* mActor) {
    for (int i = 0; i < _8C->mObjectCount; i++) {
        if (_8C->getActor(i) == mActor) {
            return true;
        }
    }
    return false;
}

#pragma ppc_iro_level 2

MiniatureGalaxy* MiniatureGalaxyHolder::findMiniatureGalaxy(const char* v1) const {
    MiniatureGalaxy* mMiniGalaxy;
    for (int i = 0; i < _8C->mObjectCount; i++) {
        mMiniGalaxy = (MiniatureGalaxy*)_8C->getActor(i);
        if (MR::isEqualString(mMiniGalaxy->_118, v1)) {
            return mMiniGalaxy;
        }
    }
    return nullptr;
}

void MiniatureGalaxyHolder::killAllMiniatureGalaxy() {
    for (int i = 0; i < _8C->mObjectCount; i++) {
        _8C->getActor(i)->kill();
    }
}

s32 MiniatureGalaxyHolder::calcIndex(const LiveActor* mActor) const {
    const char* mString = ((MiniatureGalaxy*)mActor)->_118;
    s32 mStarNum = MR::getPowerStarNumToOpenGalaxy(mString);
    s32 x = 0;
    MiniatureGalaxy* mMiniGalaxy2 = findMiniatureGalaxy(mString);
    MiniatureGalaxy* mMiniGalaxy3;
    for (int i = 0; i < _8C->mObjectCount; i++) {
        mMiniGalaxy3 = (MiniatureGalaxy*)_8C->getActor(i);
        if (mMiniGalaxy3->_8C != 2 && mMiniGalaxy3 != mMiniGalaxy2) {
            if (mMiniGalaxy2->_8C == 2) {
                x++;
            }
            else if (MR::getPowerStarNumToOpenGalaxy(mMiniGalaxy3->_118) < mStarNum) {
                    x++;
            }
        }
    }
    return x;
}

#pragma ppc_iro_level 1

void MiniatureGalaxyHolder::updateCometStatus() {
    _94 = nullptr;
    _98 = -1;
    _9C = 0;
    MiniatureGalaxy* mMiniGalaxy;
    for (int i = 0; i < _8C->mObjectCount; i++) {
        mMiniGalaxy = (MiniatureGalaxy*)_8C->getActor(i);
        if (MR::isGalaxyCometLandInStage(mMiniGalaxy->_118)) {
            _94 = mMiniGalaxy;
            break;
        }
    }
    if (_94) {
        _98 = MR::getEncounterGalaxyCometNameId(_94->_118);
    }
}

void MiniatureGalaxyFunction::registerMiniatureGalaxyToHolder(LiveActor * mActor, const JMapInfoIter & rIter) {
    MR::createSceneObj(115);
    getMiniGalaxyHolder()->registerActor(mActor, rIter);
}

s32 MiniatureGalaxyFunction::getMiniatureGalaxyNum() {
    return getMiniGalaxyHolder()->_8C->mObjectCount;
}

void MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(const LiveActor * mActor) {
    getMiniGalaxyHolder()->calcIndex(mActor);
}

void MiniatureGalaxyFunction::updateCometStatus() {
    getMiniGalaxyHolder()->updateCometStatus();
}

MiniatureGalaxy* MiniatureGalaxyFunction::getCometLandMiniatureGalaxy() {
    return getMiniGalaxyHolder()->_94;
}

s32 MiniatureGalaxyFunction::getCometNameId() {
    return getMiniGalaxyHolder()->_98;
}

MiniatureGalaxy* MiniatureGalaxyFunction::getPointingMiniatureGalaxy() {
    if (SphereSelectorFunction::isPointingTarget()) {
        LiveActor* mPTarget = SphereSelectorFunction::getPointingTarget();
        if (getMiniGalaxyHolder()->isRegisteredActor(mPTarget)) {
            return (MiniatureGalaxy*)mPTarget;
        }
    }
    return nullptr;
}

MiniatureGalaxyHolder::~MiniatureGalaxyHolder() {}
