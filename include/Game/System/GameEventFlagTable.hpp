#pragma once

#include "Game/System/GameEventFlag.hpp"

#include <revolution.h>

class GameEventFlagIter {
public:
    bool isEnd() const;
    void goNext();
    const GameEventFlag* getFlag() const;
    bool isValid() const;

    s32 mIter;  // 0x0
};

class GameEventFlagTableInstance {
public:
    GameEventFlagTableInstance();

    void initSortTable();
    const GameEventFlag* findFlag(const char*);

    struct Key {
        u16 mHashCode;
        const GameEventFlag* mFlag;
    };
    Key* mSortTable;
    s32 mLength;
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
    int getIndex(const GameEventFlag*);
    bool isDependedAnother(const char*, const char*);
    int getIndexFromHashCode(u16);
    s32 calcSpecialPowerStarNum(const char*);
    s32 getStarPieceNumToOpenExclamationGalaxy(const char*);
    s32 calcGreenPowerStarNum();
    s32 getGalaxyDependedFlags(const char**, int, const char*);
};  // namespace GameEventFlagTable
