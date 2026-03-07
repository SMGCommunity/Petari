#include "Game/Ride/SpaceCocoon.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXLighting.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXVert.h>
#include <revolution/mtx.h>
#include <revolution/wpad.h>

namespace NrvSpaceCocoon {
    NEW_NERVE(SpaceCocoonNrvStop, SpaceCocoon, Stop);
    NEW_NERVE(SpaceCocoonNrvFree, SpaceCocoon, Free);
    NEW_NERVE(SpaceCocoonNrvFreeInvalid, SpaceCocoon, FreeInvalid);
    NEW_NERVE(SpaceCocoonNrvKinopioLand, SpaceCocoon, BindLand);
    NEW_NERVE(SpaceCocoonNrvKinopioWait, SpaceCocoon, BindWait);
    NEW_NERVE(SpaceCocoonNrvWaitKinopioAimDemo, SpaceCocoon, WaitKinopioAimDemo);
    NEW_NERVE(SpaceCocoonNrvKinopioAim, SpaceCocoon, BindAim);
    NEW_NERVE(SpaceCocoonNrvKinopioAttack, SpaceCocoon, BindAttack);
    NEW_NERVE(SpaceCocoonNrvKinopioAttackSuccess, SpaceCocoon, BindAttackSuccess);
    NEW_NERVE(SpaceCocoonNrvBindLand, SpaceCocoon, BindLand);
    NEW_NERVE(SpaceCocoonNrvBindWait, SpaceCocoon, BindWait);
    NEW_NERVE(SpaceCocoonNrvBindAim, SpaceCocoon, BindAim);
    NEW_NERVE(SpaceCocoonNrvBindAttack, SpaceCocoon, BindAttack);
    NEW_NERVE(SpaceCocoonNrvBindAttackSuccess, SpaceCocoon, BindAttackSuccess);
}  // namespace NrvSpaceCocoon

namespace {
    static Color8 sColor(0xC8, 0xC8, 0xD7, 0xFF);
}

SpaceCocoon::SpaceCocoon(const char* pName)
    : LiveActor(pName), mIsKinopioCameraFocused(false), mNeutralPos(0.0f, 0.0f, 0.0f), mBasePos(0.0f, 0.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f),
      mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mHeight(100.0f), mPointerPos(0.0f, 0.0f, 0.0f), mPadChannel(-1), mCocoonPos(0.0f, 0.0f, 0.0f),
      mSpringVel(0.0f, 0.0f, 0.0f), mAttackTime(60), mTouchTime(0), mRider(nullptr), mNumPoints(0), mPlantPoints(nullptr), mCameraTargetMtx(nullptr),
      mCameraInfo(nullptr), mCocoonModel(nullptr), mTexture(nullptr) {
    mBaseMtx.identity();
    mTopMtx.identity();
}

