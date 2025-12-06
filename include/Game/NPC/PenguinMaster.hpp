#pragma once

#include "Game/NPC/NPCActor.hpp"

class PenguinMaster : public NPCActor {
public:
    PenguinMaster(const char*);

    virtual ~PenguinMaster();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    inline void exeWait();
    void exeReaction();
};
