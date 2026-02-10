#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinningBox : public LiveActor {
public:
    SpinningBox(const char*);

    virtual ~SpinningBox();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receveOtherMsg(u32, HitSensor*, HitSensor*);

    void generateIceBox(HitSensor*, HitSensor*);

    TVec4f _8C;
    f32 _9C;
    f32 _A0;
    f32 _A4;
    f32 _A8;
    f32 _AC;
    u8 _B0;
    u8 _B1;
    u8 _B2;
    u8 _B3;
    f32 _B4;
};
