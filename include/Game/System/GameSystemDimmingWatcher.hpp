#pragma once

class TriggerChecker;

class GameSystemDimmingWatcher {
public:
    /// @brief Creates a new `GameSystemDimmingWatcher`.
    GameSystemDimmingWatcher();

    void update();

    /* 0x0 */ TriggerChecker* mChecker;
    /* 0x4 */ bool _4;
    /* 0x5 */ bool _5;
};
