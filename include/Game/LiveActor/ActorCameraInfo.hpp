#pragma once

#include <revolution/types.h>

class JMapInfoIter;

class ActorCameraInfo {
public:
    ActorCameraInfo(const JMapInfoIter&);
    ActorCameraInfo(s32 cameraSetID = -1, s32 zoneID = 0);

    /* 0x0 */ s32 mCameraSetID;
    /* 0x4 */ s32 mZoneID;
};
