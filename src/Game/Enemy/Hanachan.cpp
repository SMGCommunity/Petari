#include "Game/Enemy/Hanachan.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const f32 hFarDistToPursuePlayer = 2000.0f;
    // hPursuePlayerTurnLimitRadian
    // hSearchTurnLimitRadian
    const f32 hCommonGravity = 0.5f;
    const f32 hBlowGravity = 1.0f;
    const f32 hOverturnGravity = 2.0f;
    const f32 hOverturnBoundGravity = 1.0f;
    const f32 hRecoverGravity = 1.0f;
    const f32 hWallHitEndGravity = 2.0f;
    // hGroundDamp
    // hAirDampH
    // hAirDampV
    // hUpdateUpRate
    // hUpdateFrontRate
    // hInfectionDelay
    const f32 hTrampleJumpVelV = 5.0f;
    const f32 hBecomeAngryJumpVelV = 5.0f;
    const f32 hOverturnJumpVelV = 25.0f;
    const f32 hOverturnBoundJumpVelV = 15.0f;
    const f32 hRecoverJumpVelV = 17.0f;
    const f32 hWallHitEndJumpVelV = 20.0f;
    // hToRailDiv
    const s32 hAngryPursueTime = 300;
    // hAngryEndTime
    // hOverturnRate
    // hRecoverRate
    const s32 hOverturnTime = 180;
    const f32 hWalkSpeed = 4.0f;
    const f32 hAngrySpeed = 13.0f;
    // hPartsToRailMul
    const s32 hWalkStep = 8;
    const f32 hWalkAnimRate = 1.0f;
    const f32 hAngryAnimRate = 3.0f;
    const s32 hBecomeAngryInterval = 10;
    const s32 hPunchStopSceneTime = 4;
    const s32 hSpinInterval = 3;
    const s32 hOverturnCanTrampleDeathTime = 50;
    const s32 hRecoverCanTrampleDeathTime = 60;
    const s32 hRecoverInit = 67;
    const s32 hRecoverInterval = 3;
    const s32 hRecoverAnimBegin = 55;
    const s32 hHipDroppedTime = 30;
    const f32 hNearHipDropReactionDist = 1000.0f;
    const s32 hOverturnBoundInterval = 2;
    const s32 hWallHitInterval = 3;
    const f32 hHitWallCheck = -0.9f;
    const f32 hInitBlowRadian = 0.2f;
    const s32 hBlowTime = 30;
    const f32 hBlowVelH = 30.0f;
    const f32 hBlowVelV = 20.0f;
    const s32 hBindMinStep = 10;
    const f32 hPushRate = 0.25f;
    const f32 hCollideRange = 2.0f;
    const s32 hRecoverSteamSeTime = 60;
    const s32 hRecoverGetupSeTime = 63;
    const s32 hOverturnBoundSeTime = 29;
    const f32 hAngryEndJumpVelV = 0.0f;
    // hToPrevVelMul
    // hPushPartsVel
    const s32 hTrampleInit = 0;
    const s32 hTrampleInterval = 0;
    const s32 hTrampleAnimBegin = 0;
    const s32 hSpinInit = 0;
    const s32 hOverturnAnimBegin = 0;
    const s32 hHipDroppedInit = 0;
    const s32 hHipDroppedInterval = 0;
    const s32 hOverturnBoundInit = 0;
    const s32 hOverturnBoundAnimBegin = 0;
};  // namespace

namespace NrvHanachan {
    NEW_NERVE(HanachanPartsNrvWalk, HanachanParts, Walk);
    NEW_NERVE_ONEND(HanachanPartsNrvBecomeAngry, HanachanParts, BecomeAngry, BecomeAngry);
    NEW_NERVE_ONEND(HanachanPartsNrvAngryPursue, HanachanParts, AngryPursue, AngryPursue);
    NEW_NERVE(HanachanPartsNrvAngryEnd, HanachanParts, AngryEnd);
    NEW_NERVE(HanachanPartsNrvWallHitEnd, HanachanParts, WallHitEnd);
    NEW_NERVE(HanachanPartsNrvTrample, HanachanParts, Trample);
    NEW_NERVE(HanachanPartsNrvOverturn, HanachanParts, Overturn);
    NEW_NERVE(HanachanPartsNrvOverturnBound, HanachanParts, OverturnBound);
    NEW_NERVE(HanachanPartsNrvOverturnWait, HanachanParts, OverturnWait);
    NEW_NERVE_ONEND(HanachanPartsNrvRecover, HanachanParts, Recover, Recover);
    NEW_NERVE(HanachanPartsNrvHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvOverturnHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvBlow, HanachanParts, Blow);
    NEW_NERVE_ONEND(HanachanPartsNrvStarPointerBind, HanachanParts, StarPointerBind, StarPointerBind);

