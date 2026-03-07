#pragma once

#include "Game/LiveActor/LiveActor.hpp"

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

    u16 mFloorTouchTimer; // 0x8C
    u16 _8E; // 0x8E
    u16 _90; // 0x90
    u16 mHitPoint; // 0x92
    bool mHasPowerStar; // 0x94
    bool mIsNoRespawn; // 0x95
    bool mPlaySolveSE; // 0x96
    bool _97; //0x97
    Mtx mBaseMtx; // 0x98
    s32 mCoinCount;             // 0xC8
    s32 mStarBitCount;          // 0xCC
    ModelObj* mBreakModel;  // 0xD0
    ModelObj* mStarDemoModel;   // 0xD4
    BenefitItemOneUp* mOneUp;     // 0xD8
};
