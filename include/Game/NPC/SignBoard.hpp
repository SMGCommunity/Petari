#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/NPC/NPCActor.hpp"

class FollowCollisionArea;

class SignBoard : public NPCActor {
public:
    SignBoard(const char*);

    virtual ~SignBoard();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();

    u32 _15C;
};
