#include "Game/Util/NPCUtil.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/NPC/NPCFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <cstdio>

// CLEANME: This file uses a lot of hardcoded matches that are not up to current standards.

namespace {
    static s32 sStarAppearSeStep = 103;
    static s32 sStarAppearSeStepCaretaker = 32;
    static s32 sStarAppearSeStepPenguinCoach = 95;
    static s32 sStarAppearSeStepTeresaRacer = 89;
    static s32 sStarAppearSeStepTrickRabbit = 22;
};  // namespace

namespace NrvTakeOutStar {
    NEW_NERVE(TakeOutStarNrvAnim, TakeOutStar, Anim);
    NEW_NERVE(TakeOutStarNrvDemo, TakeOutStar, Demo);
    NEW_NERVE(TakeOutStarNrvTerm, TakeOutStar, Term);
};  // namespace NrvTakeOutStar

namespace NrvFadeStarter {
    NEW_NERVE(FadeStarterNrvFade, FadeStarter, Fade);
    NEW_NERVE(FadeStarterNrvTerm, FadeStarter, Term);
};  // namespace NrvFadeStarter

namespace NrvDemoStarter {
    NEW_NERVE(DemoStarterNrvInit, DemoStarter, Init);
    NEW_NERVE(DemoStarterNrvFade, DemoStarter, Fade);
    NEW_NERVE(DemoStarterNrvWait, DemoStarter, Wait);
    NEW_NERVE(DemoStarterNrvTerm, DemoStarter, Term);
};  // namespace NrvDemoStarter

bool MR::getNPCItemData(NPCActorItem* pItem, s32 type) {
    return NPCFunction::getNPCItemData(pItem, type);
}

bool MR::isNPCItemFileExist(const char* pName) {
    char path[256];
    snprintf(path, sizeof(path), "/ObjectData/%s.arc", pName);
    return MR::isFileExist(path, false);
}

void MR::initDefaultPose(NPCActor* pActor, const JMapInfoIter& rIter) {
    MR::initDefaultPosAndQuat(pActor, rIter);
    pActor->setInitPose();
}

void MR::turnPlayerToActor(const LiveActor* pActor, f32 turnSpeed) {
    LiveActor* pPlayer = MR::getPlayerDemoActor();
    TPos3f playerMtx;
    playerMtx.set(MR::getPlayerBaseMtx());
    if (MR::isPlayerInWaterMode()) {
        if (MR::isNearPlayer(pActor, 10.0f)) {
            if (!MR::isBckPlaying(pPlayer, "SwimWait")) {
                MR::startBckPlayer("SwimWait");
            }

            return;
        }

        if (MR::faceToPoint(playerMtx, pActor->mPosition, turnSpeed)) {
            if (!MR::isBckPlaying(pPlayer, "SwimWait")) {
                MR::startBckPlayer("SwimWait");
            }
        } else {
            if (!MR::isBckPlaying(pPlayer, "WatchTurnSwim")) {
                MR::startBckPlayer("WatchTurnSwim");
            }

            MR::setPlayerBaseMtx(playerMtx);
        }
    } else if (MR::isOnGroundPlayer()) {
        if (MR::isNearPlayer(pActor, 10.0f)) {
            if (!MR::isBckPlaying(pPlayer, "Watch")) {
                MR::startBckPlayer("Watch");
            }

            return;
        }

        if (MR::faceToPoint(playerMtx, pActor->mPosition, turnSpeed)) {
            if (!MR::isBckPlaying(pPlayer, "Watch")) {
                MR::startBckPlayer("Watch");
            }
        } else {
            if (!MR::isBckPlaying(pPlayer, "WatchTurn")) {
                MR::startBckPlayer("WatchTurn");
            }

            MR::setPlayerBaseMtx(playerMtx);
        }
    }

    TVec3f separation = *MR::getPlayerCenterPos() - pActor->mPosition;
    if (separation.length() < 100.0f && MR::isOnGroundPlayer()) {
        MR::vecKillElement(separation, *MR::getPlayerGroundNormal(), &separation);
        if (!MR::normalizeOrZero(&separation)) {
            MR::pushPlayer(separation * 10.0f);
        }
    }
}

