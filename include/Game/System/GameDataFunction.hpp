#pragma once

#include <revolution.h>

class JMapInfoIter;

class GameDataFunction {
public:
    static bool isDataMario();

    static bool canOnGameEventFlag(const char *);

    static void onGameEventFlag(const char *);

    static s32 getPowerStarNumOwned(const char *);

    static bool hasPowerStar(const char *, s32);
    static bool hasGrandStar(int);

    static bool isPowerStarLeftInCometOnly();
    static bool isOnGameEventFlag(const char *);
    static bool isOnJustGameEventFlag(const char *);
    static void updateGalaxyCometStatus(s32, s16, u16);
    static void restoreGalaxyCometStatus(s32, u16 *, u16 *);

    static s32 getStarPieceNum();
    static s32 getStockedStarPieceNum();
    static void addStarPiece(int);
    static void addStockedStarPiece(int);
    static s32 getLast1upStarPieceNum();

    static void setLast1upStarPieceNum(int);

    static OSTime getSysConfigFileTimeAnnounced();
    static void updateSysConfigFileTimeAnnounced();

    static s32 getPlayerLeft();
    static void addPlayerLeft(int);

    static void addMissPoint(int);
    static void resetMissPoint();
    static void incPlayerMissNum();

    static bool isPlayerLeftSupply();
    static bool isLuigiLeftSupply();

    static void offPlayerLeftSupply();
    static void offLuigiLeftSupply();

    static bool isPointCollectForLetter();

    static bool isPassedStoryEvent(const char *);

    static bool isOnCompleteEndingMarioAndLuigi();

    static s32 setupAlreadyDoneFlag(const char *, const JMapInfoIter &, u32 *);
    static void updateAlreadyDoneFlag(int, u32);

    static bool isOnGalaxyScenarioFlagAlreadyVisited(const char *, s32);

    static bool isAppearGalaxy(const char *);
    static bool isAppearGalaxyGreenDriver(const char *);

    static bool hasPowerStarYetAtResultSequence();
    static bool isPowerStarGreenAtResultSequence();
    static bool isPowerStarRedAtResultSequence();
};
