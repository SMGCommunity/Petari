#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CrystalSwitch : public LiveActor {
public:
    CrystalSwitch(const char*);

    /* 0x08 */ virtual ~CrystalSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    bool trySwitchDown();
    bool tryOn();
    bool tryOff();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();
    void calcRotSpeed();

    u32 _8C;
    s32 _90;
    f32 mRotateSpeed;  // 0x94
    u8 _98;
};
