#include "Game/Ride/Creeper.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
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
}  // namespace NrvCreeper

namespace {
    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0x96, 0x96, 0x96, 0xFF);
    static Color8 sColorMinusX(0xC8, 0xC8, 0xC8, 0xFF);
}  // namespace

CreeperPoint::CreeperPoint(const TVec3f& rPos, const TVec3f& rUp, const CreeperPoint* pPrevPoint)
    : mPosition(rPos), mNeutralPos(rPos), mVelocity(0.0f, 0.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mUp(rUp), mFront(0.0f, 0.0f, 1.0f),
      mProjection(0.0f, 0.0f, 0.0f), mPrevPoint(pPrevPoint) {
    TVec3f front;
    PSVECCrossProduct(&mSide, &mUp, &front);
    if (MR::isNearZero(front, 0.001f)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
    } else {
        MR::makeAxisUpSide(&mFront, &mSide, mUp, mSide);
    }

    if (mPrevPoint == nullptr) {
        return;
    }

    TVec3f posDiff(mPosition);
    posDiff.sub(pPrevPoint->mPosition);
    mProjection.x = pPrevPoint->mSide.dot(posDiff);
    mProjection.y = pPrevPoint->mUp.dot(posDiff);
    mProjection.z = pPrevPoint->mFront.dot(posDiff);
}

void CreeperPoint::updateFree() {
    TVec3f restoreVec(mNeutralPos);
    restoreVec.sub(mPosition);

    TVec3f v2(restoreVec);
    v2.scale(0.05f);

    mVelocity.add(v2);

    mPosition.add(mVelocity);

    mVelocity.mult(0.9f);

    updateLocalAxis();

    // mPosition = mPrevPoint->mPosition + mUp * 50.0f;
    mPosition.set(mUp);
    mPosition.scale(50.0f);
    mPosition.add(mPrevPoint->mPosition);
}

void CreeperPoint::updateBend(bool bend, const TVec3f& bendDirection, f32 t, f32 bendFactor) {
    mVelocity.mult(0.7f);

    if (bend) {
        mVelocity.add(bendDirection.scaleInline(t).scaleInline(bendFactor));
    }

    mPosition = mPrevPoint->mSide.scaleInline(mProjection.x)
                    .addOperatorInLine(mPrevPoint->mUp.scaleInline(mProjection.y))
                    .addOperatorInLine(mPrevPoint->mFront.scaleInline(mProjection.z))
                    .addOperatorInLine(mPrevPoint->mPosition);

    mPosition.add(mVelocity);

    updateLocalAxis();

    // mPosition = mPrevPoint->mPosition + mUp * 50.0f;
    mPosition.set(mUp);
    mPosition.scale(50.0f);
    mPosition.add(mPrevPoint->mPosition);
}

void CreeperPoint::updateLocalAxis() {
    mUp.set(mPosition);
    mUp.sub(mPrevPoint->mPosition);
    MR::normalize(&mUp);

    TVec3f front;
    PSVECCrossProduct(&mSide, &mUp, &front);
    if (MR::isNearZero(front, 0.001f)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
    } else {
        MR::makeAxisUpSide(&mFront, &mSide, mUp, mSide);
    }
}

Creeper::Creeper(const char* pName)
    : LiveActor(pName), mNumPoints(0), mPoints(nullptr), mRider(nullptr), mFront(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mClimbSpeed(0.0f),
      mCoord(0), mLaunchHorizontalSpeed(25.0f), mLaunchVerticalSpeed(40.0f), mBendDirection(0.0f, 0.0f, 0.0f), mBendFactorNormal(100.0f),
      mBendFactorBee(-1.0f), mCameraInfo(nullptr), mClippingCenter(0.0f, 0.0f, 0.0f), mTexture(nullptr), mFlowerModel(nullptr), mLeafModel(nullptr) {
    mTopMtx.identity();
}

void Creeper::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mBendFactorNormal);
    MR::getJMapInfoArg1NoInit(rIter, &mLaunchHorizontalSpeed);
    MR::getJMapInfoArg2NoInit(rIter, &mLaunchVerticalSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mBendFactorBee);

    mBendFactorNormal /= 100.0f;
    if (mBendFactorBee > 0.0f) {
        mBendFactorBee /= 100.0f;
    } else {
        mBendFactorBee = mBendFactorNormal;
    }

    MR::connectToScene(this, MR::MovementType_Ride, -1, -1, MR::DrawType_Creeper);

    initHitSensor(3);
    MR::addHitSensorBinder(this, "bind", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initRailRider(rIter);

    mPosition.set(MR::getRailPos(this));
    initPoints();

    initNerve(&NrvCreeper::CreeperNrvFree::sInstance);

    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "掴まり");

    MR::initAndSetRailClipping(&mClippingCenter, this, 100.0f, 500.0f);

    initSound(4, false);

    mTexture = new JUTTexture(MR::loadTexFromArc("Creeper.arc", "Stalk.bti"), 0);

    mFlowerModel = MR::createPartsModelNoSilhouettedMapObj(this, "花（つる花）", "CreeperFlower", reinterpret_cast< MtxPtr >(&mTopMtx));
    mLeafModel = MR::createPartsModelNoSilhouettedMapObj(this, "葉（つる花）", "CreeperLeaf", nullptr);

    MR::registerDemoSimpleCastAll(reinterpret_cast< LiveActor* >(mFlowerModel));
    MR::registerDemoSimpleCastAll(reinterpret_cast< LiveActor* >(mLeafModel));
    MR::copyTransRotateScale(this, reinterpret_cast< LiveActor* >(mLeafModel));
    MR::startBck(reinterpret_cast< LiveActor* >(mLeafModel), "Wait", static_cast< const char* >(nullptr));
    mLeafModel->makeActorAppeared();
    makeActorAppeared();
}

