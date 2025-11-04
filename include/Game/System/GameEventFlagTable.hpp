#pragma once

#include "Game/System/GameEventFlag.hpp"

#include <revolution.h>

class GameEventFlagIter {
public:
    bool isEnd() const;
    void goNext();
    const GameEventFlag* getFlag() const;
    bool isValid() const;

    u32 mIter;  // 0x0
};

class GameEventFlagTableInstance {
public:
    GameEventFlagTableInstance();

    void initSortTable();
    const GameEventFlag* findFlag(const char *);

    u32 _0;
    u32 _4;
};

namespace GameEventFlagTable {
    GameEventFlagIter getBeginIter();
    GameEventFlagAccessor makeAccessor(const char*);
    s32 getTableSize();
    const GameEventFlag* getFlag(int);
    const GameEventFlag* findFlag(const char*);
    const char* getEventFlagNameSpecialPowerStar(const char*, s32);
    bool isPowerStarType(const char*, s32, const char*);
    s32 calcExclamationGalaxyNum();
    const char* getExclamationGalaxyNameFromIndex(int);
    bool isExist(const char*);
    s32 getIndex(const GameEventFlag*);
    bool isDependedAnother(const char*, const char*);
    s32 getIndexFromHashCode(u16);
    s32 calcSpecialPowerStarNum(const char*);
    s32 getStarPieceNumToOpenExclamationGalaxy(const char*);
    s32 calcGreenPowerStarNum();
    u32 getGalaxyDependedFlags(const char**, int, const char*);
};
