#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class PlantPoint;
class RailRider;

class PlantRailInfo {
public:
    PlantRailInfo(const JMapInfoIter&, f32);

    void calcPosAndAxisY(TVec3f*, TVec3f*, f32) const;
    void calcNearestPointPos(TVec3f*, const TVec3f&, s32) const;

    inline PlantPoint* getPlantPoint(s32 idx) { return mPlantPoints[idx]; }

    /* 0x00 */ s32 mNumPlantPoints;
    /* 0x04 */ PlantPoint** mPlantPoints;
    /* 0x08 */ RailRider* mRailRider;
    /* 0x0C */ f32 mRailSpeed;
};
