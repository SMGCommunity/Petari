#include "Game/Enemy/Pukupuku.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/ValueControl.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/os.h"

namespace {
    NEW_NERVE(PukupukuStateLandingLandingMoveLand, PukupukuStateLanding, LandingMoveLand);
    NEW_NERVE(PukupukuStateLandingLandingJumpFromWater, PukupukuStateLanding, LandingJumpFromWater);
    NEW_NERVE(PukupukuStateLandingLandingJumpFromLand, PukupukuStateLanding, LandingJumpFromLand);

    NEW_NERVE(PukupukuWait, Pukupuku, Wait);
    NEW_NERVE(PukupukuMoveWater, Pukupuku, MoveWater);
    NEW_NERVE(PukupukuMoveWaterAfterJump, Pukupuku, MoveWaterAfterJump);
    NEW_NERVE(PukupukuMoveWaterAfterJumpAfterPointing, Pukupuku, MoveWaterAfterJumpAfterPointing);
    NEW_NERVE(PukupukuLanding, Pukupuku, Landing);
    NEW_NERVE(PukupukuTrampled, Pukupuku, Trampled);
    NEW_NERVE(PukupukuBlownOff, Pukupuku, BlownOff);
    NEW_NERVE(PukupukuBindStarPointer, Pukupuku, BindStarPointer);

    const char* cBck2BtpTable[8] = {"Bound", "CloseEye", "FlyStart", "OpenEye", "FlyEnd", "OpenEye", "SwimStart", "OpenEye"};
};  // namespace

PukupukuStateLanding::PukupukuStateLanding(Pukupuku* pParent) : ActorStateBase< Pukupuku >("プクプク地上") {
    mParent = pParent;
    mPath = nullptr;
    mValueCtrl = nullptr;
    _18 = 0;
    mPath = new ParabolicPath();
    mValueCtrl = new ValueControl(30);
    initNerve(&PukupukuStateLandingLandingMoveLand::sInstance);
    kill();
}

/* https://decomp.me/scratch/JXtwR */
void PukupukuStateLanding::exeLandingMoveLand() {
    if (MR::isFirstStep(this)) {
        setupJumping(300.0f, 15.0f);
        mParent->startAnim("Bound", "CloseEye");
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    mParent->updatePoseByRailIgnoreUpScale();

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mParent) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* parent = mParent;
        MR::moveCoordToRailPoint(parent, MR::getNextRailPointNo(parent));

        if (MR::isBinded(mParent)) {
            mParent->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromWater() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        mParent->rotatePoseByLocalZ();
        mParent->startAnim("FlyStart", "OpenEye");
        MR::startSound(mParent, "SE_EM_PUKUPUKU_WATER_OUT", -1, -1);
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(true, false);
    updatePoseByJumpPath(0.15f);

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mParent) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* parent = mParent;
        MR::moveCoordToRailPoint(parent, MR::getNextRailPointNo(parent));

        if (MR::isBinded(mParent)) {
            mParent->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromLand() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        mParent->startAnim("FlyEnd", "OpenEye");
        MR::startSound(mParent, "SE_EM_PUKUPUKU_WING", -1, -1);
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(false, false);
    updatePoseByJumpPath(0.15f);

    if (mValueCtrl->isFrameMaxFrame()) {
        MR::moveCoordAndTransToNextPoint(mParent);
        emitWaterColumIfNeed(false, true);
        kill();
    }
}

void PukupukuStateLanding::updatePoseByJumpPath(f32 a1) {
    TVec3f direction;
    mPath->calcDirection(&direction, mValueCtrl->getValue(), 0.009f);
    MR::normalize(&direction);
    TVec3f v7 = -mParent->mVelocity;
    Pukupuku* parent = mParent;
    if (!MR::isSameDirection(direction, v7, 0.009f)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, direction, v7);
        parent->_A8.slerp(quat, a1);
    }
}

void PukupukuStateLanding::setNerveAfterJumpAccordingToNextPoint() {
    if (mParent->isReadyToJumpFromLand()) {
        setNerve(&PukupukuStateLandingLandingJumpFromLand::sInstance);
    } else {
        setNerve(&PukupukuStateLandingLandingMoveLand::sInstance);
    }
}

