#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

class TripodBossFixParts : public TripodBossFixPartsBase {
public:
    TripodBossFixParts(const char*);

    /* 0x08 */ virtual ~TripodBossFixParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();

    /* 0x7C */ virtual void activateTripodBoss();
    /* 0x80 */ virtual void exeWaitOwn();

    void initSensor(const JMapInfoIter&);

    void exeNonActive();
    void exeStartDemo();
    void exeWait();
    void exeBreak();

    /* 0xE4 */ s32 _E4;
    /* 0xE8 */ s32 _E8;
    /* 0xEC */ bool mHasCollision;
    /* 0xED */ bool _ED;
    /* 0xEE */ u8 _EE;
    /* 0xEF */ bool mIsPlayingWarning;
};
