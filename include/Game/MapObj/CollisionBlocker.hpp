#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionBlocker : public LiveActor {
public:
    /// @brief Creates a new `CollisionBlocker`.
    /// @param pName A pointer to the null-terminated name of the object.
    CollisionBlocker(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void forceBreak();
};