void SpaceCocoon::init(const JMapInfoIter& rIter) {
    // FIXME : annoying regswap, idx should be in r30
    // https://decomp.me/scratch/kjwvE
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_Ride, -1, MR::DrawType_SpaceCocoon);

    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mAttackTime);
    MR::getJMapInfoArg1NoInit(rIter, &mHeight);
    MR::getJMapInfoArg3NoInit(rIter, &mIsKinopioCameraFocused);

    mBasePos.set(mPosition);
    MR::calcActorAxis(&mSide, &mUp, &mFront, this);

    mNeutralPos.set(mUp);
    mNeutralPos.scale(mHeight);
    mNeutralPos.add(mPosition);

    mPosition.set(mNeutralPos);
    mCocoonPos.set(mNeutralPos);
    mPointerPos.set(mNeutralPos);

    initHitSensor(3);
    MR::addHitSensorTransferableBinder(this, "Bind", 8, 200.0f, TVec3f(0.0f, 100.0f, 0.0f));
    MR::addHitSensorMapObj(this, "Push", 8, 50.0f, TVec3f(0.0f, -50.0f, 0.0f));
    MR::addHitSensor(this, "Attack", 12, 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(50.0f, 50.0f, 8);
    MR::offBind(this);

    initSound(6, false);

    if (mIsKinopioCameraFocused) {
        MR::needStageSwitchWriteA(this, rIter);
    }

    initNerve(&NrvSpaceCocoon::SpaceCocoonNrvStop::sInstance);

    mNumPoints = mHeight / 10.0f;
    mPlantPoints = new PlantPoint*[mNumPoints];

    f32 delta = mHeight / (mNumPoints + 1);

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        TVec3f spawnPos(mUp);
        s32 revIdx = mNumPoints - idx;
        spawnPos.scale(delta * revIdx);
        spawnPos.add(mBasePos);

        // something got optimized out here, or this is what was written
        f32 f = MR::getEaseInValue((f32)idx / mNumPoints, 0.0f, 1.0f, 1.0f);

        f32 thickness = 1.0f;
        s32 prevRevIdx = (mNumPoints - 1) - idx;
        if (prevRevIdx < 3) {
            thickness = MR::getEaseOutValue(prevRevIdx / 3.0f, 2.0f, 1.0f, 1.0f);
        }

        mPlantPoints[idx] = new PlantPoint(spawnPos, mUp, thickness);
    }

    PlantPoint* topPoint = mPlantPoints[0];
    mTopMtx.setXYZDir(topPoint->mFront, topPoint->mUp, topPoint->mSide);
    mTopMtx.setTrans(topPoint->mPosition);

    mCocoonModel = new PartsModel(this, "先端", "SpaceCocoon", mTopMtx, MR::DrawBufferType_NoSilhouettedMapObj, false);
    mCocoonModel->initWithoutIter();

    updateDrawPoints();
    MR::initStarPointerTarget(this, 300.0f, TVec3f(0.0f, 50.0f, 0.0f));

    MR::calcGravity(this);

    MR::initShadowVolumeSphere(this, 100.0f);
    MR::setClippingFar200m(this);

    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "ウェイト");  // "wait"
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "狙い中");    // "aim"
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "攻撃中");    // "attack"

    mTexture = new JUTTexture(MR::loadTexFromArc("SpaceCocoon.arc", "SpaceCocoon.bti"), 0);
    makeActorAppeared();
}

void SpaceCocoon::exeStop() {
    if (MR::isFirstStep(this)) {
        mTouchTime = 0;
    }

    if (tryTouch()) {
        return;
    }
}

void SpaceCocoon::exeFree() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "Bind");
    }

    tryTouch();

    if (updateSpringPoint()) {
        mSpringVel.zero();
        mCocoonPos.set(mNeutralPos);
        mPosition.set(mNeutralPos);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvStop::sInstance);
    } else {
        mPosition.set(mCocoonPos);
    }
}

void SpaceCocoon::exeFreeInvalid() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    updateSpringPoint();
    mPosition.set(mCocoonPos);

    if (!MR::isGreaterStep(this, 5)) {
        return;
    }

    if (PSVECDistance(&mRider->mPosition, &mPosition) > 300.0f || MR::isOnGroundPlayer()) {
        mRider = nullptr;
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvFree::sInstance);
    }
}

void SpaceCocoon::exeBindLand() {
    if (MR::isFirstStep(this)) {
        if (!isKinopioAttached()) {
            MR::startBckPlayer("CocoonLand", 1);
            MR::startMultiActorCameraTargetOther(this, mCameraInfo, "ウェイト", CameraTargetArg(mCameraTargetMtx), -1);
            MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        } else {
            MR::startBckWithInterpole(mRider, "CocoonLand", 1);
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance);
            return;
        }
        MR::startSound(this, "SE_OJ_SPACE_COCOON_ATTACH", -1, -1);
    }

    if (isKinopioAttached()) {
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindWait::sInstance);
        return;
    }

    if (!updateBindWait()) {
        tryTouch();
        if (MR::isBckStopped(mRider)) {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindWait::sInstance);
        }
    }
}

void SpaceCocoon::exeBindWait() {
    if (MR::isFirstStep(this)) {
        if (!isKinopioAttached()) {
            MR::startBckPlayer("CocoonWait", static_cast< const char* >(0));
        } else {
            MR::startBck(mRider, "CocoonWait", static_cast< const char* >(0));
            MR::validateClipping(this);
            MR::validateClipping(mRider);
            MR::sendSimpleMsgToActor(ACTMES_NPC_EVENT_TALK_ENABLE, mRider);
        }
        mVelocity.zero();
    }

    if (updateBindWait()) {
        return;
    }

    if (tryTouch()) {
        if (!isKinopioAttached()) {
            MR::startBckPlayer("CocoonReaction", static_cast< const char* >(0));
        }
    } else {
        if (!isKinopioAttached() && MR::isBckOneTimeAndStopped(mRider)) {
            MR::startBckPlayer("CocoonWait", static_cast< const char* >(0));
        }
    }
}

