#pragma once

#include "Game/Util/JMapInfo.hpp"

class ActorCameraInfo {
public:
    ActorCameraInfo(const JMapInfoIter&);
    ActorCameraInfo(s32, s32);

    /* 0x0 */ s32 mCameraSetID;
    /* 0x4 */ s32 mZoneID;
};
