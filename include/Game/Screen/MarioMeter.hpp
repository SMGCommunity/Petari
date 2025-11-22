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

    MeterLayout* mHitPointMeter;          // 0xC
    SuddenDeathMeter* mSuddenDeathMeter;  // 0x10
    bool mUseSuddenDeath;                 // 0x14
};