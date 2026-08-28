#pragma once

#include "Game/NPC/NPCActor.hpp"

class FollowCollisionArea;

class SignBoard : public NPCActor {
public:
    /// @brief Creates a new `SignBoard`.
    /// @param pName A pointer to the null-terminated name of the object.
    SignBoard(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();

    /* 0x15C */ u32 _15C;
};
