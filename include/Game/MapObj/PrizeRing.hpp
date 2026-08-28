#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class PrizeRing : public ModelObj {
public:
    PrizeRing();

    /* 0x08 */ virtual ~PrizeRing();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    inline s32 getNumber() const;

    void setLife(int);
    void setNumber(int);
    void setNervePass();
    bool isReadyToPass() const;
    bool isReadyToKill() const;
    void exeStart();
    void exeLoop();
    void exeTimeout();
    void exeReadyToKill();
    void exeReadyToPass();
    void exePass();
    bool isPassed() const;
    void playSound() const;
    bool isOnTriggerTimeoutFlash() const;

    /* 0x90 */ int mLifeTime;
};
