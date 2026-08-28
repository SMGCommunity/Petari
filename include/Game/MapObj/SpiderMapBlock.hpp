#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpiderMapBlock : public LiveActor {
public:
    SpiderMapBlock(const char*);

    /* 0x08 */ virtual ~SpiderMapBlock();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    /* 0x8C */ f32 mPosZ;
};
