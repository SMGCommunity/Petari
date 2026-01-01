#include "Game/Ride/Plant.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Ride/PlantLeaf.hpp"
#include "Game/Ride/PlantStalk.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphBase/J3DShapeDraw.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/gx/GXTransform.h>
#include <revolution/mtx.h>
#include <revolution/types.h>
#include <revolution/wpad.h>

namespace {
    static const f32 hSin1 = 0.017453292f;  // sin(1.0f);
}

namespace NrvPlant {
    NEW_NERVE(PlantNrvWaitFar, Plant, WaitFar);
    NEW_NERVE(PlantNrvSeedWait, Plant, SeedWait);
    NEW_NERVE(PlantNrvWaitDemoWaitGrowUp, Plant, WaitDemoWaitGrowUp);
    NEW_NERVE(PlantNrvDemoWaitGrowUp, Plant, DemoWaitGrowUp);
    NEW_NERVE(PlantNrvGrowUp, Plant, GrowUp);
    NEW_NERVE(PlantNrvHangWaitGrowUp, Plant, HangWaitGrowUp);
    NEW_NERVE(PlantNrvHangUpGrowUp, Plant, HangUpGrowUp);
    NEW_NERVE(PlantNrvGrowthStop, Plant, GrowthStop);
    NEW_NERVE(PlantNrvGrowthWait, Plant, GrowthWait);
    NEW_NERVE(PlantNrvGrowthWaitInvalid, Plant, GrowthWaitInvalid);
    NEW_NERVE(PlantNrvHangStart, Plant, HangStart);
    NEW_NERVE(PlantNrvHangUp, Plant, HangUp);
    NEW_NERVE(PlantNrvHangDown, Plant, HangDown);

};  // namespace NrvPlant

Plant::Plant(const char* pName)
    : LiveActor(pName), mSeedPartsModel(nullptr), mStalk(nullptr), mTopPartsModel(nullptr), mNumLeaves(0), mLeaves(nullptr), mShapeDraw(nullptr),
      mRider(nullptr), mRailCoord(0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mRideVelocity(0.0f), mAccelTimer(0), mLaunchSpeed(30.0f),
      mLaunchNormal(0.0f), mClippingCenter(0.0f, 0.0f, 0.0f), mCameraInfo(nullptr), mPlayAppearDemo(false), mGrabbedTop(false) {
    mSeedMtx.identity();
    mTopMtx.identity();
}

void Plant::init(const JMapInfoIter& pMapInfoIter) {
    MR::createSceneObj(SceneObj_PlantStalkDrawInit);
    MR::createSceneObj(SceneObj_PlantLeafDrawInit);

    mShapeDraw = ((PlantLeafDrawInit*)MR::getSceneObjHolder()->getObj(SceneObj_PlantLeafDrawInit))->mShapeDraw;

    MR::connectToScene(this, 0x29, 7, -1, 5);

    MR::getJMapInfoArg0NoInit(pMapInfoIter, &mLaunchSpeed);
    MR::getJMapInfoArg1NoInit(pMapInfoIter, &mLaunchNormal);
    MR::getJMapInfoArg2NoInit(pMapInfoIter, &mPlayAppearDemo);

    initSound(8, false);
    initNerve(&NrvPlant::PlantNrvWaitFar::sInstance);

    initRailRider(pMapInfoIter);
    MR::initAndSetRailClipping(&mClippingCenter, this, 100.0f, 500.0f);
    MR::calcRailStartPointPos(&mPosition, this);

    mStalk = new PlantStalk();
    mStalk->init(pMapInfoIter);

    mSeedPartsModel = new PartsModel(this, "種（伸び植物）", "PlantSeed", mSeedMtx, -1, false);
    mSeedPartsModel->initWithoutIter();
    MR::initShadowVolumeSphere(mSeedPartsModel, 50.0f);
    mSeedPartsModel->mPosition.set(mPosition);
    mSeedMtx.setTrans(mPosition);

    updateTopMtx();
    mTopPartsModel = new PartsModel(this, "先端（伸び植物）", "PlantTop", mTopMtx, -1, false);
    mTopPartsModel->initWithoutIter();
    mTopPartsModel->kill();

    initLeaf();
    initHitSensor(3);
    MR::addHitSensorCallbackBinder(this, "bind", 5, 100.0f);
    MR::addHitSensorMapObj(this, "seed", 5, 300.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::addHitSensorRide(this, "Ride", 8, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));

    MR::initMultiActorCamera(this, pMapInfoIter, &mCameraInfo, "出現デモ");
    MR::initMultiActorCamera(this, pMapInfoIter, &mCameraInfo, "掴まり");

    if (MR::useStageSwitchReadAppear(this, pMapInfoIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
        MR::invalidateShadow(mSeedPartsModel, 0);
    } else {
        makeActorAppeared();
    }
}

void Plant::initAfterPlacement() {
    MR::makeMtxOnMapCollision(&mSeedMtx, mSeedPartsModel, 1000.0f);
}

void Plant::appear() {
    LiveActor::appear();
    mTopPartsModel->appear();
    MR::validateShadow(mSeedPartsModel, 0);
    MR::invalidateClipping(this);
    startGrowUp();
}

void Plant::exeWaitFar() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mSeedPartsModel, "Wait", 0);
    }

    if (MR::isNear(this, *MR::getPlayerPos(), 700.0f)) {
        setNerve(&NrvPlant::PlantNrvSeedWait::sInstance);
    }
}

