#include "Game/Ride/Creeper.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
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
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXVert.h>
#include <revolution/mtx.h>
#include <revolution/wpad.h>

namespace NrvCreeper {
    NEW_NERVE(CreeperNrvFree, Creeper, Free);
    NEW_NERVE(CreeperNrvFreeInvalid, Creeper, FreeInvalid);
    NEW_NERVE(CreeperNrvHangStart, Creeper, HangStart);
    NEW_NERVE(CreeperNrvHangUp, Creeper, HangUp);
    NEW_NERVE(CreeperNrvHangDown, Creeper, HangDown);
};  // namespace NrvCreeper

namespace {
    static const f32 sInterval = 50.0f;
    static const s32 sHeadPointNo = 1;
    static const f32 sDefaultBendRate = 100.0f;
    static const f32 sDefaultJumpSpeedFront = 25.0f;
    static const f32 sDefaultJumpSpeedUp = 40.0f;
    static const f32 sFreeBackAccelRate = 0.05f;
    static const f32 sPointFrictionFree = 0.9f;
    static const f32 sPointFrictionHang = 0.7f;
    static const f32 sHangStartRotateSpeed = 15.0f;
    static const f32 sHangUpAccel = 5.0f;
    static const f32 sHangUpAccel2P = 10.0f;
    static const f32 sHangUpSpeedMin = 5.0f;
    static const f32 sHangUpSpeedMax = 15.0f;
    static const f32 sHangDownGravity = 0.4f;
    static const f32 sHangDownSpeedMax = 20.0f;
    static const f32 sHangRotateSpeedRate = 1.5f;
    static const f32 sHangStartCoordMin = 100.0f;
    static const f32 sHangEndCoord = 80.0f;
    static const s32 sAnimInterpoleFrame = 15;
    static const s32 sStepInvalidSpin = 60;
    static const s32 sStepInvalid = 20;
    static const f32 sDistanceNear = 200.0f;
    static const f32 sDrawWidthX = 10.0f;
    static const f32 sDrawWidthZ = 10.0f;

    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0x96, 0x96, 0x96, 0xFF);
    static Color8 sColorMinusX(0xC8, 0xC8, 0xC8, 0xFF);
};  // namespace

void Creeoer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