inline void Creeper::exeFree() {}

inline void Creeper::exeFreeInvalid() {
    if (MR::isNearPlayer(this, 200.0f) && MR::isGreaterStep(this, 20)) {
        setNerve(&NrvCreeper::CreeperNrvFree::sInstance);
    }
}

void Creeper::exeHangStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantCatch", 15);
    }

    if (!updateHangUp(15.0f) && MR::isPadSwing(WPAD_CHAN0)) {
        setNerve(&NrvCreeper::CreeperNrvHangUp::sInstance);
    }
}

void Creeper::exeHangUp() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangUp", 15);
    }

    if (!updateHangUp(mClimbSpeed * 1.5f) && MR::isPadSwing(WPAD_CHAN0)) {
        mClimbSpeed += 5.0f;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
}

void Creeper::exeHangDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("GrowPlantHangDown", 15);
    }

    if (tryHangUp()) {
        return;
    }

    mClimbSpeed += 0.4f;
    mClimbSpeed = MR::clamp(mClimbSpeed, 0.0f, 20.0f);

    if (MR::isRailReachedNearGoal(this, 80.0f)) {
        MR::endMultiActorCamera(this, mCameraInfo, "掴まり", true, -1);
        MR::endBindAndPlayerJump(this, TVec3f(0.0f, 0.0f, 0.0f), 0);
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
        MR::startSound(this, "SE_OJ_CREEPER_SWING", -1, -1);
        mRider = nullptr;
        setNerve(&NrvCreeper::CreeperNrvFreeInvalid::sInstance);
    } else {
        MR::moveCoord(this, mClimbSpeed);
        MR::startLevelSound(mRider, "SE_OJ_LV_CREEPER_DOWN", -1, -1, -1);
        calcAndGetCurrentInfo(&mPosition, &mUp);
        MR::rotateVecDegree(&mFront, mUp, mClimbSpeed * 1.5f);
    }
}

void Creeper::initPoints() {
    s32 numPoints = MR::getRailTotalLength(this) / 50.0f;
    mNumPoints = numPoints + 1;

    mPoints = new CreeperPoint*[mNumPoints];

    CreeperPoint* prevPoint = nullptr;

    TVec3f bendSum(0.0f, 0.0f, 0.0f);  // cumulative direction

    for (s32 idx = 0; idx < mNumPoints - 1; idx++) {
        MR::setRailCoord(this, 50.0f * idx);
        mPoints[idx] = new CreeperPoint(MR::getRailPos(this), MR::getRailDirection(this), prevPoint);
        bendSum.add(MR::getRailDirection(this));
        prevPoint = mPoints[idx];
    }

    MR::moveCoordToEndPos(this);

    // compiler likes to optimize this direct access if not written like this... very odd
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

    mTopMtx.setVec(getHeadPoint()->mSide, getHeadPoint()->mUp, getHeadPoint()->mFront);
    mTopMtx.setPos(getHeadPoint()->mPosition);
}

