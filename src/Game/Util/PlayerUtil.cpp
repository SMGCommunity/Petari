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
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

bool MR::isOnGroundPlayer() {
    return MarioAccess::isOnGround(0);
}

Triangle* MR::getPlayerGroundingPolygon() {
    return MarioAccess::getGroundingPolygon(0);
}

void MR::forceKillPlayerByAbyss() {
    return MarioAccess::forceKill(0, 0);
}

void MR::forceKillPlayerByWaterRace() {
    return MarioAccess::forceKill(1, 0);
}

void MR::forceKillPlayerByGroundRace() {
    return MarioAccess::forceKill(5, 0);
}

void MR::forceKillPlayerByGhostRace() {
    return MarioAccess::forceKill(2, 0);
}

bool MR::isPlayerDead() {
    return !MarioAccess::getPlayerActor()->isEnableNerveChange();
}

bool MR::isPlayerRefuseTalk() {
    return MarioAccess::getPlayerActor()->isRefuseTalk();
}

bool MR::isPlayerTeresaDisappear() {
    return MarioAccess::isTeresaDisappear();
}

bool MR::isPlayerInAreaObj(const char* pName) {
    return isInAreaObj(pName, MarioAccess::getPlayerActor()->mPosition);
}

TVec3f* MR::getPlayerPos() {
    return &MarioAccess::getPlayerActor()->mPosition;
}

TVec3f* MR::getPlayerCenterPos() {
    return &MarioAccess::getPlayerActor()->_2A0;
}

void MR::getPlayerTakePos(TVec3f* pPos) {
    return MarioAccess::getTakePos(pPos);
}

void MR::setPlayerPos(const TVec3f& rPos) {
    MarioAccess::setTrans(rPos, 0);
}

void MR::setPlayerUpperRotateY(f32 angle) {
    MarioAccess::getPlayerActor()->setUpperRotateY(angle);
}

TVec3f* MR::getPlayerRotate() {
    return &MarioAccess::getPlayerActor()->mRotation;
}

TVec3f* MR::getPlayerShadowRotate() {
    return &MarioAccess::getPlayerActor()->_A18;
}

TVec3f* MR::getPlayerVelocity() {
    return MarioAccess::getVelocity();
}

TVec3f* MR::getPlayerLastMove() {
    return MarioAccess::getLastMove();
}

void MR::setPlayerJumpVec(const TVec3f& rJumpVec) {
    MarioAccess::setJumpVec(rJumpVec);
}

f32 MR::getPlayerHitRadius() {
    return MarioAccess::getPlayerActor()->mPlayerMode == (s16)6 ? 100.0f : 60.0f;
}

void MR::setPlayerWalkingResist(f32 resist) {
    return MarioAccess::setWalkingResist(resist);
}

const TVec3f* MR::getPlayerGravity() {
    return &MarioAccess::getPlayerActor()->getGravityVec();
}

void MR::calcPlayerSpinPullVelocity(TVec3f* pVelocity, const TVec3f& rPos) {
    MarioAccess::calcSpinPullVelocity(pVelocity, rPos);
}

bool MR::checkPlayerActionTrigger() {
    return MarioAccess::getPlayerActor()->_EED;
}

bool MR::checkPlayerSwingTrigger() {
    return MarioAccess::getPlayerActor()->_1E1;
}

f32 MR::calcDistanceToPlayer(const TVec3f& rPos) {
    return getPlayerPos()->distance(rPos);
}

void MR::getPlayerUpVec(TVec3f* pVec) {
    MarioAccess::getPlayerActor()->getUpVec(pVec);
}

void MR::getPlayerFrontVec(TVec3f* pVec) {
    MarioAccess::getPlayerActor()->getFrontVec(pVec);
}

void MR::getPlayerSideVec(TVec3f* pVec) {
    MarioAccess::getPlayerActor()->getSideVec(pVec);
}

void MR::getPlayerThrowVec(TVec3f* pVec) {
    MarioAccess::getThrowVec(pVec);
}

void MR::getPlayerGroundPos(TVec3f* pPos) {
    MarioAccess::getPlayerActor()->getGroundPos(pPos);
}