CreeperPoint::CreeperPoint(const TVec3f& rPos, const TVec3f& rUp, const CreeperPoint* pPrevPoint)
    : mPosition(rPos), mNeutralPos(rPos), mVelocity(0.0f, 0.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mUp(rUp), mFront(0.0f, 0.0f, 1.0f),
      mProjection(0.0f, 0.0f, 0.0f), mPrevPoint(pPrevPoint) {
    TVec3f front = mSide.cross(mUp);
    if (MR::isNearZero(front)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
    } else {
        MR::makeAxisUpSide(&mFront, &mSide, mUp, mSide);
    }

    if (mPrevPoint == nullptr) {
        return;
    }

    TVec3f posDiff = mPosition - pPrevPoint->mPosition;
    mProjection.x = pPrevPoint->mSide.dot(posDiff);
    mProjection.y = pPrevPoint->mUp.dot(posDiff);
    mProjection.z = pPrevPoint->mFront.dot(posDiff);
}

void CreeperPoint::updateFree() {
    TVec3f restoreVec(mNeutralPos - mPosition);

    mVelocity.add(restoreVec * ::sFreeBackAccelRate);
    mPosition.add(mVelocity);
    mVelocity.mult(::sPointFrictionFree);

    updateLocalAxis();

    mPosition.set(mUp);
    mPosition.scale(::sInterval);
    mPosition.add(mPrevPoint->mPosition);
}

void CreeperPoint::updateBend(bool bend, const TVec3f& bendDirection, f32 t, f32 bendRate) {
    mVelocity.mult(::sPointFrictionHang);

    if (bend) {
        mVelocity.add(bendDirection * t * bendRate);
    }

    mPosition = mPrevPoint->mSide * mProjection.x + mPrevPoint->mUp * mProjection.y + mPrevPoint->mFront * mProjection.z + mPrevPoint->mPosition;

    mPosition.add(mVelocity);

    updateLocalAxis();

    mPosition.set(mUp);
    mPosition.scale(::sInterval);
    mPosition.add(mPrevPoint->mPosition);
}

void CreeperPoint::updateLocalAxis() {
    mUp.set(mPosition);
    mUp.sub(mPrevPoint->mPosition);
    MR::normalize(&mUp);

    TVec3f front = mSide.cross(mUp);
    if (MR::isNearZero(front)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
    } else {
        MR::makeAxisUpSide(&mFront, &mSide, mUp, mSide);
    }
}

Creeper::Creeper(const char* pName)
    : LiveActor(pName), mNumPoints(), mPoints(), mRider(), mFront(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mHangSpeed(), mCoord(),
      mJumpSpeedFront(::sDefaultJumpSpeedFront), mJumpSpeedUp(::sDefaultJumpSpeedUp), mBendDirection(0.0f, 0.0f, 0.0f), mBendRate(::sDefaultBendRate),
      mBendRateBee(-1.0f), mCameraInfo(), mClippingCenter(0.0f, 0.0f, 0.0f), mTexture(), mFlowerModel(), mLeafModel() {
    mTopMtx.identity();
}

void Creeper::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mBendRate);
    MR::getJMapInfoArg1NoInit(rIter, &mJumpSpeedFront);
    MR::getJMapInfoArg2NoInit(rIter, &mJumpSpeedUp);
    MR::getJMapInfoArg3NoInit(rIter, &mBendRateBee);

    mBendRate /= ::sDefaultBendRate;
    if (mBendRateBee > 0.0f) {
        mBendRateBee /= ::sDefaultBendRate;
    } else {
        mBendRateBee = mBendRate;
    }

    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_Creeper);

    initHitSensor(3);
    MR::addHitSensorBinder(this, "bind", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initRailRider(rIter);

    mPosition.set(MR::getRailPos(this));
    initPoints();

    initNerve(GET_NERVE(Creeper, CreeperNrvFree));

    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "掴まり");

    MR::initAndSetRailClipping(&mClippingCenter, this, 100.0f, 500.0f);

    initSound(4, false);

    mTexture = new JUTTexture(MR::loadTexFromArc("Creeper.arc", "Stalk.bti"), 0);

    mFlowerModel = MR::createPartsModelNoSilhouettedMapObj(this, "花（つる花）", "CreeperFlower", mTopMtx.toMtxPtr());
    mLeafModel = MR::createPartsModelNoSilhouettedMapObj(this, "葉（つる花）", "CreeperLeaf", nullptr);

    MR::registerDemoSimpleCastAll(mFlowerModel);
    MR::registerDemoSimpleCastAll(mLeafModel);
    MR::copyTransRotateScale(this, mLeafModel);
    MR::startBck(mLeafModel, "Wait");
    mLeafModel->makeActorAppeared();
    makeActorAppeared();
}

void Creeper::exeFree() {
}

void Creeper::exeFreeInvalid() {
    if (MR::isNearPlayer(this, ::sDistanceNear) && MR::isGreaterStep(this, ::sStepInvalid)) {
        setNerve(GET_NERVE(Creeper, CreeperNrvFree));
    }
}

void Creeper::exeHangStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", ::sAnimInterpoleFrame);
    }

    if (!updateHangUp(::sHangStartRotateSpeed) && MR::isPadSwing(WPAD_CHAN0)) {
        setNerve(GET_NERVE(Creeper, CreeperNrvHangUp));
    }
}

void Creeper::exeHangUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", ::sAnimInterpoleFrame);
    }

    if (!updateHangUp(mHangSpeed * ::sHangRotateSpeedRate) && MR::isPadSwing(WPAD_CHAN0)) {
        mHangSpeed += ::sHangUpAccel;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
}

