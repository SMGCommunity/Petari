#pragma once

#include "Game/Util/TriggerChecker.hpp"
#include <revolution.h>

class GameSystemDimmingWatcher {
public:
    GameSystemDimmingWatcher();

    void update();

    TriggerChecker* getTriggerChecker() const {
        return mChecker;
    }

    TriggerChecker* mChecker;           // 0x00
    bool _4;
    bool _5;
};