void Plant::exeSeedWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mSeedPartsModel, "Bud", 0);
        MR::startSound(this, "SE_OJ_PLANT_BUD", -1, -1);
        mTopPartsModel->kill();
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
}

void Plant::exeWaitDemoWaitGrowUp() {
    if (MR::tryStartDemoWithoutCinemaFrame(this, "伸び植物（成長）")) {
        MR::startMultiActorCameraTargetOther(this, mCameraInfo, "出現デモ", CameraTargetArg(mTopPartsModel), -1);
        MR::requestMovementOn(mSeedPartsModel);
        setNerve(&NrvPlant::PlantNrvDemoWaitGrowUp::sInstance);
    }
}

void Plant::exeDemoWaitGrowUp() {
    if (MR::isStep(this, 60)) {
        MR::startBck(mSeedPartsModel, "Bud", 0);
        MR::startSound(this, "SE_OJ_PLANT_BUD", -1, -1);
    }

    if (MR::isStep(this, 90)) {
        setNerve(&NrvPlant::PlantNrvGrowUp::sInstance);
    }
}

void Plant::exeGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mSeedPartsModel, "GrowUp", 0);
        MR::startSound(this, "SE_OJ_PLANT_SEED_BREAK", -1, -1);
        MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
        MR::startSound(this, "SE_OJ_PLANT_GROW_START", -1, -1);
    }

    // interesting...
    if (updateGrowUp()) {
        return;
    }
}

void Plant::exeHangWaitGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", (const char*)0);
    }

    if (updateGrowUp()) {
        return;
    }

    f32 growthPercent = mStalk->mGrowthPercent;
    mStalk->calcPosAndAxisY(&mPosition, &mUp, mRailCoord * growthPercent);

    if (MR::isPadSwing(WPAD_CHAN0) && mRailCoord < MR::getRailTotalLength(this)) {
        setNerve(&NrvPlant::PlantNrvHangUpGrowUp::sInstance);
    }
}

void Plant::exeHangUpGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", (const char*)0);
        mAccelTimer = 0;
        mRideVelocity = 10.0f;
    }

    if (updateGrowUp()) {
        return;
    }

    mAccelTimer--;
    tryAccelHangUp();
    mRideVelocity -= 0.3f;

    if (mRideVelocity <= 0.0f) {
        mRideVelocity = 0.0f;
        MR::startBckPlayer("GrowPlantHangDown", (const char*)0);
        setNerve(&NrvPlant::PlantNrvHangWaitGrowUp::sInstance);
        return;
    }

    mRailCoord += mRideVelocity;
    MR::clampMax(&mRailCoord, MR::getRailTotalLength(this));
    f32 growthPercent = mStalk->mGrowthPercent;
    mStalk->calcPosAndAxisY(&mPosition, &mUp, mRailCoord * growthPercent);

    f32 f1 = 1.3f * mRideVelocity;
    f32 z = mFront.z;

    f1 = f1 >= 30.0f ? 30.0f : f1;
    MR::rotateVecDegree(&mFront, mUp, f1);

    if (z < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_UP", -1, -1);
    }

    updateBindLeaf();
    if (mRailCoord >= MR::getRailTotalLength(this)) {
        mRailCoord = MR::getRailTotalLength(this);
        mRideVelocity = 0.0f;
        setNerve(&NrvPlant::PlantNrvHangWaitGrowUp::sInstance);
    }
}

