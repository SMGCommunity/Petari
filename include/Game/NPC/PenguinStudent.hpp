#pragma once

#include "Game/NPC/NPCActor.hpp"

class RemovableTurtle;

class PenguinStudent : public NPCActor {
public:
    PenguinStudent(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool branchFunc(u32);
    bool tryReleaseTurtle();
    void exeSwim();
    void exeLead();
    void exeSlow();

    s32 _15C;
    RemovableTurtle* mRemovableTurtle;  //_160
    LiveActor* mActor;                  //_164
    f32 _168;
};