    NEW_NERVE(HanachanNrvHanachanWalk, Hanachan, Walk);
    NEW_NERVE(HanachanNrvHanachanBecomeAngry, Hanachan, BecomeAngry);
    NEW_NERVE(HanachanNrvHanachanAngryPursue, Hanachan, AngryPursue);
    NEW_NERVE(HanachanNrvHanachanAngryEnd, Hanachan, AngryEnd);
    NEW_NERVE(HanachanNrvHanachanWallHitEnd, Hanachan, WallHitEnd);
    NEW_NERVE(HanachanNrvHanachanTrample, Hanachan, Trample);
    NEW_NERVE(HanachanNrvHanachanOverturn, Hanachan, Overturn);
    NEW_NERVE(HanachanNrvHanachanOverturnBound, Hanachan, OverturnBound);
    NEW_NERVE(HanachanNrvHanachanOverturnWait, Hanachan, OverturnWait);
    NEW_NERVE(HanachanNrvHanachanRecover, Hanachan, Recover);
    NEW_NERVE(HanachanNrvHanachanHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanOverturnHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanBlow, Hanachan, Blow);
    NEW_NERVE_ONEND(HanachanNrvHanachanStarPointerBind, Hanachan, StarPointerBind, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEnd, Hanachan, WallHitEnd);
    NEW_NERVE_ONEND(HanachanNrvHanachanStarPointerBindOverturn, Hanachan, StarPointerBind, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEndOverturn, Hanachan, WallHitEnd);
};  // namespace NrvHanachan

inline bool HanachanParts::isLandedInNerve(const Nerve* pNerve) const {
    return isNerve(pNerve) && mIsLanded;
}

inline bool HanachanParts::isHipDroppedLanded() const {
    return (isNerve(&NrvHanachan::HanachanPartsNrvHipDropped::sInstance) || isNerve(&NrvHanachan::HanachanPartsNrvOverturnHipDropped::sInstance)) &&
           mIsLanded;
}

inline TVec3f Hanachan::calcSensorDirection(const HitSensor* pSensor, const HitSensor* pTarget) const {
    TVec3f dir = pTarget->mPosition;
    dir -= pSensor->mPosition;
    MR::normalizeOrZero(&dir);
    return dir;
}

HanachanParts::HanachanParts(Hanachan* pParent, s32 segmentIndex, const char* pType, const char* pName)
    : LiveActor(pType), mParent(pParent), mPushVelocity(0.0f), mFallVelocity(0.0f), mPartsType(PartsType_Body), mRotation(0.0f, 1.0f),
      mSegmentIndex(segmentIndex), mActionStartStep(-1), mIsLanded() {
    initModelManagerWithAnm(pName, nullptr, false);

    if (MR::isEqualString(pName, "HanachanBody")) {
        mPartsType = PartsType_Body;
    } else if (MR::isEqualString(pName, "HanachanBodyS")) {
        mPartsType = PartsType_BodyS;
    } else if (MR::isEqualString(pName, "HanachanHead")) {
        mPartsType = PartsType_Head;
    }
}

void HanachanParts::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, MR::MovementType_DemoDirector, MR::DrawType_SpinDriverPathDrawer, -1);
    initNerve(&NrvHanachan::HanachanPartsNrvWalk::sInstance);
    initHitSensor(1);

    if (mPartsType == PartsType_Body) {
        MR::addHitSensorEnemy(this, "body", 32, 85.0f, TVec3f(0.0f, 100.0f, 0.0f));
    } else if (mPartsType == PartsType_BodyS) {
        MR::addHitSensorEnemy(this, "body", 32, 85.0f, TVec3f(0.0f, 85.0f, 0.0f));
    } else if (mPartsType == PartsType_Head) {
        MR::addHitSensorEnemy(this, "body", 32, 85.0f, TVec3f(0.0f, 100.0f, 0.0f));
    }

    initBinder(100.0f, 100.0f, 0);

    initEffectKeeper(1, mPartsType == PartsType_Head ? "HanachanHead" : "HanachanBody", false);
    MR::addEffect(this, "PointerTouch");
    MR::initLightCtrl(this);
    initSound(2, false);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 70.0f);
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::startBrk(this, "Normal");

    if (mPartsType == PartsType_Head) {
        MR::startBva(this, "normal");
    }
}

const TVec3f* HanachanParts::getCommonGravity() const {
    if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanTrample::sInstance) ||
        mParent->isNerve(&NrvHanachan::HanachanNrvHanachanBecomeAngry::sInstance) ||
        mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance) ||
        mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance) ||
        mParent->isNerve(&NrvHanachan::HanachanNrvHanachanRecover::sInstance)) {
        return &mParent->mBodyParts[2]->mGravity;
    }

    return &mGravity;
}

void HanachanParts::kill() {
    LiveActor::kill();

    MR::emitEffect(this, mPartsType == PartsType_Head ? "HeadDeath" : "BodyDeath");

    if (mPartsType == PartsType_Body || mPartsType == PartsType_Head) {
        MR::appearStarPiece(mParent, mPosition, 2, 10.0f, 40.0f, false);
        MR::startSound(mParent, "SE_OJ_STAR_PIECE_BURST");
    }
}