void Plant::exeGrowthStop() {}

void Plant::exeGrowthWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    s32 numLeavesSprung = 0;
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        if (mLeaves[leaf]->updateSpring(1.0f)) {
            numLeavesSprung++;
        }
    }

    if (numLeavesSprung == mNumLeaves) {
        setNerve(&NrvPlant::PlantNrvGrowthStop::sInstance);
    }
}

void Plant::exeGrowthWaitInvalid() {
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        mLeaves[leaf]->updateSpring(1.0f);
    }

    if (MR::isStep(this, 60) || MR::isOnGroundPlayer()) {
        setNerve(&NrvPlant::PlantNrvGrowthWait::sInstance);
    }
}

void Plant::exeHangStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", (const char*)0);
    }

    if (!updateHangUp(15.0f) && MR::isBckStopped(mRider)) {
        setNerve(&NrvPlant::PlantNrvHangUp::sInstance);
    }
}

void Plant::exeHangUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", (const char*)0);
        MR::setRailDirectionToEnd(this);
        mAccelTimer = 0;
    }

    f32 f1 = mRideVelocity * 1.3f;
    f1 = f1 >= 30.0f ? 30.0f : f1;

    if (!updateHangUp(f1) && !tryReachGoal()) {
        mAccelTimer--;
        tryAccelHangUp();
    }
}

void Plant::exeHangDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangDown", (const char*)0);
        MR::setRailDirectionToStart(this);
    }

    if (tryHangUp()) {
        return;
    }

    mRideVelocity += 0.3f;
    mRideVelocity = MR::clamp(mRideVelocity, 0.0f, 20.0f);
    updateBindLeaf();

    if (MR::isRailReachedNearGoal(this, 10.0f)) {
        MR::endMultiActorCameraAtLanding(this, mCameraInfo, "掴まり", -1);
        MR::startBckPlayer("GrowPlantJump", (const char*)0);
        MR::endBindAndPlayerJump(this, TVec3f(0.0f, 0.0f, 0.0f), 0);
        mRider = nullptr;
        setNerve(&NrvPlant::PlantNrvGrowthWaitInvalid::sInstance);
        return;
    }

    MR::moveCoordAndFollowTrans(this, mRideVelocity);
    mUp.set(MR::getRailDirection(this));

    f32 f1 = mRideVelocity * 1.3f;
    f32 z = mFront.z;
    mUp.mult(-1);

    f1 = f1 >= 20.0f ? 20.0f : f1;
    MR::rotateVecDegree(&mFront, MR::getRailDirection(this), f1);

    if (z < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_DOWN", -1, -1);
    }
}

void Plant::initLeaf() {
    f32 leafPos, leafSize, leafRatio;

    mNumLeaves = (s32)(((MR::getRailTotalLength(this) - 100.0f) - 200.0f) / 200.0f) + 2;
    mLeaves = new PlantLeaf*[mNumLeaves];

    TRot3f mtx;
    mtx.identity();

    TVec3f baseRotate(0.0f, 0.0f, 1.0f);
    leafPos = MR::getRailTotalLength(this) - 100.0f;

    MR::getRailTotalLength(this);  // ?????

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        leafRatio = static_cast< f32 >(leaf) / static_cast< f32 >(mNumLeaves);
        TVec3f railDirection;
        MR::calcRailDirectionAtCoord(&railDirection, this, leafPos);

        TVec3f growDirection(baseRotate);
        f32 dot = growDirection.dot(railDirection);
        if (__fabsf(dot) > 0.7f) {
            if (dot > 0.0f) {
                TVec3f up(0.0f, 1.0f, 0.0f);
                PSVECCrossProduct(&up, &railDirection, &growDirection);
            } else {
                TVec3f up(0.0f, 1.0f, 0.0f);
                PSVECCrossProduct(&railDirection, &up, &growDirection);
            }

            MR::normalize(&growDirection);
        }

        leafSize = MR::getEaseOutValue(leafRatio, 1.0f, 1.5f, 1.0f);

        mLeaves[leaf] = new PlantLeaf(leafPos, mPosition, growDirection, leafSize);
        mLeaves[leaf]->initWithoutIter();
        leafPos -= MR::getInterpolateValue(leafRatio, 100.0f, 300.0f);

        // hey, "small angle approx" only works for SMALL ANGLES
        f32 rand = hSin1 * MR::getRandom(90.0f, 270.0f);
        mtx.setRotateInline(TVec3f(0.0f, 1.0f, 0.0f), rand);
        mtx.mult(baseRotate, baseRotate);
    }
}