const TVec3f* MR::getPlayerGroundNormal() {
    return getPlayerGroundingPolygon()->getFaceNormal();
}

void MR::setPlayerFrontTargetVec(const TVec3f& rTarget, s32 param2) {
    MarioAccess::setFrontVecTarget(rTarget, param2);
}

void MR::setPlayerFrontVec(const TVec3f& rVec, s32 param2) {
    MarioAccess::setFrontVecKeepUp(rVec, param2);
}

void MR::setPlayerSwingInhibitTimer(u16 timer) {
    MarioAccess::getPlayerActor()->_EF6 = timer;
}

void MR::setPlayerSwingPermission(bool permission) {
    MarioAccess::getPlayerActor()->_EEB = permission;
}

void MR::setPlayerStateWait() {
    MarioAccess::setStateWait();
}

void MR::startBckPlayer(const char* pName, const char* pParam2) {
    if (pParam2 != nullptr) {
        MarioAccess::changeAnimationE(pName, pParam2);
    } else {
        MarioAccess::changeAnimationE(pName, pName);
    }
}

void MR::startBckPlayer(const char* pName, const BckCtrlData& rCtrlData) {
    MarioAccess::changeAnimationE(pName, rCtrlData);
}

void MR::startBckPlayer(const char* pName, s32 param2) {
    MarioAccess::changeAnimationE(pName, param2);
}

bool MR::isBckStoppedPlayer() {
    return MR::isBckStopped(MarioAccess::getPlayerActor());
}

bool MR::isBckOneTimeAndStoppedPlayer() {
    return MR::isBckOneTimeAndStopped(MarioAccess::getPlayerActor());
}

f32 MR::getBckFrameMaxPlayer() {
    return MarioAccess::getAnimationFrameMax();
}

s16 MR::getBckFrameMaxPlayer(const char* pName) {
    return MR::getBckFrameMax(MarioAccess::getPlayerActor(), pName);
}

void MR::startBckPlayerJ(const char* pName) {
    MarioAccess::changeAnimationJ(pName);
}

void MR::becomeContinuousBckPlayer() {
    MarioAccess::keepCurrentAnimation();
}

void MR::progressPlayerBckOnPause() {
    MarioAccess::progressAnimation();
}

const char* MR::getPlayerCurrentBckName() {
    return MarioAccess::getCurrentBckName();
}

void MR::setBckBlendWeight(f32 param1, f32 param2) {
    const f32 blendWeight[] = {param1, param2};

    MarioAccess::setAnimationBlendWeight(blendWeight);
}

void MR::setBckBlendWeight(f32 param1, f32 param2, f32 param3) {
    const f32 blendWeight[] = {param1, param2, param3};

    MarioAccess::setAnimationBlendWeight(blendWeight);
}

void MR::setBckBlendWeight(f32 param1, f32 param2, f32 param3, f32 param4) {
    const f32 blendWeight[] = {param1, param2, param3, param4};

    MarioAccess::setAnimationBlendWeight(blendWeight);
}

void MR::setBckRatePlayer(f32 rate) {
    MR::setBckRate(MarioAccess::getPlayerActor(), rate);
}

XanimeResourceTable* MR::getPlayerXanimeResource() {
    return MarioAccess::getPlayerActor()->getResourceTable();
}

void MR::jumpPlayer(const TVec3f& rParam1) {
    MarioAccess::freeJump(rParam1, 0);
}

void MR::forceJumpPlayer(const TVec3f& rParam1) {
    MarioAccess::forceJump(rParam1, 0);
}

void MR::forceFlyPlayer(const TVec3f& rParam1, const TVec3f& rParam2, s32 param3) {
    MarioAccess::forceFly(rParam1, rParam2, param3);
}

void MR::tornadoJumpPlayer() {
    MarioAccess::tornadoJump();
}

void MR::miniTornadoJumpPlayer() {
    MarioAccess::tornadoJumpMini();
}

void MR::becomePlayerNormalJumpStatus() {
    MarioAccess::becomeNormalJumpStatus();
}

