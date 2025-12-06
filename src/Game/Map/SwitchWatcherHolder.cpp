#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Map/SwitchWatcher.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

SwitchWatcherHolder::SwitchWatcherHolder() : NameObj("SwitchWatcherHolder") {
    mWatcherCount = 0;
    MR::connectToScene(this, 0x1B, -1, -1, -1);
}

void SwitchWatcherHolder::movement() {
    // not quite sure what is going on here
    // but it seems to just call movement on all watchers
    mWatchers.callAllFunc(&SwitchWatcher::movement);
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
    for (SwitchWatcher** it = mWatchers.begin(); it != mWatchers.end(); it++) {
        if ((*it)->isSameSwitch(pCtrl)) {
            return *it;
        }
    }

    return nullptr;
}

void SwitchWatcherHolder::joinSwitchEventListener(const StageSwitchCtrl* pCtrl, u32 type, SwitchEventListener* pListener) {
    SwitchWatcher* watcher = findSwitchWatcher(pCtrl);
    if (watcher == nullptr) {
        watcher = new SwitchWatcher(pCtrl);
        addSwitchWatcher(watcher);
    }
    watcher->addSwitchListener(pListener, type);
}

void SwitchWatcherHolder::addSwitchWatcher(SwitchWatcher* pWatcher) {
    mWatchers[++mWatcherCount] = pWatcher;
}

namespace MR {
    SwitchWatcherHolder* getSwitchWatcherHolder() {
        return MR::getSceneObj< SwitchWatcherHolder >(SceneObj_SwitchWatcherHolder);
    }

    void requestMovementOnSwitchWatcher() {
        MR::requestMovementOn(getSwitchWatcherHolder());
    }
};  // namespace MR

SwitchWatcherHolder::~SwitchWatcherHolder() {}