void SpaceCocoon::exeWaitKinopioAimDemo() {
    if (MR::tryStartDemoWithoutCinemaFrameValidStarPointer(this, "キノピオ狙い中")) {
        MR::startMultiActorCameraTargetPlayer(this, mCameraInfo, "狙い中", -1);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAim::sInstance);
    }
}

void SpaceCocoon::exeBindAim() {
    if (MR::isFirstStep(this)) {
        if (isKinopioAttached()) {
            MR::sendSimpleMsgToActor(ACTMES_NPC_EVENT_TALK_DISABLE, mRider);
        }

        MR::startSound(this, "SE_OJ_SPACE_COCOON_DRAG_ST", -1, -1);

        if (mPadChannel == WPAD_CHAN1) {
            MR::startCSSound2P("CS_DPD_HIT", 0);
        }
    }

    f32 dist = PSVECDistance(&mPosition, &mNeutralPos);
    if (dist >= 100.0f) {
        MR::startLevelSound(this, "SE_OJ_LV_SPACE_COCOON_DRAG", ((dist - 100.0f) / 400.0f) * 100.0f, -1, -1);
    }

    if (!tryRelease()) {
        updateHang();
    }
}

void SpaceCocoon::exeBindAttack() {
    if (MR::isFirstStep(this)) {
        if (!isKinopioAttached()) {
            MR::startBckPlayer("CocoonFly", static_cast< const char* >(0));
            MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        } else {
            MR::startBck(mRider, "CocoonFly", static_cast< const char* >(0));
            MR::invalidateClipping(this);
            MR::invalidateClipping(mRider);
            MR::startSound(mRider, "SE_SV_KINOPIO_TALK_GLAD_FLY", -1, -1);
        }
        MR::startSound(this, "SE_OJ_SPACE_COCOON_LAUNCH", -1, -1);
        MR::onBind(this);
    }

    updateBindAttack();

    if (tryAttackMap() || MR::isStep(this, mAttackTime)) {
        MR::endMultiActorCamera(this, mCameraInfo, "攻撃中", true, -1);

        if (MR::isStep(this, mAttackTime) && !isKinopioAttached()) {
            MR::startBckPlayer("AirRotation", static_cast< const char* >(0));
        }

        endBind(TVec3f(0.0f, 0.0f, 0.0f), false);
    }
}

void SpaceCocoon::exeBindAttackSuccess() {
    if (MR::isFirstStep(this)) {
    }

    updateBindAttack();

    if (MR::isStep(this, 1)) {
        MR::stopScene(8);
    }

    if (MR::isStep(this, 2)) {
        if (isKinopioAttached()) {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttack::sInstance);
        } else {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAttack::sInstance);
        }
    }
}

void SpaceCocoon::calcAnim() {
    if (isKinopioAttached()) {
        updateActorMtx();
        MR::setBaseTRMtx(mRider, mBaseMtx);
        mRider->mPosition.set(mPosition);
    }
}

void SpaceCocoon::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isKinopioAttached() && mRider == nullptr && MR::isSensorNpc(pReceiver) && pReceiver->receiveMessage(ACTMES_NPC_EVENT_START, pSender)) {
        mRider = pReceiver->mHost;
        MR::emitEffect(mCocoonModel, "Stick");
        TVec3f side(mSide);
        TVec3f front(mFront);
        side.scale(mSide.dot(mRider->mVelocity));
        front.scale(mFront.dot(mRider->mVelocity));
        mSpringVel.set(side);
        mSpringVel.add(front);
        mRider->mVelocity.zero();
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance);
        return;
    }

    if (mRider == nullptr || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance) ||
        isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance)) {
        if (!MR::isSensorType(pSender, ATYPE_SPRING_ATTACKER) &&
            (isKinopioAttached() || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance))) {
            MR::sendMsgPush(pReceiver, pSender);
        }
        return;
    }

    if (MR::tryGetItem(pSender, pReceiver)) {
        return;
    }
}

bool SpaceCocoon::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgStarPieceReflect(msg)) {
        return false;
    }

    if (isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFree::sInstance) || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance)) {
        MR::addVelocitySeparateHV(this, pSender, pReceiver, 5.0f, 0.0f);
        mSpringVel.add(mVelocity);
        MR::zeroVelocity(this);
    }

    return true;
}