void MR::decidePose(NPCActor* pActor, const TVec3f& rUp, const TVec3f& rFront, const TVec3f& rPos, f32 upBlend, f32 frontBlend, f32 posBlend) {
    MR::blendVec(&pActor->mPosition, pActor->mPosition, rPos, posBlend);
    if (upBlend == 1.0f && frontBlend == 1.0f) {
        MR::makeQuatUpFront(&pActor->_A0, rUp, rFront);
    } else {
        MR::blendQuatUpFront(&pActor->_A0, rUp, rFront, upBlend, frontBlend);
    }
}

void MR::setNPCActorPos(NPCActor* pActor, const char* pName) {
    TPos3f mtx;
    mtx.identity();
    MR::findNamePos(pName, mtx);
    pActor->setBaseMtx(mtx);
    mtx.getTrans(pActor->mPosition);
    MR::resetPosition(pActor);
    MR::onCalcShadowOneTimeAll(pActor);
}

void MR::setNPCActorPos(NPCActor* pActor, const TVec3f& rPos) {
    pActor->mPosition.set(rPos);
    MR::resetPosition(pActor);
    MR::onCalcShadowOneTimeAll(pActor);
}

void MR::setNPCActorPose(NPCActor* pActor, const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rPos) {
    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, rUp, rFront, rPos);
    pActor->setBaseMtx(mtx);
    pActor->mPosition.set(rPos);
    MR::resetPosition(pActor);
    MR::onCalcShadowOneTimeAll(pActor);
}

void MR::followRailPose(NPCActor* pActor, f32 directionBlend, f32 posBlend) {
    MR::decidePose(pActor, -pActor->mGravity, MR::getRailDirection(pActor), MR::getRailPos(pActor), 1.0f, directionBlend, posBlend);
}

void MR::followRailPoseOnGround(NPCActor* pActor, f32 blend) {
    MR::followRailPoseOnGround(pActor, pActor, blend);
}

void MR::followRailPoseOnGround(NPCActor* pActor, const LiveActor* pRailActor, f32 blend) {
    TVec3f pos = MR::getRailPos(pRailActor);
    TVec3f gravity = pActor->mGravity;
    MR::getFirstPolyOnLineToMap(&pos, nullptr, MR::getRailPos(pRailActor) - gravity * 10.0f, gravity * 1000.0f);
    MR::decidePose(pActor, -gravity, MR::getRailDirection(pRailActor), pos, 1.0f, blend, 1.0f);
}

void MR::setDefaultPose(NPCActor* pActor) {
    pActor->setToDefault();
}

bool MR::convertPosOnGround(TVec3f* pPos, const TVec3f& rDirection) {
    Triangle triangle;
    TVec3f hitPos;
    if (MR::getFirstPolyOnLineToMap(&hitPos, &triangle, *pPos, rDirection)) {
        *pPos = hitPos;
        return true;
    }

    return false;
}

void MR::timeKeepDemoFadeIn() {
    MR::openWipeFade();
}

void MR::timeKeepDemoFadeOut() {
    MR::closeWipeFade();
}

void MR::startNPCTalkCamera(const TalkMessageCtrl* pTalk, MtxPtr pActorMtx, f32 distanceScale, s32 frames) {
    MR::startNPCTalkCamera(pTalk, pActorMtx, MR::getPlayerBaseMtx(), distanceScale, frames);
}

