#pragma once

#include "Game/Util.hpp"
#include "Game/Util/JMapInfo.hpp"

class ActorCameraInfo {
public:
    ActorCameraInfo(const JMapInfoIter &);
    ActorCameraInfo(s32, s32);

    s32 mCameraSetID;   // 0x0
    s32 mZoneID;        // 0x4
};