void HanachanParts::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (!MR::isPlayerDamaging()) {
            if (isNerve(&NrvHanachan::HanachanPartsNrvAngryPursue::sInstance) && MR::isOnGroundPlayer()) {
                if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                    mParent->setNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance);
                    return;
                }

                MR::sendMsgPush(pReceiver, pSender);
            } else if (isNerve(&NrvHanachan::HanachanPartsNrvWalk::sInstance) && MR::isOnGroundPlayer()) {
                if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                    return;
                }

                MR::sendMsgPush(pReceiver, pSender);
            } else if (!isNerve(&NrvHanachan::HanachanPartsNrvHipDropped::sInstance) &&
                       !isNerve(&NrvHanachan::HanachanPartsNrvOverturnHipDropped::sInstance)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else if (MR::isSensorEnemy(pReceiver)) {
        if (!mParent->isOwnSensor(pReceiver) &&
            (isNerve(&NrvHanachan::HanachanPartsNrvBecomeAngry::sInstance) || isNerve(&NrvHanachan::HanachanPartsNrvAngryPursue::sInstance)) &&
            MR::sendMsgToEnemyAttackBlow(pReceiver, pSender) && mPartsType == PartsType_Head) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance);
            return;
        }

        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool HanachanParts::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInvincibleAttack(msg)) {
        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanBlow::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanHipDropped::sInstance)) {
            return false;
        }

        mParent->setNerveBlow(pSender->mPosition);
        return true;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance)) {
            return false;
        }

        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance)) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance);
            mParent->mAttackPos = pSender->mPosition;
            return true;
        }

        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance)) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEnd::sInstance);
            mParent->mAttackPos = pSender->mPosition;
            return true;
        }

        return false;
    }

    if (isNerve(&NrvHanachan::HanachanPartsNrvHipDropped::sInstance) || isNerve(&NrvHanachan::HanachanPartsNrvOverturnHipDropped::sInstance)) {
        return false;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        if (!mParent->isNerve(&NrvHanachan::HanachanNrvHanachanHipDropped::sInstance) &&
            !mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnHipDropped::sInstance)) {
            if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance) ||
                mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance) ||
                mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance) ||
                mParent->isNerve(&NrvHanachan::HanachanNrvHanachanRecover::sInstance)) {
                mParent->setNerve(&NrvHanachan::HanachanNrvHanachanOverturnHipDropped::sInstance);
            } else {
                mParent->setNerve(&NrvHanachan::HanachanNrvHanachanHipDropped::sInstance);
            }

            mParent->mAttackPos = pSender->mPosition;
            return true;
        }

        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::startSound(this, "SE_EM_HANACHAN_TRAMPLE");

        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanAngryPursue::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanAngryEnd::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanTrample::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanBecomeAngry::sInstance)) {
            return true;
        }

        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance) ||
            (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance) && MR::isGreaterStep(mParent, ::hOverturnCanTrampleDeathTime)) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance) ||
            (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanRecover::sInstance) && MR::isLessStep(mParent, ::hRecoverCanTrampleDeathTime)) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindOverturn::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEndOverturn::sInstance)) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanOverturnHipDropped::sInstance);
            mParent->mAttackPos = pSender->mPosition;
            return true;
        }

        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance)) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanTrample::sInstance);
            mParent->mAttackPos = pSender->mPosition;
            return true;
        }

        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance) ||
            mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance)) {
            return false;
        }

        MR::stopScene(::hPunchStopSceneTime);

        if (mParent->isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance)) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance);
        } else {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance);
        }

        mParent->mAttackPos = pSender->mPosition;
        return true;
    }

    return false;
}

bool HanachanParts::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return true;
}

bool HanachanParts::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return false;
    }

    TVec3f dir = pReceiver->mPosition - pSender->mPosition;

    f32 depth;

    if (!mParent->isOwnSensor(pSender)) {
        f32 senderRadius = pSender->mRadius;
        f32 receiverRadius = pReceiver->mRadius;
        depth = (receiverRadius + senderRadius) - dir.length();
    } else {
        f32 senderRadius = pSender->mRadius;
        f32 receiverRadius = pReceiver->mRadius;
        depth = (receiverRadius / 2.0f + senderRadius / 2.0f) - dir.length();

        if (depth < 0.0f) {
            return false;
        }
    }

    dir.setLength(::hPushRate * depth);
    mPushVelocity += dir;
    return true;
}

void HanachanParts::calcAndSetBaseMtx() {
    TQuat4f rot;
    rot = mRotation;

    TPos3f mtx;
    mtx.makeQuat(rot);
    mtx.setTrans(mPosition);

    MR::setBaseTRMtx(this, mtx);
    MR::setBaseScale(this, mParent->mScaleController->_C * mScale);
}

