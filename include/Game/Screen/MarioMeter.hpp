#pragma once

#include "Game/NameObj/NameObj.hpp"

class MeterLayout;
class SuddenDeathMeter;

class MarioMeter : public NameObj {
public:
    MarioMeter(const char*);

    virtual ~MarioMeter();
    virtual void init(const JMapInfoIter& rIter);

    void initLifeCount(s32);
    void setLifeCount(s32);
    void powerUp();
    void requestForceAppearHPMeter();
    void requestPlayerMoving();
    void requestPlayerStopped();
    void activate();
    void deactivate();

    /* 0x0C */ MeterLayout* mHitPointMeter;
    /* 0x10 */ SuddenDeathMeter* mSuddenDeathMeter;
    /* 0x14 */ bool mUseSuddenDeath;
};