bool MR::isOnPlayer(const HitSensor* pSensor) {
    Triangle* polygon = getPlayerGroundingPolygon();

    if (polygon != nullptr) {
        return polygon->mSensor == pSensor;
    }

    return false;
}

bool MR::isActorOnPlayer(const LiveActor* pActor) {
    return MarioAccess::isOnActor(pActor);
}

bool MR::isOnPlayerShadow(const LiveActor* pActor) {
    Triangle* polygon = MarioAccess::getShadowingPolygon();

    if (polygon != nullptr) {
        return polygon->mSensor->mHost == pActor;
    }

    return false;
}

f32 MR::getPlayerShadowHeight() {
    return MarioAccess::getShadowHeight();
}

void MR::setPlayerPos(const char* pParam1) {
    TPos3f mtx;
    mtx.identity();

    MR::findNamePos(pParam1, mtx.toMtxPtr());
    MarioAccess::setBaseMtx(mtx.toMtxPtr());

    TVec3f trans;
    MR::extractMtxTrans(mtx.toMtxPtr(), &trans);

    setPlayerPos(trans);
}

void MR::setPlayerPosAndWait(const TVec3f& rPos) {
    setPlayerPos(rPos);
    MarioAccess::getPlayerActor()->resetCondition();
    MarioAccess::setStateWait();
}

void MR::setPlayerPosAndWait(const char* pParam1) {
    setPlayerPos(pParam1);
    MarioAccess::getPlayerActor()->resetCondition();
    MarioAccess::setStateWait();
}

void MR::setPlayerLinkPosAndWait(const NameObj* pParam1, const char* pParam2) {
    TPos3f mtx;
    mtx.identity();

    MR::findLinkNamePos(pParam1, pParam2, mtx.toMtxPtr());
    MarioAccess::setBaseMtx(mtx.toMtxPtr());
    MarioAccess::getPlayerActor()->resetCondition();
    MarioAccess::setStateWait();
}

void MR::setPlayerPosOnGround(const char* pParam1) {
    TPos3f mtx;
    mtx.identity();

    MR::findNamePosOnGround(pParam1, mtx.toMtxPtr());
    MarioAccess::setBaseMtx(mtx.toMtxPtr());
}

void MR::setPlayerPosOnGroundAndWait(const char* pParam1) {
    TPos3f mtx;
    mtx.identity();

    MR::findNamePosOnGround(pParam1, mtx.toMtxPtr());
    MarioAccess::setBaseMtx(mtx.toMtxPtr());
    MarioAccess::getPlayerActor()->resetCondition();
    MarioAccess::setStateWait();
}

bool MR::isPlayerHipDropFalling() {
    return MarioAccess::isHipDropFalling();
}

bool MR::isPlayerHipDropLand() {
    return MarioAccess::isHipDropLand();
}

void MR::incPlayerLife(u32 add) {
    MarioAccess::incLife(add);
}

void MR::incPlayerOxygen(u32 add) {
    MarioAccess::incOxygen(add);
}

bool MR::isPlayerConfrontDeath() {
    return MarioAccess::isConfrontDeath();
}

void MR::getStarPiecePlayer() {
    MarioAccess::addStarPiece();
}

void MR::notifyDirectGetStarPiecePlayer() {
    MarioAccess::getStarPieceDirect();
}

void MR::scatterStarPiecePlayer(u32 num) {
    MarioAccess::scatterStarPiece(num);
}

bool MR::isPlayerSwingAction() {
    return MarioAccess::isSwingAction();
}

bool MR::isPlayerPointedBy2POnTriggerButton() {
    return MR::isStarPointerPointing2POnTriggerButton(MarioAccess::getPlayerActor(), "弱", true, false);
}

bool MR::isPlayerSquat() {
    return MarioAccess::isSquat();
}

bool MR::isPlayerInRush() {
    return MarioAccess::isInRush();
}

bool MR::isPlayerNeedBrakingCamera() {
    return MarioAccess::isNeedBrakingCamera();
}

bool MR::isPlayerFlying() {
    return MarioAccess::isFlying();
}