void Creeper::exeHangDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangDown", ::sAnimInterpoleFrame);
    }

    if (tryHangUp()) {
        return;
    }

    mHangSpeed += ::sHangDownGravity;
    mHangSpeed = MR::clamp(mHangSpeed, 0.0f, ::sHangDownSpeedMax);

    if (MR::isRailReachedNearGoal(this, ::sHangEndCoord)) {
        MR::endMultiActorCamera(this, mCameraInfo, "掴まり", true, -1);
        MR::endBindAndPlayerJump(this, TVec3f(0.0f, 0.0f, 0.0f), 0);
        MR::startSound(mRider, "SE_PV_JUMP_S");
        MR::startSound(this, "SE_OJ_CREEPER_SWING");
        mRider = nullptr;
        setNerve(GET_NERVE(Creeper, CreeperNrvFreeInvalid));
    } else {
        MR::moveCoord(this, mHangSpeed);
        MR::startLevelSound(mRider, "SE_OJ_LV_CREEPER_DOWN");
        calcAndGetCurrentInfo(&mPosition, &mUp);
        MR::rotateVecDegree(&mFront, mUp, mHangSpeed * ::sHangRotateSpeedRate);
    }
}

void Creeper::initPoints() {
    s32 numPoints = MR::getRailTotalLength(this) / ::sInterval;
    mNumPoints = numPoints + 1;

    mPoints = new CreeperPoint*[mNumPoints];

    CreeperPoint* prevPoint = nullptr;

    TVec3f bendSum(0.0f, 0.0f, 0.0f);  // cumulative direction

    for (s32 i = 0; i < mNumPoints - 1; i++) {
        MR::setRailCoord(this, ::sInterval * i);
        mPoints[i] = new CreeperPoint(MR::getRailPos(this), MR::getRailDirection(this), prevPoint);
        bendSum.add(MR::getRailDirection(this));
        prevPoint = mPoints[i];
    }

    MR::moveCoordToEndPos(this);

    CreeperPoint* point = new CreeperPoint(MR::getRailPos(this), MR::getRailDirection(this), prevPoint);
    s32 index = mNumPoints - 1;
    mPoints[index] = point;

    bendSum.add(MR::getRailDirection(this));
    mBendDirection.set(bendSum);
    MR::vecKillElement(mBendDirection, mGravity, &mBendDirection);
    MR::normalize(&mBendDirection);
}

void Creeper::control() {
    if (mRider == nullptr) {
        updatePoints();
        MR::moveCoordAndTransToNearestRailPos(this, *MR::getPlayerPos());
    } else {
        bendPoints();
        mCoord = MR::getRailCoord(this) / MR::getRailTotalLength(this);
    }

    mTopMtx.setXYZDir(getHeadPoint()->mSide, getHeadPoint()->mUp, getHeadPoint()->mFront);
    mTopMtx.setTrans(getHeadPoint()->mPosition);
}

bool Creeper::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (isNerve(GET_NERVE(Creeper, CreeperNrvFreeInvalid)) || (!MR::isPlayerSwingAction() && MR::isOnGroundPlayer())) {
            return false;
        }

        mRider = MR::getSensorHost(pSender);
        MR::startSound(mRider, "SE_PV_CATCH");
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        MR::invalidateClipping(this);
        MR::moveCoordAndTransToNearestRailPos(this, *MR::getPlayerPos());

        if (MR::getRailCoord(this) < ::sHangStartCoordMin) {
            MR::setRailCoord(this, ::sHangStartCoordMin);
        }

        MR::getPlayerFrontVec(&mFront);
        mHangSpeed = -mGravity.dot(*MR::getPlayerVelocity());

        MR::setRailDirectionToEnd(this);
        if (mHangSpeed > 0.0f) {
            mHangSpeed = MR::clamp(mHangSpeed, ::sHangUpSpeedMin, ::sHangUpSpeedMax);
            setNerve(GET_NERVE(Creeper, CreeperNrvHangStart));
        } else {
            mHangSpeed = 0.0f;
            MR::setRailDirectionToStart(this);
            setNerve(GET_NERVE(Creeper, CreeperNrvHangDown));
        }

        MR::tryRumblePadMiddle(this, WPAD_CHAN0);

        MR::startMultiActorCameraNoTarget(this, mCameraInfo, "掴まり", -1);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFrontPos(&mtx, mUp, mFront, mPosition);
        MR::setBaseTRMtx(mRider, mtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        MR::endMultiActorCamera(this, mCameraInfo, "掴まり", true, -1);
        MR::endBindAndPlayerJump(this, TVec3f(0.0f, 0.0f, 0.0f), 0);
        mRider = nullptr;
        setNerve(GET_NERVE(Creeper, CreeperNrvFreeInvalid));
        return true;
    }

    return false;
}

