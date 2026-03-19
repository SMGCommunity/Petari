#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HitSensor;

class MogucchiHill : public LiveActor {
public:
    MogucchiHill(LiveActor*, s32, const char*);

    virtual void init();
    virtual void control();

    virtual void notifyAppearAttackToPlayer(HitSensor*, HitSensor*);
    virtual void notifyWaitAttackToPlayer(HitSensor*, HitSensor*);

    void start();
    void end();
    void startNaturally();
    void endNaturally();

    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;

    Mtx _9C;

    u8 _CC;
    f32 _D0;
    f32 _D4;
    u32 _D8;
    u32 _DC;
    u32 _E0;
    u32 _E4;
    u8 _E8;
    u32 _EC;
    u32 _F0;
    u32 _F4;
};
