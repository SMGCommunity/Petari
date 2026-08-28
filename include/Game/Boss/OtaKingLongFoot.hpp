#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class AnimScaleController;

class OtaKingLongFoot : public PartsModel {
public:
    OtaKingLongFoot(LiveActor* pHost, s32, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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
