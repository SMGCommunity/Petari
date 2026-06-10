#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class AnimScaleController;

class OtaKingLongFoot : public PartsModel {
public:
    OtaKingLongFoot(LiveActor* pHost, s32, const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void initSensor();
    void hide();
    void startAppearDemo();
    void startDownDemo();
    void endDemo();

    void exeWait();
    void exeDamage();
    void exeAppearDemo();
    void exeDownDemo();

    /* 0x9C */ s32 _9C;
    /* 0xA0 */ AnimScaleController* _A0;
};
