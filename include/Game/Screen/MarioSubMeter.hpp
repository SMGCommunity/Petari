#pragma once

#include "Game/NameObj/NameObj.hpp"

class SubMeterLayout;

class MarioSubMeter : public NameObj {
public:
    MarioSubMeter(const char* pName);

    virtual ~MarioSubMeter();

    virtual void init(const JMapInfoIter& rIter);

    void setBeeLifeRatio(f32);
    void setWaterLifeRatio(f32);
    void activeSubMeterBee();
    void activeSubMeterWater();
    void frameOutSubMeter();
    void activate();
    void deactivate();

    SubMeterLayout* mFlyMeter;     // 0xC
    SubMeterLayout* mAirMeter;     // 0x10
    SubMeterLayout* mActiveMeter;  // 0x14
};