bool SpaceCocoon::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider != nullptr && MR::isSensor(pReceiver, "Bind")) {
        MR::endBindAndPlayerDamageMsg(this, msg);
        endBind(TVec3f(0.0f, 0.0f, 0.0f), true);
        return true;
    }

    return false;
}

bool SpaceCocoon::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance) || isKinopioAttached()) {
            return false;
        }
        mRider = pSender->mHost;
        MR::invalidateClipping(this);
        TVec3f side(mSide);
        TVec3f front(mFront);
        side.scale(mSide.dot(*MR::getPlayerVelocity()));
        front.scale(mFront.dot(*MR::getPlayerVelocity()));
        mSpringVel.set(side);
        mSpringVel.add(front);
        MR::emitEffect(mCocoonModel, "Stick");
        mPosition.set(mNeutralPos);
        mCameraTargetMtx->mMatrix.set(mTopMtx);
        mCameraTargetMtx->mMatrix.setTrans(mPosition);
        MR::invalidateHitSensor(this, "Bind");
        if (MR::isStarPointerInScreen(WPAD_CHAN0)) {
            MR::calcStarPointerPosOnPlane(&mPointerPos, mPosition, mUp, WPAD_CHAN0, false);
        }
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindLand::sInstance);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        updateActorMtx();
        MR::setBaseTRMtx(mRider, mBaseMtx);
        return true;
    }

    if (MR::isMsgIsRushTakeOver(msg)) {
        return !isNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance);
    }

    if (MR::isMsgRushCancel(msg)) {
        endBind(TVec3f(0.0f, 0.0f, 0.0f), false);
        return true;
    }

    return false;
}

bool SpaceCocoon::updateBindWait() {
    if (PSVECDistance(MR::getPlayerPos(), &mPosition) < 3000.0f && (!isKinopioAttached() || MR::isOnGroundPlayer())) {
        if (MR::isStarPointerPointing(this, WPAD_CHAN0, true, "弱")) {
            MR::requestStarPointerModeBlueStarReady(this);
        }

        bool set;
        if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::testCorePadTriggerA(WPAD_CHAN0) &&
            MR::tryStartStarPointerCommandStream(this, &mPosition, WPAD_CHAN0, false)) {
            set = true;
        } else {
            set = false;
        }

        if (set) {
            mPadChannel = WPAD_CHAN0;
        }

        if (mPadChannel >= 0) {
            if (isKinopioAttached()) {
                setNerve(&NrvSpaceCocoon::SpaceCocoonNrvWaitKinopioAimDemo::sInstance);
            } else {
                MR::endMultiActorCamera(this, mCameraInfo, "ウェイト", true, -1);
                MR::startMultiActorCameraTargetOther(this, mCameraInfo, "狙い中", CameraTargetArg(mCameraTargetMtx), -1);
                setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAim::sInstance);
            }
            return true;
        }
    }

    updateSpringPoint();
    mPosition.set(mCocoonPos);
    return false;
}

bool SpaceCocoon::updateSpringPoint() {
    mSpringVel.mult(0.95);

    TVec3f v1(mNeutralPos);
    v1.sub(mCocoonPos);

    TVec3f v2(v1);
    v2.scale(0.02f);
    mSpringVel.add(v2);
    mCocoonPos.add(mSpringVel);

    updateDrawPoints();

    if (mSpringVel.length() < 0.1f && PSVECDistance(&mCocoonPos, &mNeutralPos) < 1.0f) {
        mCocoonPos.set(mNeutralPos);
        mSpringVel.zero();
        return true;
    }

    return false;
}

