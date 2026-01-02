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

    u32 _8C;
    s32 _90;
    f32 mRotateSpeed;  // 0x94
    u8 _98;
};