void HanachanParts::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
        s32 frameMax = MR::getBckFrameMax(this, "Walk") - 1;
        f32 unused = (frameMax + ::hWalkStep * mSegmentIndex) % frameMax;
        MR::setBckFrameAndStop(this, ::hWalkStep * mSegmentIndex);
        MR::setBckRate(this, ::hWalkAnimRate);
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hCommonGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeTrample() {
    if (MR::isFirstStep(this)) {
        mIsLanded = false;
    }

    if (MR::isStep(this, mActionStartStep + ::hTrampleAnimBegin)) {
        MR::startAction(this, "AngryStart");
    }

    if (MR::isStep(this, mActionStartStep)) {
        mFallVelocity = *getCommonGravity() * -::hTrampleJumpVelV;
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        if (!mIsLanded && MR::isGreaterStep(this, mActionStartStep)) {
            mIsLanded = true;
            changeHeadAngry();
            MR::startAction(this, "Walk");

            s32 frameMax = MR::getBckFrameMax(this, "Walk") - 1;
            f32 unused = (frameMax + mSegmentIndex * ::hWalkStep) % frameMax;

            MR::setBckFrameAndStop(this, mSegmentIndex * ::hWalkStep);
            MR::setBckRate(this, ::hAngryAnimRate);
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hCommonGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeBecomeAngry() {
    if (MR::isFirstStep(this)) {
        mFallVelocity = *getCommonGravity() * -::hBecomeAngryJumpVelV;
        mActionStartStep = 0;
        mIsLanded = false;
        MR::startBrk(this, "Anger");
        MR::invalidateClipping(mParent);
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        if (!mIsLanded && MR::isGreaterStep(this, mActionStartStep)) {
            mIsLanded = true;
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hCommonGravity;
        mFallVelocity.mult(0.98f);
    }
}

inline void HanachanParts::endBecomeAngry() {
    changeHeadCalmDown();
}

void HanachanParts::exeAngryPursue() {
    if (MR::isFirstStep(this)) {
        changeHeadAngry();
    }

    bool isWallHit = false;
    if (isHeadHitWall() && MR::isBindedWallOfMap(this)) {
        isWallHit = true;
    }

    if (isWallHit) {
        mParent->setNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance);
    } else {
        bool isMoveLimitHit = false;
        if (isHeadHitWall() && MR::isBindedWallOfMoveLimit(this)) {
            isMoveLimitHit = true;
        }

        if (isMoveLimitHit) {
            mParent->setNerve(&NrvHanachan::HanachanNrvHanachanAngryEnd::sInstance);
        } else {
            mVelocity = mFallVelocity;
            mVelocity += mPushVelocity;

            if (MR::isOnGround(this)) {
                mFallVelocity.zero();
            } else {
                mFallVelocity += mGravity * ::hCommonGravity;
                mFallVelocity.mult(0.98f);
            }
        }
    }
}

inline void HanachanParts::endAngryPursue() {
    changeHeadCalmDown();
}

void HanachanParts::exeAngryEnd() {
    if (MR::isFirstStep(this)) {
        mIsLanded = false;
    }

    if (MR::isStep(this, mActionStartStep)) {
        MR::startAction(this, "AngryEnd");
        MR::startBrk(this, "Normal");
        MR::validateClipping(mParent);
    }

    if (MR::isStep(this, mActionStartStep)) {
        mFallVelocity = *getCommonGravity() * -::hAngryEndJumpVelV;
    }

    bool isWallHit = false;
    if (isHeadHitWall() && MR::isBindedWallOfMap(this)) {
        isWallHit = true;
    }

    if (isWallHit) {
        mParent->setNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance);
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        if (!mIsLanded && MR::isGreaterStep(this, mActionStartStep)) {
            mIsLanded = true;
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hCommonGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeWallHitEnd() {
    if (MR::isFirstStep(this)) {
        mIsLanded = false;
    }

    if (MR::isStep(this, mActionStartStep)) {
        MR::startBrk(this, "Normal");
        MR::validateClipping(mParent);
        mFallVelocity = *getCommonGravity() * -::hWallHitEndJumpVelV;
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        if (!mIsLanded && MR::isGreaterStep(this, mActionStartStep)) {
            mIsLanded = true;
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hWallHitEndGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeOverturn() {
    if (MR::isStep(this, mActionStartStep + ::hOverturnAnimBegin)) {
        MR::startAction(this, "Overturn");
        MR::startBrk(this, "Normal");
        MR::validateClipping(mParent);
        changeHeadCalmDown();
    }

    if (MR::isStep(this, mActionStartStep)) {
        mFallVelocity = *getCommonGravity() * -::hOverturnJumpVelV;
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        mFallVelocity.zero();

        if (MR::isGreaterStep(this, mActionStartStep)) {
            setNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance);
        }
    } else {
        mFallVelocity += *getCommonGravity() * ::hOverturnGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeOverturnBound() {
    if (MR::isStep(this, mActionStartStep + ::hOverturnBoundAnimBegin)) {
        MR::startAction(this, "OverturnBound");
    }

    if (MR::isStep(this, mActionStartStep)) {
        mFallVelocity = *getCommonGravity() * -::hOverturnBoundJumpVelV;
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        mFallVelocity.zero();

        if (MR::isGreaterStep(this, mActionStartStep)) {
            setNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance);
        }
    } else {
        mFallVelocity += *getCommonGravity() * ::hOverturnBoundGravity;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeOverturnWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "OverturnLand");
        mIsLanded = false;
    }

    if (!mIsLanded && MR::isBckStopped(this)) {
        MR::startAction(this, "OverturnWait");
        s16 frameMax = MR::getBckFrameMax(this, "OverturnWait");
        s32 frame = mSegmentIndex * ::hWalkStep;
        f32 unused = ((frameMax - 1) + frame) % (frameMax - 1);
        MR::setBckFrameAndStop(this, frame);
        MR::setBckRate(this, ::hWalkAnimRate);
        mIsLanded = true;
    }

    mVelocity.zero();
    mVelocity += mPushVelocity;
}

void HanachanParts::exeRecover() {
    if (MR::isFirstStep(this)) {
        mIsLanded = false;
    }

    if (MR::isStep(this, mActionStartStep - ::hRecoverAnimBegin)) {
        MR::startBrk(this, "Anger");
        MR::invalidateClipping(mParent);
        MR::startAction(this, "Recover");
    }

    if (MR::isStep(this, mActionStartStep)) {
        mFallVelocity = *getCommonGravity() * -::hRecoverJumpVelV;
    }

    if (MR::isGreaterStep(this, mActionStartStep - 50) && mPartsType == PartsType_Head) {
        MR::startBva(this, "Anger");
    }

    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isOnGround(this)) {
        if (mActionStartStep == 0) {
            if (MR::isBckOneTimeAndStopped(this)) {
                mIsLanded = true;
                MR::startAction(this, "Walk");
                MR::setBckRate(this, ::hAngryAnimRate);
            }
        } else if (MR::isGreaterStep(this, mActionStartStep)) {
            MR::startAction(this, "RecoverLand");
            mActionStartStep = 0;
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * ::hRecoverGravity;
        mFallVelocity.mult(0.98f);
    }
}

inline void HanachanParts::endRecover() {
    changeHeadCalmDown();
}

void HanachanParts::exeHipDropped() {
    mVelocity = mFallVelocity;
    mVelocity += mPushVelocity;

    if (MR::isFirstStep(this)) {
        MR::forceBindOnGround(this, 0.0f, 1.0f);
        mIsLanded = false;
    }

    if (MR::isStep(this, mActionStartStep)) {
        s32 nearestId = mParent->calcNearestInfectionId();
        bool isOverturn = isNerve(&NrvHanachan::HanachanPartsNrvOverturnHipDropped::sInstance);

        if (mSegmentIndex == nearestId) {
            MR::startAction(this, isOverturn ? "OverturnPress" : "Press");
        } else if (nearestId == 1 && mPartsType == PartsType_Head) {
            MR::startAction(this, isOverturn ? "OverturnPress" : "Press");
        } else {
            MR::stopBck(this);
        }
    }

    if (MR::isOnGround(this)) {
        if (MR::isGreaterStep(this, mActionStartStep)) {
            if (mSegmentIndex == mParent->calcNearestInfectionId()) {
                MR::isBckStopped(this);
                mIsLanded = true;
            } else {
                mIsLanded = true;
            }
        }

        mFallVelocity.zero();
    } else {
        mFallVelocity += *getCommonGravity() * 2.0f;
        mFallVelocity.mult(0.98f);
    }
}

void HanachanParts::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Rotate");
        mVelocity = mPushVelocity;

        if (mPartsType == PartsType_Head) {
            MR::invalidateExCollisionParts(this);
        }
    }

    mVelocity += mGravity * ::hBlowGravity;

    if ((MR::isGreaterStep(this, 15) && MR::isBinded(this)) || MR::isGreaterStep(this, ::hBlowTime)) {
        kill();
    }
}

void HanachanParts::exeStarPointerBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::stopBck(this);
        mVelocity.zero();
        MR::emitEffect(this, "PointerTouch");
    }

    MR::startDPDFreezeLevelSound(this);
}

inline void HanachanParts::endStarPointerBind() {
    MR::deleteEffect(this, "PointerTouch");
    MR::setBckRate(this, 1.0f);
}

bool HanachanParts::isHeadHitWall() {
    if (mPartsType == PartsType_Head && MR::isBindedWall(this)) {
        TVec3f wallNormal = *MR::getWallNormal(this);

        if (wallNormal.dot(mParent->mFrontDir) <= ::hHitWallCheck) {
            return true;
        }
    }

    return false;
}

void HanachanParts::changeHeadAngry() {
    if (mPartsType == PartsType_Head) {
        MR::startBva(this, "Anger");
        MR::emitEffect(this, "AngrySmoke");
    }
}

void HanachanParts::changeHeadCalmDown() {
    if (mPartsType == PartsType_Head) {
        MR::startBva(this, "Normal");
        MR::deleteEffect(this, "AngrySmoke");
    }
}

Hanachan::Hanachan(const char* pName) : LiveActor(pName), mAttackPos(0, 0, 0), mFrontDir(0, 0, 1), mIsChasePlayer(true), mScaleController() {
}

void Hanachan::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::isConnectedWithRail(rIter);
    }

    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);

    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == 1) {
        mIsChasePlayer = false;
    } else {
        mIsChasePlayer = true;
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::connectToSceneEnemyMovement(this);
    initNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance);
    initSound(6, false);
    MR::validateClipping(this);
    mScaleController = new AnimScaleController(nullptr);
    MR::declareStarPiece(this, 6);
    appear();

    mBodyParts[0] = new HanachanParts(this, 0, "頭パーツ", "HanachanHead");
    mBodyParts[1] = new HanachanParts(this, 1, "体パーツ", "HanachanBodyS");
    mBodyParts[2] = new HanachanParts(this, 2, "体パーツ", "HanachanBody");
    mBodyParts[3] = new HanachanParts(this, 3, "体パーツ", "HanachanBodyS");
    mBodyParts[4] = new HanachanParts(this, 4, "体パーツ", "HanachanBody");

    mBodyParts[0]->initWithoutIter();
    mBodyParts[1]->initWithoutIter();
    mBodyParts[2]->initWithoutIter();
    mBodyParts[3]->initWithoutIter();
    mBodyParts[4]->initWithoutIter();

    MR::moveCoordToNearestPos(this, mPosition);
    MR::moveTransToOtherActorRailPos(mBodyParts[0], this);
    MR::reverseRailDirection(this);

    MR::moveCoord(this, mBodyParts[0]->getSensor("body")->getRadius() / 2.0f + mBodyParts[1]->getSensor("body")->getRadius() / 2.0f);
    MR::moveTransToOtherActorRailPos(mBodyParts[1], this);
    MR::moveCoord(this, mBodyParts[1]->getSensor("body")->getRadius() / 2.0f + mBodyParts[2]->getSensor("body")->getRadius() / 2.0f);
    MR::moveTransToOtherActorRailPos(mBodyParts[2], this);
    MR::moveCoord(this, mBodyParts[2]->getSensor("body")->getRadius() / 2.0f + mBodyParts[3]->getSensor("body")->getRadius() / 2.0f);
    MR::moveTransToOtherActorRailPos(mBodyParts[3], this);
    MR::moveCoord(this, mBodyParts[3]->getSensor("body")->getRadius() / 2.0f + mBodyParts[4]->getSensor("body")->getRadius() / 2.0f);
    MR::moveTransToOtherActorRailPos(mBodyParts[4], this);
    MR::reverseRailDirection(this);

    mBodyParts[0]->appear();
    mBodyParts[1]->appear();
    mBodyParts[2]->appear();
    mBodyParts[3]->appear();
    mBodyParts[4]->appear();
}

