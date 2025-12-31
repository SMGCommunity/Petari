#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

ParabolicPath::ParabolicPath() : mPosition(0, 0, 0), mAxisY(0, 1, 0), mAxisZ(0, 0, 1) {
    _24 = 0.0f;
    _28 = 0.0f;
    _2C = 0.0f;
}

void ParabolicPath::initFromMaxHeight(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3) {
    f32 scalar;
    TVec3f direction;

    MR::separateScalarAndDirection(&scalar, &direction, a3 - a2);

    initFromUpVector(a1, a2, direction, direction.dot(a3 - a1));
}

void ParabolicPath::initFromUpVector(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, f32 f1) {
    mAxisY.set(a3);

    f32 dot = mAxisY.dot(a2 - a1);
    mAxisZ.setPS2(a2 - a1 - (mAxisY * dot));

    MR::separateScalarAndDirection(&_2C, &mAxisZ, mAxisZ);
    MR::calcParabolicFunctionParam(&_24, &_28, f1, dot);

    mPosition.set(a1);
}

void ParabolicPath::initFromUpVectorAddHeight(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, f32 a4) {
    f32 dot = a3.dot(a2 - a1);

    if (dot < 0.0f) {
        dot = 0.0f;
    }

    initFromUpVector(a1, a2, a3, a4 + dot);
}

void ParabolicPath::calcPosition(TVec3f* pOutPosition, f32 a2) const {
    f32 thing = _2C * a2;
    f32 v5 = (a2 * (_28 + (_24 * a2)));

    pOutPosition->set(mPosition + mAxisY * v5 + mAxisZ * (thing));
}

void ParabolicPath::calcDirection(TVec3f* pOutDirection, f32 a2, f32 a3) const {
    f32 f0;
    f32 f31;

    if (a2 < a3) {
        f31 = a3;
        f0 = 0.0f;
    } else {
        if (a2 > 1.0f - a3) {
            f0 = 1.0f;
            f31 = 1.0f - a3;
        } else {
            f0 = a2;
            f31 = a2 + a3;
        }
    }

    TVec3f stack_20;
    calcPosition(&stack_20, f0);
    TVec3f stack_14;
    calcPosition(&stack_14, f31);
    pOutDirection->set< f32 >(stack_14 - stack_20);
    MR::normalizeOrZero(pOutDirection);
}

f32 ParabolicPath::getLength(f32 startParam, f32 endParam, s32 numSegments) const {
    if (numSegments <= 0) {
        numSegments = 1;
    }
    f32 parameterLengthSegment = (endParam - startParam) / numSegments;
    f32 length = 0.0f;

    f32 f6 = (_2C * parameterLengthSegment) * (_2C * parameterLengthSegment);
    f32 f2 = startParam * (_24 * startParam + _28);

    for (int i = 0; i < numSegments; i++) {
        f32 endSegmentParam = startParam + parameterLengthSegment * (i + 1);
        f32 f4 = endSegmentParam * (_24 * endSegmentParam + _28);

        length += MR::fastSqrtf((f4 - f2) * (f4 - f2) + f6);
        f2 = f4;
    }
    return length;
};

f32 ParabolicPath::getTotalLength(s32 a1) const {
    return getLength(0.0f, 1.0f, a1);
}

f32 ParabolicPath::calcPathSpeedFromAverageSpeed(f32 speed) const {
    return speed / getLength(0.0f, 1.0f, 10);
}
