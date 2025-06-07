#include "Game/Util/ValueControl.hpp"

ValueControl::ValueControl(int maxFrame) :
    mFrame(0),
    mMaxFrame(maxFrame),
    mDirection(Direction_Forward)
{}

void ValueControl::setDirToOne() {
    mDirection = Direction_Forward;
}

void ValueControl::setDirToOneResetFrame() {
    setDirToOne();
    resetFrame();
}

void ValueControl::setDirToZero() {
    mDirection = Direction_Backward;
}

void ValueControl::setDirToZeroResetFrame() {
    setDirToZero();
    resetFrame();
}

void ValueControl::setMaxFrame(int maxFrame) {
    f32 value = getValue();

    mMaxFrame = maxFrame;
    mFrame = value * mMaxFrame;
}

void ValueControl::update() {
    if ((mDirection > 0 && mFrame != mMaxFrame) || (isDirToZero(mDirection) && mFrame != 0)) {
        mFrame += getDirection();
    }
}

void ValueControl::setZero() {
    setDirToZeroResetFrame();

    mFrame = 0;
}

void ValueControl::setOne() {
    setDirToOneResetFrame();

    mFrame = mMaxFrame;
}

f32 ValueControl::getValue() const {
    return static_cast<f32>(mFrame) / mMaxFrame;
}

s32 ValueControl::getDirection() const {
    return mDirection;
}

void ValueControl::resetFrame() {
    if (mDirection > 0) {
        mFrame = 0;
    }
    else {
        mFrame = mMaxFrame;
    }
}
