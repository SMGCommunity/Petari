#include "Game/Ride/Plant.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Ride/PlantLeaf.hpp"
#include "Game/Ride/PlantStalk.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphBase/J3DShapeDraw.hpp>
#include <revolution/gx/GXTransform.h>
#include <revolution/mtx.h>
#include <revolution/types.h>
#include <revolution/wpad.h>

void Plant_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

void DUMMY() {
    f32 a;
    MR::clampMax(&a, 0.0f);
}

namespace {
    static const f32 sDistanceNear = 700.0f;
    static const f32 sDistanceBody = 300.0f;
    static const f32 sDistancePush = 50.0f;
    static const s32 sStepDemoAppearEffect = 60;
    static const s32 sStepDemoWaitGrowUp = 90;
    static const f32 sHangUpStartTopLength = 150.0f;
    static const f32 sHangUpPlayerSpeedMin = 10.0f;
    static const f32 sHangStartRotateSpeed = 15.0f;
    static const f32 sGrowUpPlayerSpeed = 10.0f;
    static const f32 sHangUpAccel = 1.0f;
    // static const f32 sHangUpAccel2P =
    static const f32 sHangUpSpeedStartMin = -2.0f;
    static const f32 sHangDownGravity = 0.3f;
    static const f32 sHangUpSpeedMin = 15.0f;
    static const f32 sHangUpSpeedMax = 35.0f;
    static const f32 sHangDownSpeedMax = 20.0f;
    static const f32 sHangUpRotateSpeedMax = 30.0f;
    static const f32 sHangDownRotateSpeedMax = 20.0f;
    static const f32 sHangRotateSpeedRate = 1.3f;
    static const f32 sHangReachedDistance = 10.0f;
    static const f32 sDefaultJumpSpeed = 30.0f;
    static const s32 sTimeInvalid = 60;
    static const f32 sLeafIntervalMin = 100.0f;
    static const f32 sLeafIntervalMax = 300.0f;
    static const f32 sLeafPosOffsetStart = 100.0f;
    static const f32 sLeafPosOffsetEnd = 200.0f;
    static const f32 sLeafScaleMin = 1.0f;
    static const f32 sLeafScaleMax = 1.5f;
    static const f32 sLeafRotateMin = 90.0f;
    static const f32 sLeafRotateMax = 270.0f;
    static const f32 sLeafAxisDotMax = 0.7f;
    // static const f32 sPlayerPosOffset =

    static const s32 sHangAccelInvalidTime = 0;
    static const f32 sDefaultJumpSpeedOffsetY = 0.0f;
};  // namespace

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
    : LiveActor(pName), mSeedPartsModel(), mStalk(), mTopPartsModel(), mNumLeaves(), mLeaves(), mShapeDraw(), mRider(), mRailCoord(),
      mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mRideVelocity(), mAccelTimer(), mJumpSpeed(::sDefaultJumpSpeed),
      mJumpSpeedOffsetY(::sDefaultJumpSpeedOffsetY), mClippingCenter(0.0f, 0.0f, 0.0f), mCameraInfo(), mPlayAppearDemo(), mGrabbedTop() {
    mSeedMtx.identity();
    mTopMtx.identity();
}

void Plant::init(const JMapInfoIter& pMapInfoIter) {
    MR::createSceneObj(SceneObj_PlantStalkDrawInit);
    MR::createSceneObj(SceneObj_PlantLeafDrawInit);

    mShapeDraw = MR::getSceneObj< PlantLeafDrawInit >(SceneObj_PlantLeafDrawInit)->mShapeDraw;

    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_Ride, MR::DrawBufferType_None, MR::DrawType_Plant);

    MR::getJMapInfoArg0NoInit(pMapInfoIter, &mJumpSpeed);
    MR::getJMapInfoArg1NoInit(pMapInfoIter, &mJumpSpeedOffsetY);
    MR::getJMapInfoArg2NoInit(pMapInfoIter, &mPlayAppearDemo);

    initSound(8, false);
    initNerve(GET_NERVE(Plant, PlantNrvWaitFar));

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

    if (MR::isNear(this, *MR::getPlayerPos(), ::sDistanceNear)) {
        setNerve(GET_NERVE(Plant, PlantNrvSeedWait));
    }
}