void SpaceCocoon::updateHang() {
    f32 dist = PSVECDistance(&mPosition, &mNeutralPos);

    if (MR::isStarPointerInScreen(mPadChannel) && 100.0f > dist) {
        f32 dummy = 0.0f;
    }

    if (dist >= 500.0f) {
        MR::startLevelSound(this, "SE_OJ_LV_SPACE_COCOON_DRAG3", -1, -1, -1);
    } else if (dist >= 100.0f) {
        MR::startLevelSound(this, "SE_OJ_LV_SPACE_COCOON_DRAG2", -1, -1, -1);
    }

    if (MR::isStarPointerInScreen(mPadChannel)) {
        MR::calcStarPointerPosOnPlane(&mPointerPos, mNeutralPos, mUp, mPadChannel, false);
    }

    TVec3f pos(mPointerPos);
    if (PSVECDistance(&pos, &mNeutralPos) > 500.0f) {
        pos.set(mPointerPos);
        pos.sub(mNeutralPos);
        MR::normalize(&pos);
        pos.scale(500.0f);
        pos.add(mNeutralPos);
    }

    mPosition.setPS2(pos.scaleInline(0.03f).addOperatorInLine(mPosition.scaleInline(0.97f)));
    mCocoonPos.set(mPosition);
    updateDrawPoints();

    if (mRider == nullptr) {
        return;
    }

    if (dist < 200.0f && !MR::isBckPlaying(mRider, "CocoonWait")) {
        if (isKinopioAttached()) {
            MR::startBckWithInterpole(mRider, "CocoonWait", 10);
        } else {
            MR::startBckPlayer("CocoonWait", 10);
        }
    }

    if (dist > 300.0f && !MR::isBckPlaying(mRider, "CocoonStretch")) {
        if (isKinopioAttached()) {
            MR::startBckWithInterpole(mRider, "CocoonStretch", 10);
        } else {
            MR::startBckPlayer("CocoonStretch", 10);
        }
    }
}

void SpaceCocoon::updateBindAttack() {
    if (getNerveStep() >= mAttackTime - 90) {
        mVelocity.mult(0.98);
    }
    updateSpringPoint();
}

void SpaceCocoon::updateActorMtx() {
    if (isNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAttack::sInstance) || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAttackSuccess::sInstance) ||
        isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttack::sInstance) || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttackSuccess::sInstance)) {
        mBaseMtx.setTrans(mPosition);
    } else {
        mBaseMtx.set(mTopMtx);
    }
}

void SpaceCocoon::updateDrawPoints() {
    TVec3f prevPos(mCocoonPos);
    f32 delta = mHeight / (mNumPoints + 1);

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        f32 t = MR::getEaseOutValue(static_cast< f32 >(idx + 1) / static_cast< f32 >(mNumPoints + 1), 0.0f, 1.0f, 1.0f);
        t = MR::getEaseOutValue(t, 0.0f, 1.0f, 1.0f);

        TVec3f baseUp(mUp);
        baseUp.scale((idx + 1) * delta);

        TVec3f pos;
        pos.x = mCocoonPos.x * (1.0f - t) + mNeutralPos.x * t;
        pos.y = mCocoonPos.y * (1.0f - t) + mNeutralPos.y * t;
        pos.z = mCocoonPos.z * (1.0f - t) + mNeutralPos.z * t;

        pos.sub(baseUp);

        TVec3f up(prevPos);
        up.sub(pos);
        MR::normalize(&up);

        TVec3f side(mSide);
        TVec3f front(mFront);

        if (MR::isNearZero(side.dot(up), 0.001f)) {
            PSVECCrossProduct(&up, &front, &side);
            MR::normalize(&side);
            PSVECCrossProduct(&side, &up, &front);
            MR::normalize(&front);
        } else {
            PSVECCrossProduct(&side, &up, &front);
            MR::normalize(&front);
            PSVECCrossProduct(&up, &front, &side);
            MR::normalize(&side);
        }

        mPlantPoints[idx]->setAxisAndPos(side, up, front, pos);

        prevPos.set(pos);
    }

    PlantPoint* top = mPlantPoints[0];
    mTopMtx.setXYZDir(mPlantPoints[0]->mSide, mPlantPoints[0]->mUp, mPlantPoints[0]->mFront);
    mTopMtx.setTrans(top->mPosition);
}

namespace {
    bool tryTouchPointer(LiveActor* pActor, s32 padChannel) {
        if (MR::isStarPointerPointing(pActor, padChannel, false, "弱") && MR::getStarPointerScreenSpeed(padChannel) > 5.0f) {
            return true;
        }
        return false;
    }
}  // namespace

