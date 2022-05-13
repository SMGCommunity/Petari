#pragma once

#include "Game/Camera/CameraTargetArg.h"
#include "Game/Util/JMapInfo.h"
#include <revolution.h>

class ActorCameraInfo;
class LiveActor;

namespace MR {
    ActorCameraInfo* createActorCameraInfo(const JMapInfoIter &);
    void startActorCameraTargetSelf(const LiveActor *, const ActorCameraInfo *, s32);
    void startAnimCameraTargetSelf(const LiveActor *, const ActorCameraInfo *, const char *, s32, f32);
    void startActorCameraTargetOther(const LiveActor *, const ActorCameraInfo *, const CameraTargetArg &, s32);

    void initAnimCamera(const LiveActor *, const ActorCameraInfo *, const char *);

    void endActorCamera(const LiveActor *, const ActorCameraInfo *, bool, s32);
};