void Plant::exeSeedWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mSeedPartsModel, "Bud", 0);
        MR::startSound(this, "SE_OJ_PLANT_BUD");
        mTopPartsModel->kill();
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
}

void Plant::exeWaitDemoWaitGrowUp() {
    if (MR::tryStartDemoWithoutCinemaFrame(this, "伸び植物（成長）")) {
        MR::startMultiActorCameraTargetOther(this, mCameraInfo, "出現デモ", CameraTargetArg(mTopPartsModel), -1);
        MR::requestMovementOn(mSeedPartsModel);
        setNerve(GET_NERVE(Plant, PlantNrvDemoWaitGrowUp));
    }
}

void Plant::exeDemoWaitGrowUp() {
    if (MR::isStep(this, ::sStepDemoAppearEffect)) {
        MR::startBck(mSeedPartsModel, "Bud", 0);
        MR::startSound(this, "SE_OJ_PLANT_BUD");
    }

    if (MR::isStep(this, ::sStepDemoWaitGrowUp)) {
        setNerve(GET_NERVE(Plant, PlantNrvGrowUp));
    }
}

void Plant::exeGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mSeedPartsModel, "GrowUp", 0);
        MR::startSound(this, "SE_OJ_PLANT_SEED_BREAK");
        MR::startSystemSE("SE_SY_ITEM_APPEAR");
        MR::startSound(this, "SE_OJ_PLANT_GROW_START");
    }

    if (updateGrowUp()) {
        return;
    }
}

void Plant::exeHangWaitGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", static_cast< const char* >(nullptr));
    }

    if (updateGrowUp()) {
        return;
    }

    f32 growthPercent = mStalk->mGrowthPercent;
    mStalk->calcPosAndAxisY(&mPosition, &mUp, mRailCoord * growthPercent);

    if (MR::isPadSwing(WPAD_CHAN0) && mRailCoord < MR::getRailTotalLength(this)) {
        setNerve(GET_NERVE(Plant, PlantNrvHangUpGrowUp));
    }
}

void Plant::exeHangUpGrowUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", static_cast< const char* >(nullptr));
        mAccelTimer = 0;
        mRideVelocity = ::sGrowUpPlayerSpeed;
    }

    if (updateGrowUp()) {
        return;
    }

    mAccelTimer--;
    tryAccelHangUp();
    mRideVelocity -= ::sHangDownGravity;

    if (mRideVelocity <= 0.0f) {
        mRideVelocity = 0.0f;
        MR::startBckPlayer("GrowPlantHangDown", static_cast< const char* >(nullptr));
        setNerve(GET_NERVE(Plant, PlantNrvHangWaitGrowUp));
        return;
    }

    mRailCoord += mRideVelocity;
    MR::clampMax(&mRailCoord, MR::getRailTotalLength(this));
    f32 growthPercent = mStalk->mGrowthPercent;
    mStalk->calcPosAndAxisY(&mPosition, &mUp, mRailCoord * growthPercent);

    f32 rotateSpeed = ::sHangRotateSpeedRate * mRideVelocity;
    f32 z = mFront.z;

    rotateSpeed = rotateSpeed >= ::sHangUpRotateSpeedMax ? ::sHangUpRotateSpeedMax : rotateSpeed;
    MR::rotateVecDegree(&mFront, mUp, rotateSpeed);

    if (z < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_UP");
    }

    updateBindLeaf();
    if (mRailCoord >= MR::getRailTotalLength(this)) {
        mRailCoord = MR::getRailTotalLength(this);
        mRideVelocity = 0.0f;
        setNerve(GET_NERVE(Plant, PlantNrvHangWaitGrowUp));
    }
}

void Plant::exeGrowthStop() {
}

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
        setNerve(GET_NERVE(Plant, PlantNrvGrowthStop));
    }
}

void Plant::exeGrowthWaitInvalid() {
    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        mLeaves[leaf]->updateSpring(1.0f);
    }

    if (MR::isStep(this, ::sTimeInvalid) || MR::isOnGroundPlayer()) {
        setNerve(GET_NERVE(Plant, PlantNrvGrowthWait));
    }
}