void PukupukuStateLanding::setupJumping(f32 a1, f32 a2) {
    TVec3f curPos;
    mParent->getCurrentRailPointPos(&curPos);
    TVec3f nextPos;
    mParent->getNextRailPointPos(&nextPos);

    TVec3f v16(mParent->mPosition);
    v16.sub(curPos);
    TVec3f v15 = -mParent->mGravity;
    f32 dot = v15.dot(v16);

    if (0.0f < dot) {
        a1 = (a1 - dot);
    }

    Pukupuku* parent = mParent;
    TVec3f v14 = -parent->mGravity;
    mPath->initFromUpVectorAddHeight(parent->mPosition, nextPos, v14, a1);
    f32 speed = mPath->calcPathSpeedFromAverageSpeed(a2);
    f32 v19 = (1.0f / speed);
    mValueCtrl->setMaxFrame(v19);
    mValueCtrl->setDirToOneResetFrame();
}

void PukupukuStateLanding::updateJumping() {
    TVec3f pos;
    mPath->calcPosition(&pos, mValueCtrl->getValue());
    pos.sub(mParent->mPosition);
    mParent->mVelocity.set< f32 >(pos);
}

void PukupukuStateLanding::emitWaterColumIfNeed(bool a1, bool a2) {
    if (!_18) {
        TVec3f centerJointPos;
        MR::copyJointPos(mParent, "Center", &centerJointPos);

        if (a2 || a1 != MR::isInWater(centerJointPos)) {
            TPos3f hitMtx;
            mParent->calcGroundHitMtx(&hitMtx);
            f32 num = 1.5f;
            hitMtx.mMtx[0][3] = centerJointPos.x;
            hitMtx.mMtx[1][3] = centerJointPos.y;
            hitMtx.mMtx[2][3] = centerJointPos.z;
            hitMtx.mMtx[0][0] = hitMtx.mMtx[0][0] * num;
            hitMtx.mMtx[0][1] = hitMtx.mMtx[0][1] * num;
            hitMtx.mMtx[0][2] = hitMtx.mMtx[0][2] * num;
            hitMtx.mMtx[1][0] = hitMtx.mMtx[1][0] * num;
            hitMtx.mMtx[1][1] = hitMtx.mMtx[1][1] * num;
            hitMtx.mMtx[1][2] = hitMtx.mMtx[1][2] * num;
            hitMtx.mMtx[2][0] = hitMtx.mMtx[2][0] * num;
            hitMtx.mMtx[2][1] = hitMtx.mMtx[2][1] * num;
            hitMtx.mMtx[2][2] = hitMtx.mMtx[2][2] * num;
            MR::emitEffectHit(mParent, hitMtx, "WaterColumn");
            _18 = 1;
        }
    }
}

void PukupukuStateLanding::emitGroundHitEffect() {
    if (!MR::isBindedGround(mParent)) {
        Triangle triangle;
        Pukupuku* parent = mParent;
        TVec3f v3(parent->mGravity);
        v3.scale(100.0f);

        TVec3f poly;
        if (MR::getFirstPolyOnLineToMap(&poly, &triangle, parent->mPosition, v3)) {
            MR::updateEffectFloorCode(mParent, &triangle);
        }
    }

    TPos3f hitMtx;
    mParent->calcGroundHitMtx(&hitMtx);
    MR::emitEffectHit(mParent, hitMtx, "Land");
    MR::startSound(mParent, "SE_EM_PUKUPUKU_LAND", -1, -1);
}

Pukupuku::Pukupuku(const char* pName) : LiveActor(pName) {
    mStateLanding = nullptr;
    mScaleCtrl = nullptr;
    mStarPointer = nullptr;
    _C0 = 0;
    _C4 = 0;
    _C8 = 0.0f;
    _CC = 0.0f;
    _D3 = 0;
    _90.zero();
    _9C.zero();
    _A8.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
}

void Pukupuku::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::onCalcGravity(this);
    MR::useStageSwitchWriteDead(this, rIter);
    initModelManagerWithAnm("Pukupuku", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensorAtJoint(this, "body", "center", 34, 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(70.0f, 0.0f, 0);
    MR::setBinderOffsetVec(this, &_9C, false);
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    TRot3f mtx;
    MR::makeMtxTR(mtx, this);
    mtx.getQuat(_A8);
    updatePoseByRail();
    MR::initAndSetRailClipping(&_90, this, 100.0f, 500.0f);
    initEffectKeeper(1, nullptr, false);
    MR::declareCoin(this, 1);
    MR::declareStarPiece(this, 3);
    MR::initShadowVolumeSphere(this, 50.0f);
    initSound(8, false);
    initNerve(&PukupukuWait::sInstance);
    mStateLanding = new PukupukuStateLanding(this);
    mStateLanding->kill();
    TVec3f v8;
    v8.x = 0.0f;
    v8.y = 0.0f;
    v8.z = 0.0f;
    MR::initStarPointerTarget(this, 60.0f, v8);
    mScaleCtrl = new AnimScaleController(nullptr);
    mStarPointer = new WalkerStateBindStarPointer(this, mScaleCtrl);
    MR::addToAttributeGroupSearchTurtle(this);
    makeActorAppeared();
}

void Pukupuku::kill() {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::emitEffect(this, "DeathWater");
        MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER", -1, -1);
    } else {
        MR::emitEffect(this, "Death");
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    }

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (_D3) {
        MR::appearCoinPop(this, mPosition, 1);
    } else {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W", -1, -1);
        } else {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }
    }

    LiveActor::kill();
}

