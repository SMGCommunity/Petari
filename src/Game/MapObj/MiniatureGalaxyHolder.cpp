#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
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

MiniatureGalaxyHolder::MiniatureGalaxyHolder()
    : LiveActor("ミニチュアギャラクシー保持"), mMiniatureGalaxyGroup(), _90(), mCometGalaxy(), mCometID(-1), _9C() {
    mMiniatureGalaxyGroup = new LiveActorGroup("ミニチュアギャラクシーグループ", 16);
}

void MiniatureGalaxyHolder::registerActor(LiveActor* pActor, const JMapInfoIter& rIter) {
    mMiniatureGalaxyGroup->registerActor(pActor);

    if (_90) {
        return;
    }

    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &MiniatureGalaxyHolder::killAllMiniatureGalaxy), "飛び出す");

    _90 = true;
}

void MiniatureGalaxyHolder::init(const JMapInfoIter& rIter) {
    makeActorAppeared();
}

#pragma push
#pragma ppc_iro_level 1

bool MiniatureGalaxyHolder::isRegisteredActor(const LiveActor* pActor) {
    for (int i = 0; i < mMiniatureGalaxyGroup->mObjectCount; i++) {
        if (mMiniatureGalaxyGroup->getActor(i) == pActor) {
            return true;
        }
    }

    return false;
}

#pragma ppc_iro_level 2

MiniatureGalaxy* MiniatureGalaxyHolder::findMiniatureGalaxy(const char* pGalaxyName) const {
    MiniatureGalaxy* miniatureGalaxy;

    for (int i = 0; i < mMiniatureGalaxyGroup->mObjectCount; i++) {
        miniatureGalaxy = static_cast< MiniatureGalaxy* >(mMiniatureGalaxyGroup->getActor(i));

        if (MR::isEqualString(miniatureGalaxy->mGalaxyName, pGalaxyName)) {
            return miniatureGalaxy;
        }
    }

    return nullptr;
}

void MiniatureGalaxyHolder::killAllMiniatureGalaxy() {
    for (int i = 0; i < mMiniatureGalaxyGroup->mObjectCount; i++) {
        mMiniatureGalaxyGroup->getActor(i)->kill();
    }
}

s32 MiniatureGalaxyHolder::calcIndex(const LiveActor* pActor) const {
    const char* galaxyName = static_cast< const MiniatureGalaxy* >(pActor)->mGalaxyName;
    s32 mStarNum = MR::getPowerStarNumToOpenGalaxy(galaxyName);
    s32 index = 0;
    MiniatureGalaxy* actorGalaxy = findMiniatureGalaxy(galaxyName);

    for (int i = 0; i < mMiniatureGalaxyGroup->mObjectCount; i++) {
        MiniatureGalaxy* currMiniGalaxy = static_cast< MiniatureGalaxy* >(mMiniatureGalaxyGroup->getActor(i));

        if (currMiniGalaxy->mType == MiniatureGalaxyType_Koopa) {
            continue;
        }
        if (currMiniGalaxy == actorGalaxy) {
            continue;
        }

        if (actorGalaxy->mType == MiniatureGalaxyType_Koopa) {
            index++;
        } else if (MR::getPowerStarNumToOpenGalaxy(currMiniGalaxy->mGalaxyName) < mStarNum) {
            index++;
        }
    }

    return index;
}

#pragma ppc_iro_level 1

void MiniatureGalaxyHolder::updateCometStatus() {
    mCometGalaxy = nullptr;
    mCometID = -1;
    _9C = 0;

    for (int i = 0; i < mMiniatureGalaxyGroup->mObjectCount; i++) {
        MiniatureGalaxy* miniatureGalaxy = static_cast< MiniatureGalaxy* >(mMiniatureGalaxyGroup->getActor(i));

        if (MR::isGalaxyCometLandInStage(miniatureGalaxy->mGalaxyName)) {
            mCometGalaxy = miniatureGalaxy;
            break;
        }
    }

    if (mCometGalaxy != nullptr) {
        mCometID = MR::getEncounterGalaxyCometNameId(mCometGalaxy->mGalaxyName);
    }
}

void MiniatureGalaxyFunction::registerMiniatureGalaxyToHolder(LiveActor* pActor, const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_MiniatureGalaxyHolder);
    ::getHolder()->registerActor(pActor, rIter);
}

s32 MiniatureGalaxyFunction::getMiniatureGalaxyNum() {
    return ::getHolder()->mMiniatureGalaxyGroup->mObjectCount;
}

s32 MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(const LiveActor* pActor) {
    return ::getHolder()->calcIndex(pActor);
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
