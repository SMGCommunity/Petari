#include "Game/Boss/TripodBossGuardWall.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

struct WallPart2Angle {
    f32 angle;
    s32 partNo;
};

namespace {
    static WallPart2Angle sWallPartPlacementAngleTable[8] = {
        { 0.0f, 4 },
        { 45.0f, 8 },
        { 90.0f, 0xC },
        { 135.0f, 0x10 },
        { 180.0f, 0x14 },
        { 225.0f, 0x18 },
        { 270.0f, 0x1C },
        { 315.0f, 0x20 }
    };

    static s32 sMoveSeLength = 0x3A;
};

namespace NrvTriPodBossGuardWall {
    NEW_NERVE(TripodBossGuardWallNrvWait, TripodBossGuardWall, Wait);
    NEW_NERVE(TripodBossGuardWallNrvTryDemo, TripodBossGuardWall, TryDemo);
    NEW_NERVE(TripodBossGuardWallNrvDemo, TripodBossGuardWall, Demo);
    NEW_NERVE(TripodBossGuardWallNrvRotate, TripodBossGuardWall, Rotate);
};

TripodBossGuardWall::TripodBossGuardWall(const char *pName) : LiveActor(pName),
    mCameraTargetMtx(nullptr) {
    mCameraInfo = nullptr;
    _580.x = 0.0f;
    _580.y = 0.0f;
    _580.z = 0.0f;
    mBaseMtx.identity();
}

TripodBossGuardWallPart::~TripodBossGuardWallPart() {
    
}

void TripodBossGuardWall::makeActorAppeared() {
    LiveActor::makeActorAppeared();
}

void TripodBossGuardWall::makeActorDead() {
    for (s32 i = 0; i < 8; i++) {
        mWallParts[i].makeActorDead();
    }

    LiveActor::makeActorDead();
}

void TripodBossGuardWall::kill() {
    for (s32 i = 0; i < 8; i++) {
        mWallParts[i].kill();
    }

    LiveActor::kill();
}

void TripodBossGuardWall::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjDecorationMovement(this);
    _580.setPS(mPosition);
    initModelManagerWithAnm("TripodBossGuardWall", nullptr, false);
    MR::getJMapInfoArg0NoInit(rIter, &_58C);
    initSound(4, false);
    initNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvWait::sInstance);
    mCameraInfo = MR::createActorCameraInfo(rIter);
    MR::initAnimCamera(this, mCameraInfo, "2ndDemo");
    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::FunctorV0M<TripodBossGuardWall *, void (TripodBossGuardWall::*)()> validateFunc = MR::Functor_Inline<TripodBossGuardWall>(this, &TripodBossGuardWall::requestStart);
        MR::listenStageSwitchOnAppear(this, validateFunc);
    }

    MR::invalidateClipping(this);
    initParts();
    updateMatrix();
    MR::addTripodBossParts(this);
    makeActorDead();
}

void TripodBossGuardWall::initParts() {
    for (s32 i = 0; i < 8; i++) {
        mWallParts[i].setHostMatrix(&mBaseMtx);
        mWallParts[i].setPlacementAngle(sWallPartPlacementAngleTable[i].angle);
        mWallParts[i].setStartTiminig(sWallPartPlacementAngleTable[i].partNo);
        mWallParts[i].initWithoutIter();
    }
}

void TripodBossGuardWall::control() {
    updateMatrix();
}

void TripodBossGuardWall::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}

bool TripodBossGuardWall::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 170) {
        for (s32 i = 0; i < 8; i++) {
            mWallParts[i].makeActorAppeared();
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

/* more small inlining issues */
void TripodBossGuardWall::exeDemo() {
    MR::isFirstStep(this);

    if (MR::isStep(this, 150)) {
        updateCameraTarget();
        CameraTargetArg arg(nullptr, mCameraTargetMtx, nullptr, nullptr);
        MR::startAnimCameraTargetOther(this, mCameraInfo, "2ndDemo", arg, 20, 1.0f);
    }

    if (MR::isStep(this, 180)) {
        for (s32 i = 0; i < 8; i++) {
            mWallParts[i].requestStartDemo();
        }
    }

    if (MR::isGreaterStep(this, 180)) {
        bool isDemoEndAny = true;
        for (s32 i = 0; i < 8; i++) {
            if (mWallParts[i].isEndDemo()) {
                isDemoEndAny = false;
            }
        }

        if (MR::isLessStep(this, sMoveSeLength + 0xB4)) {
            MR::startLevelSound(this, "SE_BM_LV_TRIPOD_WALL_UP", -1, -1, -1);
        }

        if (isDemoEndAny) {
            MR::endAnimCamera(this, mCameraInfo, "2ndDemo", 30, true);
            MR::requestEndDamageDemoTripodBoss();
            setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvRotate::sInstance);
        }
    }
}

void TripodBossGuardWall::exeRotate() {
    mRotation.y += -0.2f;
    mRotation.y = MR::wrapAngleTowards(0.0f, mRotation.y);
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_C_WALL_MOVE", -1, -1, -1);
    if (MR::isEndBreakDownDemoTripodBoss()) {
        kill();
    }
}

void TripodBossGuardWall::exeTryDemo() {
    if (MR::tryDamageDemoTripodBoss()) {
        setNerve(&NrvTriPodBossGuardWall::TripodBossGuardWallNrvDemo::sInstance);
    }
}

void TripodBossGuardWall::updateMatrix() {
    MR::makeMtxTR(mBaseMtx, _580, mRotation);
    MR::concatTripodBossAttachJointMatrix(&mBaseMtx, _58C);
    f32 z = mBaseMtx.mMtx[2][3];
    f32 y = mBaseMtx.mMtx[1][3];
    f32 x = mBaseMtx.mMtx[0][3];
    mPosition.set<f32>(x, y, z);
}

void TripodBossGuardWall::updateCameraTarget() {
    TVec3f front;
    TVec3f up;
    TVec3f blah;
    JMathInlineVEC::PSVECSubtract(MR::getPlayerPos(), &mPosition, &blah);
    front.x = blah.x;
    front.y = blah.y;
    front.z = blah.z;
    
    f32 z = mBaseMtx.mMtx[2][1];
    f32 y = mBaseMtx.mMtx[1][1];
    f32 x = mBaseMtx.mMtx[0][1];
    up.set<f32>(x, y, z);
    f32 v9 = up.dot(front);
    JMAVECScaleAdd(&up, &front, &front, -v9);

    if (MR::isNearZero(front, 0.001f)) {
        f32 z = mBaseMtx.mMtx[2][2];
        f32 y = mBaseMtx.mMtx[1][2];
        f32 x = mBaseMtx.mMtx[0][2];
        front.set<f32>(x, y, z);
    }
    else {
        MR::normalize(&front);
    }

    TPos3f pos;
    pos.identity();
    MR::makeMtxUpFrontPos(&pos, up, front, mPosition);
    mCameraTargetMtx->mMatrix.setInline(pos);
}

MtxPtr TripodBossGuardWall::getBaseMtx() const {
    return (MtxPtr)mBaseMtx.mMtx;
}
