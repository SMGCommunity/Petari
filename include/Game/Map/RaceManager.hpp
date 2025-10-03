#pragma once

#include "JSystem/JGeometry.hpp"

class PlayerRacer;

class RaceManager {
public:

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

class AbstractAudience {
public:

};

namespace RaceManagerFunction {
    void entryRacerOthers(AbstractRacer*);
    void entryAudience(AbstractAudience*);
    void entryRacerPlayer(PlayerRacer*);
    void startRaceWithWipe();
    void startRaceImmediately();
    u32 getRaceRank();
    u32 getRaceTime();
    const char* getRaceName(int);
    const char* getRaceMessageId(int);
    s32 getRaceId(const char*, s32);
    bool hasPowerStarRaceScenario(int);
};
