#include "Game/Boss/TripodBoss.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossLeg.hpp"
#include "Game/Boss/TripodBossMovableArea.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/Boss/TripodBossStepSequence.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include <cstdio>

namespace {
    static const char* sLegBoneNameTable[3] = {
        "LeftLeg", "RightLeg", "BackLeg"
    };

    static s32 sKillerGeneraterIncreaseSeTiming = 0x5A;
    static s32 sHeadExplodeSeTiming;

    static TVec3f sPowerStarOffset(0.0f, 3200.0f, 0.0f);
    static TVec3f sAppearStarPieceOffset(0.0, 3600.0f, 0.0f);
    static TVec3f sEndMarioPosition(0.0f, 2160.0f, 1260.0f);
};

namespace NrvTripodBoss {
    NEW_NERVE(TripodBossNrvTryStartDemo, TripodBoss, TryStartDemo);
    NEW_NERVE(TripodBossNrvNonActive, TripodBoss, NonActive);
    NEW_NERVE(TripodBossNrvWait, TripodBoss, Wait);
    NEW_NERVE(TripodBossNrvStep, TripodBoss, Step);
    NEW_NERVE(TripodBossNrvWaitStep, TripodBoss, WaitStep);
    NEW_NERVE(TripodBossNrvDamage, TripodBoss, Damage);
    NEW_NERVE(TripodBossNrvChangeSequence, TripodBoss, ChangeSequence);
    NEW_NERVE(TripodBossNrvLeaveLegOutOfPlayer, TripodBoss, LeaveLegOutOfPlayer);
    NEW_NERVE(TripodBossNrvStartDemo, TripodBoss, StartDemo);
    NEW_NERVE(TripodBossNrvDamageDemo, TripodBoss, DamageDemo);
    NEW_NERVE(TripodBossNrvPainDemo, TripodBoss, PainDemo);
    NEW_NERVE(TripodBossNrvBreakDownDemo, TripodBoss, BreakDownDemo);
    NEW_NERVE(TripodBossNrvExplosionDemo, TripodBoss, ExplosionDemo);
};

TripodBoss::TripodBoss(const char *pName) : LiveActor(pName), mLowModel(nullptr), mMovableArea(nullptr), mDummyModel(nullptr),
    _5BC(0.0f, 3200.0f), _5C8(0, 0, 0), _5D4(0, 0, 0), _5E0(0, 0, 0), _5EC(0, 0, 0) {

    _5F8 = 7500.0f;
    _5FC = 0.0f;
    _600 = 1.0f;
    _604 = 3000.0f;
    _608 = 4000.0f;
    _60C = 4000.0f;
    _610 = 2400.0f;
    _614 = 0.0f;
    _618 = 0.1f;
    _61C = 3000.0f;
    _620 = 2.0f;
    mCurrentStepSeq = -1;
    mNextStepSeq = -1;
    _630 = 0;
    _634 = 2;
    _638 = 0;
    _63C = 1;
    _640 = 0;
    mEventCamera = nullptr;
    mBodyMtx.identity();
    _EC.identity();
    TripodBossAccesser* accessor = TripodBossAccesser::createSceneObj();
    accessor->setTriPodBoss(this);

    for (u32 i = 0; i < 3; i++) {
        mLegs[i] = nullptr;
    }

    mStepSequence = new TripodBossStepSequence[32];
    _62C = 32;
}

void TripodBoss::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneCollisionMapObjMovementCalcAnim(this);
    TPos3f v14;
    v14.identity();
    MR::getJMapInfoMatrixFromRT(rIter, &v14);
    _5D4 = mPosition;
    initMovableArea(v14);
    JMath::gekko_ps_copy12(mBodyMtx, v14);
    initBodyPosition();
    const char* objName;
    MR::getObjectName(&objName, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    char lowName[0x20];
    sprintf(lowName, "%sLow", objName);
    mLowModel = new ModelObj("三脚ボスLODモデル", lowName, getBaseMtx(), 31, -2, -2, false);
    mLowModel->initWithoutIter();
    MR::invalidateClipping(mLowModel);
    mLowModel->makeActorAppeared();
    initLeg(rIter);
    initBoneInfo();
    calcLegMovement();
    initEventCamera(rIter);
    initSound(8, false);
    initEffectKeeper(0, "TripodBoss", false);
    MR::setEffectHostMtx(this, "BreakLight", mBodyMtx);
    initNerve(&NrvTripodBoss::TripodBossNrvNonActive::sInstance);
    MR::invalidateClipping(this);
    MR::needStageSwitchReadA(this, rIter);
    MR::FunctorV0M<TripodBoss *, void (TripodBoss::*)()> demoFunc = MR::Functor_Inline<TripodBoss>(this, &TripodBoss::requestOpeningDemo);
    MR::listenStageSwitchOnA(this, demoFunc);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::declareStarPiece(this, 24);
    MR::declarePowerStar(this);
    JMath::gekko_ps_copy12(_BC, mBodyMtx);
    MR::addTransMtxLocal(_BC, _5BC);
    mDummyModel = MR::createDummyDisplayModel(this, rIter, _BC, 13, TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    MR::startBrk(mDummyModel, "Recover");
    MR::setBrkFrameAndStop(mDummyModel, 0.0f);
    makeActorAppeared();
    MR::startBck(this, "StartDemo", nullptr);
    MR::setBckFrameAndStop(this, 0.0f);
    initPose();
    MR::offCalcAnim(this);
}

void TripodBoss::initAfterPlacement() {
    _638 = 1;
    MR::hideTripodBossParts();
}

void TripodBoss::initEventCamera(const JMapInfoIter &rIter) {
    mEventCamera = MR::createActorCameraInfo(rIter);
    MR::initAnimCamera(this, mEventCamera, "StartDemo");
    MR::initAnimCamera(this, mEventCamera, "EndDemo");
}

void TripodBoss::initLeg(const JMapInfoIter &rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);
    char lowName[0x20];
    sprintf(lowName, "%sLegShadow", objName);

    for (u32 i = 0; i < 3; i++) {
        mLegs[i] = new TripodBossLeg("三脚ボス足");
        mStepPoints[i] = new TripodBossStepPoint("ステップ位置");
        mLegs[i]->setBody(this);
        mLegs[i]->setMovableArea(mMovableArea);
        mLegs[i]->initWithoutIter();
        mLegs[i]->initShadow(lowName);
        mStepPoints[i]->initWithoutIter();
    }

    initLegIKPlacement();

    for (u32 i = 0; i < 3; i++) {
        MR::addTripodBossParts(mLegs[i]);
    }
}

