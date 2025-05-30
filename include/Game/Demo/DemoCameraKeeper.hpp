#pragma once

#include <revolution.h>

class ActorCameraInfo;
class LiveActor;

class DemoCameraInfo {
public:
    DemoCameraInfo();

    u32 _0;
    const char* _4;
    s32 _8;
    const char* _C;
    s32 _10;
    s32 _14;
    const char* _1C;
    ActorCameraInfo* _20;
    LiveActor* _24;
};
