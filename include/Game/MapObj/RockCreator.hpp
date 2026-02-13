#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Rock;

class RockCreator : public LiveActor {
public:
    RockCreator(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();

    void invalidate();
    void initMapToolInfo(const JMapInfoIter& rIter);
    void create();

    void exeActive();
    void exeDeactive();

    inline bool isReadyToSpawn() const {
        return mSpawnTimer % mSpawnDelay == 0;
    }

    /* 0x8C */ Rock* mRocks;
    /* 0x90 */ f32 mRockSpeed;
    /* 0x94 */ bool mBreakModelOnRailGoal;
    /* 0x98 */ s32 mSpawnDelay;
    /* 0x9C */ s32 mNumRocks;
    /* 0xA0 */ u32 mSpawnTimer;
    /* 0xA4 */ bool mIsValid;
};