// https://decomp.me/scratch/UDA4k
void TripodBoss::initLegIKPlacement() {
    f32 temp618 = _618;
    f32 v5 = MR::speedySqrtf((1.0f - (_618 * _618)));

    TVec3f v29(mMovableArea->mBaseAxis);
    TVec3f v28(mMovableArea->mFront);
    TVec3f v27;
    PSVECCrossProduct(&v29, &v28, &v27);
    v29 *= (temp618 * mMovableArea->mRadius);
    v28 *= (v5 * mMovableArea->mRadius);
    v27 *= (v5 * mMovableArea->mRadius);

    f32 ONEPOINTFIVEPI = 2.0943952f;
    for (u32 i = 0; i < 3; i++) {
        f32 cur = -(f32)i * ONEPOINTFIVEPI;
        f32 v8 = ((0.5f * ONEPOINTFIVEPI) + cur);
        f32 v9 = JMath::sSinCosTable.sinLap2(v8);

        if (v8 < 0.0f) {
            v8 = -v8;
        }

        TVec3f v26;
        v26.x = v9;
        v26.y = 0.0f;
        f32 v10 = JMath::sSinCosTable.getMult(v8);
        f32 v11 = v10;
        v26.z = v10;
        TVec3f v25(0.0f, 1.0f, 0.0f);
        f32 val = _614;
        TVec3f v19(v25);
        v19 *= val;
        f32 other_val = _610;
        TVec3f v20(v26);
        v20 *= other_val;
        TVec3f v24(v20);
        v24 += v19;
        getLeg(i)->setIKParam(_608, _60C, v24, v26, v25);
        TVec3f* center = &mMovableArea->mCenter;
        TVec3f v15(v28);
        v15 *= v11;
        TVec3f v16(v27);
        v16 *= v9;
        TVec3f v17(v29);
        v17 += v16;
        TVec3f v18(v17);
        v18 += v15;
        TVec3f v23(v18);
        v23 += *center;
        TVec3f v22;
        mMovableArea->calcLandingNormal(&v22, v23);
        TVec3f v21;
        mMovableArea->calcLandingFront(&v21, v23);
        getStepPoint(i)->setStepPosition(v23);
        getStepPoint(i)->setStepNormal(v22);
        getStepPoint(i)->setStepFront(v21);
        getLeg(i)->setStepTarget(getStepPoint(i));
        getLeg(i)->setWait();
    }
}

void TripodBoss::initMovableArea(const TPos3f &rPos) {
    TVec3f trans;
    rPos.getTrans(trans);
    mMovableArea = new TripodBossMovableArea();
    mMovableArea->setCenter(trans);
    mMovableArea->setRadius(_61C);
    TVec3f yDir;
    rPos.getYDir(yDir);
    TVec3f front;
    front.set<f32>(rPos.get(0, 2), rPos.get(1, 2), rPos.get(2, 2));
    mMovableArea->setBaseAxis(yDir);
    mMovableArea->setFrontVector(front);
}

void TripodBoss::initBodyPosition() {
    f32 xxx = _604;
    f32 yyy = mMovableArea->mRadius;
    f32 v3 = xxx + yyy;
    
    TVec3f v4(mMovableArea->mBaseAxis);
    v4 *= v3;
    TVec3f v5(mMovableArea->mCenter);
    v5 += v4;
    _5D4 = v5;
    _5C8 = _5D4;
    MR::makeMtxTR(mBodyMtx, _5D4, mRotation);
}

