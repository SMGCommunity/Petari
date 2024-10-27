#include "Game/Screen/MarioMeter.hpp"
#include "Game/Screen/MeterLayout.hpp"
#include "Game/Screen/SuddenDeathMeter.hpp"
#include "Game/Util/ObjUtil.hpp"

MarioMeter::MarioMeter(const char *pName) : NameObj(pName) {
    mHitPointMeter = nullptr;
    mSuddenDeathMeter = nullptr;
    mUseSuddenDeath = false;
}

void MarioMeter::initLifeCount(s32 life) {
    if (life == 1) {
        mUseSuddenDeath = true;
        mSuddenDeathMeter->initCount(life);
        mHitPointMeter->kill();
    }
    else {
        mUseSuddenDeath = false;
        mHitPointMeter->initCount(life);
        mSuddenDeathMeter->kill();
    }
}

void MarioMeter::setLifeCount(s32 life) {
    mUseSuddenDeath ? mSuddenDeathMeter->setCount(life) : mHitPointMeter->setCount(life);
}

void MarioMeter::powerUp() {
    if (mUseSuddenDeath) {
        MR::requestMovementOn(mSuddenDeathMeter);
        mSuddenDeathMeter->requestPowerUp();
    }
    else {
        MR::requestMovementOn(mHitPointMeter);
        mHitPointMeter->requestPowerUp();
    }
}

void MarioMeter::requestForceAppearHPMeter() {
    mUseSuddenDeath ? mSuddenDeathMeter->requestForceAppear() : mHitPointMeter->requestForceAppear();
}

void MarioMeter::requestPlayerMoving() {
    mUseSuddenDeath ? mSuddenDeathMeter->requestPlayerMoving() : mHitPointMeter->requestPlayerMoving();
}

void MarioMeter::requestPlayerStopped() {
    mUseSuddenDeath ? mSuddenDeathMeter->requestPlayerMoving() : mHitPointMeter->requestPlayerStopped();
}

void MarioMeter::activate() {
    mUseSuddenDeath ? mSuddenDeathMeter->requestActive() : mHitPointMeter->requestActive();
}

void MarioMeter::deactivate() {
    mUseSuddenDeath ? mSuddenDeathMeter->requestDeactivate() : mHitPointMeter->requestDeactivate();
}

MarioMeter::~MarioMeter() {

}

void MarioMeter::init(const JMapInfoIter &rIter) {
    mHitPointMeter = new MeterLayout("通常HPメーター", "HitPointMeter");
    mHitPointMeter->initWithoutIter();
    mSuddenDeathMeter = new SuddenDeathMeter("サドンデスHPメーター", "SuddenDeathMeter");
    mSuddenDeathMeter->initWithoutIter();
}
