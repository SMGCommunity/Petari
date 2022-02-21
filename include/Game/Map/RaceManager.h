#pragma once

#include "JSystem/JGeometry.h"

class PlayerRacer;

class RaceManager {
public:

};

class RaceManagerFunction {
public:
    static void startRaceImmediately();
    static void entryRacerPlayer(PlayerRacer *);
};

class AbstractRacer {
public:
    AbstractRacer() {
        initRacer();
    }

    virtual void initRacer() {
        _4 = 0.0f;
        _8 = 0.0f;
        _C.zero();
        _18.zero();
    }

    virtual void prepRacer(const RaceManager *) = 0;
    virtual void startRacer() = 0;
    virtual void updateRacer(const RaceManager *) = 0;
    virtual bool goalRacer() = 0;
    virtual void loseRacer() = 0;
    virtual void resetRacer(const RaceManager *) = 0;
    virtual void exitRacer() = 0;

    f32 _4;
    f32 _8;
    TVec3f _C;
    TVec3f _18;
};