void TripodBoss::initBoneInfo() {
    mBossBones[21]._30 = &mBodyMtx;
    mBossBones[0]._30 = getLegMatrixPtr(LeftLeg, Part_RootJoint);
    mBossBones[1]._30 = getLegMatrixPtr(LeftLeg, Part_MiddleJoint);
    mBossBones[2]._30 = getLegMatrixPtr(LeftLeg, Part_EndJoint);
    mBossBones[3]._30 = getLegMatrixPtr(LeftLeg, Part_RootLocalY);
    mBossBones[4]._30 = getLegMatrixPtr(LeftLeg, Part_RootLocalYZ);
    mBossBones[5]._30 = getLegMatrixPtr(LeftLeg, Part_AnkleLocalX);
    mBossBones[6]._30 = getLegMatrixPtr(LeftLeg, Part_AnkleLocalXZ);
    mBossBones[7]._30 = getLegMatrixPtr(MiddleLeg, Part_RootJoint);
    mBossBones[8]._30 = getLegMatrixPtr(MiddleLeg, Part_MiddleJoint);
    mBossBones[9]._30 = getLegMatrixPtr(MiddleLeg, Part_EndJoint);
    mBossBones[10]._30 = getLegMatrixPtr(MiddleLeg, Part_RootLocalY);
    mBossBones[11]._30 = getLegMatrixPtr(MiddleLeg, Part_RootLocalYZ);
    mBossBones[12]._30 = getLegMatrixPtr(MiddleLeg, Part_AnkleLocalX);
    mBossBones[13]._30 = getLegMatrixPtr(MiddleLeg, Part_AnkleLocalXZ);
    mBossBones[14]._30 = getLegMatrixPtr(RightLeg, Part_RootJoint);
    mBossBones[15]._30 = getLegMatrixPtr(RightLeg, Part_MiddleJoint);
    mBossBones[16]._30 = getLegMatrixPtr(RightLeg, Part_EndJoint);
    mBossBones[17]._30 = getLegMatrixPtr(RightLeg, Part_RootLocalY);
    mBossBones[18]._30 = getLegMatrixPtr(RightLeg, Part_RootLocalYZ);
    mBossBones[19]._30 = getLegMatrixPtr(RightLeg, Part_AnkleLocalX);
    mBossBones[20]._30 = getLegMatrixPtr(RightLeg, Part_AnkleLocalXZ);
}

void TripodBoss::initPose() {
    calcAnim();
    for (u32 i = 0; i < 3; i++) {
        mLegs[i]->requestStartDemo();
    }

    calcDemoMovement();
    calcClippingSphere();
}

void TripodBoss::kill() {
    MR::startAfterBossBGM();
    LiveActor::kill();

    for (u32 i = 0; i < 3; i++) {
        mLegs[i]->kill();
    }

    MR::requestAppearPowerStar(this, _5D4);
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void TripodBoss::control() {
    JMath::gekko_ps_copy12(_BC, mBodyMtx);
    MR::addTransMtxLocal(_BC, _5BC);
    mDummyModel->mRotation.y = MR::wrapAngleTowards(0.0f, mDummyModel->mRotation.y - _620);

    if (isNerve(&NrvTripodBoss::TripodBossNrvNonActive::sInstance)) {
        clippingModel();
    }
    else {
        mNextStepSeq = -1;
        checkRideMario();
        changeBgmState();
    }
}

void TripodBoss::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
}

bool TripodBoss::tryStartStep() {
    if (mNextStepSeq == -1 || isStateSomething()) {
        return false;
    }

    mCurrentStepSeq = mNextStepSeq;
    mStepSequence[mCurrentStepSeq].reset();
    setNerve(&NrvTripodBoss::TripodBossNrvStep::sInstance);
    return true;
}

bool TripodBoss::tryChangeSequence() {
    if (mNextStepSeq == -1 || mNextStepSeq == mCurrentStepSeq || isStateSomething()) {
        return false;
    }

    s32 legIdx = getCurrentStepSequence()->getCurrentLeg();
    if (!mLegs[legIdx]->canCancelStep()) {
        return false;
    }

    TripodBossStepSequence* seq = getNextStepSequence();
    seq->reset();
    s32 v9 = seq->getCurrentLeg();
    bool v10 = false;
    for (u32 i = 0; i < 3; i++) {
        if (i != v9 && !mLegs[i]->isStop()) {
            mLegs[i]->requestStepTarget(mStepPoints[i]);
            v10 = true;
        }
    }

    mCurrentStepSeq = mNextStepSeq;
    if (!v10) {
        setNerve(&NrvTripodBoss::TripodBossNrvStep::sInstance);
    }
    else {
        setNerve(&NrvTripodBoss::TripodBossNrvChangeSequence::sInstance);
    }

    return true;
}

bool TripodBoss::tryEndSequence() {
    if (mNextStepSeq != -1) {
        return false;
    }

    mCurrentStepSeq = -1;
    setNerve(&NrvTripodBoss::TripodBossNrvWait::sInstance);
    return true;
}

bool TripodBoss::tryNextSequence() {
    if (isStopAllLeg()) {
        bool v4 = isStateSomething();
        if (!v4) {
            setNerve(&NrvTripodBoss::TripodBossNrvStep::sInstance);
            return true;
        }
    }

    return false;
}