void Pukupuku::updatePoseByRailIgnoreUpScale() {
    TVec3f v4 = -mGravity;
    const TVec3f* railDir = &MR::getRailDirection(this);
    if (!MR::isSameDirection(*railDir, v4, 0.009f)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, *railDir, v4);
        _A8.slerp(quat, 0.079f);
    }
}

void Pukupuku::rotatePoseByLocalZ() {
    Quaternion quat;
    f32 _s = sin(1.570796370506287);
    f32 _c = cos(1.570796370506287);
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = _s;
    quat.w = _c;
    PSQUATMultiply(&_A8, &quat, &_A8);
}

void Pukupuku::calcGroundHitMtx(TPos3f* pMtx) const {
    TVec3f zDir;
    _A8.getZDir(zDir);
    MR::calcMtxFromGravityAndZAxis(pMtx, this, mGravity, zDir);
}

bool Pukupuku::isReadyToJumpFromLand() const {
    s32 arg0 = -1;
    MR::getRailPointArg0NoInit(this, MR::getCurrentRailPointNo(this), &arg0);
    return arg0 == 1;
}

void Pukupuku::exeWait() {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        setNerve(&PukupukuMoveWater::sInstance);
    } else {
        s32 pointNum = MR::getRailPointNum(this);

        for (s32 i = 0; i < pointNum; i++) {
            s32 arg = -1;
            MR::getRailPointArg0NoInit(this, MR::getCurrentRailPointNo(this), &arg);
        }

        PukupukuStateLanding* state = mStateLanding;
        state->appear();
        state->setNerve(&PukupukuStateLandingLandingMoveLand::sInstance);
        setNerve(&PukupukuLanding::sInstance);
    }
}

void Pukupuku::exeMoveWater() {
    bool v2 = false;

    if (isNerve(&PukupukuTrampled::sInstance) || isNerve(&PukupukuBlownOff::sInstance)) {
        v2 = true;
    }

    if (!v2 && !tryBindStarPointer()) {
        if (MR::isFirstStep(this)) {
            startAnim("Swim", "OpenEye");
        }

        MR::startLevelSound(this, "SE_EM_LV_PUKUPUKU_SWIM", -1, -1, -1);
        if (MR::isEqualStringCase(MR::getPlayingBckName(this), "BodyAttack")) {
            if (MR::isBckStopped(this)) {
                startAnim("Swim", "OpenEye");
            }
        }

        exeMoveWaterCommon();
    }
}

void Pukupuku::exeMoveWaterAfterJump() {
    if (MR::isFirstStep(this)) {
        startAnim("SwimStart", "OpenEye");
        rotatePoseByLocalZ();
        MR::startSound(this, "SE_EM_PUKUPUKU_WATER_IN", -1, -1);
    }

    updateMoveWaterAfterJump();
}

void Pukupuku::exeLanding() {
    if (!tryBindStarPointer()) {
        mStateLanding->update();
        if (mStateLanding->mIsDead) {
            setNerve(&PukupukuMoveWaterAfterJump::sInstance);
        }
    }
}

void Pukupuku::exeTrampled() {
    if (MR::isFirstStep(this)) {
        TPos3f hitMtx;
        calcGroundHitMtx(&hitMtx);
        hitMtx.getQuat(_A8);
        TVec3f v4(mGravity);
        v4.scale(10.0f);
        mVelocity.set< f32 >(v4);
        startAnim("Flat", "CloseEye");
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
    } else if (!MR::isBinded(this)) {
        mVelocity.x *= 0.88999999f;
        mVelocity.y *= 0.88999999f;
        mVelocity.z *= 0.88999999f;
    }

    if (MR::isStep(this, 30)) {
        kill();
    }
}