inline void setMatrix(MtxPtr posMtx, MtxPtr camMtx, MtxPtr baseMtx) {
    PSMTXConcat(camMtx, baseMtx, posMtx);
}

void Plant::calcAnim() {
    // register alloc mismatch
    // https://decomp.me/scratch/A8p2y

    if (!MR::isValidCalcViewAndEntry(this) || isNerve(&NrvPlant::PlantNrvWaitFar::sInstance) || isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) {
        return;
    }
    // FIXME
    MtxPtr posMtx, camViewMtx, leafBaseMtx;
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        posMtx = mLeaves[leaf]->mPosMtx;
        camViewMtx = MR::getCameraViewMtx();
        leafBaseMtx = mLeaves[leaf]->getBaseMtx();
        PSMTXConcat(camViewMtx, leafBaseMtx, posMtx);
    }
}

void Plant::startClipped() {
    LiveActor::startClipped();
    mStalk->startClipped();
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        mLeaves[leaf]->startClipped();
    }
}

void Plant::endClipped() {
    LiveActor::endClipped();
    mStalk->endClipped();
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        mLeaves[leaf]->endClipped();
    }
}

void Plant::updateHitSensor(HitSensor* pSensor) {
    if (isNerve(&NrvPlant::PlantNrvWaitFar::sInstance) || isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) {
        pSensor->mPosition.set(mSeedPartsModel->mPosition);
    } else {
        mStalk->calcNearestPos(&pSensor->mPosition, *MR::getPlayerPos());
    }
}

void Plant::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) {
        f32 r = pReceiver->mRadius;
        if (MR::isNear(pSender, pReceiver, r + 50.0f)) {
            pSender->mRadius = 50.0f;
            MR::sendMsgPush(pReceiver, pSender);
            pSender->mRadius = 300.0f;
        }
    } else {
        if (mRider != nullptr) {
            MR::tryGetItem(pSender, pReceiver);
        }
    }
}

bool Plant::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) {
            MR::invalidateClipping(this);
            mTopPartsModel->appear();
            startGrowUp();
            return true;
        } else {
            return false;  // necessary to match
        }
    }

    return false;
}

