#include "Game/Boss/TripodBossGuardWall.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    struct WallPartPlacementAngle {
        /* 0x00 */ f32 mAngle;
        /* 0x04 */ s32 mStartTiming;
    };

    static WallPartPlacementAngle sWallPartPlacementAngleTable[] = {{0.0f, 4},    {45.0f, 8},   {90.0f, 12},  {135.0f, 16},
                                                                    {180.0f, 20}, {225.0f, 24}, {270.0f, 28}, {315.0f, 32}};
    static const s32 sEventCameraStartTime = 150;
    static const s32 sEventAnimeStartTime = 180;
    static const s32 sStartCameraBlendTime = 20;
    static const s32 sEndCameraBlendTime = 30;
    static const f32 sRotateSpeed = -0.2f;
    static s32 sMoveSeLength = 58;
};  // namespace

void TripodBossGuardWall_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace NrvTriPodBossGuardWall {
    NEW_NERVE(TripodBossGuardWallNrvWait, TripodBossGuardWall, Wait);
    NEW_NERVE(TripodBossGuardWallNrvTryDemo, TripodBossGuardWall, TryDemo);
    NEW_NERVE(TripodBossGuardWallNrvDemo, TripodBossGuardWall, Demo);
    NEW_NERVE(TripodBossGuardWallNrvRotate, TripodBossGuardWall, Rotate);
};  // namespace NrvTriPodBossGuardWall

TripodBossGuardWall::TripodBossGuardWall(const char* pName) : LiveActor(pName), mCameraTargetMtx(), mCameraInfo(), mStartTrans(0.0f, 0.0f, 0.0f) {
    mBaseMtx.identity();
}

void TripodBossGuardWall::makeActorAppeared() {
    LiveActor::makeActorAppeared();
}

void TripodBossGuardWall::makeActorDead() {
    for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
        mParts[i].makeActorDead();
    }

    LiveActor::makeActorDead();
}

void TripodBossGuardWall::kill() {
    for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
        mParts[i].kill();
    }

    LiveActor::kill();
}

void TripodBossGuardWall::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjDecorationMovement(this);
    mStartTrans = mPosition;
    initModelManagerWithAnm("TripodBossGuardWall", nullptr, false);
    MR::getJMapInfoArg0NoInit(rIter, &mJointID);
    initSound(4, false);
    initNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvWait::sInstance);
    mCameraInfo = MR::createActorCameraInfo(rIter);
    MR::initAnimCamera(this, mCameraInfo, "2ndDemo");
    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &TripodBossGuardWall::requestStart));
    }

    MR::invalidateClipping(this);
    initParts();
    updateMatrix();
    MR::addTripodBossParts(this);
    makeActorDead();
}

void TripodBossGuardWall::initParts() {
    for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
        mParts[i].setHostMatrix(&mBaseMtx);
        mParts[i].setPlacementAngle(::sWallPartPlacementAngleTable[i].mAngle);
        mParts[i].setStartTiming(::sWallPartPlacementAngleTable[i].mStartTiming);
        mParts[i].initWithoutIter();
    }
}

void TripodBossGuardWall::control() {
    updateMatrix();
}

void TripodBossGuardWall::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}

bool TripodBossGuardWall::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_TRIPODBOSS_STARTED) {
        for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
            mParts[i].makeActorAppeared();
        }

        return true;
    }

    return false;
}

void TripodBossGuardWall::requestStart() {
    setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvTryDemo::sInstance);

    if (MR::tryDamageDemoTripodBoss()) {
        setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvDemo::sInstance);
    }
}

void TripodBossGuardWall::exeWait() {
}

void TripodBossGuardWall::exeTryDemo() {
    if (MR::tryDamageDemoTripodBoss()) {
        setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvDemo::sInstance);
    }
}

void TripodBossGuardWall::exeDemo() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isStep(this, ::sEventCameraStartTime)) {
        updateCameraTarget();
        CameraTargetArg arg(nullptr, mCameraTargetMtx, nullptr, nullptr);
        MR::startAnimCameraTargetOther(this, mCameraInfo, "2ndDemo", arg, ::sStartCameraBlendTime, 1.0f);
    }

    if (MR::isStep(this, ::sEventAnimeStartTime)) {
        for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
            mParts[i].requestStartDemo();
        }
    }

    if (MR::isGreaterStep(this, ::sEventAnimeStartTime)) {
        bool isAnyPartDemoEnd = true;

        for (s32 i = 0; i < ARRAY_SIZE(mParts); i++) {
            if (!mParts[i].isEndDemo()) {
                isAnyPartDemoEnd = false;
                break;
            }
        }

        if (MR::isLessStep(this, ::sMoveSeLength + ::sEventAnimeStartTime)) {
            MR::startLevelSound(this, "SE_BM_LV_TRIPOD_WALL_UP");
        }

        if (isAnyPartDemoEnd) {
            MR::endAnimCamera(this, mCameraInfo, "2ndDemo", ::sEndCameraBlendTime, true);
            MR::requestEndDamageDemoTripodBoss();
            setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvRotate::sInstance);
        }
    }
}

void TripodBossGuardWall::exeRotate() {
    mRotation.y += ::sRotateSpeed;

    MR::repeatDegree(&mRotation.y);
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_C_WALL_MOVE");

    if (MR::isEndBreakDownDemoTripodBoss()) {
        kill();
    }
}

void TripodBossGuardWall::updateMatrix() {
    MR::makeMtxTR(mBaseMtx, mStartTrans, mRotation);
    MR::concatTripodBossAttachJointMatrix(&mBaseMtx, mJointID);
    mBaseMtx.getTrans(mPosition);
}

void TripodBossGuardWall::updateCameraTarget() {
    TVec3f front;
    TVec3f up;
    TVec3f fromPlayer = mPosition - *MR::getPlayerPos();
    front.x = fromPlayer.x;
    front.y = fromPlayer.y;
    front.z = fromPlayer.z;

    mBaseMtx.getYDir(up);
    front.orthogonalize(up);

    if (MR::isNearZero(front)) {
        mBaseMtx.getZDir(front);
    } else {
        MR::normalize(&front);
    }

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxUpFrontPos(&mtx, up, front, mPosition);
    mCameraTargetMtx->mMatrix.set(mtx);
}
