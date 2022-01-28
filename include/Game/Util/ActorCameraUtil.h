#pragma once

#include "Game/Util/JMapInfo.h"
#include <revolution.h>

class ActorCameraInfo;
class LiveActor;

namespace MR {
    ActorCameraInfo* createActorCameraInfo(const JMapInfoIter &);
    void startActorCameraTargetSelf(const LiveActor *, const ActorCameraInfo *, s32);

    void endActorCamera(const LiveActor *, const ActorCameraInfo *, bool, s32);
};