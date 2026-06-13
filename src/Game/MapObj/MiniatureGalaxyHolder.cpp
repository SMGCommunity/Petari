#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    MiniatureGalaxyHolder* getHolder() {
        return MR::getSceneObj< MiniatureGalaxyHolder >(SceneObj_MiniatureGalaxyHolder);
    }
};  // namespace

MiniatureGalaxyHolder::MiniatureGalaxyHolder() : LiveActor("ミニチュアギャラクシー保持"), _8C(), _90(), mCometGalaxy(), mCometID(-1), _9C() {
    _8C = new LiveActorGroup("ミニチュアギャラクシーグループ", 16);
}

void MiniatureGalaxyHolder::registerActor(LiveActor* pActor, const JMapInfoIter& rIter) {
    _8C->registerActor(pActor);

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
        if (MR::isEqualString(mMiniGalaxy->mName, v1)) {
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
    const char* mString = ((MiniatureGalaxy*)mActor)->mName;
    s32 mStarNum = MR::getPowerStarNumToOpenGalaxy(mString);
    s32 x = 0;
    MiniatureGalaxy* mMiniGalaxy2 = findMiniatureGalaxy(mString);
    MiniatureGalaxy* mMiniGalaxy3;
    for (int i = 0; i < _8C->mObjectCount; i++) {
        mMiniGalaxy3 = (MiniatureGalaxy*)_8C->getActor(i);
        if (mMiniGalaxy3->mGalaxyType != 2 && mMiniGalaxy3 != mMiniGalaxy2) {
            if (mMiniGalaxy2->mGalaxyType == 2) {
                x++;
            } else if (MR::getPowerStarNumToOpenGalaxy(mMiniGalaxy3->mName) < mStarNum) {
                x++;
            }
        }
    }
    return x;
}

#pragma ppc_iro_level 1

void MiniatureGalaxyHolder::updateCometStatus() {
    mCometGalaxy = nullptr;
    mCometID = -1;
    _9C = 0;
    MiniatureGalaxy* mMiniGalaxy;
    for (int i = 0; i < _8C->mObjectCount; i++) {
        mMiniGalaxy = (MiniatureGalaxy*)_8C->getActor(i);
        if (MR::isGalaxyCometLandInStage(mMiniGalaxy->mName)) {
            mCometGalaxy = mMiniGalaxy;
            break;
        }
    }
    if (mCometGalaxy != nullptr) {
        mCometID = MR::getEncounterGalaxyCometNameId(mCometGalaxy->mName);
    }
}

void MiniatureGalaxyFunction::registerMiniatureGalaxyToHolder(LiveActor* mActor, const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_MiniatureGalaxyHolder);
    ::getHolder()->registerActor(mActor, rIter);
}

s32 MiniatureGalaxyFunction::getMiniatureGalaxyNum() {
    return ::getHolder()->_8C->mObjectCount;
}

s32 MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(const LiveActor* mActor) {
    return ::getHolder()->calcIndex(mActor);
}

void MiniatureGalaxyFunction::updateCometStatus() {
    ::getHolder()->updateCometStatus();
}

MiniatureGalaxy* MiniatureGalaxyFunction::getCometLandMiniatureGalaxy() {
    return ::getHolder()->mCometGalaxy;
}

s32 MiniatureGalaxyFunction::getCometNameId() {
    return ::getHolder()->mCometID;
}

MiniatureGalaxy* MiniatureGalaxyFunction::getPointingMiniatureGalaxy() {
    if (SphereSelectorFunction::isPointingTarget()) {
        LiveActor* pointingTarget = SphereSelectorFunction::getPointingTarget();

        if (::getHolder()->isRegisteredActor(pointingTarget)) {
            return static_cast< MiniatureGalaxy* >(pointingTarget);
        }
    }

    return nullptr;
}

MiniatureGalaxyHolder::~MiniatureGalaxyHolder() {
}
