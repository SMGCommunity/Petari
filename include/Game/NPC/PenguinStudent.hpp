#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/Penguin.hpp"

class PenguinStudent : public NPCActor {
public:
    PenguinStudent(const char*);

    virtual ~PenguinStudent();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