void Plant::exeHangStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", static_cast< const char* >(nullptr));
    }

    if (!updateHangUp(::sHangStartRotateSpeed) && MR::isBckStopped(mRider)) {
        setNerve(GET_NERVE(Plant, PlantNrvHangUp));
    }
}

void Plant::exeHangUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", static_cast< const char* >(nullptr));
        MR::setRailDirectionToEnd(this);
        mAccelTimer = 0;
    }

    f32 rotateSpeed = mRideVelocity * ::sHangRotateSpeedRate;
    rotateSpeed = rotateSpeed >= ::sHangUpRotateSpeedMax ? ::sHangUpRotateSpeedMax : rotateSpeed;

    if (!updateHangUp(rotateSpeed) && !tryReachGoal()) {
        mAccelTimer--;
        tryAccelHangUp();
    }
}

void Plant::exeHangDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangDown", static_cast< const char* >(nullptr));
        MR::setRailDirectionToStart(this);
    }

    if (tryHangUp()) {
        return;
    }

    mRideVelocity += ::sHangDownGravity;
    mRideVelocity = MR::clamp(mRideVelocity, 0.0f, ::sHangDownSpeedMax);
    updateBindLeaf();

    if (MR::isRailReachedNearGoal(this, ::sHangReachedDistance)) {
        MR::endMultiActorCameraAtLanding(this, mCameraInfo, "掴まり", -1);
        MR::startBckPlayer("GrowPlantJump", static_cast< const char* >(nullptr));
        MR::endBindAndPlayerJump(this, TVec3f(0.0f, 0.0f, 0.0f), 0);
        mRider = nullptr;
        setNerve(GET_NERVE(Plant, PlantNrvGrowthWaitInvalid));
        return;
    }

    MR::moveCoordAndFollowTrans(this, mRideVelocity);
    mUp.set(MR::getRailDirection(this));

    f32 rotateSpeed = mRideVelocity * ::sHangRotateSpeedRate;
    f32 z = mFront.z;
    mUp.mult(-1.0f);

    rotateSpeed = rotateSpeed >= ::sHangDownRotateSpeedMax ? ::sHangDownRotateSpeedMax : rotateSpeed;
    MR::rotateVecDegree(&mFront, MR::getRailDirection(this), rotateSpeed);

    if (z < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_DOWN");
    }
}

void Plant::initLeaf() {
    f32 leafRatio, leafScale, leafPos;

    mNumLeaves = (s32)((MR::getRailTotalLength(this) - ::sLeafPosOffsetStart - ::sLeafPosOffsetEnd) / (::sLeafIntervalMax - ::sLeafIntervalMin)) + 2;
    mLeaves = new PlantLeaf*[mNumLeaves];

    TRot3f mtx;
    mtx.identity();

    TVec3f baseRotate(0.0f, 0.0f, 1.0f);
    leafPos = MR::getRailTotalLength(this) - ::sLeafPosOffsetStart;

    MR::getRailTotalLength(this);

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        leafRatio = static_cast< f32 >(leaf) / static_cast< f32 >(mNumLeaves);
        TVec3f railDirection;
        MR::calcRailDirectionAtCoord(&railDirection, this, leafPos);

        TVec3f growDirection = baseRotate;
        f32 dot = growDirection.dot(railDirection);
        if (MR::abs(dot) > ::sLeafAxisDotMax) {
            if (dot > 0.0f) {
                growDirection.cross(TVec3f(0.0f, 1.0f, 0.0f), railDirection);
            } else {
                growDirection.cross(railDirection, TVec3f(0.0f, 1.0f, 0.0f));
            }

            MR::normalize(&growDirection);
        }

        leafScale = MR::getEaseOutValue(leafRatio, ::sLeafScaleMin, ::sLeafScaleMax, 1.0f);

        mLeaves[leaf] = new PlantLeaf(leafPos, mPosition, growDirection, leafScale);
        mLeaves[leaf]->initWithoutIter();
        leafPos -= MR::getInterpolateValue(leafRatio, ::sLeafIntervalMin, ::sLeafIntervalMax);

        mtx.setRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(MR::getRandom(::sLeafRotateMin, ::sLeafRotateMax)));
        mtx.mult(baseRotate, baseRotate);
    }
}