void MR::startNPCTalkCamera(const TalkMessageCtrl* pTalk, MtxPtr pActorMtx, MtxPtr pPlayerMtx, f32 distanceScale, s32 frames) {
    TVec3f actorPos;
    TVec3f up;
    TVec3f side;
    TVec3f playerPos;
    TVec3f offset = MR::getMessageBalloonFollowOffset(pTalk);
    if (MR::getMessageBalloonFollowMatrix(pTalk)) {
        pActorMtx = MR::getMessageBalloonFollowMatrix(pTalk);
    }

    up.set< f32 >(pPlayerMtx[0][1], pPlayerMtx[1][1], pPlayerMtx[2][1]);
    actorPos.set< f32 >(pActorMtx[0][3], pActorMtx[1][3], pActorMtx[2][3]);
    playerPos.set< f32 >(pPlayerMtx[0][3], pPlayerMtx[1][3], pPlayerMtx[2][3]);
    if (MR::normalizeOrZero(&up)) {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    f32 distance = playerPos.distance(actorPos);
    f32 angle = MR::atan2(1.0f, MR::cosDegree(67.5f));
    f32 height = MR::vecKillElement(actorPos - playerPos, up, &side);
    f32 scale = (height + offset.y) / distance / 0.75f;
    scale = scale >= 1.0f ? scale : 1.0f;
    distance *= angle;
    distance *= scale;
    f32 cameraDistance = 2.0f * distance;
    f32 cameraHeight = height / 6.0f + offset.y;
    cameraDistance *= distanceScale;
    cameraDistance = cameraDistance >= 450.0f ? cameraDistance : 450.0f;
    MR::startTalkCamera(actorPos, up, cameraHeight, cameraDistance, frames);
}

void MR::endNPCTalkCamera(bool interpolate, s32 frames) {
    MR::endTalkCamera(interpolate, frames);
}

void MR::initDefaultPosAndQuat(NPCActor* pActor, const JMapInfoIter& rIter) {
    MR::initDefaultPos(pActor, rIter);
    MR::makeQuatRotateDegree(&pActor->_A0, pActor->mRotation);
    pActor->setInitPose();
}

PartsModel* MR::createNPCGoods(LiveActor* pActor, const char* pModelName, const char* pJointName) {
    PartsModel* pGoods = nullptr;
    if (!MR::isNullOrEmptyString(pModelName) && MR::isNPCItemFileExist(pModelName) && MR::isExistJoint(pActor, pJointName)) {
        pGoods = MR::createPartsModelNpcAndFix(pActor, "グッズ", pModelName, pJointName);
        pGoods->appear();
        if (MR::getLightNumMax(pGoods) > 0) {
            MR::initLightCtrl(pGoods);
        }
    }

    return pGoods;
}

PartsModel* MR::createIndirectNPCGoods(LiveActor* pActor, const char* pModelName, const char* pJointName) {
    PartsModel* pGoods = nullptr;
    if (!MR::isNullOrEmptyString(pModelName) && MR::isNPCItemFileExist(pModelName) && MR::isExistJoint(pActor, pJointName)) {
        pGoods = MR::createPartsModelIndirectNpc(pActor, "グッズ", pModelName, MR::getJointMtx(pActor, pJointName));
        pGoods->appear();
        if (MR::getLightNumMax(pGoods) > 0) {
            MR::initLightCtrl(pGoods);
        }
    }

    return pGoods;
}

bool MR::calcPlayerFaceStareVector(TVec3f* pOut, MtxPtr pActorMtx, MtxPtr pBaseMtx) {
    TPos3f mtx;
    TVec3f facePos;
    TVec3f actorPos;
    TVec3f front;
    TVec3f side;
    TVec3f up;
    TVec3f direction;
    bool facing = true;
    MR::calcPlayerJointMtx(&mtx, "Face0");
    mtx.getTrans(facePos);
    mtx.set(pActorMtx);
    mtx.getTrans(actorPos);
    mtx.set(pBaseMtx);
    mtx.getXDir(side);
    mtx.getZDir(front);
    mtx.getYDir(up);
    direction.set(facePos - actorPos);
    if (front.dot(direction) < 0.0f) {
        f32 projection = MR::vecKillElement(direction, front, &direction);
        direction = direction - (front * 2.0f) * projection;
        facing = false;
    }

    pOut->set(direction);
    return facing;
}

bool MR::calcPlayerFaceStarePos(TVec3f* pOut, MtxPtr pActorMtx, MtxPtr pBaseMtx) {
    TVec3f pos;
    MR::extractMtxTrans(pActorMtx, &pos);
    bool facing = MR::calcPlayerFaceStareVector(pOut, pActorMtx, pBaseMtx);
    pOut->add(pos);
    return facing;
}

bool MR::isActionContinuous(const LiveActor* pActor) {
    return MR::getBckCtrl(pActor)->getAttribute() == 0 && !MR::isBckStopped(pActor);
}

bool MR::isActionLoopedOrStopped(const LiveActor* pActor) {
    if (MR::getBckCtrl(pActor)->getAttribute() == 0) {
        return MR::isBckStopped(pActor);
    }

    return MR::isBckLooped(pActor);
}

void MR::invalidateLodCtrl(const NPCActor* pActor) {
    pActor->mLodCtrl->invalidate();
}

void MR::startMoveAction(NPCActor* pActor) {
    if (MR::isExistRail(pActor)) {
        MR::adjustmentRailCoordSpeed(pActor, pActor->_10C, pActor->_110);
        MR::moveRailRider(pActor);
        if (pActor->_124) {
            MR::followRailPoseOnGround(pActor, pActor->_114);
        } else {
            MR::followRailPose(pActor, pActor->_114, pActor->_114);
        }

        if (MR::isRailReachedGoal(pActor)) {
            MR::reverseRailDirection(pActor);
        }
    }
}

bool MR::tryStartTalkAction(NPCActor* pActor) {
    const char* pAction;
    if (MR::isTalkTalking(pActor->mMsgCtrl)) {
        if (pActor->mParam._1 && !pActor->turnToPlayer(pActor->mParam._8, pActor->mParam._C, pActor->mParam._10)) {
            pAction = pActor->mParam._20;
        } else {
            pAction = pActor->mParam._1C;
        }
    } else {
        return MR::tryStartTurnAction(pActor);
    }

    if (!MR::isNullOrEmptyString(pAction)) {
        return MR::tryStartAction(pActor, pAction);
    }

    return false;
}

bool MR::tryStartMoveTalkAction(NPCActor* pActor) {
    TalkMessageCtrl* pTalk = pActor->mMsgCtrl;
    if (!MR::isExistRail(pActor)) {
        return MR::tryStartTalkAction(pActor);
    }

    bool movingTalk = false;
    const char* pAction;
    if (MR::isTalkTalking(pTalk) && !MR::isShortTalk(pTalk)) {
        if (pActor->mParam._1 && !pActor->turnToPlayer(pActor->mParam._8, pActor->mParam._C, pActor->mParam._10)) {
            pAction = pActor->mParam._20;
        } else {
            pAction = pActor->mParam._1C;
        }
    } else {
        if (MR::isNearZero(pActor->_10C) && MR::isNearZero(MR::getRailCoordSpeed(pActor))) {
            return MR::tryStartTalkAction(pActor);
        }

        MR::startMoveAction(pActor);
        if (MR::isTalkTalking(pTalk)) {
            pAction = pActor->_120;
            movingTalk = true;
        } else {
            pAction = pActor->_11C;
        }
    }

    if (!MR::isNullOrEmptyString(pAction)) {
        bool started = MR::tryStartAction(pActor, pAction);
        if (movingTalk) {
            MR::setBckRate(pActor, pActor->_118);
        } else {
            MR::setBckRate(pActor, 1.0f);
        }

        return started;
    }

    return false;
}

bool MR::tryStartTurnAction(NPCActor* pActor) {
    const char* pAction;
    if (MR::isNearPlayer(pActor, pActor->mParam._4)) {
        if (pActor->mParam._0) {
            if (pActor->turnToPlayer(pActor->mParam._8, pActor->mParam._C, pActor->mParam._10)) {
                pAction = pActor->mParam._14;
            } else {
                pAction = pActor->mParam._18;
            }
        } else {
            pAction = pActor->mParam._14;
        }
    } else {
        if (pActor->mParam._0 || pActor->mParam._1) {
            if (pActor->turnToDefault(pActor->mParam._8)) {
                pAction = pActor->mParam._14;
            } else {
                pAction = pActor->mParam._18;
            }
        } else {
            pAction = pActor->mParam._14;
        }
    }

    if (MR::isNullOrEmptyString(pAction)) {
        return false;
    }

    return MR::tryStartAction(pActor, pAction);
}

bool MR::tryStartMoveTurnAction(NPCActor* pActor) {
    if (!MR::isExistRail(pActor)) {
        return MR::tryStartTurnAction(pActor);
    }

    MR::startMoveAction(pActor);
    if (MR::isNullOrEmptyString(pActor->_11C)) {
        return false;
    }

    return MR::tryStartAction(pActor, pActor->_11C);
}

bool MR::tryStartReaction(NPCActor* pActor) {
    const char* pAction = nullptr;
    bool started = false;
    if (pActor->_128) {
        if (pActor->isTrampledStart()) {
            pAction = pActor->mReactionParam.mTrampledActionName;
        } else if (pActor->isReactionStart()) {
            pAction = pActor->mReactionParam.mReactionActionName;
        } else if (pActor->isSpinAttackedStart()) {
            pAction = pActor->mReactionParam.mSpinAttackedActionName;
        } else if (pActor->_E4) {
            pAction = pActor->mReactionParam.mPointingActionName;
        }
    }

    if (!MR::isNullOrEmptyString(pAction)) {
        if (pActor->isTrampledStart() || pActor->isReactionStart()) {
            MR::stopBck(pActor);
            MR::startAction(pActor, pAction);
            started = true;
        } else if (pActor->isSpinAttackedStart()) {
            started = MR::tryStartAction(pActor, pAction);
        } else if (pActor->_E4) {
            if (!(pActor->mReactionParam.mTrampledActionName != nullptr && MR::isActionStart(pActor, pActor->mReactionParam.mTrampledActionName)) &&
                !(pActor->mReactionParam.mReactionActionName != nullptr && MR::isActionStart(pActor, pActor->mReactionParam.mReactionActionName)) &&
                !(pActor->mReactionParam.mSpinAttackedActionName != nullptr &&
                  MR::isActionStart(pActor, pActor->mReactionParam.mSpinAttackedActionName))) {
                if (pActor->isPointingStart()) {
                    started = MR::tryStartAction(pActor, pAction);
                } else if (MR::isActionLoopedOrStopped(pActor)) {
                    MR::startAction(pActor, pAction);
                }
            }
        }
    } else {
        bool hasScale = pActor->mScaleController && pActor->mDelegator;
        if (hasScale) {
            if (pActor->isPointingStart() || pActor->isSpinAttackedStart() || pActor->isTrampledStart() || pActor->isReactionStart()) {
                started = true;
            }
        }
    }

    return started;
}

bool MR::tryTalkNearPlayerAndStartTalkAction(NPCActor* pActor) {
    MR::tryStartTalkAction(pActor);
    return MR::tryTalkNearPlayer(pActor->mMsgCtrl);
}

bool MR::tryTalkNearPlayerAndStartMoveTalkAction(NPCActor* pActor) {
    MR::tryStartMoveTalkAction(pActor);
    return MR::tryTalkNearPlayer(pActor->mMsgCtrl);
}

bool MR::tryTalkNearPlayerAtEndAndStartTalkAction(NPCActor* pActor) {
    MR::tryStartTalkAction(pActor);
    return MR::tryTalkNearPlayerAtEnd(pActor->mMsgCtrl);
}

bool MR::tryTalkNearPlayerAtEndAndStartMoveTalkAction(NPCActor* pActor) {
    MR::tryStartMoveTalkAction(pActor);
    return MR::tryTalkNearPlayerAtEnd(pActor->mMsgCtrl);
}

bool MR::tryTalkForceAndStartMoveTalkAction(NPCActor* pActor) {
    MR::tryStartMoveTalkAction(pActor);
    return MR::tryTalkForce(pActor->mMsgCtrl);
}

bool MR::tryTalkForceAtEndAndStartTalkAction(NPCActor* pActor) {
    MR::tryStartTalkAction(pActor);
    return MR::tryTalkForceAtEnd(pActor->mMsgCtrl);
}

bool MR::tryStartReactionAndPushNerve(NPCActor* pActor, const Nerve* pNerve) {
    if (MR::tryStartReaction(pActor)) {
        pActor->pushNerve(pNerve);
        return true;
    }

    return false;
}

bool MR::tryStartReactionAndPopNerve(NPCActor* pActor) {
    if (MR::tryStartReaction(pActor)) {
        pActor->pushNerve(pActor->popNerve());
        return false;
    }

    if (pActor->isScaleAnim()) {
        return false;
    }

    if (MR::isActionLoopedOrStopped(pActor)) {
        pActor->popNerve();
        return true;
    }

    return false;
}

bool MR::tryChangeTalkActionRandom(NPCActor* pActor, const char* pAction0, const char* pAction1, const char* pAction2) {
    if (MR::getBckCtrl(pActor)->getAttribute() != 2 || !MR::isBckLooped(pActor)) {
        return false;
    }

    switch (MR::getRandom(0L, 3L)) {
    case 0:
        if (pAction0) {
            pActor->mParam._1C = pAction0;
        }

        break;
    case 1:
        if (pAction1) {
            pActor->mParam._1C = pAction1;
        }

        break;
    case 2:
        if (pAction2) {
            pActor->mParam._1C = pAction2;
        }

        break;
    }

    return true;
}

f32 MR::calcFloatOffset(const NPCActor* pActor, f32 currentOffset, f32 maxOffset) {
    TalkMessageCtrl* pTalk = pActor->mMsgCtrl;
    currentOffset -= 0.5f;
    currentOffset = currentOffset >= 0.0f ? currentOffset : 0.0f;
    if (pTalk && MR::isTalkTalking(pTalk) && !MR::isShortTalk(pTalk)) {
        TVec3f up;
        TVec3f direction(pActor->mPosition - *MR::getPlayerPos());
        MR::getPlayerUpVec(&up);
        if (direction.dot(up) > 0.0f && direction.length() < 200.0f) {
            f32 target = MR::getLinerValueFromMinMax(direction.length(), 0.0f, 200.0f, maxOffset, 0.0f);
            f32 limit = 0.5f + (5.0f + currentOffset);
            currentOffset = limit >= target ? target : limit;
        }
    }

    return currentOffset;
}

void MR::calcAndSetFloatBaseMtx(NPCActor* pActor, f32 offset) {
    TVec3f up;
    TVec3f pos(pActor->mPosition);
    pActor->_A0.getYDir(up);
    up.scale(offset);
    pActor->mPosition.add(up);
    pActor->NPCActor::calcAndSetBaseMtx();
    pActor->mPosition.set(pos);
}

TakeOutStar::TakeOutStar(NPCActor* pActor, const char* pActionName, const char* pAnimName, const Nerve* pNerve)
    : NerveExecutor("パワースター取り出しデモ実行者"), mActor(pActor), mNerve(pNerve), mActionName(pActionName), mAnimName(pAnimName) {
    mStarModel = MR::createPowerStarDemoModel(mActor, "パワースターデモモデル", pActor->getBaseMtx());
    mStarModel->makeActorDead();

    initNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvAnim));
}