bool Creeper::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (isNerve(&NrvCreeper::CreeperNrvFreeInvalid::sInstance) || (!MR::isPlayerSwingAction() && MR::isOnGroundPlayer())) {
            return false;
        }

        mRider = MR::getSensorHost(pSender);
        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::invalidateClipping(this);
        MR::moveCoordAndTransToNearestRailPos(this, *MR::getPlayerPos());

        if (MR::getRailCoord(this) < 100.0f) {
            MR::setRailCoord(this, 100.0f);
        }

        MR::getPlayerFrontVec(&mFront);
        mClimbSpeed = -mGravity.dot(*MR::getPlayerVelocity());

        MR::setRailDirectionToEnd(this);
        if (mClimbSpeed > 0.0f) {
            mClimbSpeed = MR::clamp(mClimbSpeed, 5.0f, 15.0f);
            setNerve(&NrvCreeper::CreeperNrvHangStart::sInstance);
        } else {
            mClimbSpeed = 0.0f;
            MR::setRailDirectionToStart(this);
            setNerve(&NrvCreeper::CreeperNrvHangDown::sInstance);
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
        setNerve(&NrvCreeper::CreeperNrvFreeInvalid::sInstance);
        return true;
    }

    return false;
}

void Creeper::updatePoints() {
    for (s32 idx = 1; idx < mNumPoints; idx++) {
        mPoints[idx]->updateFree();
    }
}

void Creeper::bendPoints() {
    s32 numPoints = static_cast< s32 >(MR::getRailCoord(this) / 50.0f) + 1;
    if (numPoints > mNumPoints) {
        numPoints = mNumPoints;
    }

    f32 bendFactor = mBendFactorNormal;
    if (MR::isPlayerElementModeBee()) {
        bendFactor = mBendFactorBee;
    }

    for (s32 idx = 1; idx < numPoints; idx++) {
        mPoints[idx]->updateBend(true, mBendDirection, mCoord, bendFactor);
    }

    for (s32 idx = numPoints; idx < mNumPoints; idx++) {
        mPoints[idx]->updateBend(false, mBendDirection, mCoord, bendFactor);
    }
}

