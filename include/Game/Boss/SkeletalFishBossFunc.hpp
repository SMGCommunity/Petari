#pragma once

#include <revolution.h>
#include <JSystem/JGeometry/TVec.hpp>

class SkeletalFishBossFunc {
public:
    static s32 getNearestGuardPosID(const TVec3f &);
    static bool copyGuardPos(TVec3f *, s32);
};