bool SpaceCocoon::tryTouch() {
    if (mTouchTime > 0) {
        mTouchTime--;
        return false;
    }

    s32 touchChannel = -1;
    if (tryTouchPointer(this, WPAD_CHAN0)) {
        touchChannel = WPAD_CHAN0;
    } else if (tryTouchPointer(this, WPAD_CHAN1)) {
        touchChannel = WPAD_CHAN1;
    }

    if (touchChannel >= 0) {
        TVec3f pointerVel(0.0f, 0.0f, 0.0f);
        MR::calcStarPointerWorldVelocityDirectionOnPlane(&pointerVel, mNeutralPos, mUp, touchChannel);
        pointerVel.scale(5.0f);
        mSpringVel.add(pointerVel);

        MR::startSound(this, "SE_OJ_SPACE_COCOON_BOUND", -1, -1);

        if (isNerve(&NrvSpaceCocoon::SpaceCocoonNrvStop::sInstance)) {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvFree::sInstance);
        }

        mTouchTime = 20;
        return true;
    }

    return false;
}

bool SpaceCocoon::tryRelease() {
    if (MR::testCorePadButtonA(mPadChannel)) {
        return false;
    }

    endCommandStream();
    MR::endMultiActorCamera(this, mCameraInfo, "狙い中", true, -1);

    if (PSVECDistance(&mPosition, &mNeutralPos) < 100.0f) {
        if (isKinopioAttached()) {
            MR::endDemo(this, "キノピオ狙い中");
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance);
        } else {
            MR::startMultiActorCameraTargetOther(this, mCameraInfo, "ウェイト", CameraTargetArg(mCameraTargetMtx), -1);
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindWait::sInstance);
        }
        return true;
    }

    MR::emitEffect(mCocoonModel, "Stick");

    if (mIsKinopioCameraFocused) {
        MR::onSwitchA(this);
        mIsKinopioCameraFocused = false;
    }

    mVelocity.set(mNeutralPos);
    mVelocity.sub(mPosition);
    MR::normalize(&mVelocity);

    // turn mario so he flies feet-first
    TVec3f reaxisUp(mVelocity);
    reaxisUp.scale(-1.0f);
    TVec3f reaxisFront(mUp);
    TVec3f reaxisSide;
    PSVECCrossProduct(&reaxisUp, &reaxisFront, &reaxisSide);
    MR::normalize(&reaxisSide);
    PSVECCrossProduct(&reaxisSide, &reaxisUp, &reaxisFront);
    MR::normalize(&reaxisFront);
    mBaseMtx.setXYZDir(reaxisSide, reaxisUp, reaxisFront);

    mVelocity.mult(60.0f);
    MR::emitEffect(mRider, "SpaceCocoonBlur");

    MR::startMultiActorCameraNoTarget(this, mCameraInfo, "攻撃中", -1);

    if (isKinopioAttached()) {
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttack::sInstance);
    } else {
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAttack::sInstance);
    }

    return true;
}

bool SpaceCocoon::tryAttackMap() {
    if (!MR::isBinded(this)) {
        return false;
    }

    if (MR::isLessStep(this, 20)) {
        return false;
    }

    bool attackSuccess = true;
    for (s32 plane = 0; plane < MR::getBindedPlaneNum(this); plane++) {
        HitSensor* planeSensor = MR::getBindedPlaneSensor(this, plane);
        if (!MR::sendMsgEnemyAttack(planeSensor, getSensor("Attack"))) {
            if (!planeSensor->receiveMessage(ACTMES_IS_BROKEN, getSensor("Attack"))) {
                attackSuccess = false;
            }
        }
    }

    if (attackSuccess) {
        if (isKinopioAttached()) {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttackSuccess::sInstance);
        } else {
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindAttackSuccess::sInstance);
        }
        return false;
    }

    return true;
}

bool SpaceCocoon::isKinopioAttached() const {
    if (isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance) || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvWaitKinopioAimDemo::sInstance) ||
        isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAim::sInstance) || isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttack::sInstance) ||
        isNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAttackSuccess::sInstance)) {
        return true;
    }

    return false;
}

void SpaceCocoon::endBind(const TVec3f& rJumpVec, bool b) {
    MR::endMultiActorCamera(this, mCameraInfo, "ウェイト", true, -1);
    MR::endMultiActorCamera(this, mCameraInfo, "狙い中", true, -1);
    MR::endMultiActorCamera(this, mCameraInfo, "攻撃中", true, -1);
    MR::deleteEffect(mRider, "SpaceCocoonBlur");

    if (!b) {
        if (isKinopioAttached()) {
            MR::validateClipping(mRider);
            mRider->mVelocity.set(mVelocity);
            mRider->receiveMessage(ACTMES_NPC_EVENT_END, MR::getMessageSensor(), MR::getMessageSensor());
            MR::endDemo(this, "キノピオ狙い中");
        } else {
            MR::endBindAndPlayerJump(this, rJumpVec, 0);
        }
    }

    endCommandStream();
    mVelocity.zero();
    mPosition.set(mNeutralPos);
    setNerve(&NrvSpaceCocoon::SpaceCocoonNrvFreeInvalid::sInstance);
}

