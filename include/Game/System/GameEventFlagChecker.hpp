#pragma once

#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameDataHolder.h"

class GameEventFlagChecker {
public:
    bool canOn(const char *) const;
    bool isOn(const char *) const;
    bool tryOn(const char *);
    void reset();

    bool isOnGalaxy(const char *) const;

    GameDataHolder* mDataHolder;    // 0x0
    u32* _4;
};