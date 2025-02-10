#include "Game/Boss/SkeletalFishBaby.hpp"
#include "Game/Boss/SkeletalFishJointCalc.hpp"
#include "Game/Boss/SkeletalFishRailControl.hpp"
#include "Game/MapObj/AirBubbleHolder.hpp"
#include <cstdio>

namespace {
    const Vec sStarPointerTargetOffset[4] = { 
        { 0.0f, 0.0f, 150.0f, },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f }
    };

    const f32 sStarPointerTargetSize[4] = {
        220.0f,
        150.0f,
        120.0f,
        120.0f
    };

    const char* sStarPointerTargetJoint[4] = { "Joint00", "Joint01", "Joint02", "Joint03" };
};

SkeletalFishBaby::SkeletalFishBaby(const char *pName) : LiveActor(pName) {
    mJointIndicies = nullptr;
    _A0 = 20.0f;
    _A4 = 20.0f;
    mRailControl = new SkeletalFishRailControl();
    _DC = 3000.0f;
    mScaleController = new AnimScaleController(nullptr);

    for (s32 i = 0; i < 4; i++) {
        mControllers[i] = nullptr;
    }

    _AC.identity();
}

void SkeletalFishBaby::init(const JMapInfoIter &rIter) {
    MR::createAirBubbleHolder();
    MR::initDefaultPos(this, rIter);

    if (rIter.isValid()) {
        MR::getJMapInfoArg0NoInit(rIter, &_A0);
        MR::getJMapInfoArg1NoInit(rIter, &_DC);
    }

    _A4 = _A0;
    initModelManagerWithAnm("SnakeFish", nullptr, false);
    initRail(rIter);
    initJoint();
    initNerve(&::SkeletalFishBabyNrvSwim::sInstance);
    initSensor();
    MR::setClippingTypeSphere(this, 500.0f);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSound(6, false);
    MR::initShadowFromCSV(this, "Shadow");
    MR::setGroupClipping(this, rIter, 32);
    initEffectKeeper(0, nullptr, false);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::declareStarPiece(this, 10);
    MR::onCalcGravity(this);

    for (s32 i = 0; i < 4; i++) {
        mStarPieceTargets[i] = new LiveActor("StarPointerTargetDummy");
        mStarPieceTargets[i]->makeActorDead();

        TVec3f ptrTarget(sStarPointerTargetOffset[i]);
        MR::initStarPointerTargetAtMtx(mStarPieceTargets[i], sStarPointerTargetSize[i], MR::getJointMtx(this, sStarPointerTargetJoint[i]), ptrTarget);
    }
}

void SkeletalFishBaby::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::startBck(this, "Swim", nullptr);
    setNerve(&::SkeletalFishBabyNrvSwim::sInstance);
    MR::validateClipping(this);
}

void SkeletalFishBaby::appear() {
    LiveActor::appear();
}

void SkeletalFishBaby::kill() {
    LiveActor::kill();
    MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER", -1, -1);

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void SkeletalFishBaby::control() {
    mRailControl->_14 = _A4;
    mRailControl->update();
    mRailControl->getPos(&mPosition, 0.0f);
    mScaleController->updateNerve();

    if (isNerve(&::SkeletalFishBabyNrvSwim::sInstance)) {
        MR::startLevelSound(this, "SE_EM_LV_SNAKEFISH_SWIM", -1, -1, -1);
    }
}

void SkeletalFishBaby::calcAnim() {
    LiveActor::calcAnim();
}

void SkeletalFishBaby::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayerOrRide(a2)) {
        if (isAttackable() && MR::sendMsgEnemyAttackStrong(a2, a1)) {
            MR::shakeCameraStrong();
        }
        else {
            MR::sendMsgPush(a2, a1);
        }
    }
}

bool SkeletalFishBaby::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgJetTurtleAttack(msg)) {
        return damage(a2->mPosition, true);
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        mScaleController->startHitReaction();
        return true;
    }

    return false;   
}

bool SkeletalFishBaby::receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *) {
    return false;
}

bool SkeletalFishBaby::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    switch (msg) {
        case 190:
            MR::invalidateClipping(this);
            return true;
        case 191:
            MR::validateClipping(this);
            return true;
        default:
            break;
    }

    return false;
}

void SkeletalFishBaby::exeSwim() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swim", nullptr);
    }

    f32 v3 = _A4 + 0.1f;
    _A4 = v3;

    if (v3 > _A0) {
        _A4 = _A0;
    }

    if (isStarPointerPointing()) {
        setNerve(&::SkeletalFishBabyNrvBind::sInstance);
    }
}

void SkeletalFishBaby::exeBind() {
    if (MR::isFirstStep(this)) {
        mScaleController->startDpdHitVibration();
        MR::startDPDHitSound();
        _A4 = 0.0f;
        MR::emitEffect(this, "StarPointerHolder");
    }

    MR::startDPDFreezeLevelSound(this);

    if (!isStarPointerPointing()) {
        setNerve(&::SkeletalFishBabyNrvSwim::sInstance);
    }
}

