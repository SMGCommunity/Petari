#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MarioSearchLight : public PartsModel {
public:
    MarioSearchLight(LiveActor* pActor);
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

private:
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ u16 _A0;
    /* 0xA4 */ HitSensor* _A4;
};
