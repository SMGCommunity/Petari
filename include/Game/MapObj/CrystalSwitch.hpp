#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CrystalSwitch : public LiveActor {
public:
    CrystalSwitch(const char*);

    virtual ~CrystalSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    bool trySwitchDown();
    bool tryOn();
    bool tryOff();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();
    void calcRotSpeed();

    /* 0x8C */ u32 _8C;
    /* 0x90 */ s32 _90;
    /* 0x94 */ f32 mRotateSpeed;
    /* 0x98 */ u8 _98;
};
