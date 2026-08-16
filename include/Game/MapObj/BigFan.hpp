#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelObj;

class BigFan : public LiveActor {
public:
    BigFan(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void initWindModel();
    void calcWindInfo(TVec3f*, const TVec3f&);
    void start();

    void exeStop();
    void exeStart();
    void exeWait();

    bool isStartOrWait();

    /* 0x8C */ ModelObj* mWindModel;
    /* 0x90 */ TVec3f mClippingCenter;
    /* 0x9C */ f32 mWindLength;
    /* 0xA0 */ f32 mWindSpeed;
    /* 0xA4 */ bool mIsTeresaGalaxy;
};
