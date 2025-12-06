#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class HomingKiller : public LiveActor {
public:
    HomingKiller(const char*);

    virtual ~HomingKiller();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void appear(const TVec3f&, const TVec3f&);
    bool isMoveStart() const;

    s32 _8C;
    f32 mChaseStartDist;  // 0x90
    f32 mChaseEndDist;    // 0x94
    u32 _98;
    u32 _9C;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f _B8;
    TVec3f _C4;
    TVec3f _D0;
    TMtx34f _DC;
    TVec3f _110;
    u32 _118;
    u32 _11C;
    u32 _120;
    u32 _124;
    TMtx34f _128;
    HitSensor* _158;
    u8 _15C;
    u8 _15D;
    u8 _15E;
    PartsModel* mPropeller;   // 0x160
    ModelObj* mTorpedoLight;  // 0x164
};
