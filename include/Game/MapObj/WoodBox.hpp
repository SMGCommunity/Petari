#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BenefitItemObj;

class WoodBox : public LiveActor {
public:
    WoodBox(const char *);

    virtual ~WoodBox();
    virtual void init(const JMapInfoIter &);
    virtual void calcViewAndEntry();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeHit();
    void doHit(HitSensor *, HitSensor *);

    u16 _8C;
    u16 _8E;
    u16 _90;
    u16 _92;
    bool _94;
    bool _95;
    bool _96;
    bool _97;
    Mtx _98;
    s32 mCoinCount;             // 0xC8
    s32 mStarBitCount;          // 0xCC
    ModelObj* mBrokenBoxModel;  // 0xD0
    ModelObj* mStarDemoModel;   // 0xD4
    BenefitItemObj* mOneUp;     // 0xD8
};