void Hanachan::initAfterPlacement() {
    MR::trySetMoveLimitCollision(mBodyParts[0]);
}

void Hanachan::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void Hanachan::startClipped() {
    LiveActor::startClipped();

    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        (*current)->startClipped();
    }
}

void Hanachan::endClipped() {
    LiveActor::endClipped();

    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        (*current)->endClipped();
    }
}

void Hanachan::control() {
    mScaleController->updateNerve();

    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        if (!MR::isDead(*current)) {
            (*current)->movement();
            (*current)->mPushVelocity.zero();
        }
    }

    if ((isNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance) || isNerve(&NrvHanachan::HanachanNrvHanachanAngryPursue::sInstance) ||
         isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance)) &&
        isStarPointerPointing()) {
        if (isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance)) {
            setNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindOverturn::sInstance);
        } else {
            setNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBind::sInstance);
        }
    }

    mPosition = (mBodyParts[2]->mPosition);

    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance)) {
            MR::validateClipping(this);
        } else {
            MR::invalidateClipping(this);
        }
    }
}

void Hanachan::exeWalk() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvWalk::sInstance);
    }

    moveHeadAlongRail(::hWalkSpeed);
    moveBodyAlongHead();
    applyPlayerHipDropReaction();
}

void Hanachan::exeTrample() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvTrample::sInstance);
        setDelayAllPartsAtId(calcNearestInfectionId(), ::hTrampleInit, ::hTrampleInterval);
        MR::startSound(this, "SE_EV_HANACHAN_TRAMPLE");
    }

    moveBodyAlongHead();

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvTrample::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvTrample::sInstance)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanBecomeAngry::sInstance);
    }
}