void Plant::calcAnim() {
    if (!MR::isValidCalcViewAndEntry(this) || isNerve(GET_NERVE(Plant, PlantNrvWaitFar)) || isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) {
        return;
    }

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        MtxPtr mtx = mLeaves[leaf]->getPosMtx();
        MR::multMtx(mtx, mLeaves[leaf]->getBaseMtx(), MR::getCameraViewMtx());
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
    if (isNerve(GET_NERVE(Plant, PlantNrvWaitFar)) || isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) {
        pSensor->mPosition.set(mSeedPartsModel->mPosition);
    } else {
        mStalk->calcNearestPos(&pSensor->mPosition, *MR::getPlayerPos());
    }
}

void Plant::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) {
        f32 r = pReceiver->mRadius;
        if (MR::isNear(pSender, pReceiver, r + ::sDistancePush)) {
            pSender->mRadius = ::sDistancePush;
            MR::sendMsgPush(pReceiver, pSender);
            pSender->mRadius = ::sDistanceBody;
        }
    } else {
        if (mRider != nullptr) {
            MR::tryGetItem(pSender, pReceiver);
        }
    }
}

bool Plant::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) {
            MR::invalidateClipping(this);
            mTopPartsModel->appear();
            startGrowUp();
            return true;
        }
        return false;
    }

    return false;
}

bool Plant::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if ((isNerve(GET_NERVE(Plant, PlantNrvWaitFar)) || isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) ||
            (!MR::isPlayerSwingAction() && MR::isOnGroundPlayer()) || (isNerve(GET_NERVE(Plant, PlantNrvGrowUp)) && MR::isOnGroundPlayer()) ||
            isNerve(GET_NERVE(Plant, PlantNrvGrowthWaitInvalid))) {
            return false;
        }

        if (mPlayAppearDemo && (isNerve(GET_NERVE(Plant, PlantNrvGrowUp)) || isNerve(GET_NERVE(Plant, PlantNrvWaitDemoWaitGrowUp)) ||
                                isNerve(GET_NERVE(Plant, PlantNrvDemoWaitGrowUp)))) {
            return false;
        }

        mRider = pSender->mHost;
        MR::invalidateClipping(this);
        MR::moveCoordAndTransToNearestRailPos(this, *MR::getPlayerPos());
        MR::getPlayerFrontVec(&mFront);
        mRideVelocity = -MR::getPlayerVelocity()->dot(*MR::getPlayerGravity());
        mRailCoord = mStalk->calcPlayerCoord();

        if (isNerve(GET_NERVE(Plant, PlantNrvGrowUp))) {
            mRideVelocity = 0.0f;
            setNerve(GET_NERVE(Plant, PlantNrvHangWaitGrowUp));
        } else if (MR::getRailCoord(this) >= MR::getRailTotalLength(this) - ::sHangUpStartTopLength) {
            mRideVelocity = 0.0f;
            mRailCoord = MR::getRailTotalLength(this) - ::sHangUpStartTopLength;
            MR::setRailDirectionToStart(this);
            mGrabbedTop = true;
            setNerve(GET_NERVE(Plant, PlantNrvHangStart));
        } else {
            if (mRideVelocity >= ::sHangUpSpeedStartMin) {
                mRideVelocity = MR::clamp(mRideVelocity, ::sHangUpSpeedMin, ::sHangUpSpeedMax);
                MR::setRailDirectionToEnd(this);
                setNerve(GET_NERVE(Plant, PlantNrvHangStart));
            } else {
                mRideVelocity = 0.0f;
                MR::setRailDirectionToStart(this);
                setNerve(GET_NERVE(Plant, PlantNrvHangStart));
            }
        }

        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        MR::startSound(mRider, "SE_PV_CATCH");
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
        setNerve(GET_NERVE(Plant, PlantNrvGrowthWaitInvalid));
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
    MR::startLevelSound(this, "SE_OJ_LV_PLANT_GROW");

    if (growStalk) {
        if (mPlayAppearDemo) {
            MR::endDemo(this, "伸び植物（成長）");
            MR::endMultiActorCamera(this, mCameraInfo, "出現デモ", false, -1);
        }

        if (mRider != nullptr) {
            MR::moveCoordAndTransToNearestRailPos(this, mPosition);
            if (isNerve(GET_NERVE(Plant, PlantNrvHangUpGrowUp))) {
                MR::setRailDirectionToEnd(this);
                setNerve(GET_NERVE(Plant, PlantNrvHangUp));
            } else {
                MR::setRailDirectionToStart(this);
                setNerve(GET_NERVE(Plant, PlantNrvHangDown));
            }
        } else {
            setNerve(GET_NERVE(Plant, PlantNrvGrowthWait));
        }
        return true;
    }

    return false;
}