void Pukupuku::exeBlownOff() {
    if (MR::isFirstStep(this)) {
        startAnim("Rotate", "CloseEye");

        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::emitEffect(this, "SpinDamageWater");
        } else {
            MR::emitEffect(this, "SpinDamage");
        }

        MR::startBlowHitSound(this);
    } else if (!MR::isBinded(this)) {
        TVec3f v11(mGravity);
        v11.scale(getBlownOffSpeedRate());
        mVelocity.addInline(v11);
    }

    MR::stopSceneAtStep(this, 2, 4);
    if (MR::isStep(this, 15)) {
        kill();
    }
}

/* todo -- figure out what is going on here */
void Pukupuku::exeBindStarPointer() {
    if (MR::updateActorStateAndNextNerve(this, mStarPointer, _C4) && _C4 != &PukupukuMoveWater::sInstance && _C8 != 0.0f) {
        const char* btp = nullptr;
        const char* val = _C0;

        u32 v3 = 0;
        u32 v4 = 0;

        do {
            if (MR::isEqualStringCase(cBck2BtpTable[v4], val)) {
                btp = cBck2BtpTable[v4 + 1];
                break;
            }

            v3++;
            v4 += 2;
        } while (v3 < 4);

        MR::startBck(this, _C0, nullptr);
        MR::startBtp(this, btp);
        MR::setBckFrame(this, _C8);
        MR::setBtpFrame(this, _CC);
    }
}

void Pukupuku::updatePoseByRail() {
    TVec3f yDir;
    _A8.getYDir(yDir);

    const TVec3f* railDir = &MR::getRailDirection(this);
    if (!MR::isSameDirection(*railDir, yDir, 0.009999999f)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, *railDir, yDir);
        _A8.slerp(quat, 0.079999998f);
    }
}

void Pukupuku::updateMoveWaterAfterJump() {
    if (!tryBindStarPointer()) {
        exeMoveWaterCommon();

        if (MR::isBckStopped(this)) {
            setNerve(&PukupukuMoveWater::sInstance);
        }
    }
}

bool Pukupuku::tryBindStarPointer() {
    if (mStarPointer->tryStartPointBind()) {
        _C4 = 0;
        _C8 = 0.0f;
        _C0 = MR::getPlayingBckName(this);

        if (isNerve(&PukupukuMoveWater::sInstance)) {
            _C4 = &PukupukuMoveWater::sInstance;
        }

        if (isNerve(&PukupukuMoveWaterAfterJump::sInstance) || isNerve(&PukupukuMoveWaterAfterJumpAfterPointing::sInstance)) {
            _C4 = &PukupukuMoveWaterAfterJumpAfterPointing::sInstance;
        }

        if (isNerve(&PukupukuLanding::sInstance)) {
            _C4 = &PukupukuLanding::sInstance;
        }

        _C8 = MR::getBckFrame(this);
        _CC = MR::getBtpFrame(this);
        setNerve(&PukupukuBindStarPointer::sInstance);
        return true;
    }

    return false;
}

void Pukupuku::exeMoveWaterCommon() {
    MR::moveCoordAndFollowTrans(this, 5.0f);
    updatePoseByRail();

    s32 arg0 = -1;
    MR::getRailPointArg0NoInit(this, MR::getCurrentRailPointNo(this), &arg0);

    if (arg0 == 1) {
        PukupukuStateLanding* state = mStateLanding;
        state->appear();
        state->setNerve(&PukupukuStateLandingLandingJumpFromWater::sInstance);
        setNerve(&PukupukuLanding::sInstance);
    }
}

f32 Pukupuku::getBlownOffSpeedRate() const {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        return 0.6f;
    } else {
        return 1.0f;
    }
}

void Pukupuku::control() {
    _9C.set< f32 >(-mGravity);
    _9C.x *= 70.0f;
    _9C.y *= 70.0f;
    _9C.z *= 70.0f;
    mScaleCtrl->updateNerve();

    if (isNerve(&PukupukuBindStarPointer::sInstance)) {
        MR::setSensorRadius(this, "body", 100.0f);
    } else {
        MR::setSensorRadius(this, "body", 60.0f);
    }
}

// ...

void Pukupuku::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A8);
    TVec3f v7(mScale);
    JMathInlineVEC::PSVECMultiply(&v7, &mScaleCtrl->_C, &v7);
    MR::setBaseScale(this, v7);
}

PukupukuStateLanding::~PukupukuStateLanding() {}

Pukupuku::~Pukupuku() {}