bool MR::isPlayerElementMode(s32 mode) {
    return MarioAccess::getPlayerActor()->mPlayerMode == mode;
}

bool MR::isPlayerElementModeTornado() {
    return isPlayerElementMode(PlayerMode_Tornado);
}

bool MR::isPlayerElementModeInvincible() {
    return isPlayerElementMode(PlayerMode_Invincible);
}

bool MR::isPlayerElementModeBee() {
    return isPlayerElementMode(PlayerMode_Bee);
}

bool MR::isPlayerElementModeHopper() {
    return isPlayerElementMode(PlayerMode_Hopper);
}

bool MR::isPlayerElementModeTeresa() {
    return isPlayerElementMode(PlayerMode_Teresa);
}

bool MR::isPlayerElementModeIce() {
    return isPlayerElementMode(PlayerMode_Ice);
}

bool MR::isPlayerElementModeNormal() {
    return isPlayerElementMode(PlayerMode_Normal);
}

bool MR::isPlayerSkating() {
    return MarioAccess::isSkating();
}

void MR::changePlayerItemStatus(s32 param1) {
    MarioAccess::changeItemStatus(param1);
}

void MR::curePlayerElementMode() {
    changePlayerItemStatus(8);
}

bool MR::isPlayerParalyzing() {
    return MarioAccess::isParalyzing();
}

bool MR::isPlayerDamaging() {
    return MarioAccess::getPlayerActor()->isDamaging();
}

bool MR::isPlayerStaggering() {
    return MarioAccess::getPlayerActor()->isStaggering();
}

bool MR::isPlayerSwimming() {
    return MarioAccess::isSwimming();
}

bool MR::isPlayerSleeping() {
    return MarioAccess::getPlayerActor()->isSleeping();
}

bool MR::isPlayerJumpRising() {
    return MarioAccess::getPlayerActor()->isJumpRising();
}

void MR::validatePlayerSensor() {
    return MarioAccess::validateSensor();
}

bool MR::isPlayerInBind() {
    return MarioAccess::isInRush();
}

