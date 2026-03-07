#pragma once

#include "Game/System/GameEventFlag.hpp"
#include <revolution/types.h>

class GameEventFlagIter {
public:
    bool isEnd() const;
    void goNext();
    const GameEventFlag* getFlag() const;
    bool isValid() const;

    /* 0x00 */ s32 mIter;
};

class GameEventFlagTableInstance {
public:
    struct Key {
        u16 mHashCode;
        const GameEventFlag* mFlag;
    };

    GameEventFlagTableInstance();

    void initSortTable();
    const GameEventFlag* findFlag(const char*);

    /* 0x00 */ Key* mSortTable;
    /* 0x04 */ s32 mLength;
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
