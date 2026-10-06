#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BenefitItemOneUp;
class ModelObj;

class WoodBox : public LiveActor {
public:
    WoodBox(const char*);

    virtual ~WoodBox();
    virtual void init(const JMapInfoIter&);
    virtual void calcViewAndEntry();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeHit();
    void exeKilled();
    void doHit(HitSensor*, HitSensor*);

    /* 0x8C */ u16 mFloorTouchTimer;
    /* 0x8E */ u16 _8E;
    /* 0x90 */ u16 _90;
    /* 0x92 */ u16 mHitPoint;
    /* 0x94 */ bool mHasPowerStar;
    /* 0x95 */ bool mIsNoRespawn;
    /* 0x96 */ bool mPlaySolveSE;
    /* 0x97 */ bool _97;
    /* 0x98 */ Mtx mBaseMtx;
    /* 0xC8 */ s32 mCoinCount;
    /* 0xCC */ s32 mStarBitCount;
    /* 0xD0 */ ModelObj* mBreakModel;
    /* 0xD4 */ ModelObj* mStarDemoModel;
    /* 0xD8 */ BenefitItemOneUp* mOneUp;
};