bool TripodBoss::tryDamage() {
    s32 leg = getCurrentStepSequence()->getCurrentLeg();  
    if (mLegs[leg]->isDamage()) {
        setNerve(&NrvTripodBoss::TripodBossNrvDamage::sInstance);
        return true;
    }  

    return false;
}

bool TripodBoss::tryWaitStep() {
    s32 leg = getCurrentStepSequence()->getCurrentLeg();
    if (mLegs[leg]->isLanding()) {
        setNerve(&NrvTripodBoss::TripodBossNrvWaitStep::sInstance);
        return true;
    }
    return false;
}

bool TripodBoss::tryNextStep() {
    bool v3 = isStateSomething();
    if (v3) {
        return false;
    }

    TripodBossStepSequence* seq = getCurrentStepSequence();
    s32 leg = seq->getCurrentLeg();
    s32 waitTime = seq->getCurrentWaitTime();
    if (MR::isGreaterStep(this, waitTime) || mLegs[leg]->isBroken()) {
        seq->nextStep();
        setNerve(&NrvTripodBoss::TripodBossNrvStep::sInstance);
        return true;
    }

    return false;
}

bool TripodBoss::tryLeaveLegOutOfPlayer() {
    if (MR::isPlayerOnPress()) {
        bool isPressed = false;
        for (u32 i = 0; i < 3; i++) {
            if (mLegs[i]->isPressPlayer()) {
                mLegs[i]->requestLeaveOut();
                isPressed = true;
                break;
            }
        }

        if (isPressed) {
            setNerve(&NrvTripodBoss::TripodBossNrvLeaveLegOutOfPlayer::sInstance);
            return true;
        }
    }

    return false;
}

bool TripodBoss::tryEndLeaveLegOutOfPlayer() {
    if (!MR::isPlayerOnPress()) {
        setNerve(&NrvTripodBoss::TripodBossNrvWait::sInstance);
        return true;
    }
    return false;
}

bool TripodBoss::tryEndDamage() {
    if (MR::isGreaterStep(this, 129)) {
        setNerve(&NrvTripodBoss::TripodBossNrvWait::sInstance);
        return true;
    }
    return false;
}

bool TripodBoss::tryBreak() {
    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        for (u32 i = 0; i < 3; i++) {
            mLegs[i]->requestBreak();
        }
        
        setNerve(&NrvTripodBoss::TripodBossNrvTryStartDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "破壊", &NrvTripodBoss::TripodBossNrvPainDemo::sInstance, nullptr);
        return true;
    }

    return false;
}

void TripodBoss::requestOpeningDemo() {
    if (isNerve(&NrvTripodBoss::TripodBossNrvNonActive::sInstance)) {
        if (!MR::isDead(mLowModel)) {
            mLowModel->makeActorDead();
        }

        _638 = 0;
        MR::activeTripodBossParts();
        setNerve(&NrvTripodBoss::TripodBossNrvTryStartDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "開始", &NrvTripodBoss::TripodBossNrvStartDemo::sInstance, nullptr);
    }
}

bool TripodBoss::tryDamageDemo() {
    setNerve(&NrvTripodBoss::TripodBossNrvTryStartDemo::sInstance);
    if (MR::tryStartDemo(this, "ダメージ")) {
        setNerve(&NrvTripodBoss::TripodBossNrvDamageDemo::sInstance);
        return true;
    }
    return false;
}

