#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util.hpp"

ParabolicPath::ParabolicPath() : mPosition(0, 0, 0), mAxisY(0, 1, 0), mAxisZ(0, 0, 1) {
    _24 = 0.0f;
    _28 = 0.0f;
    _2C = 0.0f;
}

void ParabolicPath::initFromUpVectorAddHeight(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3, f32 a4) {
    f32 dot = a3.dot(a2 - a1);

    if (dot < 0.0f) {
        dot = 0.0f;
    }

    initFromUpVector(a1, a2, a3, a4 + dot);
}

/*
void ParabolicPath::calcPosition(TVec3f *pOutPosition, f32 a2) const {
    f32 thing = _24 * a2;
    f32 v5 = (a2 * (_28 + (thing)));
    TVec3f stack_8 = mAxisZ * (_2C * a2);
    TVec3f stack_14 = mAxisY * v5;
    TVec3f stack_20 = mPosition + stack_14;
    TVec3f stack_2C = stack_20 + stack_8;
    pOutPosition->set(stack_2C);
}
*/

void ParabolicPath::calcDirection(TVec3f *pOutDirection, f32 a2, f32 a3) const {
    f32 f0;
    f32 f31;

    if (a2 < a3) {
        f31 = a3;
        f0 = 0.0f;
    }
    else {
        f0 = 1.0f;
        f31 = 1.0f - a3;

        if (a2 > f31) {
            f0 = a2;
            f31 = a2 + a3;
        }
    }

    TVec3f stack_20;
    calcPosition(&stack_20, f0);
    TVec3f stack_14;
    calcPosition(&stack_14, f31);
    pOutDirection->set(stack_14 - stack_20);
    MR::normalizeOrZero(pOutDirection);
}

f32 ParabolicPath::getTotalLength(s32 a1) const {
    return getLength(0.0f, 1.0f, a1);
}

f32 ParabolicPath::calcPathSpeedFromAverageSpeed(f32 speed) const {
    return speed / getLength(0.0f, 1.0f, 10);
}