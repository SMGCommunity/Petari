#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpiderMapBlock : public LiveActor {
public:
    SpiderMapBlock(const char*);

    virtual ~SpiderMapBlock();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    f32 _8C;
};