bool TakeOutStar::takeOut() {
    if (isNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvTerm))) {
        return true;
    }

    updateNerve();

    return false;
}

bool TakeOutStar::isFirstStep() {
    return isNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvAnim)) && MR::isFirstStep(this);
}

bool TakeOutStar::isLastStep() {
    return isNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvTerm));
}

void TakeOutStar::exeAnim() {
    if (MR::isFirstStep(this)) {
        if (mNerve != nullptr) {
            mActor->pushNerve(mNerve);
        } else {
            mActor->tryPushNullNerve();
        }

        mStarModel->appear();
        MR::invalidateClipping(mStarModel);
        MR::requestMovementOn(mStarModel);
        MR::startBck(mStarModel, mAnimName);
        MR::startAction(mActor, mActionName);
    }

    s32 step = ::sStarAppearSeStep;

    if (MR::isEqualString(mAnimName, "TakeOutStarCaretaker")) {
        step = ::sStarAppearSeStepCaretaker;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarTeresaRacer")) {
        step = ::sStarAppearSeStepTeresaRacer;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarPenguinCoach")) {
        step = ::sStarAppearSeStepPenguinCoach;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarTrickRabbit")) {
        step = ::sStarAppearSeStepTrickRabbit;
    }

    if (MR::isGreaterStep(this, step)) {
        if (MR::isInWater(mStarModel, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(mStarModel, "SE_OJ_LV_POW_STAR_EXIST_W");
        } else {
            MR::startLevelSound(mStarModel, "SE_OJ_LV_POW_STAR_EXIST");
        }
    }

    if (MR::isAnyAnimOneTimeAndStopped(mActor, mActionName)) {
        setNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvDemo));
    }
}