void SkeletalFishBaby::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Death", nullptr);
        MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER", -1, -1);
        MR::startSound(this, "SE_EV_SNAKEHEAD_DAMAGE", -1, -1);
        MR::invalidateClipping(this);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&::SkeletalFishBabyNrvDead::sInstance);
    }
}

void SkeletalFishBaby::exeDead() {
    if (MR::isFirstStep(this)) {
        TVec3f jointPos;
        MR::copyJointPos(this, "Center", &jointPos);
        MR::appearStarPiece(this, jointPos, 10, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W_F", -1, -1);
        kill();
    } 
}

bool SkeletalFishBaby::calcJoint(TPos3f *pJointPos, const JointControllerInfo &rInfo) {
    if (mJointIndicies[rInfo._4->_14] == -1) {
        return false;
    }

    SkeletalFishJointCalc::calcJoint(pJointPos, &_AC, mRailControl, rInfo);
    return true; 
}

bool SkeletalFishBaby::damage(const TVec3f &rAirBubblePos, bool shakeCameraNormal) {
    bool isDeadOrBroke = false;

    if (isNerve(&::SkeletalFishBabyNrvDead::sInstance) || isNerve(&::SkeletalFishBabyNrvBreak::sInstance)) {
        isDeadOrBroke = true;
    }

    if (!isDeadOrBroke) {
        if (shakeCameraNormal) {
            MR::shakeCameraNormal();
        }
        else {
            MR::shakeCameraWeak();
        }

        MR::appearAirBubble(rAirBubblePos, -1);
        setNerve(&::SkeletalFishBabyNrvBreak::sInstance);
        return true;
    }

    return false;
}

/*
void SkeletalFishBaby::calcAndSetBaseMtx() {
    if (mRailControl->_8 != nullptr) {
        for (s32 i = 0; i < 4; i++) {
            mControllers[i]->registerCallBack();
        }
 
        TPos3f railMtx;
        mRailControl->getMtx(&railMtx, 0.0f);
        MR::setBaseTRMtx(this, railMtx);
        _AC.setInline(railMtx);
        _AC.invert(_AC);
        TVec3f scale;
        scale.multPS(mScale, mScaleController->_C);
        MR::setBaseScale(this, scale);
    }
}
*/

void SkeletalFishBaby::initRail(const JMapInfoIter &rIter) {
    if (rIter.isValid()) {
        initRailRider(rIter);
        mRailControl->setRailActor(this, nullptr, true);
        mRailControl->_14 = _A4;
        MR::moveCoordToNearestPos(this, mPosition);
    }
}

void SkeletalFishBaby::initJoint() {
    mJointIndicies = new s32[MR::getJointNum(this)];

    for (u32 i = 0; i < MR::getJointNum(this); i++) {
        mJointIndicies[i] = -1;
    }

    for (s32 i = 0; i < 4; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "Joint%02d", i);
        mControllers[i] = MR::createJointDelegatorWithNullMtxFunc(this, &SkeletalFishBaby::calcJoint, buf);
        mJointIndicies[MR::getJointIndex(this, buf)] = i;
    }
}

void SkeletalFishBaby::initSensor() {
    initHitSensor(4);
    MR::addHitSensorAtJointEnemy(this, "head", "Joint00", 16, 190.0f, TVec3f(0.0f, 20.0f, 100.0f));
    MR::addHitSensorAtJointEnemy(this, "bust", "Joint01", 16, 120.0f, TVec3f(0.0f, 00.0f, 30.0f));
    MR::addHitSensorAtJointEnemy(this, "waist", "Joint02", 16, 120.0f, TVec3f(0.0f, 00.0f, 20.0f));
    MR::addHitSensorAtJointEnemy(this, "tail", "Joint03", 16, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

bool SkeletalFishBaby::isAttackable() const {
    bool isAttackable = false;
    if (!isNerve(&::SkeletalFishBabyNrvDead::sInstance)
        && !isNerve(&::SkeletalFishBabyNrvBreak::sInstance)
        && !isNerve(&::SkeletalFishBabyNrvBind::sInstance)) {
            isAttackable = true;
        }

    return isAttackable;
}

bool SkeletalFishBaby::isStarPointerPointing() const {
    for (u32 i = 0; i < 4; i++) {
        if (MR::isStarPointerPointing2POnPressButton(mStarPieceTargets[i], "弱", true, false)) {
            return true;
        }   
    }

    return false;
}

namespace {
    INIT_NERVE(SkeletalFishBabyNrvSwim);
    INIT_NERVE(SkeletalFishBabyNrvBind);
    INIT_NERVE(SkeletalFishBabyNrvBreak);
    INIT_NERVE(SkeletalFishBabyNrvDead);
};

SkeletalFishBaby::~SkeletalFishBaby() {

}
