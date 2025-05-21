#pragma once

#include <revolution.h>

class GameDataFunction {
public:
    static bool canOnGameEventFlag(const char *);

    static s32 getPowerStarNumOwned(const char *);

    static bool hasPowerStar(const char *, s32);

    static bool isPowerStarLeftInCometOnly();
    static bool isOnGameEventFlag(const char *);
    static bool isOnJustGameEventFlag(const char *);
    static void updateGalaxyCometStatus(s32, s16, u16);
    static void restoreGalaxyCometStatus(s32, u16 *, u16 *);

    static s32 getStarPieceNum();
    static void addStarPiece(int);
    static s32 getLast1upStarPieceNum();

    static void setLast1upStarPieceNum(int);

    static OSTime getSysConfigFileTimeAnnounced();
    static void updateSysConfigFileTimeAnnounced();
};