void TripodBoss::requestEndDamageDemo() {
    endDemo("ダメージ");
    setNerve(&NrvTripodBoss::TripodBossNrvWait::sInstance);
    TVec3f appearOffs;
    mBodyMtx.mult(sAppearStarPieceOffset,appearOffs);
    TVec3f yDir;
    mBodyMtx.getYDir(yDir);
    MR::appearStarPieceToDirection(this, appearOffs, yDir, 24, 50.0f, 60.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_F", -1, -1);
}

void TripodBoss::exeWait() {
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    calcBodyMovement();
    calcLegMovement();
    if (!tryBreak()) {
        if (tryStartStep()) {
            return;
        }
    }
}

void TripodBoss::exeStep() {
    if (MR::isFirstStep(this)) {
        TripodBossStepSequence* stepSeq = getCurrentStepSequence();
        s32 curr = stepSeq->getCurrentLeg();
        if (mLegs[curr]->canStep()) {
            mLegs[curr]->requestStepTarget(stepSeq->getCurrentStepPoint());
        }
        else {
            stepSeq->nextStep();  
        }
    }

    calcBodyMovement();
    calcLegMovement();
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    if (!tryBreak() && !tryDamage() && !tryChangeSequence()) {
        if (tryWaitStep()) {
            return;
        }
    }
}

void TripodBoss::exeWaitStep() {
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    calcBodyMovement();
    calcLegMovement();
    if (!tryBreak() && !tryLeaveLegOutOfPlayer() && !tryChangeSequence() && !tryEndSequence()) {
        if (tryNextStep()) {
            return;
        }
    }
}

void TripodBoss::exeChangeSequence() {
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    calcBodyMovement();
    calcLegMovement();
    if (!tryBreak()) {
        if (tryNextSequence()) {
            return;
        }
    }
}

void TripodBoss::exeLeaveLegOutOfPlayer() {
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    calcBodyMovement();
    calcLegMovement();
    if (tryEndLeaveLegOutOfPlayer()) {
        return;
    }
}

void TripodBoss::exeDamage() {
    if (MR::isGreaterStep(this, 30)) {
        TVec3f* center = &mMovableArea->mCenter;
        TVec3f v5(_5D4);
        v5 -= *center;
        MR::normalizeOrZero(&v5);
        if (getNerveStep() % 6 < 3) {
            TVec3f v3(v5);
            v3 *= 80.0f;
            _5E0 -= v3;
        }
        else {
            TVec3f v4(v5);
            v4 *= 79.5f;
            _5E0 += v4;
        }
    }

    calcBodyMovement();
    calcLegMovement();
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BOTTOM_MOVE", -1, -1, -1);
    if (tryEndDamage()) {
        return;
    }
}

void TripodBoss::exeStartDemo() {
    if (MR::isFirstStep(this)) {
        startDemo();
        MR::startBckPlayer("BattleWait", (const char *)0);
        MR::stopStageBGM(10);
        _600 = 1.0f;
        MR::startAnimCameraTargetSelf(this, mEventCamera, "StartDemo", 0, 1.0f);
        MR::startBck(this, "StartDemo", nullptr);
    }

    if (MR::isLessStep(this, 120)) {
        MR::startLevelSound(this, "SE_BM_LV_TRIPOD_SIREN", -1, -1, -1);
    }

    if (MR::isStep(this, 120)) {
        MR::startBossBGM(2);
    }

    if (MR::isGreaterStep(this, 120)) {
        MR::startLevelSound(this, "SE_BM_LV_TRIPOD_START_DEMO", -1, -1, -1);
    }
    
    calcDemoMovement();

    if (MR::isBckStopped(this)) {
        endDemo("開始");
        MR::endAnimCamera(this, mEventCamera, "StartDemo", 150, true);
        initLegIKPlacement();
        setNerve(&NrvTripodBoss::TripodBossNrvWait::sInstance);
    }
}

void TripodBoss::exeDamageDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
        startDemo();
    }

    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_SIREN", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_TRIPOD_MID_DEMO", -1, -1, -1);
    if (MR::isStep(this, sKillerGeneraterIncreaseSeTiming)) {
        MR::startSound(this, "SE_BM_TRIPOD_CANNON_APPEAR", -1, -1);
    }

    _640 = 1;
}

void TripodBoss::exePainDemo() {
    if (MR::isFirstStep(this)) {
        bool isOnGround = MR::isOnGroundPlayer();
        MR::startSound(this, "SE_BM_TRIPOD_HALT", -1, -1);
        MR::stopStageBGM(30);
        startDemo();
        if (isOnGround) {
            MR::startBckPlayer("BattleWait", (const char *)0);
        }

        MR::startBrk(mDummyModel, "Recover");
        _620 = 5.0f;
        _600 = 0.0f;
    }

    if (MR::isStep(this, 90)) {
        TPos3f mtx;
        JMath::gekko_ps_copy12(&mtx, getBaseMtx());
        TVec3f v5;
        mtx.mult(sEndMarioPosition, v5);
        mtx.mMtx[0][3] = v5.x;
        mtx.mMtx[1][3] = v5.y;
        mtx.mMtx[2][3] = v5.z;
        MR::setPlayerBaseMtx(mtx);
        MR::startBckPlayer("Wait", (const char *)0);
        MR::startAnimCameraTargetSelf(this, mEventCamera, "EndDemo", 0, 1.0f);
        MR::startBck(this, "EndDemo", nullptr);
        MR::emitEffect(this, "BreakLight");
    }
    if (MR::isLessStep(this, 90)) {
        _5BC.y = MR::calcNerveEaseInOutValue(this, 70, 3200.0f, 3400.0f);
    }
    else {
        _5BC.y = MR::calcNerveEaseInValue(this, 90, 150, 3400.0f, 5000.0f);
    }

    if (MR::isGreaterStep(this, 90)) {
        MR::startLevelSound(this, "SE_BM_LV_TRIPOD_END_DEMO", -1, -1, -1);
        MR::startLevelSound(this, "SE_BM_LV_TRIPOD_BREAK_LIGHT", -1, -1, -1);
        calcDemoMovement();
        _600 += 0.033333335f;
        if (_600 > 1.0f) {
            _600 = 1.0f;
        }

        if (MR::isBckStopped(this)) {
            mDummyModel->kill();
            setNerve(&NrvTripodBoss::TripodBossNrvBreakDownDemo::sInstance);
        }
    }
}

void TripodBoss::exeBreakDownDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraStrong();
        MR::startSound(this, "SE_BM_TRIPOD_ALL_BREAK", -1, -1);
    }

    if (MR::isGreaterStep(this, 240)) {
        setNerve(&NrvTripodBoss::TripodBossNrvExplosionDemo::sInstance);
    }
}

