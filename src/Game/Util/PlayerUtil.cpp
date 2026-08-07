#include "Game/Util/PlayerUtil.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/PlayerEvent.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace MR {
    bool isOnGroundPlayer() {
        return MarioAccess::isOnGround(0);
    }

    Triangle* getPlayerGroundingPolygon() {
        return MarioAccess::getGroundingPolygon(0);
    }

    void forceKillPlayerByAbyss() {
        return MarioAccess::forceKill(0, 0);
    }

    void forceKillPlayerByWaterRace() {
        return MarioAccess::forceKill(1, 0);
    }

    void forceKillPlayerByGroundRace() {
        return MarioAccess::forceKill(5, 0);
    }

    void forceKillPlayerByGhostRace() {
        return MarioAccess::forceKill(2, 0);
    }

    bool isPlayerDead() {
        return !MarioAccess::getPlayerActor()->isEnableNerveChange();
    }

    bool isPlayerRefuseTalk() {
        return MarioAccess::getPlayerActor()->isRefuseTalk();
    }

    bool isPlayerTeresaDisappear() {
        return MarioAccess::isTeresaDisappear();
    }

    bool isPlayerInAreaObj(const char* pName) {
        return isInAreaObj(pName, MarioAccess::getPlayerActor()->mPosition);
    }

    TVec3f* getPlayerPos() {
        return &MarioAccess::getPlayerActor()->mPosition;
    }

    TVec3f* getPlayerCenterPos() {
        return &MarioAccess::getPlayerActor()->_2A0;
    }

    void getPlayerTakePos(TVec3f* pPos) {
        return MarioAccess::getTakePos(pPos);
    }

    void setPlayerPos(const TVec3f& rPos) {
        MarioAccess::setTrans(rPos, 0);
    }

    void setPlayerUpperRotateY(f32 angle) {
        MarioAccess::getPlayerActor()->setUpperRotateY(angle);
    }

    TVec3f* getPlayerRotate() {
        return &MarioAccess::getPlayerActor()->mRotation;
    }

    TVec3f* getPlayerShadowRotate() {
        return &MarioAccess::getPlayerActor()->_A18;
    }

    TVec3f* getPlayerVelocity() {
        return MarioAccess::getVelocity();
    }

    TVec3f* getPlayerLastMove() {
        return MarioAccess::getLastMove();
    }

    void setPlayerJumpVec(const TVec3f& rJumpVec) {
        MarioAccess::setJumpVec(rJumpVec);
    }

    f32 getPlayerHitRadius() {
        return MarioAccess::getPlayerActor()->mPlayerMode == (s16)6 ? 100.0f : 60.0f;
    }

    void setPlayerWalkingResist(f32 resist) {
        return MarioAccess::setWalkingResist(resist);
    }

    const TVec3f* getPlayerGravity() {
        return &MarioAccess::getPlayerActor()->getGravityVec();
    }

    void calcPlayerSpinPullVelocity(TVec3f* pVelocity, const TVec3f& rPos) {
        MarioAccess::calcSpinPullVelocity(pVelocity, rPos);
    }

    bool checkPlayerActionTrigger() {
        return MarioAccess::getPlayerActor()->_EED;
    }

    bool checkPlayerSwingTrigger() {
        return MarioAccess::getPlayerActor()->_1E1;
    }

    f32 calcDistanceToPlayer(const TVec3f& rPos) {
        return getPlayerPos()->distance(rPos);
    }

    void getPlayerUpVec(TVec3f* pVec) {
        MarioAccess::getPlayerActor()->getUpVec(pVec);
    }

    void getPlayerFrontVec(TVec3f* pVec) {
        MarioAccess::getPlayerActor()->getFrontVec(pVec);
    }

    void getPlayerSideVec(TVec3f* pVec) {
        MarioAccess::getPlayerActor()->getSideVec(pVec);
    }

    void getPlayerThrowVec(TVec3f* pVec) {
        MarioAccess::getThrowVec(pVec);
    }

    void getPlayerGroundPos(TVec3f* pPos) {
        MarioAccess::getPlayerActor()->getGroundPos(pPos);
    }

    const TVec3f* getPlayerGroundNormal() {
        return getPlayerGroundingPolygon()->getFaceNormal();
    }

    void setPlayerFrontTargetVec(const TVec3f& rTarget, s32 param2) {
        MarioAccess::setFrontVecTarget(rTarget, param2);
    }

    void setPlayerFrontVec(const TVec3f& rVec, s32 param2) {
        MarioAccess::setFrontVecKeepUp(rVec, param2);
    }

    void setPlayerSwingInhibitTimer(u16 timer) {
        MarioAccess::getPlayerActor()->_EF6 = timer;
    }

    void setPlayerSwingPermission(bool permission) {
        MarioAccess::getPlayerActor()->_EEB = permission;
    }

    void setPlayerStateWait() {
        MarioAccess::setStateWait();
    }

    void startBckPlayer(const char* pName, const char* pParam2) {
        if (pParam2 != nullptr) {
            MarioAccess::changeAnimationE(pName, pParam2);
        } else {
            MarioAccess::changeAnimationE(pName, pName);
        }
    }

    void startBckPlayer(const char* pName, const BckCtrlData& rCtrlData) {
        MarioAccess::changeAnimationE(pName, rCtrlData);
    }

    void startBckPlayer(const char* pName, s32 param2) {
        MarioAccess::changeAnimationE(pName, param2);
    }

    bool isBckStoppedPlayer() {
        return MR::isBckStopped(MarioAccess::getPlayerActor());
    }

    bool isBckOneTimeAndStoppedPlayer() {
        return MR::isBckOneTimeAndStopped(MarioAccess::getPlayerActor());
    }

    f32 getBckFrameMaxPlayer() {
        return MarioAccess::getAnimationFrameMax();
    }

    s16 getBckFrameMaxPlayer(const char* pName) {
        return MR::getBckFrameMax(MarioAccess::getPlayerActor(), pName);
    }

    void startBckPlayerJ(const char* pName) {
        MarioAccess::changeAnimationJ(pName);
    }

    void becomeContinuousBckPlayer() {
        MarioAccess::keepCurrentAnimation();
    }

    void progressPlayerBckOnPause() {
        MarioAccess::progressAnimation();
    }

    const char* getPlayerCurrentBckName() {
        return MarioAccess::getCurrentBckName();
    }

    void setBckBlendWeight(f32 param1, f32 param2) {
        const f32 blendWeight[] = {param1, param2};

        MarioAccess::setAnimationBlendWeight(blendWeight);
    }

    void setBckBlendWeight(f32 param1, f32 param2, f32 param3) {
        const f32 blendWeight[] = {param1, param2, param3};

        MarioAccess::setAnimationBlendWeight(blendWeight);
    }

    void setBckBlendWeight(f32 param1, f32 param2, f32 param3, f32 param4) {
        const f32 blendWeight[] = {param1, param2, param3, param4};

        MarioAccess::setAnimationBlendWeight(blendWeight);
    }

    void setBckRatePlayer(f32 rate) {
        MR::setBckRate(MarioAccess::getPlayerActor(), rate);
    }

    XanimeResourceTable* getPlayerXanimeResource() {
        return MarioAccess::getPlayerActor()->getResourceTable();
    }

    void jumpPlayer(const TVec3f& rParam1) {
        MarioAccess::freeJump(rParam1, 0);
    }

    void forceJumpPlayer(const TVec3f& rParam1) {
        MarioAccess::forceJump(rParam1, 0);
    }

    void forceFlyPlayer(const TVec3f& rParam1, const TVec3f& rParam2, s32 param3) {
        MarioAccess::forceFly(rParam1, rParam2, param3);
    }

    void tornadoJumpPlayer() {
        MarioAccess::tornadoJump();
    }

    void miniTornadoJumpPlayer() {
        MarioAccess::tornadoJumpMini();
    }

    void becomePlayerNormalJumpStatus() {
        MarioAccess::becomeNormalJumpStatus();
    }

    bool isOnPlayer(const HitSensor* pSensor) {
        Triangle* polygon = getPlayerGroundingPolygon();

        if (polygon != nullptr) {
            return polygon->mSensor == pSensor;
        }

        return false;
    }

    bool isActorOnPlayer(const LiveActor* pActor) {
        return MarioAccess::isOnActor(pActor);
    }

    bool isOnPlayerShadow(const LiveActor* pActor) {
        Triangle* polygon = MarioAccess::getShadowingPolygon();

        if (polygon != nullptr) {
            return polygon->mSensor->mHost == pActor;
        }

        return false;
    }

    f32 getPlayerShadowHeight() {
        return MarioAccess::getShadowHeight();
    }

    void setPlayerPos(const char* pParam1) {
        TPos3f mtx;
        mtx.identity();

        MR::findNamePos(pParam1, mtx.toMtxPtr());
        MarioAccess::setBaseMtx(mtx.toMtxPtr());

        TVec3f trans;
        MR::extractMtxTrans(mtx.toMtxPtr(), &trans);

        setPlayerPos(trans);
    }

    void setPlayerPosAndWait(const TVec3f& rPos) {
        setPlayerPos(rPos);
        MarioAccess::getPlayerActor()->resetCondition();
        MarioAccess::setStateWait();
    }

    void setPlayerPosAndWait(const char* pParam1) {
        setPlayerPos(pParam1);
        MarioAccess::getPlayerActor()->resetCondition();
        MarioAccess::setStateWait();
    }

    void setPlayerLinkPosAndWait(const NameObj* pParam1, const char* pParam2) {
        TPos3f mtx;
        mtx.identity();

        MR::findLinkNamePos(pParam1, pParam2, mtx.toMtxPtr());
        MarioAccess::setBaseMtx(mtx.toMtxPtr());
        MarioAccess::getPlayerActor()->resetCondition();
        MarioAccess::setStateWait();
    }

    void setPlayerPosOnGround(const char* pParam1) {
        TPos3f mtx;
        mtx.identity();

        MR::findNamePosOnGround(pParam1, mtx.toMtxPtr());
        MarioAccess::setBaseMtx(mtx.toMtxPtr());
    }

    void setPlayerPosOnGroundAndWait(const char* pParam1) {
        TPos3f mtx;
        mtx.identity();

        MR::findNamePosOnGround(pParam1, mtx.toMtxPtr());
        MarioAccess::setBaseMtx(mtx.toMtxPtr());
        MarioAccess::getPlayerActor()->resetCondition();
        MarioAccess::setStateWait();
    }

    bool isPlayerHipDropFalling() {
        return MarioAccess::isHipDropFalling();
    }

    bool isPlayerHipDropLand() {
        return MarioAccess::isHipDropLand();
    }

    void incPlayerLife(u32 add) {
        MarioAccess::incLife(add);
    }

    void incPlayerOxygen(u32 add) {
        MarioAccess::incOxygen(add);
    }

    bool isPlayerConfrontDeath() {
        return MarioAccess::isConfrontDeath();
    }

    void getStarPiecePlayer() {
        MarioAccess::addStarPiece();
    }

    void notifyDirectGetStarPiecePlayer() {
        MarioAccess::getStarPieceDirect();
    }

    void scatterStarPiecePlayer(u32 num) {
        MarioAccess::scatterStarPiece(num);
    }

    bool isPlayerSwingAction() {
        return MarioAccess::isSwingAction();
    }

    bool isPlayerPointedBy2POnTriggerButton() {
        return MR::isStarPointerPointing2POnTriggerButton(MarioAccess::getPlayerActor(), "弱", true, false);
    }

    bool isPlayerSquat() {
        return MarioAccess::isSquat();
    }

    bool isPlayerInRush() {
        return MarioAccess::isInRush();
    }

    bool isPlayerNeedBrakingCamera() {
        return MarioAccess::isNeedBrakingCamera();
    }

    bool isPlayerFlying() {
        return MarioAccess::isFlying();
    }

    bool isPlayerElementMode(s32 mode) {
        return MarioAccess::getPlayerActor()->mPlayerMode == mode;
    }

    bool isPlayerElementModeTornado() {
        return isPlayerElementMode(9);
    }

    bool isPlayerElementModeInvincible() {
        return isPlayerElementMode(1);
    }

    bool isPlayerElementModeBee() {
        return isPlayerElementMode(4);
    }

    bool isPlayerElementModeHopper() {
        return isPlayerElementMode(5);
    }

    bool isPlayerElementModeTeresa() {
        return isPlayerElementMode(6);
    }

    bool isPlayerElementModeIce() {
        return isPlayerElementMode(3);
    }

    bool isPlayerElementModeNormal() {
        return isPlayerElementMode(0);
    }

    bool isPlayerSkating() {
        return MarioAccess::isSkating();
    }

    void changePlayerItemStatus(s32 param1) {
        MarioAccess::changeItemStatus(param1);
    }

    void curePlayerElementMode() {
        changePlayerItemStatus(8);
    }

    bool isPlayerParalyzing() {
        return MarioAccess::isParalyzing();
    }

    bool isPlayerDamaging() {
        return MarioAccess::getPlayerActor()->isDamaging();
    }

    bool isPlayerStaggering() {
        return MarioAccess::getPlayerActor()->isStaggering();
    }

    bool isPlayerSwimming() {
        return MarioAccess::isSwimming();
    }

    bool isPlayerSleeping() {
        return MarioAccess::getPlayerActor()->isSleeping();
    }

    bool isPlayerJumpRising() {
        return MarioAccess::getPlayerActor()->isJumpRising();
    }

    void validatePlayerSensor() {
        return MarioAccess::validateSensor();
    }

    bool isPlayerInBind() {
        return MarioAccess::isInRush();
    }

    void endBindAndPlayerWait(LiveActor* pActor) {
        RushEndInfo info = RushEndInfo(pActor, 0, TVec3f(0.0f, 0.0f, 0.0f), false, 0);

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerJump(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
        RushEndInfo info = RushEndInfo(pActor, 2, rParam2, true, param3);

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerForceJump(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
        RushEndInfo info = RushEndInfo(pActor, 2, rParam2, true, param3);
        info._20 |= 0x40000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerWeakGravityJump(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerForceWeakGravityJump(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 |= 0x40000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerForceWeakGravityJumpInputOff(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerWeakGravityLimitJump(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 |= 0x800000;

        MarioAccess::endRush(&info);
    }

    void endBindAndSpinDriverJump(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerDamage(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerFlip(LiveActor* pActor, const TVec3f& rParam2) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerJumpWithRollLanding(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
        RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, param3);
        info._20 |= 0x400000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerDamageMsg(LiveActor* pActor, u32 param2) {
        endBindAndPlayerDamageMsg(pActor, param2, TVec3f(0.0f, 0.0f, 0.0f));
    }

    void endBindAndPlayerDamageMsg(LiveActor* pActor, u32 param2, const TVec3f& rParam3) {
        switch (param2) {
        case 0x57:
        case 0x58:
        case 0x59:
            endBindAndPlayerFireDamage(pActor);
            break;
        case 0x5C:
            endBindAndPlayerAcidDamage(pActor);
            break;
        case 0x5D:
            endBindAndPlayerFreezeDamage(pActor);
            break;
        case 0x5A:
        case 0x5B:
            endBindAndPlayerElectricDamage(pActor);
            break;
        case 0x56:
            endBindAndPlayerDamage(pActor, rParam3);
            break;
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x51:
        case 0x52:
            endBindAndPlayerWeakGravityLimitJump(pActor, rParam3);
            break;
        case 0x50:
            endBindAndPlayerFlip(pActor, rParam3);
            break;
        default:
            endBindAndPlayerDamage(pActor, rParam3);
            break;
        }
    }

    void endBindAndPlayerAcidDamage(LiveActor* pActor) {
        RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerFreezeDamage(LiveActor* pActor) {
        RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerFireDamage(LiveActor* pActor) {
        RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    void endBindAndPlayerElectricDamage(LiveActor* pActor) {
        RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
        info._20 &= ~0xF000000;
        info._20 |= 0xC0000000;

        MarioAccess::endRush(&info);
    }

    LiveActor* getCurrentRushActor() {
        if (isPlayerInRush() == false) {
            return nullptr;
        }

        return MarioAccess::getPlayerActor()->_924->mHost;
    }

    HitSensor* getCurrentRushSensor() {
        if (isPlayerInRush() == false) {
            return nullptr;
        }

        return MarioAccess::getPlayerActor()->_924;
    }

    void tryPlayerCoinPull() {
        MarioAccess::tryCoinPull();
    }

    void tryPlayerPullActor(HitSensor* pSensor) {
        MarioAccess::getPlayerActor()->tryTornadoPull(pSensor);
    }

    void tryPlayerDropTakingActor() {
        MarioAccess::dropTakingActor();
    }

    void tryPlayerKillTakingActor() {
        MarioAccess::killTakingActor();
    }

    bool isPlayerTakingActor(const char* pName) {
        if (MarioAccess::getTakingSensor() != nullptr) {
            if (strcmp(MarioAccess::getTakingSensor()->mHost->mName, pName) == 0) {
                return true;
            }
        }

        return false;
    }

    bool isPlayerCarryAny() {
        return MarioAccess::getPlayerActor()->_468 != 0;
    }

    void startSoundPlayer(const char* pName, s32 param1) {
        MR::startSound(MarioAccess::getPlayerActor(), pName, param1);
    }

    void startLevelSoundPlayer(const char* pName, s32 param1) {
        MR::startLevelSound(MarioAccess::getPlayerActor(), pName, param1);
    }

    void stopSoundPlayer(const char* pName, u32 param1) {
        MR::stopSound(MarioAccess::getPlayerActor(), pName, param1);
    }

    void startSoundPlayerJ(const char* pName) {
        MarioAccess::getPlayerActor()->playSound(pName, -1);
    }

    void showPlayer() {
        MarioAccess::show();
    }

    void hidePlayer() {
        MarioAccess::hide();
    }

    void showPlayerJoint(const char* pName) {
        MR::showJoint(MarioAccess::getPlayerActor(), pName);
    }

    void hidePlayerJoint(const char* pName) {
        MR::hideJoint(MarioAccess::getPlayerActor(), pName);
    }

    void setPlayerSpot(f32 param1, u32 param2) {
        MarioAccess::setSpot(param1, param2);
    }

    void startPlayerDownWipe() {
        MarioAccess::startDownWipe();
    }

    void setCameraTargetToPlayer(CameraTargetArg* pTarget) {
        pTarget->mMarioActor = MarioAccess::getPlayerActor();
        pTarget->mTargetObj = nullptr;
        pTarget->mTargetMtx = nullptr;
        pTarget->mLiveActor = nullptr;
    }

    bool isPlayerDisableFpView() {
        return MarioAccess::isDisableFpView();
    }

    bool isFpViewChangingFailure() {
        return MarioAccess::isFpViewChangingFailure();
    }

    void stopPlayerFpView() {
        return MarioAccess::stopFpView();
    }

    void setRasterScroll(s32 param1, s32 param2, s32 param3) {
        MarioAccess::getPlayerActor()->setRasterScroll(param1, param2, param3);
    }

    void noticePlayerDashChance() {
        MarioAccess::noticeDashChance();
    }

    void startPlayerTalk(const LiveActor* pActor) {
        MarioAccess::startTalk(pActor);
    }

    void endPlayerTalk() {
        MarioAccess::endTalk();
    }

    void preventPlayerRush() {
        MarioAccess::preventRush();
    }

    bool isExistMario() {
        return MR::isExistSceneObj(SceneObj_MarioHolder);
    }

    void startPlayerEvent(const char* pName) {
        EventSequencer* eventSequencer;

        eventSequencer = MR::getSceneObj< EventSequencer >(SceneObj_EventSequencer);
        eventSequencer->startEvent(pName);

        requestMovementOn(eventSequencer);
        requestMovementOnPlayer();
    }

    void offPlayerControl() {
        MarioAccess::offControl();
    }

    void onPlayerControl(bool param1) {
        MarioAccess::onControl(param1);
    }

    bool isOffPlayerControl() {
        return MarioAccess::isOffControl();
    }

    void unlockPlayerAnimation() {
        MarioAccess::getPlayerActor()->setB90(false);
    }

    void resetPlayerStatus() {
        MarioAccess::getPlayerActor()->resetCondition();
    }

    void resetPlayerEffect() {
        MR::forceDeleteEffectAll(MarioAccess::getPlayerActor());
        resetChasingStarPiece();
    }

    void setPlayerBaseMtx(MtxPtr pMtx) {
        MarioAccess::setBaseMtx(pMtx);
    }

    MtxPtr getPlayerBaseMtx() {
        return MarioAccess::getBaseMtx();
    }

    LiveActor* getPlayerDemoActor() {
        return MarioAccess::getPlayerActor();
    }

    void initPlayerAfterOpeningDemo() {
        MarioAccess::getPlayerActor()->initAfterOpeningDemo();
    }

    void readyPlayerDemo() {
        MarioAccess::readyDemo();
    }

    void onFollowDemoEffect() {
        MarioAccess::onFollowDemo();
    }

    void requestMovementOnPlayer() {
        requestMovementOn(MarioAccess::getPlayerActor());
    }

    void calcPlayerJointMtx(TPos3f* pMtx, const char* pName) {
        pMtx->set(MarioAccess::getJointMtx(pName));
    }

    bool isPlayerOnPress() {
        return MarioAccess::isOnPress();
    }

    void pushPlayer(const TVec3f& rAccel) {
        MarioAccess::addVelocity(rAccel);
    }

    void pushPlayerFromArea(const TVec3f& rAccel) {
        MarioAccess::addVelocityFromArea(rAccel);
    }

    bool isPlayerInWaterMode() {
        return MarioAccess::isInWaterMode();
    }

    bool isPlayerOnWaterSurface() {
        return MarioAccess::isOnWaterSurface();
    }

    bool isPlayerHidden() {
        return isPlayerElementMode(6);
    }

    void calcPlayerWorldPadDir(TVec3f* pDir, f32 param2, f32 param3) {
        MarioAccess::calcWorldPadDir(pDir, param2, param3);
    }

    JUTTexture* getFullScreenBlurTexture() {
        return MarioAccess::getPlayerActor()->_B7C;
    }

    u16 getPlayerMovementTimer() {
        return MarioAccess::getPlayerActor()->_378;
    }

    CubeCameraArea* getCameraCube() {
        return MarioAccess::getCameraCubeCode();
    }
};  // namespace MR