void TakeOutStar::exeDemo() {
    if (MR::isFirstStep(this)) {
        TVec3f trans;
        MR::extractMtxTrans(MR::getJointMtx(mStarModel, "PowerStar"), &trans);
        MR::appearPowerStarContinueCurrentDemo(mActor, trans);
        mStarModel->kill();
    }

    if (MR::isEndPowerStarAppearDemo(mActor)) {
        MR::validateClipping(mStarModel);
        mActor->popNerve();
        setNerve(GET_NERVE(TakeOutStar, TakeOutStarNrvTerm));
    }
}

void TakeOutStar::exeTerm() {
}

FadeStarter::FadeStarter(NPCActor* pActor, s32 a2) : NerveExecutor("フェード開始制御"), mActor(pActor), _C(), _10(a2) {
    initNerve(GET_NERVE(FadeStarter, FadeStarterNrvFade));
}

bool FadeStarter::update() {
    if (isNerve(GET_NERVE(FadeStarter, FadeStarterNrvTerm))) {
        return true;
    }

    updateNerve();

    return false;
}

void FadeStarter::exeFade() {
    if (MR::isFirstStep(this)) {
        if (!mActor->isEmptyNerve()) {
            _C = mActor->popNerve();
        }

        mActor->tryPushNullNerve();
        MR::closeWipeFade(_10);
    }

    if (MR::isWipeActive()) {
        return;
    }

    mActor->popNerve();

    if (_C != nullptr) {
        mActor->pushNerve(_C);
        _C = nullptr;
    }

    MR::openWipeFade(_10);
    setNerve(GET_NERVE(FadeStarter, FadeStarterNrvTerm));
}

void FadeStarter::exeTerm() {
}

DemoStarter::DemoStarter(NPCActor* pActor) : NerveExecutor("デモ開始制御"), mActor(pActor) {
    initNerve(GET_NERVE(DemoStarter, DemoStarterNrvInit));
}

bool DemoStarter::update() {
    updateNerve();

    return isNerve(GET_NERVE(DemoStarter, DemoStarterNrvTerm));
}

void DemoStarter::start() {
    if (isNerve(GET_NERVE(DemoStarter, DemoStarterNrvInit))) {
        setNerve(GET_NERVE(DemoStarter, DemoStarterNrvFade));
    }
}

void DemoStarter::exeInit() {
}

void DemoStarter::exeFade() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(mActor);
        MR::offPlayerControl();
        MR::closeWipeFade();
    }

    if (MR::isWipeActive()) {
        return;
    }

    setNerve(GET_NERVE(DemoStarter, DemoStarterNrvWait));
}

void DemoStarter::exeWait() {
    if (MR::isLessStep(this, 30)) {
        return;
    }

    if (MR::canStartDemo()) {
        setNerve(GET_NERVE(DemoStarter, DemoStarterNrvTerm));
    }
}

void DemoStarter::exeTerm() {
}
