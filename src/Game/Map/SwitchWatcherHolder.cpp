#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Map/SwitchWatcher.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

SwitchWatcherHolder::SwitchWatcherHolder() : NameObj("SwitchWatcherHolder"), mSwitchWatcher() {
    MR::connectToScene(this, MR::MovementType_SwitchWatcherHolder, -1, -1, -1);
}

void SwitchWatcherHolder::movement() {
    // not quite sure what is going on here
    // but it seems to just call movement on all watchers
    mSwitchWatcher.mArray.callAllFunc(&SwitchWatcher::movement);
}

void SwitchWatcherHolder::joinSwitchEventListenerA(const StageSwitchCtrl* pCtrl, SwitchEventListener* pListener) {
    joinSwitchEventListener(pCtrl, 1, pListener);
}

void SwitchWatcherHolder::joinSwitchEventListenerB(const StageSwitchCtrl* pCtrl, SwitchEventListener* pListener) {
    joinSwitchEventListener(pCtrl, 2, pListener);
}

void SwitchWatcherHolder::joinSwitchEventListenerAppear(const StageSwitchCtrl* pCtrl, SwitchEventListener* pListener) {
    joinSwitchEventListener(pCtrl, 4, pListener);
}

SwitchWatcher* SwitchWatcherHolder::findSwitchWatcher(const StageSwitchCtrl* pCtrl) {
    for (SwitchWatcher** it = mSwitchWatcher.begin(); it != mSwitchWatcher.end(); it++) {
        if ((*it)->isSameSwitch(pCtrl)) {
            return *it;
        }
    }

    return nullptr;
}

void SwitchWatcherHolder::joinSwitchEventListener(const StageSwitchCtrl* pCtrl, u32 type, SwitchEventListener* pListener) {
    SwitchWatcher* pSwitchWatcher = findSwitchWatcher(pCtrl);

    if (pSwitchWatcher == nullptr) {
        pSwitchWatcher = new SwitchWatcher(pCtrl);
        addSwitchWatcher(pSwitchWatcher);
    }

    pSwitchWatcher->addSwitchListener(pListener, type);
}

void SwitchWatcherHolder::addSwitchWatcher(SwitchWatcher* pSwitchWatcher) {
    mSwitchWatcher.push_back(pSwitchWatcher);
}

namespace MR {
    SwitchWatcherHolder* getSwitchWatcherHolder() {
        return MR::getSceneObj< SwitchWatcherHolder >(SceneObj_SwitchWatcherHolder);
    }

    void requestMovementOnSwitchWatcher() {
        MR::requestMovementOn(getSwitchWatcherHolder());
    }
};  // namespace MR
