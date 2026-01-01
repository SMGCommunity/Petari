#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ArrowSwitch : public LiveActor {
public:
    ArrowSwitch(const char*);

    virtual ~ArrowSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    bool requestPunch(HitSensor*, HitSensor*);
    void listenOnSwitch();
    void listenOffSwitch();
    void exeWait();
    void exeRotate();
    void exeLock();
    bool isPlusLimit() const;
    bool isMinusLimit() const;
    s32 getOneStep() const;

    f32 _8C;
    s32 mRotationIdx;  // 0x90
    f32 _94;
    s32 _98;
    bool _9C;
    u8 _9D;
    u8 _9E;
    u8 _9F;
};