void TripodBoss::exeExplosionDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadVeryStrongLong(this, 0);
    }

    if (MR::isStep(this, sHeadExplodeSeTiming)) {
        MR::startSound(this, "SE_BM_TRIPOD_KILL_HEAD", -1, -1);
    }

    if (MR::isGreaterStep(this, 150)) {
        endDemo("破壊");
        MR::endAnimCamera(this, mEventCamera, "EndDemo", 0, true);
        kill();
    }
}

void TripodBoss::exeNonActive() {

}

void TripodBoss::exeTryStartDemo() {

}

bool TripodBoss::isStopLeg(s32 idx) const {
    bool ret = false;
    if (idx >= 0 && idx <= 2) {
        ret = true;
    }

    if (ret) {
        return mLegs[idx]->isStop();
    }

    return false;
}

bool TripodBoss::isStopAllLeg() const {
    for (u32 i = 0; i < 3; i++) {
        if (!mLegs[i]->isStop()) {
            return false;
        }
    } 

    return true;
}

bool TripodBoss::isStarted() const {
    return !isNerve(&NrvTripodBoss::TripodBossNrvNonActive::sInstance);
}

bool TripodBoss::isDemo() const {
    if (isStartDemo() || isDamageDemo() || isEndDemo()) {
        return true;
    }
    return false;
}

bool TripodBoss::isStartDemo() const {
    return isNerve(&NrvTripodBoss::TripodBossNrvStartDemo::sInstance);
}

bool TripodBoss::isDamageDemo() const {
    return isNerve(&NrvTripodBoss::TripodBossNrvDamageDemo::sInstance);
}

bool TripodBoss::isEndDemo() const {
    if (isEndPainDemo() || isEndBreakDownDemo() || isEndExplosionDemo()) {
        return true;
    }
    return false;
}

bool TripodBoss::isEndPainDemo() const {
    return isNerve(&NrvTripodBoss::TripodBossNrvPainDemo::sInstance);
}

bool TripodBoss::isEndBreakDownDemo() const {
    return isNerve(&NrvTripodBoss::TripodBossNrvBreakDownDemo::sInstance);
}

bool TripodBoss::isEndExplosionDemo() const {
    return isNerve(&NrvTripodBoss::TripodBossNrvExplosionDemo::sInstance);
}

bool TripodBoss::isBroken() const {
    if (isEndBreakDownDemo() || isEndExplosionDemo() || MR::isDead(this)) {
        return true;
    }
    return false;
}

bool TripodBoss::isRideMario() const {
    return (_634 == 0) || (_634 == 1);
}

bool TripodBoss::isLeaveMarioNow() const {
    return _634 == 3;
}

void TripodBoss::setJointAttachBaseMatrix(const TPos3f &rPos, s32 idx) {
    mBossBones[idx].setAttachBaseMatrix(rPos);
}

void TripodBoss::addStepPoint(TripodBossStepPoint *pPoint) {
    s32 idx = pPoint->_B8;
    TVec3f nearPos;
    mMovableArea->calcNearLandingPosition(&nearPos, pPoint->mStepPosition);
    TVec3f landingNormal;
    mMovableArea->calcLandingNormal(&landingNormal, nearPos);
    TVec3f landingFront;
    mMovableArea->calcLandingFront(&landingFront, nearPos);
    pPoint->setStepPosition(nearPos);
    pPoint->setStepNormal(landingNormal);
    pPoint->setStepFront(landingFront);
    mStepSequence[idx].addStepPoint(pPoint);
}

void TripodBoss::getBodyMatrix(TPos3f *pMtx) const {
    JMath::gekko_ps_copy12(pMtx, mBodyMtx);
}

void TripodBoss::getJointMatrix(TPos3f *pMtx, s32 a2) const {
    JMath::gekko_ps_copy12(pMtx, mBossBones[a2]._30);
}

void TripodBoss::getJointAttachMatrix(TPos3f *pMtx, s32 a2) const {
    pMtx->concat(*mBossBones[a2]._30, mBossBones[a2]._0);
}

void TripodBoss::requestStartStepSequence(s32 seq) {
    TripodBossStepSequence* sequence = &mStepSequence[seq];
    if (!sequence->_88 || sequence->isEmpty()) {
        return;
    }

    if (mCurrentStepSeq == -1) {
        mNextStepSeq = seq;
    }
    else if (mCurrentStepSeq == seq) {
        mNextStepSeq = seq;
    }
    else if (mNextStepSeq == -1) {
        mNextStepSeq = seq;
    }
}

TripodBossStepSequence* TripodBoss::getCurrentStepSequence() {
    if (mCurrentStepSeq == -1) {
        return nullptr;
    }

    return &mStepSequence[mCurrentStepSeq];
}

TripodBossStepSequence* TripodBoss::getNextStepSequence() {
    if (mNextStepSeq == -1) {
        return nullptr;
    }

    return &mStepSequence[mNextStepSeq];
}

