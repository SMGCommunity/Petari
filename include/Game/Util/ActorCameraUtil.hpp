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

    void startMultiActorCameraNoTarget(const LiveActor *, const ActorCameraInfo *, const char *, s32);

    bool initActorCamera(const LiveActor *, const JMapInfoIter &, ActorCameraInfo **);

    void startActorCameraNoTarget(const LiveActor *, const ActorCameraInfo *, s32);

    void initAnimCamera(const LiveActor *, const ActorCameraInfo *, const char *);

    bool isExistActorCamera(const ActorCameraInfo *);

    void endActorCamera(const LiveActor *, const ActorCameraInfo *, bool, s32);

    void initMultiActorCamera(const LiveActor *, const JMapInfoIter &, ActorCameraInfo **, const char *);

    u32 getActorCameraFrames(const LiveActor *, const ActorCameraInfo *);

    u32 getMultiActorCameraFrames(const LiveActor *, const ActorCameraInfo *, const char *);

    u32 startMultiActorCameraTargetOther(const LiveActor *, const ActorCameraInfo *, const char *, const CameraTargetArg &, s32);

    void endMultiActorCamera(const LiveActor *, const ActorCameraInfo *, const char *, bool, s32);

    void startRumbleWithShakeCameraWeak(const LiveActor *, const char *, const char *, f32, f32);
};