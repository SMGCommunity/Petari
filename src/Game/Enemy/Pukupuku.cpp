#include "Game/Enemy/Pukupuku.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/ValueControl.hpp"
#include <JSystem/JMath/JMath.hpp>

void Pukupuku_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
    (void)300.0f;
    (void)15.0f;
    (void)500.0f;
    (void)0.15f;
    (void)0.01f;
    (void)1.5f;
    (void)100.0f;
    (void)60.0f;
    (void)70.0f;
    (void)50.0f;
    (void)10.0f;
    (void)40.0f;
    (void)0.08f;
    (void)0.89f;
    (void)35.0f;
    (void)20.0f;
    (void)5.0f;
    (void)0.6f;
}

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

    const char* const cBck2BtpTable[8] = {"Bound", "CloseEye", "FlyStart", "OpenEye", "FlyEnd", "OpenEye", "SwimStart", "OpenEye"};
    inline const char* getBtpName(const char* pBck) {
        for (u32 i = 0; i < 4; i++) {
            const char* const* pEntry = &cBck2BtpTable[i * 2];
            if (MR::isEqualStringCase(pEntry[0], pBck)) {
                return pEntry[1];
            }
        }

        return nullptr;
    }
};  // namespace

PukupukuStateLanding::PukupukuStateLanding(Pukupuku* pParent) : ActorStateBase< Pukupuku >("プクプク地上", pParent), mPath(), mValueCtrl(), _18() {
    mPath = new ParabolicPath();
    mValueCtrl = new ValueControl(30);
    initNerve(GET_NERVE_ANON(PukupukuStateLandingLandingMoveLand));
    kill();
}

void PukupukuStateLanding::exeLandingMoveLand() {
    if (MR::isFirstStep(this)) {
        setupJumping(300.0f, 15.0f);
        getHost()->startAnim("Bound", "CloseEye");
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    mHost->updatePoseByRailIgnoreUpScale();

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mHost) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* pParent = mHost;
        MR::moveCoordToRailPoint(pParent, MR::getNextRailPointNo(pParent));

        if (MR::isBinded(mHost)) {
            mHost->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromWater() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        mHost->rotatePoseByLocalZ();
        getHost()->startAnim("FlyStart", "OpenEye");
        MR::startSound(mHost, "SE_EM_PUKUPUKU_WATER_OUT");
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(true, false);
    updatePoseByJumpPath(0.15f);

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mHost) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* pParent = mHost;
        MR::moveCoordToRailPoint(pParent, MR::getNextRailPointNo(pParent));

        if (MR::isBinded(mHost)) {
            mHost->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromLand() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        getHost()->startAnim("FlyEnd", "OpenEye");
        MR::startSound(mHost, "SE_EM_PUKUPUKU_WING");
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(false, false);
    updatePoseByJumpPath(0.15f);

    if (mValueCtrl->isFrameMaxFrame()) {
        MR::moveCoordAndTransToNextPoint(mHost);
        emitWaterColumIfNeed(false, true);
        kill();
    }
}

void PukupukuStateLanding::updatePoseByJumpPath(f32 a1) {
    TVec3f direction;
    mPath->calcDirection(&direction, mValueCtrl->getValue(), 0.01f);
    MR::normalize(&direction);
    TVec3f v7 = -mHost->mGravity;
    Pukupuku* pParent = mHost;
    if (!MR::isSameDirection(direction, v7)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, direction, v7);
        pParent->_A8.slerp(quat, a1);
    }
}

void PukupukuStateLanding::setNerveAfterJumpAccordingToNextPoint() {
    if (mHost->isReadyToJumpFromLand()) {
        setNerve(GET_NERVE_ANON(PukupukuStateLandingLandingJumpFromLand));
    } else {
        setNerve(GET_NERVE_ANON(PukupukuStateLandingLandingMoveLand));
    }
}

