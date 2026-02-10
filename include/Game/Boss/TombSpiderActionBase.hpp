#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class TombSpider;

class TombSpiderActionBase : public ActorStateBase< TombSpider > {
public:
    TombSpiderActionBase(TombSpider*, const char*);

    virtual void init();
    virtual void appear();

    bool updateChanceStart();
    bool updateChanceEnd();
    bool tryWaitChanceStartDemo(const Nerve*);
    bool tryWaitChanceEndDemo(const Nerve*);
    void startChanceDamage();
    bool updateWait(s32, s32);
    bool updateDamageEye();
    bool updateDamageHip();
    bool updateDamageGland();
    bool updateAttackStart();
    bool updateAttackLoop(s32, s32, f32, f32);
    bool updateAttackEnd();
    bool updateChanceStartNoDemo();
    bool updateChanceStartDemo();
    bool updateChanceStartWait(s32);
    bool updateChanceDamage();
    bool updateChanceEndNoDemo();
    bool updateChanceEndDemo();

    /* 0x0C */ TombSpider* _C;  // ?
    /* 0x10 */ TombSpider* mParent;
    /* 0x14 */ f32 mRotateAccel;
    /* 0x18 */ f32 mRotateVel;
    /* 0x1C */ s32 _1C;  // acidTime?
    /* 0x20 */ s32 mWaitTime;
    /* 0x24 */ s32 mAttackTime;
    /* 0x28 */ s32 mHP;
    /* 0x2C */ f32 mPosZ;
    /* 0x30 */ bool mInDemo;
};
