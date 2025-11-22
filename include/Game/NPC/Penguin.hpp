#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/JMapInfo.hpp"

class RemovableTurtle {
public:
    RemovableTurtle(LiveActor*, bool);

    bool isPullRange() const;
    bool tryRemove();
    bool tryAttach();
};

class Penguin : public NPCActor {
public:
    Penguin(const char*);

    virtual ~Penguin();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeReaction();
    void exeWait();
    void exeDive();
    void exeFlow();
    bool tryReleaseTurtle();
};
