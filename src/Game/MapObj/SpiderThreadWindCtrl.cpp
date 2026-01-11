#include "Game/MapObj/SpiderThreadWindCtrl.hpp"
#include "Game/Util/MathUtil.hpp"

SpiderThreadWindCtrl::SpiderThreadWindCtrl()
    : mWindTargetSpeed(0.5), mWindSpeed(0.5), mWindAccel(0.0f), mWindDirection(0.0f, 0.0f, -1.0f), mWind(mWindDirection) {}

void SpiderThreadWindCtrl::update() {
    f32 target, speed, accel;
    speed = mWindSpeed;
    target = mWindTargetSpeed;

    // converge speed towards target... MR::converge maybe?
    if (speed != target) {
        accel = mWindAccel;
        if (speed < target) {
            speed += accel;
            if (speed > target) {
                speed = target;
            }
        } else {
            speed -= accel;
            if (speed < target) {
                speed = target;
            }
        }
        mWindSpeed = speed;
    }

    mWind.x = mWindDirection.x * mWindSpeed;
    mWind.y = mWindDirection.y * mWindSpeed;
    mWind.z = mWindDirection.z * mWindSpeed;
}

void SpiderThreadWindCtrl::startWindBattle() {
    mWindSpeed = 0.5f;
    mWindTargetSpeed = 0.5f;
    mWindAccel = 0.0f;
}

void SpiderThreadWindCtrl::startWindChance() {
    mWindSpeed = 1.0f;
    mWindTargetSpeed = 1.0f;
    mWindAccel = 0.0f;
}

void SpiderThreadWindCtrl::startWindBattleEnd() {
    mWindTargetSpeed = 2.0f;
    mWindSpeed = 100.0f;
    mWindAccel = 0.4f;
}

s32 SpiderThreadWindCtrl::getTimeToStartWind() const {
    if (mWindSpeed != mWindTargetSpeed) {
        return 30;
    }

    return MR::getRandom(static_cast< s32 >(30), static_cast< s32 >(120));
}

s32 SpiderThreadWindCtrl::getWindTime() const {
    return MR::getRandom(static_cast< s32 >(60), static_cast< s32 >(120));
}
