#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class CameraTargetArg;
class LiveActor;
class HitSensor;
class BckCtrlData;

namespace MR {
    TVec3f* getPlayerPos();
    TVec3f* getPlayerCenterPos();

    TVec3f* getPlayerRotate();

    TVec3f* getPlayerVelocity();

    TVec3f* getPlayerLastMove();

    TVec3f* getPlayerVelocity();

    MtxPtr getPlayerBaseMtx();

    bool isPlayerDead();

    bool isPlayerInBind();

    bool isPlayerBindedBigBubble();

    bool isPlayerHidden();

    bool isPlayerHipDropLand();

    bool isPlayerSwingAction();

    bool isNearPlayerAnyTime(const LiveActor *, f32);

    void forceKillPlayerByAbyss();

    void setCameraTargetToPlayer(CameraTargetArg *);

    bool isPlayerInAreaObj(const char *);

    bool isOnGroundPlayer();

    bool isPlayerTeresaDisappear();

    bool isPlayerInRush();

    bool isPlayerHipDropFalling();

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

    void scatterStarPiecePlayer(u32);

    void startSoundPlayer(const char *, s32);
    void stopSoundPlayer(const char *, u32);

    void showPlayer();

    void becomeContinuousBckPlayer();

    bool isPlayerElementModeBee();
    bool isPlayerElementModeIce();

    bool isPlayerInWaterMode();
    bool isPlayerOnWaterSurface();

    void changePlayerItemStatus(s32);

    void setPlayerStateWait();
    void setPlayerPos(const char *);
    void setPlayerPos(const TVec3f &);

    void setPlayerPosOnGroundAndWait(const char *);

    void endBindAndPlayerWait(LiveActor *);
    void endBindAndPlayerJump(LiveActor *, const TVec3f &, u32);

    void endBindAndPlayerDamageMsg(LiveActor *, u32);

    bool isPlayerPointedBy2POnTriggerButton();

    void endBindAndSpinDriverJump(LiveActor *, const TVec3f &);

    void endBindAndPlayerJumpWithRollLanding(LiveActor *, const TVec3f &, u32);

    void endBindAndPLayerForceWeakGravityJumpInputOff(LiveActor *, const TVec3f &);

    void setPlayerFrontTargetVec(const TVec3f &, s32);
    void setPlayerFrontVec(const TVec3f &, s32);

    void hidePlayer();

    bool isPlayerSwimming();

    void startBckPlayer(const char *, const char *);
    void startBckPlayer(const char *, const BckCtrlData &);
    void startBckPlayer(const char *, s32);
    bool isBckStoppedPlayer();

    f64 getBckFrameMaxPlayer();

    LiveActor* getCurrentRushActor();
};
