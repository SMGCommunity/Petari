#pragma once

#include "Game/NPC/NPCActor.hpp"

class FollowCollisionArea;

class SignBoard : public NPCActor {
public:
    /// @brief Creates a new `SignBoard`.
    /// @param pName A pointer to the null-terminated name of the object.
    SignBoard(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();

    /* 0x15C */ u32 _15C;
};