void TripodBoss::calcLegUpVector(TVec3f *pUp, const TVec3f &a2) {
    TVec3f* center = &mMovableArea->mCenter;
    TVec3f v8(a2);
    v8 -= *center;
    MR::normalizeOrZero(&v8);
    TVec3f* axis = &mMovableArea->mBaseAxis;
    f32 v7 = axis->dot(v8);
    JMAVECScaleAdd(axis, &v8, &v8, -v7);
    MR::normalizeOrZero(&v8);
    pUp->set<f32>(v8);
}

void TripodBoss::calcDemoMovement() {
    TPos3f mtx;
    JMath::gekko_ps_copy12(&mtx, MR::getJointMtx(this, "Body"));
    MR::blendMtx(_EC, mtx, _600, mBodyMtx);
    mBodyMtx.getTrans(_5D4);

    for (u32 i = 0; i < 3; i++) {
        MtxPtr jointMtx = MR::getJointMtx(this, sLegBoneNameTable[i]);
        TPos3f v8;
        JMath::gekko_ps_copy12(&v8, jointMtx);
        TVec3f v7;
        v8.getTrans(v7);
        TVec3f v6;
        v6.x = JGeometry::TUtil<f32>::sqrt(v8.dotX());
        v6.y = JGeometry::TUtil<f32>::sqrt(v8.dotY());
        v6.z = JGeometry::TUtil<f32>::sqrt(v8.dotZ());
        mLegs[i]->setForceEndPoint(v7);
        mLegs[i]->setDemoEffectTiming(v6.x > 1.5f);
    }

    calcLegMovement();
}

void TripodBoss::calcBodyMovement() {
    addAccelToWeightPosition();
    _5D4 += _5E0;
    _5E0.x *= 0.94999999f;
    _5E0.y *= 0.94999999f;
    _5E0.z *= 0.94999999f;
    MR::makeMtxTR(mBodyMtx, _5D4, mRotation);
}

void TripodBoss::calcLegMovement() {
    for (u32 i = 0; i < 3; i++) {
        mLegs[i]->movement();
    }
}

void TripodBoss::addAccelToWeightPosition() {
    TBox3f v21;
    TVec3f v18, v19, v20;
    TBox3f v22;
    v22.mMin.set<f32>(_5C8);
    v22.mMax.set<f32>(_5C8);
    v21.mMin.set<f32>(_5C8);
    v21.mMax.set<f32>(_5C8);

    for (u32 i = 0; i < 3; i++) {
        if (mLegs[i]->canWeighting()) {
            v22.extend(mLegs[i]->mForceEndPoint);
        }

        v21.extend(mLegs[i]->mForceEndPoint);
    }

    JMAVECLerp(&v22.mMax, &v22.mMin, &v20, 0.5f);
    
    JMAVECLerp(&v21.mMax, &v21.mMin, &v19, 0.5f);
    
    MR::vecBlend(v19, v20, &v18, 0.30000001f);
    TVec3f* center = &mMovableArea->mCenter;
    TVec3f v14(v18);
    v14 -= *center;
    TVec3f* axis = &mMovableArea->mBaseAxis;
    f32 v7 = axis->dot(v14);
    JMAVECScaleAdd(axis, &v14, &v18, -v7);
    f32 v9 = (_5FC + (_604 + mMovableArea->mRadius));
    TVec3f v13(mMovableArea->mBaseAxis);
    v13 *= v9;
    TVec3f v17(v13);
    v17 += v18;
    MR::normalizeOrZero(&v17);
    TVec3f v12(v17);
    v12 *= v9;
    TVec3f v16(mMovableArea->mCenter);
    v16 += v12;
    TVec3f v15(v16);
    v15 -= _5D4;
    f32 v10 = PSVECMag(&v15);
    if (v10 < 500.0f) {
        v10 = 500.0f;
    }

    v15 *= (1.0f / v10);
    TVec3f v11(v15);
    v11 *= 0.80000001f;
    _5E0 += v11;
}

void TripodBoss::calcClippingSphere() {
    TBox3f v4;
    v4.mMin.set<f32>(_5D4);
    v4.mMax.set<f32>(_5D4);

    for (u32 i = 0; i < 3; i++) {
        v4.extend(mLegs[i]->mForceEndPoint);
    }

    JMAVECLerp(&v4.mMax, &v4.mMin, &_5EC, 0.5f);
}

void TripodBoss::clippingModel() {
    if (MR::isJudgedToClipFrustum(_5EC, _5F8)) {
        if (!MR::isDead(mLowModel)) {
            mLowModel->makeActorDead();
        }

        if (!_638) {
            MR::hideTripodBossParts();
            _638 = 1;
        }
    }
    else if (MR::calcCameraDistanceZ(_5EC) < 34000.0f) {
        if (!MR::isDead(mLowModel)) {
            mLowModel->makeActorDead();
        }

        if (_638) {
            MR::showTripodBossParts();
            _638 = 0;
        }
    }
    else {
        if (MR::isDead(mLowModel)) {
            mLowModel->makeActorAppeared();
        }

        if (!_638) {
            MR::hideTripodBossParts();
            _638 = 1;
        }
    }
}

