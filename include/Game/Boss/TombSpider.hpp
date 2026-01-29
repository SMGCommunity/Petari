#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class TombSpiderSensorCtrl;
class TombSpiderParts;
class TombSpiderEnvironment;
class TombSpiderActionCocoon;
class TombSpiderAction1st;
class TombSpiderAction2nd;
class TombSpiderDemo;

class TombSpider : public LiveActor {
public:
    TombSpider(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWaitPlayer();
    void exeDemoGateOpen();
    void exeActionCocoon();
    void exeDemoBattle1stStart();
    void exeDemoBattle1stStartJumpToPlayer();
    void exeAction1st();
    void exeDemoBattle2ndStart();
    void exeDemoBattle2ndStartJumpToPlayer();
    void exeAction2nd();
    void exeDemoDeath();
    void exeWaitDemo();

    /* 0x8C */ TombSpiderSensorCtrl* mSensorCtrl;
    /* 0x90 */ TombSpiderParts* mParts;
    /* 0x94 */ TombSpiderEnvironment* mEnvironment;
    /* 0x98 */ TombSpiderActionCocoon* mActionCocoon;
    /* 0x9C */ TombSpiderAction1st* mAction1st;
    /* 0xA0 */ TombSpiderAction2nd* mAction2nd;
    /* 0xA4 */ TombSpiderDemo* mDemo;
    /* 0xA8 */ bool mNoctilucaActive;
    /* 0xAC */ ActorCameraInfo* mCameraInfo;
};
