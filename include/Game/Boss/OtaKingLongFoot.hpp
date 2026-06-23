#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class AnimScaleController;

class OtaKingLongFoot : public PartsModel {
public:
    OtaKingLongFoot(LiveActor* pHost, s32, const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void hide();
    void startAppearDemo();
    void startDownDemo();
    void endDemo();
    void initSensor();

    void exeWait();
    void exeDamage();
    void exeAppearDemo();
    void exeDownDemo();

    /* 0x9C */ s32 mDemoBckStep;
    /* 0xA0 */ AnimScaleController* mScaleController;
};