void TripodBoss::startDemo() {
    MR::onCalcAnim(this);
    JMath::gekko_ps_copy12(&_EC, mBodyMtx);
    MR::requestMovementOn(mDummyModel);
    MR::requestMovementTripodBossParts();

    for (u32 i = 0; i < 3; i++) {
        mLegs[i]->requestStartDemo();
    }
}

void TripodBoss::endDemo(const char *pName) {
    MR::endDemo(this, pName);
    MR::offCalcAnim(this);

    for (u32 i = 0; i < 3; i++) {
        mLegs[i]->requestEndDemo();
    }
}

void TripodBoss::checkRideMario() {
    GravityInfo info;
    TVec3f v8;
    MR::calcGravityAndMagnetVector(this, *MR::getPlayerPos(), v8, &info, 0);

    if (info.mGravityInstance != nullptr && MR::isTripoddBossParts((const NameObj*)info.mGravityInstance->mHost)) {
        _630 = 120;
    }    
    else {
        if (_630 > 0) {
            _630--;
        }
    }

    if (_630 > 0) {
        bool v5 = 1;
        v5 = isStateSomething();
        
        if (v5) {
            _634 = 0;
        }
        else {
            _634 = 1;
        }
    }
    else {
        bool v7 = 1;
        v7 = isStateSomething();

        if (v7) {
            _634 = 3;
        }
        else {
            _634 = 2;
        }
    }
}

const TPos3f* TripodBoss::getLegMatrixPtr(PART_ID partID, SUB_PART_ID subPartID) const {
    bool v3 = false;
    if (partID >= LeftLeg && partID <= RightLeg) {
        v3 = true;
    }

    if (!v3) {
        return nullptr;
    }

    switch (subPartID) {
        case Part_RootLocalY:
            return &mLegs[partID]->getRootLocalYMatrix();
        case Part_RootLocalYZ:
            return &mLegs[partID]->getRootLocalYZMatrix();
        case Part_RootJoint:
            return &mLegs[partID]->getRootJointMatrix();
        case Part_MiddleJoint:
            return &mLegs[partID]->getMiddleJointMatrix();
        case Part_AnkleLocalX:
            return &mLegs[partID]->getAnkleLocalXMatrix();
        case Part_AnkleLocalXZ:
            return &mLegs[partID]->getAnkleLocalXZMatrix();
        case Part_EndJoint:
            return &mLegs[partID]->getEndJointMatrix();
        default:
            return nullptr;
    }
}

void TripodBoss::changeBgmState() {
    if (_640 && !isDemo()) {
        if (!MR::isPlayingStageBgm()) {
            MR::startBossBGM(3);
        }

        _640 = 0;
    }

    bool v3 = isStateSomething();
    if (v3) {
        if (_63C != 3) {
            MR::setStageBGMState(3, 60);
        }

        _63C = 3;
    }
    else {
        if (_63C != 1) {
            MR::setStageBGMState(1, 60);
        }

        _63C = 1;
    }
}

s32 TripodBoss::getPartIDFromBoneID(s32 boneID) {
    switch (boneID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return 0;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return 1;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            return 2;
        case 21:
            return 3;
        default:
            return -1;
    }
}

void TripodBossBone::setAttachBaseMatrix(const TPos3f &rPos) {
    _0.invert(rPos);
    JGeometry::TUtil<f32>::sqrt(_0.dot());

    if (_0) {
        f32 v3 = JGeometry::TUtil<f32>::inv_sqrt((_0.mMtx[1][0] * _0.mMtx[1][0]) + (_0.mMtx[0][0] * _0.mMtx[0][0]) + (_0.mMtx[2][0] * _0.mMtx[2][0]));
        _0.mMtx[0][0] = v3 * _0.mMtx[0][0];
        _0.mMtx[1][0] = v3 * _0.mMtx[1][0];
        _0.mMtx[2][0] = v3 * _0.mMtx[2][0];
        f32 v8 = JGeometry::TUtil<f32>::inv_sqrt((_0.mMtx[2][1] * _0.mMtx[2][1]) + ((_0.mMtx[1][1] * _0.mMtx[1][1]) + (_0.mMtx[0][1] * _0.mMtx[0][1])));
        _0.mMtx[0][1] = v8 * _0.mMtx[0][1];
        _0.mMtx[1][1] = v8 * _0.mMtx[1][1];
        _0.mMtx[2][1] = v8 * _0.mMtx[2][1];
        f32 v13 = JGeometry::TUtil<f32>::inv_sqrt( (_0.mMtx[0][2] * _0.mMtx[0][2]) + ((_0.mMtx[1][2] * _0.mMtx[1][2]) + (_0.mMtx[2][2] * _0.mMtx[2][2])));
        _0.mMtx[0][2] = v13 * _0.mMtx[0][2];
        _0.mMtx[1][2] = v13 * _0.mMtx[1][2];
        _0.mMtx[2][2] = v13 * _0.mMtx[2][2];
    }
}

namespace MR {
    NameObj* createTripodBoss(const char *pName) {
        return new TripodBoss(pName);
    }

    NameObj* createTripod2Boss(const char *pName) {
        return new TripodBoss(pName);
    }
};
