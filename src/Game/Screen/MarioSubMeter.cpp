#include "Game/Screen/MarioSubMeter.hpp"
#include "Game/Screen/SubMeterLayout.hpp"

MarioSubMeter::MarioSubMeter(const char *pName) : NameObj(pName)
{
    mFlyMeter = nullptr;
    mAirMeter = nullptr;
    mActiveMeter = nullptr;
}

void MarioSubMeter::setBeeLifeRatio(f32 a1) {
    mFlyMeter->setLifeRatio(a1);
}

void MarioSubMeter::setWaterLifeRatio(f32 a1) {
    mAirMeter->setLifeRatio(a1);
}

void MarioSubMeter::activeSubMeterBee() {
    if (mActiveMeter != mFlyMeter) {
        if (mActiveMeter != nullptr) {
            mActiveMeter->requestDeactivate();
            mActiveMeter = nullptr;
        }
        mFlyMeter->requestAppear();
        mActiveMeter = mFlyMeter;
    }
}

void MarioSubMeter::activeSubMeterWater() {
    if (mActiveMeter != mAirMeter) {
        if (mActiveMeter != nullptr) {
            mActiveMeter->requestDeactivate();
            mActiveMeter = nullptr;
        }
        mAirMeter->requestAppear();
        mActiveMeter = mAirMeter;
    }
}

void MarioSubMeter::frameOutSubMeter() {
    if (mActiveMeter != nullptr) {
        mActiveMeter->requestFrameOut();
        mActiveMeter = nullptr;
    }
}

void MarioSubMeter::activate() {
    if (mActiveMeter != nullptr) {
        mActiveMeter->requestFrameIn();
    }
}

void MarioSubMeter::deactivate() {
    mFlyMeter->requestDeactivate();
    mAirMeter->requestDeactivate();
}

MarioSubMeter::~MarioSubMeter() {

}

void MarioSubMeter::init(const JMapInfoIter &rIter) {
    mFlyMeter = new SubMeterLayout("ハチメーター", "FlyMeter");
    mFlyMeter->initWithoutIter();

    mAirMeter = new SubMeterLayout("水中メーター", "AirMeter");
    mAirMeter->initWithoutIter();
}