bool Plant::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if ((isNerve(&NrvPlant::PlantNrvWaitFar::sInstance) || isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) ||
            (!MR::isPlayerSwingAction() && MR::isOnGroundPlayer()) || (isNerve(&NrvPlant::PlantNrvGrowUp::sInstance) && MR::isOnGroundPlayer()) ||
            isNerve(&NrvPlant::PlantNrvGrowthWaitInvalid::sInstance)) {
            return false;
        }

        if (mPlayAppearDemo && (isNerve(&NrvPlant::PlantNrvGrowUp::sInstance) || isNerve(&NrvPlant::PlantNrvWaitDemoWaitGrowUp::sInstance) ||
                                isNerve(&NrvPlant::PlantNrvDemoWaitGrowUp::sInstance))) {
            return false;
        }

        mRider = pSender->mHost;
        MR::invalidateClipping(this);
        MR::moveCoordAndTransToNearestRailPos(this, *MR::getPlayerPos());
        MR::getPlayerFrontVec(&mFront);
        mRideVelocity = -(MR::getPlayerVelocity()->dot(*MR::getPlayerGravity()));
        mRailCoord = mStalk->calcPlayerCoord();

        if (isNerve(&NrvPlant::PlantNrvGrowUp::sInstance)) {
            mRideVelocity = 0.0f;
            setNerve(&NrvPlant::PlantNrvHangWaitGrowUp::sInstance);
        } else if (MR::getRailCoord(this) >= MR::getRailTotalLength(this) - 150.0f) {
            mRideVelocity = 0.0f;
            mRailCoord = MR::getRailTotalLength(this) - 150.0f;
            MR::setRailDirectionToStart(this);
            mGrabbedTop = true;
            setNerve(&NrvPlant::PlantNrvHangStart::sInstance);
        } else {
            if (mRideVelocity >= 2.0f) {
                mRideVelocity = MR::clamp(mRideVelocity, 15.0f, 35.0f);
                MR::setRailDirectionToEnd(this);
                setNerve(&NrvPlant::PlantNrvHangStart::sInstance);
            } else {
                mRideVelocity = 0.0f;
                MR::setRailDirectionToStart(this);
                setNerve(&NrvPlant::PlantNrvHangStart::sInstance);
            }
        }

        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        MR::startMultiActorCameraNoTarget(this, mCameraInfo, "掴まり", -1);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TVec3f side;
        MR::makeAxisUpFront(&side, &mFront, mUp, mFront);
        TPos3f mtx;
        mtx.setXYZDir(side, mUp, mFront);
        mtx.setTrans(mPosition);
        MR::setBaseTRMtx(mRider, mtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        MR::endMultiActorCamera(this, mCameraInfo, "掴まり", false, -1);
        mRider = nullptr;
        setNerve(&NrvPlant::PlantNrvGrowthWaitInvalid::sInstance);
        return true;
    }

    return false;
}

void Plant::updateTopMtx() {
    PlantPoint* topPoint = mStalk->mPlantPoints[0];
    mTopMtx.setXYZDir(topPoint->mSide, topPoint->mUp, topPoint->mFront);
    mTopMtx.setTrans(topPoint->mPosition);
}

bool Plant::updateGrowUp() {
    bool growStalk = mStalk->updateGrowUp();

    updateTopMtx();
    f32 growthPercent = mStalk->mGrowthPercent;
    mTopPartsModel->mScale.set(growthPercent);
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        TVec3f axisY;
        TVec3f pos;
        f32 leafCoord = mLeaves[leaf]->mLeafCoord;
        mStalk->calcPosAndAxisY(&pos, &axisY, leafCoord * mStalk->mGrowthPercent);
        mLeaves[leaf]->updateGrowUp(pos, axisY, growthPercent, 10.0f);
    }

    MR::tryRumblePadWeak(this, WPAD_CHAN0);
    MR::startLevelSound(this, "SE_OJ_LV_PLANT_GROW", -1, -1, -1);

    if (growStalk) {
        if (mPlayAppearDemo) {
            MR::endDemo(this, "伸び植物（成長）");
            MR::endMultiActorCamera(this, mCameraInfo, "出現デモ", false, -1);
        }

        if (mRider != nullptr) {
            MR::moveCoordAndTransToNearestRailPos(this, mPosition);
            if (isNerve(&NrvPlant::PlantNrvHangUpGrowUp::sInstance)) {
                MR::setRailDirectionToEnd(this);
                setNerve(&NrvPlant::PlantNrvHangUp::sInstance);
            } else {
                MR::setRailDirectionToStart(this);
                setNerve(&NrvPlant::PlantNrvHangDown::sInstance);
            }
        } else {
            setNerve(&NrvPlant::PlantNrvGrowthWait::sInstance);
        }
        return true;
    }

    return false;
}

bool Plant::updateHangUp(f32 angleRate) {
    mRideVelocity -= 0.3f;
    mRideVelocity = MR::clamp(mRideVelocity, 0.0f, 35.0f);

    updateBindLeaf();

    if (mRideVelocity <= 0.0f) {
        MR::setRailDirectionToStart(this);
        setNerve(&NrvPlant::PlantNrvHangDown::sInstance);
        return true;
    }

    MR::moveCoordAndFollowTrans(this, mRideVelocity);
    mUp.set(MR::getRailDirection(this));
    f32 frontZ = mFront.z;
    MR::rotateVecDegree(&mFront, MR::getRailDirection(this), angleRate);
    if (frontZ < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_UP", -1, -1);
    }

    return false;
}