void PukupukuStateLanding::setupJumping(f32 a1, f32 a2) {
    TVec3f curPos;
    mHost->getCurrentRailPointPos(&curPos);
    TVec3f nextPos;
    mHost->getNextRailPointPos(&nextPos);

    TVec3f v16(mHost->mPosition);
    v16.sub(curPos);
    TVec3f v15 = -mHost->mGravity;
    f32 dot = v15.dot(v16);

    if (0.0f < dot) {
        a1 = (a1 - dot);
    }

    Pukupuku* pParent = mHost;
    TVec3f v14 = -pParent->mGravity;
    mPath->initFromUpVectorAddHeight(pParent->mPosition, nextPos, v14, a1);
    f32 speed = mPath->calcPathSpeedFromAverageSpeed(a2);
    f32 v19 = (1.0f / speed);
    mValueCtrl->setMaxFrame(v19);
    mValueCtrl->setDirToOneResetFrame();
}

void PukupukuStateLanding::updateJumping() {
    TVec3f pos;
    mPath->calcPosition(&pos, mValueCtrl->getValue());
    pos.sub(mHost->mPosition);
    mHost->mVelocity.set(pos);
}

void PukupukuStateLanding::emitWaterColumIfNeed(bool a1, bool a2) {
    if (!_18) {
        TVec3f centerJointPos;
        MR::copyJointPos(mHost, "Center", &centerJointPos);

        if (a2 || a1 != MR::isInWater(centerJointPos)) {
            TPos3f hitMtx;
            mHost->calcGroundHitMtx(&hitMtx);
            hitMtx.setTrans(centerJointPos);
            hitMtx.scaleXYZ(1.5f);
            MR::emitEffectHit(mHost, hitMtx, "WaterColumn");
            _18 = 1;
        }
    }
}

void PukupukuStateLanding::emitGroundHitEffect() {
    if (!MR::isBindedGround(mHost)) {
        Triangle triangle;
        Pukupuku* pParent = mHost;

        TVec3f poly;
        if (MR::getFirstPolyOnLineToMap(&poly, &triangle, pParent->mPosition, mHost->mGravity * 100.0f)) {
            MR::updateEffectFloorCode(mHost, &triangle);
        }
    }

    TPos3f hitMtx;
    mHost->calcGroundHitMtx(&hitMtx);
    MR::emitEffectHit(mHost, hitMtx, "Land");
    MR::startSound(mHost, "SE_EM_PUKUPUKU_LAND");
}

Pukupuku::Pukupuku(const char* pName) : LiveActor(pName), mStateLanding(), mScaleCtrl(), mStarPointer(), _C0(), _C4(), _C8(), _CC(), _D3() {
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
    MR::addHitSensorAtJoint(this, "body", "Center", ATYPE_PUKUPUKU, 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
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
    initNerve(GET_NERVE_ANON(PukupukuWait));
    mStateLanding = new PukupukuStateLanding(this);
    mStateLanding->kill();
    MR::initStarPointerTarget(this, 60.0f);
    mScaleCtrl = new AnimScaleController(nullptr);
    mStarPointer = new WalkerStateBindStarPointer(this, mScaleCtrl);
    MR::addToAttributeGroupSearchTurtle(this);
    makeActorAppeared();
}

void Pukupuku::kill() {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::emitEffect(this, "DeathWater");
        MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER");
    } else {
        MR::emitEffect(this, "Death");
        MR::startSound(this, "SE_EM_EXPLODE_S");
    }

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (_D3) {
        MR::appearCoinPop(this, mPosition, 1);
    } else {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W");
        } else {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        }
    }

    LiveActor::kill();
}

void Pukupuku::updatePoseByRailIgnoreUpScale() {
    TVec3f v4 = -mGravity;
    const TVec3f* pRailDir = &MR::getRailDirection(this);
    if (!MR::isSameDirection(*pRailDir, v4)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, *pRailDir, v4);
        _A8.slerp(quat, 0.08f);
    }
}