void Hanachan::exeBecomeAngry() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvBecomeAngry::sInstance);
        setDelayAllPartsAtId(0, 0, ::hBecomeAngryInterval);
    }

    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_STEAM");
    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_PRE_ANGRY");
    moveBodyAlongHead();

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvBecomeAngry::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvBecomeAngry::sInstance)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanAngryPursue::sInstance);
    }
}

void Hanachan::exeAngryPursue() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvAngryPursue::sInstance);
    }

    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_WHISTLE");
    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_MOVE_ANGRY");

    if (mIsChasePlayer) {
        moveHeadToPlayer(::hAngrySpeed, 0.08f);
    } else {
        moveHeadAlongRail(::hAngrySpeed);
    }

    moveBodyAlongHead();
    applyPlayerHipDropReaction();

    if (MR::isGreaterStep(this, ::hAngryPursueTime)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanAngryEnd::sInstance);
    }
}

void Hanachan::exeAngryEnd() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvAngryEnd::sInstance);
        setDelayAllPartsAtId(4, 0, 0);
        MR::startSound(this, "SE_EV_HANACHAN_COOLDOWN");
    }

    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_BRAKE");

    f32 speed = 1.0f - getNerveStep() / 60.0f;
    speed *= ::hAngrySpeed;
    mBodyParts[0]->mRotation.getZDir(mFrontDir);
    mBodyParts[0]->mPushVelocity += mFrontDir * speed;

    HanachanParts* pHead = mBodyParts[0];
    MR::blendQuatUpFront(&pHead->mRotation, -pHead->mGravity, mFrontDir, 0.5f, 0.5f);

    moveBodyAlongHead();
    applyPlayerHipDropReaction();

    if (MR::isGreaterStep(this, 60)) {
        if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvAngryEnd::sInstance) &&
            mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvAngryEnd::sInstance)) {
            setNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance);
        }
    }
}

void Hanachan::exeWallHitEnd() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EV_HANACHAN_COOLDOWN");
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvWallHitEnd::sInstance);

        if (isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEnd::sInstance) ||
            isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEndOverturn::sInstance)) {
            setDelayAllPartsAtId(calcNearestInfectionId(), 0, ::hWallHitInterval);
        } else if (isNerve(&NrvHanachan::HanachanNrvHanachanWallHitEnd::sInstance)) {
            setDelayAllPartsAtId(0, 0, ::hWallHitInterval);
            MR::emitEffect(mBodyParts[0], "WallHit");
        }
    }

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvWallHitEnd::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvWallHitEnd::sInstance)) {
        if (isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEndOverturn::sInstance)) {
            setNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance);
        } else {
            setNerve(&NrvHanachan::HanachanNrvHanachanWalk::sInstance);
        }
    }
}

void Hanachan::exeOverturn() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvOverturn::sInstance);
        setDelayAllPartsAtId(calcNearestInfectionId(), ::hSpinInit, ::hSpinInterval);
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EV_HANACHAN_OVERTURN");
    }

    if (MR::isStep(this, ::hOverturnBoundSeTime)) {
        MR::startSound(this, "SE_EM_HANACHAN_BOUND");
    }

    moveBodyAlongHead();

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance);
    }
}

