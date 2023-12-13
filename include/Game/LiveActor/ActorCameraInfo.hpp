#pragma once

#include "Game/Util.hpp"

class ActorCameraInfo {
public:
    ActorCameraInfo(const JMapInfoIter &);
    ActorCameraInfo(s32, s32);

    s32 mCameraSetID;   // _0
    s32 mZoneID;        // _4
};