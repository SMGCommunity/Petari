#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"

class TripodBossFixParts : public TripodBossFixPartsBase {
public:
    TripodBossFixParts(const char*);

    virtual ~TripodBossFixParts();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void activateTripodBoss();
    virtual void exeWaitOwn();

    void initSensor(const JMapInfoIter&);
    void exeStartDemo();
    void exeWait();
    void exeBreak();
    inline void exeNonActive();

    s32 _E4;
    s32 _E8;
    bool mHasCollision;  // 0xEC
    bool _ED;
    u8 _EE;
    bool mIsPlayingWarning;  // 0xEF
};