void Pukupuku::rotatePoseByLocalZ() {
    TQuat4f rotZ;
    rotZ.setEulerZ(MR::pi());
    _A8.mult(_A8, rotZ);
}

void Pukupuku::calcGroundHitMtx(TPos3f* pMtx) const {
    TVec3f zDir;
    _A8.getZDir(zDir);
    MR::calcMtxFromGravityAndZAxis(pMtx, this, mGravity, zDir);
}

bool Pukupuku::isReadyToJumpFromLand() const NO_INLINE {
    s32 arg0 = -1;
    MR::getRailPointArg0NoInit(this, MR::getCurrentRailPointNo(this), &arg0);
    return arg0 == 1;
}

inline bool Pukupuku::isDamaged() const {
    return isNerve(GET_NERVE_ANON(PukupukuTrampled)) || isNerve(GET_NERVE_ANON(PukupukuBlownOff));
}

void Pukupuku::exeWait() {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        setNerve(GET_NERVE_ANON(PukupukuMoveWater));
    } else {
        s32 pointNum = MR::getRailPointNum(this);

        for (s32 i = 0; i < pointNum; i++) {
            s32 arg = -1;
            MR::getRailPointArg0NoInit(this, MR::getCurrentRailPointNo(this), &arg);
        }

        PukupukuStateLanding* pState = mStateLanding;
        pState->appear();
        pState->setNerve(GET_NERVE_ANON(PukupukuStateLandingLandingMoveLand));
        setNerve(GET_NERVE_ANON(PukupukuLanding));
    }
}

void Pukupuku::exeMoveWater() {
    bool v2 = isDamaged();

    if (!v2 && !tryBindStarPointer()) {
        if (MR::isFirstStep(this)) {
            startAnim("Swim", "OpenEye");
        }

        MR::startLevelSound(this, "SE_EM_LV_PUKUPUKU_SWIM");
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
        MR::startSound(this, "SE_EM_PUKUPUKU_WATER_IN");
    }

    updateMoveWaterAfterJump();
}

void Pukupuku::exeLanding() {
    if (!tryBindStarPointer()) {
        mStateLanding->update();
        if (mStateLanding->mIsDead) {
            setNerve(GET_NERVE_ANON(PukupukuMoveWaterAfterJump));
        }
    }
}

void Pukupuku::exeTrampled() {
    if (MR::isFirstStep(this)) {
        TPos3f hitMtx;
        calcGroundHitMtx(&hitMtx);
        hitMtx.getQuat(_A8);
        mVelocity.set(mGravity * 10.0f);
        startAnim("Flat", "CloseEye");
        MR::startSound(this, "SE_EM_STOMPED_S");
    } else if (!MR::isBinded(this)) {
        mVelocity.x *= 0.89f;
        mVelocity.y *= 0.89f;
        mVelocity.z *= 0.89f;
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
        mVelocity.add(v11);
    }

    MR::stopSceneAtStep(this, 2, 4);
    if (MR::isStep(this, 15)) {
        kill();
    }
}

void Pukupuku::exeBindStarPointer() {
    if (MR::updateActorStateAndNextNerve(this, mStarPointer, _C4) && _C4 != GET_NERVE_ANON(PukupukuMoveWater)) {
        if (!_C8) {
            return;
        }

        const char* pBtp = ::getBtpName(_C0);

        MR::startBck(this, _C0);
        MR::startBtp(this, pBtp);
        MR::setBckFrame(this, _C8);
        MR::setBtpFrame(this, _CC);
    }
}

void Pukupuku::updatePoseByRail() {
    TVec3f yDir;
    _A8.getYDir(yDir);

    const TVec3f* pRailDir = &MR::getRailDirection(this);
    if (!MR::isSameDirection(*pRailDir, yDir)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, *pRailDir, yDir);
        _A8.slerp(quat, 0.079999998f);
    }
}