void Hanachan::exeOverturnWait() {
    if (MR::isGreaterStep(this, ::hOverturnTime)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanRecover::sInstance);
    } else {
        MR::startLevelSound(this, "SE_EM_LV_HANACHAN_SWOON");
        applyPlayerHipDropReaction();
        moveBodyAlongHead();
    }
}

void Hanachan::exeOverturnBound() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvOverturnBound::sInstance);
        setDelayAllPartsAtId(calcNearestInfectionId(), ::hOverturnBoundInit, ::hOverturnBoundInterval);
        MR::startBlowHitSound(this);
    }

    if (MR::isStep(this, ::hOverturnBoundSeTime)) {
        MR::startSound(this, "SE_EM_HANACHAN_BOUND");
    }

    moveBodyAlongHead();

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvOverturnWait::sInstance)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance);
    }
}

void Hanachan::exeRecover() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvRecover::sInstance);
        setDelayAllPartsAtId(0, ::hRecoverInit, ::hRecoverInterval);
    }

    MR::startLevelSound(this, "SE_EM_LV_HANACHAN_PRE_ANGRY");

    if (MR::isGreaterEqualStep(this, ::hRecoverSteamSeTime)) {
        MR::startLevelSound(this, "SE_EM_LV_HANACHAN_STEAM");
    }

    if (MR::isStep(this, ::hRecoverGetupSeTime)) {
        MR::startSound(this, "SE_EM_HANACHAN_GETUP");
        MR::startSound(this, "SE_EV_HANACHAN_ANGRY");
    }

    moveBodyAlongHead();

    if (mBodyParts[0]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvRecover::sInstance) &&
        mBodyParts[4]->isLandedInNerve(&NrvHanachan::HanachanPartsNrvRecover::sInstance)) {
        setNerve(&NrvHanachan::HanachanNrvHanachanAngryPursue::sInstance);
    }
}

void Hanachan::exeHipDropped() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvHanachan::HanachanNrvHanachanOverturnHipDropped::sInstance)) {
            setNerveAllParts(&NrvHanachan::HanachanPartsNrvOverturnHipDropped::sInstance);
        } else {
            setNerveAllParts(&NrvHanachan::HanachanPartsNrvHipDropped::sInstance);
        }

        setDelayAllPartsAtId(calcNearestInfectionId(), ::hHipDroppedInit, ::hHipDroppedInterval);
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::startSound(this, "SE_EV_HANACHAN_DEAD");
        mScaleController->startHitReaction();
    }

    if (MR::isGreaterStep(this, ::hHipDroppedTime)) {
        if (mBodyParts[0]->isHipDroppedLanded() && mBodyParts[4]->isHipDroppedLanded()) {
            for (s32 i = 0; i < ARRAY_SIZE(mBodyParts); i++) {
                mBodyParts[i]->kill();
            }

            kill();
        }
    }
}

void Hanachan::exeBlow() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvBlow::sInstance);
        MR::startBlowHitSound(this);
    }

    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        if (!MR::isDead(*current)) {
            return;
        }
    }

    kill();
}

void Hanachan::exeStarPointerBind() {
    if (MR::isFirstStep(this)) {
        setNerveAllParts(&NrvHanachan::HanachanPartsNrvStarPointerBind::sInstance);
        mScaleController->startDpdHitVibration();
    }

    if (MR::isGreaterStep(this, ::hBindMinStep) && !isStarPointerPointing()) {
        if (isNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindOverturn::sInstance)) {
            setNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEndOverturn::sInstance);
        } else {
            setNerve(&NrvHanachan::HanachanNrvHanachanStarPointerBindEnd::sInstance);
        }

        mAttackPos = *MR::getStarPointerWorldPosUsingDepth(0);
    }
}

inline void Hanachan::endStarPointerBind() {
    mScaleController->startAnim();
}

s32 Hanachan::calcNearestInfectionId() {
    f32 nearestDist = 9999999.0f;
    s32 nearestId = -1;

    for (s32 i = 0; i < ARRAY_SIZE(mBodyParts); i++) {
        f32 dist = mBodyParts[i]->getSensor("body")->mPosition.squared(mAttackPos);

        if (dist < nearestDist) {
            nearestDist = dist;
            nearestId = i;
        }
    }

    if (nearestId == -1) {
        return 0;
    }

    return nearestId;
}

bool Hanachan::isOwnSensor(HitSensor* pSensor) {
    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        if ((*current)->getSensor("body") == pSensor) {
            return true;
        }
    }

    return false;
}

// NON_MATCHING
// decomp.me: https://decomp.me/scratch/5SUWO
void Hanachan::setNerveBlow(const TVec3f& rPos) {
    TVec3f dir = mPosition;
    dir -= rPos;
    MR::vecKillElement(dir, mGravity, &dir);
    MR::normalizeOrZero(&dir);

    TVec3f side;
    side.cross(dir, mGravity);

    f32 angle = ::hInitBlowRadian;
    f32 numSegments = ARRAY_SIZE(mBodyParts) - 1;
    f32 angleStep = (PI - ::hInitBlowRadian * 2.0f) / numSegments;
    TVec3f blow;
    TVec3f up;

    TVec3f headDir = mBodyParts[0]->mPosition;
    headDir -= rPos;

    if (headDir.dot(side) < 0.0f) {
        angleStep = -angleStep;
        angle = PI - angle;
    }

    for (HanachanParts** current = mBodyParts; current < &mBodyParts[ARRAY_SIZE(mBodyParts)]; current++) {
        blow = side * MR::cos(angle);
        blow += dir * MR::sin(angle);
        blow.setLength(::hBlowVelH);

        up = (*current)->mGravity * -1.0f;
        up *= ::hBlowVelV;
        (*current)->mPushVelocity = blow + up;

        angle += angleStep;
    }

    setNerve(&NrvHanachan::HanachanNrvHanachanBlow::sInstance);
}

