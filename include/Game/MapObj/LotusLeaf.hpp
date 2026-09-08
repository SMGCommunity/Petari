#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LotusLeaf : public LiveActor {
public:
    /// @brief Creates a new `LotusLeaf`.
    /// @param pName A pointer to the null-terminated name of the object.
    LotusLeaf(const char* pName);

    virtual void init(const JMapInfoIter&);

    void convergeToInitPos();

    void exeWait();
    void exeWaitPlayerOn();
    void exeShake();

    /* 0x8C */ TVec3f mInitPos;
    /* 0x98 */ f32 mShakeSpeed;
    /* 0x9C */ f32 mShakePeriod;
};
