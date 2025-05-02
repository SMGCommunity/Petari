#include "Game/Util/ValueControl.hpp"

ValueControl::ValueControl(int a1) {
    mCurrentFrame = 0;
    mMaxFrame = a1;
    mDirection = DIRECTION_FORWARD;
}

inline s32 ValueControl::getDirection() const {
    return mDirection;
}

void ValueControl::setDirToOne() {
    mDirection = DIRECTION_FORWARD;
}

void ValueControl::setDirToOneResetFrame() {
    mDirection = DIRECTION_FORWARD;
    resetFrame();
}

void ValueControl::setDirToZero() {
    mDirection = DIRECTION_BACKWARD;
}

void ValueControl::setDirToZeroResetFrame() {
    mDirection = DIRECTION_BACKWARD;
    resetFrame();
}

void ValueControl::setMaxFrame(int newMaxFrame) {
    f32 value = (f32)(mCurrentFrame) / (f32)(mMaxFrame);

    mMaxFrame = newMaxFrame;
    mCurrentFrame = static_cast<int>(value * static_cast<f32>(newMaxFrame));
}

f32 ValueControl::getValue() const {
    return (f32)mCurrentFrame / (f32)mMaxFrame;
}

inline bool ValueControl::goingBackward(s32 currentDirection) {
    return (u32)(-currentDirection & ~currentDirection) >> 31 == 0;
}

void ValueControl::update()
{
    s32 currentDirection = mDirection;
    if ((currentDirection > 0 && mCurrentFrame != mMaxFrame) ||
    (goingBackward(currentDirection) && mCurrentFrame))
    {
        mCurrentFrame += getDirection();
    }
}

void ValueControl::setZero() {
    mDirection = DIRECTION_BACKWARD;
    resetFrame();
    mCurrentFrame = 0;
}

void ValueControl::setOne() {
    mDirection = DIRECTION_FORWARD;
    resetFrame();
    mCurrentFrame = mMaxFrame;
}

void ValueControl::resetFrame() {
    if (getDirection() > 0) {
        mCurrentFrame = 0;
    }
    else {
       mCurrentFrame = mMaxFrame;
    }
}
