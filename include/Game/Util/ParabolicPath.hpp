#pragma once

#include "JSystem/JGeometry.hpp"

class ParabolicPath {
public:
    ParabolicPath();

    void initFromMaxHeight(const TVec3f&, const TVec3f&, const TVec3f&);
    void initFromUpVector(const TVec3f&, const TVec3f&, const TVec3f&, f32);
    void initFromUpVectorAddHeight(const TVec3f&, const TVec3f&, const TVec3f&, f32);
    void calcPosition(TVec3f*, f32) const;
    void calcDirection(TVec3f*, f32, f32) const;
    f32 getLength(f32, f32, s32) const;
    f32 getTotalLength(s32) const;
    f32 calcPathSpeedFromAverageSpeed(f32) const;

    TVec3f mPosition;  // 0x0
    TVec3f mAxisY;     // 0xC
    TVec3f mAxisZ;     // 0x18
    f32 _24;
    f32 _28;
    f32 _2C;
};