#pragma once

#include "Game/System/GameEventFlagTable.h"
#include "Game/System/GameDataHolder.h"

class GameEventFlagChecker {
public:
    bool canOn(const char *) const;
    bool isOn(const char *) const;
    bool tryOn(const char *);
    void reset();

    bool isOnGalaxy(const char *) const;

    GameDataHolder* mDataHolder;    // _0
    u32* _4;
};