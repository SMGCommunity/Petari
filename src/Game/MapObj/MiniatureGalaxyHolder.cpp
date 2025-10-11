#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
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

MiniatureGalaxyHolder::~MiniatureGalaxyHolder() {}