void Creeper::updatePoints() {
    for (s32 i = ::sHeadPointNo; i < mNumPoints; i++) {
        mPoints[i]->updateFree();
    }
}

void Creeper::bendPoints() {
    s32 numPoints = static_cast< s32 >(MR::getRailCoord(this) / ::sInterval) + 1;
    if (numPoints > mNumPoints) {
        numPoints = mNumPoints;
    }

    f32 bendRate = mBendRate;
    if (MR::isPlayerElementModeBee()) {
        bendRate = mBendRateBee;
    }

    for (s32 i = ::sHeadPointNo; i < numPoints; i++) {
        mPoints[i]->updateBend(true, mBendDirection, mCoord, bendRate);
    }

    for (s32 i = numPoints; i < mNumPoints; i++) {
        mPoints[i]->updateBend(false, mBendDirection, mCoord, bendRate);
    }
}

bool Creeper::tryHangUp() {
    if (MR::isPadSwing(WPAD_CHAN0)) {
        // BUG: doubly-reversed logic: should be checking WPAD_CHAN1 and accel are swapped
        if (MR::isPadSwing(WPAD_CHAN0)) {
            mHangSpeed = ::sHangUpAccel;
        } else {
            mHangSpeed = ::sHangUpAccel2P;
        }

        MR::reverseRailDirection(this);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        setNerve(GET_NERVE(Creeper, CreeperNrvHangUp));
        return true;
    }

    return false;
}

bool Creeper::tryJump() {
    if (!MR::isRailReachedGoal(this)) {
        return false;
    }

    MR::moveCoordToEndPos(this);
    mCoord = 1.0f;
    for (s32 i = 0; i < 5; i++) {
        bendPoints();
    }

    calcAndGetCurrentInfo(&mPosition, &mUp);

    TVec3f launchFront = -mBendDirection;

    MR::vecKillElement(launchFront, mGravity, &launchFront);
    MR::normalizeOrZero(&launchFront);
    if (!MR::isNearZero(launchFront)) {
        MR::setPlayerFrontTargetVec(launchFront, 1);
    }

    TVec3f launch;
    launch = launchFront * mJumpSpeedFront - mGravity * mJumpSpeedUp;

    MR::startBckPlayer("GrowPlantJump");
    MR::endMultiActorCamera(this, mCameraInfo, "掴まり", true, -1);
    MR::endBindAndPlayerForceWeakGravityJump(this, launch);

    MR::setPlayerSwingInhibitTimer(::sStepInvalidSpin);
    MR::stopSound(mRider, "SE_OJ_CREEPER_UP_START");
    MR::startSound(mRider, "SE_PV_JUMP_JOY");
    MR::startSound(mRider, "SE_PM_JUMP_L");
    MR::startSound(this, "SE_OJ_CREEPER_FLIP");
    mRider = nullptr;
    setNerve(GET_NERVE(Creeper, CreeperNrvFreeInvalid));

    return true;
}