void SpaceCocoon::endCommandStream() {
    if (mPadChannel >= 0 && MR::isStarPointerCommandStream(this, mPadChannel)) {
        MR::tryEndStarPointerCommandStream(this, mPadChannel);
        mPadChannel = -1;
    }
}

void SpaceCocoon::draw() const {
    // FIXME: shenanigans afoot at the beginning here, and honestly all over...
    // https://decomp.me/scratch/6froM

    initDraw();

    f32 f1 = -0.5f;
    f32 f2 = 0.0f;

    drawPlane(25.0f, -10.0f, 0.0f, -25.0f, sColor, sColor, f1, f2);  // -0.5f, 0.0f
    f2 = f1;
    f1 += -0.5f;
    drawPlane(25.0f, 10.0f, 25.0f, -10.0f, sColor, sColor, f1, f2);  // -1.0f, -0.5f
    f2 = f1;
    f1 += -0.5f;
    drawPlane(0.0f, 25.0f, 25.0f, 10.0f, sColor, sColor, f1, f2);  // -1.5f, -1.0f
    f2 = f1;
    f1 += -0.5f;
    drawPlane(-25.0f, 10.0f, 0.0f, 25.0f, sColor, sColor, f1, f2);  // -2.0f, -1.5f
    f2 = f1;
    f1 += -0.5f;
    drawPlane(-25.0f, -10.0f, -25.0f, 10.0f, sColor, sColor, f1, f2);  // -2.5f, -2.0f
    f2 = f1;
    f1 += -0.5f;
    drawPlane(0.0f, -25.0f, -25.0f, -10.0f, sColor, sColor, f1, f2);  // -3.0f, -2.5f
}

namespace {
    void drawPoints(const TVec3f& rPos, const TVec3f& rSide, const TVec3f& rFront, f32 thickness, f32 x1, f32 y1, f32 x2, f32 y2, Color8 color1,
                    Color8 color2, f32 texX1, f32 texX2, f32 texY) {
        TVec3f side(rSide);
        TVec3f front(rFront);
        side.scale(thickness);
        front.scale(thickness);

        GXPosition3f32(rPos.x + side.x * x1 + front.x * y1, rPos.y + side.y * x1 + front.y * y1, rPos.z + side.z * x1 + front.z * y1);
        GXColor1u32(color1);
        GXTexCoord2f32(texX1, texY);

        GXPosition3f32(rPos.x + side.x * x2 + front.x * y2, rPos.y + side.y * x2 + front.y * y2, rPos.z + side.z * x2 + front.z * y2);
        GXColor1u32(color2);
        GXTexCoord2f32(texX2, texY);
    }
}  // namespace

void SpaceCocoon::drawPlane(f32 x1, f32 y1, f32 x2, f32 y2, Color8 color1, Color8 color2, f32 texX1, f32 texX2) const {
    s32 numPoints = mNumPoints + 2;
    f32 delta = 1.0f / numPoints;

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, numPoints * 2);

    drawPoints(mCocoonPos, mSide, mFront, 1.0f, x1, y1, x2, y2, color1, color2, texX1, texX2, 0.0f);

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        f32 thickness = mPlantPoints[idx]->mThickness;

        drawPoints(mPlantPoints[idx]->mPosition, mPlantPoints[idx]->mSide, mPlantPoints[idx]->mFront, thickness, x1, y1, x2, y2, color1, color2,
                   texX1, texX2, (idx + 1) * delta);
    }

    drawPoints(mBasePos, mSide, mFront, 2.0f, x1, y1, x2, y2, color1, color2, texX1, texX2, 1.0f);
}

void SpaceCocoon::initDraw() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0x00, 0x00, 0xFF});

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    mTexture->load(GX_TEXMAP0);

    GXSetNumIndStages(0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetZCompLoc(GX_ENABLE);
    GXSetCullMode(GX_CULL_BACK);
}
