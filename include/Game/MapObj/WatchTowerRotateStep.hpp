#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PartsModel;

class WatchTowerRotateStep : public LiveActor {
public:
    /// @brief Creates a new `WatchTowerRotateStep`.
    /// @param pName A pointer to the null-terminated name of the object.
    WatchTowerRotateStep(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();

    void initLift(const JMapInfoIter& rIter);
    void attachLift();

    void exeWait();
    void exeMoveStart();
    void exeMove();

    /* 0x8C */ TVec3f mRotateVec;
    /* 0x98 */ PartsModel** mLift;
};
