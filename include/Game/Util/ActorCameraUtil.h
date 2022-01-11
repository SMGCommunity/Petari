#pragma once

#include <revolution.h>

class ActorCameraInfo;
class LiveActor;

namespace MR {
    void startActorCameraTargetSelf(const LiveActor *, const ActorCameraInfo *, s32);
};