bool Creeper::updateHangUp(f32 rotateSpeed) {
    if (MR::isPadSwing(WPAD_CHAN0)) {
        mHangSpeed += sHangUpAccel;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    if (MR::isFirstStep(this) && mHangSpeed >= 5.0f) {
        MR::startSound(mRider, "SE_OJ_CREEPER_UP_START");
    }

    mHangSpeed -= 0.4f;

    mHangSpeed = mHangSpeed >= ::sHangUpSpeedMax ? ::sHangUpSpeedMax : mHangSpeed;

    if (mHangSpeed < 0.0f) {
        mHangSpeed = 0.0f;
        MR::reverseRailDirection(this);
        setNerve(GET_NERVE(Creeper, CreeperNrvHangDown));
        return true;
    }

    MR::moveCoord(this, mHangSpeed);
    MR::startLevelSound(mRider, "SE_OJ_LV_CREEPER_UP");

    if (tryJump()) {
        return true;
    }

    calcAndGetCurrentInfo(&mPosition, &mUp);
    MR::rotateVecDegree(&mFront, mUp, rotateSpeed);
    return false;
}

void Creeper::calcAndGetCurrentInfo(TVec3f* pPosition, TVec3f* pUp) const {
    f32 t0 = MR::getRailCoord(this) / ::sInterval;
    s32 i = t0;
    f32 t = t0 - i;

    if (i < mNumPoints - 1) {
        s32 nextIdx = i + 1;
        *pPosition = mPoints[i]->mPosition * (1.0f - t) + mPoints[nextIdx]->mPosition * t;
        *pUp = mPoints[nextIdx]->mPosition - mPoints[i]->mPosition;
    } else {
        s32 prevIdx = i - 1;
        pPosition->set(mPoints[i]->mPosition);
        *pUp = mPoints[i]->mPosition - mPoints[prevIdx]->mPosition;
    }
    MR::normalize(pUp);
}

namespace {
    void sendVertex(const TVec3f& rPos, const TVec3f& rSide, const TVec3f& rFront, f32 x, f32 y, u32 color, f32 texX, f32 texY) {
        GXPosition3f32(rPos.x + rSide.x * x + rFront.x * y, rPos.y + rSide.y * x + rFront.y * y, rPos.z + rSide.z * x + rFront.z * y);
        GXColor1u32(color);
        GXTexCoord2f32(texX, texY);
    }

    void drawPolygon(const TVec3f& rPos, const TVec3f& rSide, const TVec3f& rFront, f32 x1, f32 y1, f32 x2, f32 y2, u32 color1, u32 color2,
                     f32 texY) {
        ::sendVertex(rPos, rSide, rFront, x1, y1, color1, 1.0f, texY);
        ::sendVertex(rPos, rSide, rFront, x2, y2, color2, 0.0f, texY);
    }
};  // namespace

void Creeper::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    loadMaterial();

    f32 delta = 1.0f / (mNumPoints - 1);

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 i = 0; i < mNumPoints; i++) {
        ::drawPolygon(mPoints[i]->getPosition(), mPoints[i]->getSide(), mPoints[i]->getFront(), ::sDrawWidthX, -::sDrawWidthZ, 0.0f, ::sDrawWidthZ,
                      ::sColorPlusX, ::sColorPlusZ, delta * ((mNumPoints - 1) - i));
    }
    GXEnd();

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 i = 0; i < mNumPoints; i++) {
        ::drawPolygon(mPoints[i]->getPosition(), mPoints[i]->getSide(), mPoints[i]->getFront(), 0.0f, ::sDrawWidthZ, -::sDrawWidthX, -::sDrawWidthZ,
                      ::sColorPlusZ, ::sColorMinusX, delta * ((mNumPoints - 1) - i));
    }
    GXEnd();

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 i = 0; i < mNumPoints; i++) {
        ::drawPolygon(mPoints[i]->getPosition(), mPoints[i]->getSide(), mPoints[i]->getFront(), -::sDrawWidthX, -::sDrawWidthZ, ::sDrawWidthX,
                      -::sDrawWidthZ, ::sColorMinusX, ::sColorPlusX, delta * ((mNumPoints - 1) - i));
    }
    GXEnd();
}

void Creeper::loadMaterial() const {
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

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    mTexture->load(GX_TEXMAP0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetZCompLoc(GX_ENABLE);
    GXSetCullMode(GX_CULL_BACK);
}

Creeper::~Creeper() {
}
