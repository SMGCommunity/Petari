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
    GameEventFlag* findFlag(const char *);

    u32 _0;
    u32 _4;
};

class GameEventFlagTable {
public:
    static GameEventFlagIter getBeginIter();
    static GameEventFlagAccessor makeAccessor(const char *);
    static s32 getTableSize();
    static const GameEventFlag* getFlag(int);
    static const GameEventFlag* findFlag(const char *);
    const char* getEventFlagNameSpecialPowerStar(const char *, s32);
    bool isPowerStarType(const char *, s32, const char *);
    s32 calcExclamationGalaxyNum();
    const char* getExclamationGalaxyNameFromIndex(int);
    static bool isExist(const char *);
    static int getIndex(const GameEventFlag *);
    static bool isDependedAnother(const char *, const char *);
    int getIndexFromHashCode(u16);
    s32 calcSpecialPowerStarNum(const char *);
    u8 getStarPieceNumToOpenExclamationGalaxy(const char *);
    s32 calcGreenStarPowerNum();
    u32 getGalaxyDependFlags(const char **, int, const char *);
    
};