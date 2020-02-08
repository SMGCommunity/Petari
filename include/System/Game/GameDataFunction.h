#ifndef GAMEDATAFUNCTION_H
#define GAMEDATAFUNCTION_H

#include <revolution.h>

class GameDataFunction
{
public:
    static bool isPowerStarLeftInCometOnly();
    static bool isOnGameEventFlag(const char *);
    static bool isOnJustGameEventFlag(const char *);
    static void updateGalaxyCometStatus(s32, s16, u16);
    static void restoreGalaxyCometStatus(s32, u16 *, u16 *);
};

#endif // GAMEDATAFUNCTION_H