bool Plant::updateHangUp(f32 angleRate) {
    mRideVelocity -= ::sHangDownGravity;
    mRideVelocity = MR::clamp(mRideVelocity, 0.0f, ::sHangUpSpeedMax);

    updateBindLeaf();

    if (mRideVelocity <= 0.0f) {
        MR::setRailDirectionToStart(this);
        setNerve(GET_NERVE(Plant, PlantNrvHangDown));
        return true;
    }

    MR::moveCoordAndFollowTrans(this, mRideVelocity);
    mUp.set(MR::getRailDirection(this));
    f32 frontZ = mFront.z;
    MR::rotateVecDegree(&mFront, MR::getRailDirection(this), angleRate);
    if (frontZ < 0.0f && mFront.z >= 0.0f) {
        MR::startSound(mRider, "SE_OJ_PLANT_MARIO_UP");
    }

    return false;
}

void Plant::updateBindLeaf() {
    f32 springPower = mRideVelocity;
    if (!MR::isRailGoingToEnd(this)) {
        springPower *= -1.0f;
    }

    TVec3f railDir = MR::getRailDirection(this) * 20.0f;
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
        setNerve(GET_NERVE(Plant, PlantNrvWaitDemoWaitGrowUp));
    } else {
        setNerve(GET_NERVE(Plant, PlantNrvGrowUp));
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
    mRideVelocity = ::sHangUpPlayerSpeedMin;
    setNerve(GET_NERVE(Plant, PlantNrvHangUp));
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

    TVec3f endFront = mStalk->mPlantPoints[0]->mUp;
    MR::vecKillElement(endFront, mGravity, &endFront);

    if (!MR::isNearZero(endFront)) {
        MR::normalize(&endFront);
        MR::setPlayerFrontTargetVec(endFront, 1);
    }

    TVec3f endUp = mStalk->mPlantPoints[0]->mUp;
    endUp.scale(mJumpSpeed);

    endUp.add(mGravity * -mJumpSpeedOffsetY);

    MR::startBckPlayer("GrowPlantJump", static_cast< const char* >(nullptr));
    MR::stopSound(mRider, "SE_OJ_PLANT_MARIO_UP_START");
    MR::startSound(mRider, "SE_PM_JUMP_L");
    MR::startSound(mRider, "SE_PV_JUMP_JOY");

    MR::endMultiActorCameraAtLanding(this, mCameraInfo, "掴まり", -1);
    MR::endBindAndPlayerForceWeakGravityJump(this, endUp);
    mRider = nullptr;

    setNerve(GET_NERVE(Plant, PlantNrvGrowthWaitInvalid));
    return true;
}

bool Plant::tryAccelHangUp() {
    if (mAccelTimer > 0) {
        return false;
    }

    if (MR::isPadSwing(WPAD_CHAN0)) {
        mAccelTimer = ::sHangAccelInvalidTime;
        mRideVelocity += ::sHangUpAccel;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        f32 speed;
        if (mRideVelocity >= ::sHangUpPlayerSpeedMin) {
            speed = mRideVelocity;
        } else {
            speed = ::sHangUpPlayerSpeedMin;
        }
        mRideVelocity = speed;
        return true;
    }

    return false;
}

void Plant::draw() const {
    if (!MR::isValidDraw(this) || isNerve(GET_NERVE(Plant, PlantNrvWaitFar)) || isNerve(GET_NERVE(Plant, PlantNrvSeedWait))) {
        return;
    }

    for (s32 leaf = 0; leaf < mNumLeaves; leaf++) {
        GXLoadPosMtxImm(mLeaves[leaf]->mPosMtx, 0);
        GXLoadNrmMtxImm(mLeaves[leaf]->mPosMtx, 0);
        mShapeDraw->draw();
    }
}
