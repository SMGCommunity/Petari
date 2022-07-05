#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class CameraTargetArg;
class LiveActor;

namespace MR {
    TVec3f* getPlayerPos();
    TVec3f* getPlayerCenterPos();

    TVec3f* getPlayerRotate();

    bool isPlayerDead();

    bool isPlayerInBind();

    bool isPlayerBindedBigBubble();

    bool isPlayerHidden();

    bool isNearPlayerAnyTime(const LiveActor *, f32);

    void forceKillPlayerByAbyss();

    void setCameraTargetToPlayer(CameraTargetArg *);

    bool isPlayerInAreaObj(const char *);

    bool isOnGroundPlayer();

    bool isPlayerTeresaDisappear();

    f32 getPlayerHitRadius();

    void pushPlayerFromArea(const TVec3f &);
    bool isOnPlayer(const LiveActor *);
    bool isOnPlayer(const HitSensor *);

    bool isPlayerJumpRising();
    TVec3f* getPlayerGroundingPolygon();
    TVec3f* getPlayerGroundNormal();

    f32 calcDistanceToPlayer(const TVec3f &);

    void startBckPlayer(const char *, const char *);
    LiveActor* getPlayerDemoActor();

    void setPlayerBaseMtx(MtxPtr);

    void startLevelSoundPlayer(const char *, long);
    void incPlayerOxygen(u32);

    void stopSoundPlayer(const char *, u32);

    void showPlayer();

    bool isPlayerElementModeBee();

    bool isPlayerInWaterMode();
    bool isPlayerOnWaterSurface();

    void changePlayerItemStatus(s32);

    void setPlayerStateWait();
    void setPlayerPos(const char *);
};
