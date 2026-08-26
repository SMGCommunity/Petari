#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MeteorStrike;

class MeteorStrikeLauncher : public LiveActor {
public:
    MeteorStrikeLauncher(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();

    void initMapToolInfo(const JMapInfoIter&);
    bool initRail(const JMapInfoIter&);
    MeteorStrike* getUnusedMeteorStrike();
    bool create();

    void exeCreate();
    void exeInterval();

private:
    /* 0x8C */ MeteorStrike** mMeteorStrikeArray;
    /* 0x90 */ s32 mMeteorStrikeCount;
    /* 0x94 */ s32 mSpawnDelay;  // Obj_arg1, in seconds
    /* 0x98 */ s32 mSpawnDistance;
    /* 0x9C */ bool mIsMeteorStrike;
    /* 0x9D */ bool mAllowMultipleMeteors;  // Obj_arg2
};
