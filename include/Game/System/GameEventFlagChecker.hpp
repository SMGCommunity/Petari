#pragma once

#include "Game/System/GameDataHolder.hpp"
#include "Game/System/GameEventFlagStorage.hpp"
#include "Game/System/GameEventFlagTable.hpp"

class GameEventFlagChecker {
public:
    GameEventFlagChecker(GameDataHolder*);
    bool canOn(const char*) const;
    bool isOn(const char*) const;
    bool tryOn(const char*);
    void reset();
    GameEventFlagStorage* getChunk();
    bool isOnGalaxy(const char*) const;
    bool isOnGalaxyDepended(const char*) const;
    bool isOnComet(const GameEventFlag*) const;

    GameDataHolder* mDataHolder;         // 0x0
    GameEventFlagStorage* mFlagStorage;  // 0x4
};