void Pukupuku::updateMoveWaterAfterJump() {
    if (!tryBindStarPointer()) {
        exeMoveWaterCommon();

        if (MR::isBckStopped(this)) {
            setNerve(GET_NERVE_ANON(PukupukuMoveWater));
        }
    }
}

bool Pukupuku::tryBindStarPointer() {
    if (mStarPointer->tryStartPointBind()) {
        _C4 = nullptr;
        _C8 = 0.0f;
        _C0 = MR::getPlayingBckName(this);

        if (isNerve(GET_NERVE_ANON(PukupukuMoveWater))) {
            _C4 = GET_NERVE_ANON(PukupukuMoveWater);
        }

        if (isNerve(GET_NERVE_ANON(PukupukuMoveWaterAfterJump)) || isNerve(GET_NERVE_ANON(PukupukuMoveWaterAfterJumpAfterPointing))) {
            _C4 = GET_NERVE_ANON(PukupukuMoveWaterAfterJumpAfterPointing);
        }

        if (isNerve(GET_NERVE_ANON(PukupukuLanding))) {
            _C4 = GET_NERVE_ANON(PukupukuLanding);
        }

        _C8 = MR::getBckFrame(this);
        _CC = MR::getBtpFrame(this);
        setNerve(GET_NERVE_ANON(PukupukuBindStarPointer));
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
        PukupukuStateLanding* pState = mStateLanding;
        pState->appear();
        pState->setNerve(GET_NERVE_ANON(PukupukuStateLandingLandingJumpFromWater));
        setNerve(GET_NERVE_ANON(PukupukuLanding));
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
    _9C.set(-mGravity);
    _9C.x *= 70.0f;
    _9C.y *= 70.0f;
    _9C.z *= 70.0f;
    mScaleCtrl->updateNerve();

    if (isNerve(GET_NERVE_ANON(PukupukuBindStarPointer))) {
        MR::setSensorRadius(this, "body", 100.0f);
    } else {
        MR::setSensorRadius(this, "body", 60.0f);
    }
}

void Pukupuku::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && !isDamaged()) {
        if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            if (isNerve(GET_NERVE_ANON(PukupukuMoveWater))) {
                startAnim("BodyAttack", "CloseEye");
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else if (isNerve(GET_NERVE_ANON(PukupukuBindStarPointer)) && MR::isSensorType(pReceiver, ATYPE_PUKUPUKU) &&
               MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        f32 rate = getBlownOffSpeedRate();
        MR::setVelocityBlowAttack(this, pReceiver, pSender, 35.0f * rate, 20.0f * rate, 4);
        setNerve(GET_NERVE_ANON(PukupukuBlownOff));
    }
}

bool Pukupuku::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (!isDamaged()) {
        if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
            setNerve(GET_NERVE_ANON(PukupukuTrampled));
            _D3 = true;
            return true;
        }

        if (MR::isMsgPlayerHitAll(msg)) {
            f32 rate = getBlownOffSpeedRate();
            MR::setVelocityBlowAttack(this, pSender, pReceiver, 35.0f * rate, 20.0f * rate, 4);
            setNerve(GET_NERVE_ANON(PukupukuBlownOff));
            return true;
        }
    }

    return false;
}

bool Pukupuku::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isDamaged() && MR::isSensorType(pSender, ATYPE_PUKUPUKU)) {
        f32 rate = getBlownOffSpeedRate();
        MR::setVelocityBlowAttack(this, pSender, pReceiver, 35.0f * rate, 20.0f * rate, 4);
        setNerve(GET_NERVE_ANON(PukupukuBlownOff));
        return true;
    }

    return false;
}

void Pukupuku::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A8);
    TVec3f scale = mScale;
    scale.mul(scale, mScaleCtrl->_C);
    MR::setBaseScale(this, scale);
}

PukupukuStateLanding::~PukupukuStateLanding() {
}

Pukupuku::~Pukupuku() {
}