bool Creeper::tryHangUp() {
    if (MR::isPadSwing(WPAD_CHAN0)) {
        // strange
        if (MR::isPadSwing(WPAD_CHAN0)) {
            mClimbSpeed = 5.0f;
        } else {
            mClimbSpeed = 10.0f;
        }

        MR::reverseRailDirection(this);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        setNerve(&NrvCreeper::CreeperNrvHangUp::sInstance);
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

    TVec3f launchFront = mBendDirection.negateInline();

    MR::vecKillElement(launchFront, mGravity, &launchFront);
    MR::normalizeOrZero(&launchFront);
    if (!MR::isNearZero(launchFront, 0.001f)) {
        MR::setPlayerFrontTargetVec(launchFront, 1);
    }

    TVec3f launch;
    launch = (launchFront.scaleInline(mLaunchHorizontalSpeed)).subOperatorInLine(mGravity.scaleInline(mLaunchVerticalSpeed));

    MR::startBckPlayer("GrowPlantJump", static_cast< const char* >(nullptr));
    MR::endMultiActorCamera(this, mCameraInfo, "掴まり", true, -1);
    MR::endBindAndPlayerForceWeakGravityJump(this, launch);

    MR::setPlayerSwingInhibitTimer(60);
    MR::stopSound(mRider, "SE_OJ_CREEPER_UP_START", 0);
    MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
    MR::startSound(mRider, "SE_PM_JUMP_L", -1, -1);
    MR::startSound(this, "SE_OJ_CREEPER_FLIP", -1, -1);
    mRider = nullptr;
    setNerve(&NrvCreeper::CreeperNrvFreeInvalid::sInstance);

    return true;
}

bool Creeper::updateHangUp(f32 angleVel) {
    if (MR::isPadSwing(WPAD_CHAN0)) {
        mClimbSpeed += 5.0f;
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    if (MR::isFirstStep(this) && mClimbSpeed >= 5.0f) {
        MR::startSound(mRider, "SE_OJ_CREEPER_UP_START", -1, -1);
    }

    mClimbSpeed -= 0.4f;

    mClimbSpeed = mClimbSpeed >= 15.0f ? 15.0f : mClimbSpeed;

    if (mClimbSpeed < 0.0f) {
        mClimbSpeed = 0.0f;
        MR::reverseRailDirection(this);
        setNerve(&NrvCreeper::CreeperNrvHangDown::sInstance);
        return true;
    }

    MR::moveCoord(this, mClimbSpeed);
    MR::startLevelSound(mRider, "SE_OJ_LV_CREEPER_UP", -1, -1, -1);

    if (tryJump()) {
        return true;
    }

    calcAndGetCurrentInfo(&mPosition, &mUp);
    MR::rotateVecDegree(&mFront, mUp, angleVel);
    return false;
}

void Creeper::calcAndGetCurrentInfo(TVec3f* pPosition, TVec3f* pUp) const {
    f32 t0 = MR::getRailCoord(this) / 50.0f;
    s32 idx = t0;
    f32 t = t0 - idx;

    if (idx < mNumPoints - 1) {
        s32 nextIdx = idx + 1;
        *pPosition = (mPoints[idx]->mPosition.scaleInline(1.0f - t)).addOperatorInLine(mPoints[nextIdx]->mPosition.scaleInline(t));
        *pUp = (mPoints[nextIdx]->mPosition).subOperatorInLine(mPoints[idx]->mPosition);
    } else {
        s32 prevIdx = idx - 1;
        pPosition->set(mPoints[idx]->mPosition);
        *pUp = (mPoints[idx]->mPosition).subOperatorInLine(mPoints[prevIdx]->mPosition);
    }
    MR::normalize(pUp);
}

namespace {
    void sendVertex(const TVec3f& rPos, const TVec3f& rSide, const TVec3f& rFront, f32 x, f32 y, u32 color, f32 texX, f32 texY) {
        GXPosition3f32(rPos.x + rSide.x * x + rFront.x * y, rPos.y + rSide.y * x + rFront.y * y, rPos.z + rSide.z * x + rFront.z * y);
        GXColor1u32(color);
        GXTexCoord2f32(texX, texY);
    }
}  // namespace

void Creeper::draw() const {
    // FIXME register scheduling issues :/
    // https://decomp.me/scratch/Rx30C

    if (!MR::isValidDraw(this)) {
        return;
    }

    loadMaterial();

    f32 delta = 1.0f / (mNumPoints - 1);

    u32 color1, color2;
    const TVec3f* front;
    const TVec3f* side;
    const TVec3f* pos;

    f32 texY;

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        texY = delta * ((mNumPoints - 1) - idx);
        color1 = sColorPlusX;
        color2 = sColorPlusZ;

        front = &mPoints[idx]->mFront;
        side = &mPoints[idx]->mSide;
        pos = &mPoints[idx]->mPosition;

        sendVertex(*pos, *side, *front, 10.0f, -10.0f, color1, 1.0f, texY);
        sendVertex(*pos, *side, *front, 0.0f, 10.0f, color2, 0.0f, texY);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        texY = delta * ((mNumPoints - 1) - idx);
        color1 = sColorPlusZ;
        color2 = sColorMinusX;

        front = &mPoints[idx]->mFront;
        side = &mPoints[idx]->mSide;
        pos = &mPoints[idx]->mPosition;

        sendVertex(*pos, *side, *front, 0.0f, 10.0f, color1, 1.0f, texY);
        sendVertex(*pos, *side, *front, -10.0f, -10.0f, color2, 0.0f, texY);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mNumPoints * 2);
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        texY = delta * ((mNumPoints - 1) - idx);
        color1 = sColorMinusX;
        color2 = sColorPlusX;

        front = &mPoints[idx]->mFront;
        side = &mPoints[idx]->mSide;
        pos = &mPoints[idx]->mPosition;

        sendVertex(*pos, *side, *front, -10.0f, -10.0f, color1, 1.0f, texY);
        sendVertex(*pos, *side, *front, 10.0f, -10.0f, color2, 0.0f, texY);
    }
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

const CreeperPoint* Creeper::getHeadPoint() const {
    return getPoint(mNumPoints - 1);
}
