#pragma once

#include "Game/System/GameEventFlag.h"

#include <revolution.h>

class GameEventFlagIter {
public:
    bool isEnd() const;
    void goNext();
    const GameEventFlag* getFlag() const;
    bool isValid() const;

    u32 mIter;  // _0
};

class GameEventFlagTableInstance {
public:
    void initSortTable();

    u32 _0;
    u32 _4;
};

class GameEventFlagTable {
public:
    static GameEventFlagIter getBeginIter();
    static GameEventFlagAccessor makeAccessor(const char *);
    s32 getTableSize();
    const GameEventFlag* getFlag(int);
    const GameEventFlag* findFlag(const char *);
    const char* getEventFlagNameSpecialPowerStar(const char *, s32);
    bool isPowerStarType(const char *, s32, const char *);
    s32 calcExclamationGalaxyNum();
    const char* getExclamationGalaxyNameFromIndex(int);
    static bool isExist(const char *);
    int getIndex(const GameEventFlag *);
    static bool isDependedAnother(const char *, const char *);
    int getIndexFromHashCode(u16);
    s32 calcSpecialPowerStarNum(const char *);
    u8 getStarPieceNumToOpenExclamationGalaxy(const char *);
    s32 calcGreenStarPowerNum();
    u32 getGalaxyDependFlags(const char **, int, const char *);
    
};