#include "Game/System/GameSystemDimmingWatcher.hpp"
#include "Game/Util/TriggerChecker.hpp"
#include "revolution/vi.h"

GameSystemDimmingWatcher::GameSystemDimmingWatcher() {
    mChecker = nullptr;
    _4 = true;
    _5 = false;
    mChecker = new TriggerChecker();
}

void GameSystemDimmingWatcher::update() {
    mChecker->update(_4 || _5);
    if (mChecker->getOnTrigger()) {
        VIEnableDimming(1);
    }
    else if (mChecker->getOffTrigger()) {
        VIEnableDimming(0);
    }
}