void Hanachan::applyPlayerHipDropReaction() {
    if (MR::isPlayerHipDropLand()) {
        mAttackPos = *MR::getPlayerCenterPos();

        f32 nearestDist = 999999.0f;

        for (s32 i = 0; i < ARRAY_SIZE(mBodyParts); i++) {
            f32 dist = MR::calcDistanceToPlayer(mBodyParts[i]);

            if (dist < nearestDist) {
                nearestDist = dist;
            }
        }

        if (::hNearHipDropReactionDist < nearestDist) {
            return;
        }

        if (isNerve(&NrvHanachan::HanachanNrvHanachanOverturnWait::sInstance)) {
            setNerve(&NrvHanachan::HanachanNrvHanachanOverturnBound::sInstance);
        } else {
            setNerve(&NrvHanachan::HanachanNrvHanachanOverturn::sInstance);
        }
    }
}

void Hanachan::setNerveAllParts(const Nerve* pNerve) {
    for (s32 i = 0; i < ARRAY_SIZE(mBodyParts); i++) {
        mBodyParts[i]->setNerve(pNerve);
        mBodyParts[i]->mIsLanded = false;
    }
}

void Hanachan::setDelayAllPartsAtId(s32 id, s32 delay, s32 step) {
    s32 i;
    s32 currentDelay = delay;

    for (i = id; i >= 0; i--) {
        mBodyParts[i]->mActionStartStep = currentDelay;
        currentDelay += step;
    }

    s32 nextDelay = delay + step;

    for (i = id + 1; i < ARRAY_SIZE(mBodyParts); i++) {
        mBodyParts[i]->mActionStartStep = nextDelay;
        nextDelay += step;
    }
}

bool Hanachan::isStarPointerPointing() {
    for (s32 i = 0; i < ARRAY_SIZE(mBodyParts); i++) {
        if (MR::isStarPointerPointing2POnPressButton(mBodyParts[i], "弱", true, false)) {
            return true;
        }
    }

    return false;
}

void Hanachan::moveHeadAlongRail(f32 speed) {
    mBodyParts[0]->mRotation.getZDir(mFrontDir);

    TVec3f railDir;

    if (::hFarDistToPursuePlayer < MR::calcMovingDirectionAlongRail(this, &railDir, mBodyParts[0]->mPosition, 800.0f, false, nullptr)) {
        moveHeadToPlayer(speed, 0.04f);
    } else {
        MR::vecKillElement(railDir, mBodyParts[0]->mGravity, &railDir);
        MR::normalizeOrZero(&railDir);

        if (!MR::isNearZero(railDir)) {
            MR::turnVecToVecRadian(&mFrontDir, mFrontDir, railDir, 0.08f, mBodyParts[0]->mGravity);
        }

        mBodyParts[0]->mPushVelocity += mFrontDir * speed;

        HanachanParts* pHead = mBodyParts[0];
        MR::blendQuatUpFront(&pHead->mRotation, -pHead->mGravity, mFrontDir, 0.5f, 0.5f);
    }
}

void Hanachan::moveHeadToPlayer(f32 speed, f32 turnSpeed) {
    mBodyParts[0]->mRotation.getZDir(mFrontDir);

    TVec3f playerDir;
    MR::calcVecToPlayerH(&playerDir, mBodyParts[0], nullptr);
    MR::turnVecToVecRadian(&mFrontDir, mFrontDir, playerDir, turnSpeed, mBodyParts[0]->mGravity);

    mBodyParts[0]->mPushVelocity += mFrontDir * speed;

    HanachanParts* pHead = mBodyParts[0];
    MR::blendQuatUpFront(&pHead->mRotation, -pHead->mGravity, mFrontDir, 0.5f, 0.5f);
}

void Hanachan::moveBodyAlongHead() {
    TVec3f* grav;
    for (s32 i = 1; i < ARRAY_SIZE(mBodyParts); i++) {
        TVec3f toPrev = mBodyParts[i - 1]->getSensor("body")->mPosition;
        toPrev -= mBodyParts[i]->getSensor("body")->mPosition;

        TVec3f push = -toPrev;
        push.setLength(::hCollideRange + mBodyParts[i - 1]->getSensor("body")->getRadius() / 2.0f +
                       mBodyParts[i]->getSensor("body")->getRadius() / 2.0f);

        TVec3f total = toPrev;
        total += push;
        mBodyParts[i]->mPushVelocity += total;

        grav = &mBodyParts[i]->mGravity;
        HitSensor* pTarget = mBodyParts[i - 1]->getSensor("body");
        HitSensor* pSensor = mBodyParts[i]->getSensor("body");
        TQuat4f* pRotation = &mBodyParts[i]->mRotation;

        MR::blendQuatUpFront(pRotation, -*grav, calcSensorDirection(pSensor, pTarget), 0.3f, 0.5f);
    }
}
