#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MarioSearchLight : public PartsModel {
public:
    MarioSearchLight(LiveActor* pActor);
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor* pSensor);
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    /* 0x9C */ f32 _9C;
    /* 0xA0 */ u16 _A0;
    /* 0xA4 */ HitSensor* _A4;
};
