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
    const GameEventFlag* findFlag(const char *);
    
    struct Key {
        u16 mHashCode;
        const GameEventFlag* mFlag;
    };
    Key* mSortTable;
    s32 mLength;
};

class GameEventFlagTable {
public:
    static GameEventFlagIter getBeginIter();
    static GameEventFlagAccessor makeAccessor(const char *);
    static s32 getTableSize();
    static const GameEventFlag* getFlag(int);
    static const GameEventFlag* findFlag(const char *);
    static const char* getEventFlagNameSpecialPowerStar(const char *, s32);
    static bool isPowerStarType(const char *, s32, const char *);
    static s32 calcExclamationGalaxyNum();
    static const char* getExclamationGalaxyNameFromIndex(int);
    static bool isExist(const char *);
    static int getIndex(const GameEventFlag *);
    static bool isDependedAnother(const char *, const char *);
    static int getIndexFromHashCode(u16);
    static s32 calcSpecialPowerStarNum(const char *);
    static s32 getStarPieceNumToOpenExclamationGalaxy(const char *);
    static s32 calcGreenPowerStarNum();
    static s32 getGalaxyDependedFlags(const char **, int, const char *);
    
};