void MR::endBindAndPlayerWait(LiveActor* pActor) {
    TVec3f velocity(0.0f, 0.0f, 0.0f);
    RushEndInfo info(pActor, 0, velocity, false, 0);

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerJump(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
    RushEndInfo info = RushEndInfo(pActor, 2, rParam2, true, param3);

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerForceJump(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
    RushEndInfo info = RushEndInfo(pActor, 2, rParam2, true, param3);
    info._20 |= 0x40000000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerWeakGravityJump(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerForceWeakGravityJump(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0x40000000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerForceWeakGravityJumpInputOff(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0xC0000000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerWeakGravityLimitJump(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0x800000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndSpinDriverJump(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0xC0000000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerDamage(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 1;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerFlip(LiveActor* pActor, const TVec3f& rParam2) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 6;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerJumpWithRollLanding(LiveActor* pActor, const TVec3f& rParam2, u32 param3) {
    RushEndInfo info = RushEndInfo(pActor, 3, rParam2, true, param3);
    info._20 |= 0x400000;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerDamageMsg(LiveActor* pActor, u32 msg) {
    endBindAndPlayerDamageMsg(pActor, msg, TVec3f(0.0f, 0.0f, 0.0f));
}

void MR::endBindAndPlayerDamageMsg(LiveActor* pActor, u32 msg, const TVec3f& rParam3) {
    switch (msg) {
    case ACTMES_ENEMY_ATTACK_FIREBACK:
    case ACTMES_ENEMY_ATTACK_FIRERUN:
    case ACTMES_ENEMY_ATTACK_FIRERUN_STRONG:
        endBindAndPlayerFireDamage(pActor);
        break;
    case ACTMES_ENEMY_ATTACK_ACID:
        endBindAndPlayerAcidDamage(pActor);
        break;
    case ACTMES_ENEMY_ATTACK_FREEZE:
        endBindAndPlayerFreezeDamage(pActor);
        break;
    case ACTMES_ENEMY_ATTACK_ELECTRIC:
    case ACTMES_ENEMY_ATTACK_HEATBEAM:
        endBindAndPlayerElectricDamage(pActor);
        break;
    case ACTMES_ENEMY_ATTACK_EXPLOSION:
        endBindAndPlayerDamage(pActor, rParam3);
        break;
    case ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK:
    case ACTMES_ENEMY_ATTACK_FLIP_WEAK:
    case ACTMES_ENEMY_ATTACK_FLIP_WEAK_JUMP:
    case ACTMES_ENEMY_ATTACK_FLIP_ROT:
    case ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM:
        endBindAndPlayerWeakGravityLimitJump(pActor, rParam3);
        break;
    case ACTMES_ENEMY_ATTACK_FLIP:
        endBindAndPlayerFlip(pActor, rParam3);
        break;
    default:
        endBindAndPlayerDamage(pActor, rParam3);
        break;
    }
}

void MR::endBindAndPlayerAcidDamage(LiveActor* pActor) {
    RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 4;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerFreezeDamage(LiveActor* pActor) {
    RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 3;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerFireDamage(LiveActor* pActor) {
    RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 2;

    MarioAccess::endRush(&info);
}

void MR::endBindAndPlayerElectricDamage(LiveActor* pActor) {
    RushEndInfo info = RushEndInfo(pActor, 3, TVec3f(0.0f, 0.0f, 0.0f), true, 0);
    info._20 |= 0xC0000000;
    info.mFlags.mDamageType = 5;

    MarioAccess::endRush(&info);
}

LiveActor* MR::getCurrentRushActor() {
    if (isPlayerInRush() == false) {
        return nullptr;
    }

    return MarioAccess::getPlayerActor()->_924->mHost;
}

HitSensor* MR::getCurrentRushSensor() {
    if (isPlayerInRush() == false) {
        return nullptr;
    }

    return MarioAccess::getPlayerActor()->_924;
}

void MR::tryPlayerCoinPull() {
    MarioAccess::tryCoinPull();
}

void MR::tryPlayerPullActor(HitSensor* pSensor) {
    MarioAccess::getPlayerActor()->tryTornadoPull(pSensor);
}

void MR::tryPlayerDropTakingActor() {
    MarioAccess::dropTakingActor();
}

void MR::tryPlayerKillTakingActor() {
    MarioAccess::killTakingActor();
}

bool MR::isPlayerTakingActor(const char* pName) {
    if (MarioAccess::getTakingSensor() != nullptr) {
        if (strcmp(MarioAccess::getTakingSensor()->mHost->mName, pName) == 0) {
            return true;
        }
    }

    return false;
}

bool MR::isPlayerCarryAny() {
    return MarioAccess::getPlayerActor()->_468 != 0;
}

void MR::startSoundPlayer(const char* pName, s32 param1) {
    MR::startSound(MarioAccess::getPlayerActor(), pName, param1);
}

void MR::startLevelSoundPlayer(const char* pName, s32 param1) {
    MR::startLevelSound(MarioAccess::getPlayerActor(), pName, param1);
}

void MR::stopSoundPlayer(const char* pName, u32 param1) {
    MR::stopSound(MarioAccess::getPlayerActor(), pName, param1);
}

void MR::startSoundPlayerJ(const char* pName) {
    MarioAccess::getPlayerActor()->playSound(pName, -1);
}

void MR::showPlayer() {
    MarioAccess::show();
}

void MR::hidePlayer() {
    MarioAccess::hide();
}

void MR::showPlayerJoint(const char* pName) {
    MR::showJoint(MarioAccess::getPlayerActor(), pName);
}

void MR::hidePlayerJoint(const char* pName) {
    MR::hideJoint(MarioAccess::getPlayerActor(), pName);
}

void MR::setPlayerSpot(f32 param1, u32 param2) {
    MarioAccess::setSpot(param1, param2);
}

void MR::startPlayerDownWipe() {
    MarioAccess::startDownWipe();
}

void MR::setCameraTargetToPlayer(CameraTargetArg* pTarget) {
    pTarget->mMarioActor = MarioAccess::getPlayerActor();
    pTarget->mTargetObj = nullptr;
    pTarget->mTargetMtx = nullptr;
    pTarget->mLiveActor = nullptr;
}

bool MR::isPlayerDisableFpView() {
    return MarioAccess::isDisableFpView();
}

bool MR::isFpViewChangingFailure() {
    return MarioAccess::isFpViewChangingFailure();
}

void MR::stopPlayerFpView() {
    return MarioAccess::stopFpView();
}

void MR::setRasterScroll(s32 param1, s32 param2, s32 param3) {
    MarioAccess::getPlayerActor()->setRasterScroll(param1, param2, param3);
}

void MR::noticePlayerDashChance() {
    MarioAccess::noticeDashChance();
}

void MR::startPlayerTalk(const LiveActor* pActor) {
    MarioAccess::startTalk(pActor);
}

void MR::endPlayerTalk() {
    MarioAccess::endTalk();
}

void MR::preventPlayerRush() {
    MarioAccess::preventRush();
}

bool MR::isExistMario() {
    return MR::isExistSceneObj(SceneObj_MarioHolder);
}

void MR::startPlayerEvent(const char* pName) {
    EventSequencer* eventSequencer;

    eventSequencer = MR::getSceneObj< EventSequencer >(SceneObj_EventSequencer);
    eventSequencer->startEvent(pName);

    requestMovementOn(eventSequencer);
    requestMovementOnPlayer();
}

void MR::offPlayerControl() {
    MarioAccess::offControl();
}

void MR::onPlayerControl(bool param1) {
    MarioAccess::onControl(param1);
}

bool MR::isOffPlayerControl() {
    return MarioAccess::isOffControl();
}

void MR::unlockPlayerAnimation() {
    MarioAccess::getPlayerActor()->setB90(false);
}

void MR::resetPlayerStatus() {
    MarioAccess::getPlayerActor()->resetCondition();
}

void MR::resetPlayerEffect() {
    MR::forceDeleteEffectAll(MarioAccess::getPlayerActor());
    resetChasingStarPiece();
}

void MR::setPlayerBaseMtx(MtxPtr pMtx) {
    MarioAccess::setBaseMtx(pMtx);
}

MtxPtr MR::getPlayerBaseMtx() {
    return MarioAccess::getBaseMtx();
}

LiveActor* MR::getPlayerDemoActor() {
    return MarioAccess::getPlayerActor();
}

void MR::initPlayerAfterOpeningDemo() {
    MarioAccess::getPlayerActor()->initAfterOpeningDemo();
}

void MR::readyPlayerDemo() {
    MarioAccess::readyDemo();
}

void MR::onFollowDemoEffect() {
    MarioAccess::onFollowDemo();
}

void MR::requestMovementOnPlayer() {
    requestMovementOn(MarioAccess::getPlayerActor());
}

void MR::calcPlayerJointMtx(TPos3f* pMtx, const char* pName) {
    pMtx->set(MarioAccess::getJointMtx(pName));
}

bool MR::isPlayerOnPress() {
    return MarioAccess::isOnPress();
}

void MR::pushPlayer(const TVec3f& rAccel) {
    MarioAccess::addVelocity(rAccel);
}

void MR::pushPlayerFromArea(const TVec3f& rAccel) {
    MarioAccess::addVelocityFromArea(rAccel);
}

bool MR::isPlayerInWaterMode() {
    return MarioAccess::isInWaterMode();
}

bool MR::isPlayerOnWaterSurface() {
    return MarioAccess::isOnWaterSurface();
}

bool MR::isPlayerHidden() {
    return isPlayerElementMode(PlayerMode_Teresa);
}

void MR::calcPlayerWorldPadDir(TVec3f* pDir, f32 param2, f32 param3) {
    MarioAccess::calcWorldPadDir(pDir, param2, param3);
}

JUTTexture* MR::getFullScreenBlurTexture() {
    return MarioAccess::getPlayerActor()->_B7C;
}

u16 MR::getPlayerMovementTimer() {
    return MarioAccess::getPlayerActor()->_378;
}

CubeCameraArea* MR::getCameraCube() {
    return MarioAccess::getCameraCubeCode();
}