void Plant::updateBindLeaf() {
    f32 springPower = mRideVelocity;
    if (!MR::isRailGoingToEnd(this)) {
        springPower *= -1.0f;
    }

    TVec3f railDir(MR::getRailDirection(this));
    railDir.scale(20.0f);
    if (MR::isRailGoingToEnd(this)) {
        railDir.scale(-1.0f);
    }
    railDir.add(mPosition);

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        f32 growthPercent = mStalk->mGrowthPercent;
        mLeaves[leaf]->updateSpring(railDir, springPower, growthPercent);
    }
}

void Plant::startGrowUp() {
    if (mPlayAppearDemo) {
        setNerve(&NrvPlant::PlantNrvWaitDemoWaitGrowUp::sInstance);
    } else {
        setNerve(&NrvPlant::PlantNrvGrowUp::sInstance);
    }
}

bool Plant::tryHangUp() {
    if (!MR::isPadSwing(WPAD_CHAN0)) {
        if (mGrabbedTop) {
            mGrabbedTop = false;
        }
        return false;
    }

    if (mGrabbedTop) {
        return false;
    }

    MR::reverseRailDirection(this);
    MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    mRideVelocity = 10.0f;
    setNerve(&NrvPlant::PlantNrvHangUp::sInstance);
    return true;
}

bool Plant::tryReachGoal() {
    if (!MR::isRailReachedGoal(this)) {
        return false;
    }

    MR::moveCoordToEndPos(this);
    TVec3f endPoint;
    MR::calcRailEndPointPos(&endPoint, this);
    MR::setPlayerPos(endPoint);

    TVec3f endFront(mStalk->mPlantPoints[0]->mUp);
    MR::vecKillElement(endFront, mGravity, &endFront);

    if (!MR::isNearZero(endFront)) {
        MR::normalize(&endFront);
        MR::setPlayerFrontTargetVec(endFront, 1);
    }

    TVec3f endUp(mStalk->mPlantPoints[0]->mUp);
    endUp.scale(mLaunchSpeed);

    f32 launchVel = -mLaunchNormal;
    TVec3f up(mGravity);
    up.scale(launchVel);
    endUp.add(up);

    MR::startBckPlayer("GrowPlantJump", (const char*)0);
    MR::stopSound(mRider, "SE_OJ_PLANT_MARIO_UP_START", 0);
    MR::startSound(mRider, "SE_PM_JUMP_L", -1, -1);
    MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);

    MR::endMultiActorCameraAtLanding(this, mCameraInfo, "掴まり", -1);
    MR::endBindAndPlayerForceWeakGravityJump(this, endUp);
    mRider = nullptr;

    setNerve(&NrvPlant::PlantNrvGrowthWaitInvalid::sInstance);
    return true;
}

bool Plant::tryAccelHangUp() {
    if (mAccelTimer > 0) {
        // prevent swing-accel while timer is active (unused behavior!)
        return false;
    }

    if (MR::isPadSwing(WPAD_CHAN0)) {
        mAccelTimer = 0;  // this "delay" system is disabled because of this being set to zero here.
        mRideVelocity += 1.0f;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        f32 speed;
        if (mRideVelocity >= 10.0f) {
            speed = mRideVelocity;
        } else {
            speed = 10.0f;
        }
        mRideVelocity = speed;
        return true;
    }

    return false;
}

void Plant::draw() const {
    if (!MR::isValidDraw(this) || isNerve(&NrvPlant::PlantNrvWaitFar::sInstance) || isNerve(&NrvPlant::PlantNrvSeedWait::sInstance)) {
        return;
    }

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        GXLoadPosMtxImm(mLeaves[leaf]->mPosMtx, 0);
        GXLoadNrmMtxImm(mLeaves[leaf]->mPosMtx, 0);
        mShapeDraw->draw();
    }
}

namespace MR {
    // should this be in MathUtil?
    void clampMax(f32* val, f32 max) {
        f32 ret;
        if (*val >= max) {
            ret = max;
        } else {
            ret = *val;
        }
        *val = ret;
    }
};  // namespace MR

MtxPtr PlantLeaf::getBaseMtx() const {
    return (MtxPtr)&mBaseMtx;
}
