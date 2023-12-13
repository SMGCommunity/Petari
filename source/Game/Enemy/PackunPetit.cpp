#include "Game/Enemy/PackunPetit.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include <JSystem/JMath/JMath.hpp>

PackunPetit::PackunPetit(const char *pName) : LiveActor(pName) {
    mScaleController = nullptr;
    mStarPointerState = nullptr;
    _94.x = 0.0f;
    _94.y = 0.0f;
    _94.z = 1.0f;
    mBlownModel = nullptr;
    mDontTurn = false;
}

void PackunPetit::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PackunPetit", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(3);
    MR::addHitSensorAtJointMapObj(this, "body", "Head", 8, 90.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemyAttack(this, "attack", "Head", 8, 50.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObj(this, "stalk", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::initLightCtrl(this);
    MR::initShadowFromCSV(this, "Shadow");
    MR::calcGravity(this);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::declareCoin(this, 1);
    MR::declareStarPiece(this, 6);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::initStarPointerTargetAtJoint(this, "Head", 100.0f, offs);
    mScaleController = new AnimScaleController(nullptr);
    mStarPointerState = new WalkerStateBindStarPointer(this, mScaleController);
    MR::getJMapInfoArg0NoInit(rIter, &mDontTurn);
    MR::calcFrontVec(&_94, this);
    initBlowModel();
    initNerve(&NrvPackunPetit::PackunPetitNrvWait::sInstance);
    makeActorAppeared();
}

void PackunPetit::initAfterPlacement() {
    MR::initEffectAfterPlacement(this);
}

void PackunPetit::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::offCalcAnim(this);
        MR::offCalcShadow(this, "JawA");
    }

    if (MR::isNearPlayerAnyTime(this, 5000.0f)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvWait::sInstance);
    }
}

void PackunPetit::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckWithInterpole(this, "Wait", 60);
    }

    if (MR::isNearPlayer(this, 1700.0f)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvFind::sInstance);
    }
    else {
        tryNonActive();
    }
}

void PackunPetit::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Suspect", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvThreat::sInstance);
    }
}

void PackunPetit::exeThreat() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Threat", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_PACKUNPETIT_THREAT", -1, -1, -1);
    if (!mDontTurn) {
        if (tryTurn()) {
            return;
        }

        TVec3f* pos = MR::getPlayerPos();
        MR::turnDirectionToTargetUseGroundNormalDegree(this, &_94, *pos, 2.0f);
    }

    if (MR::isGreaterStep(this, 50) && MR::isNearPlayer(this, 1100.0f) && (MR::isFaceToPlayerHorizontalDegree(this, _94, 10.0f) || mDontTurn)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvAttackStart::sInstance);
    }
    else if (!MR::isNearPlayer(this, 2000.0f)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvWait::sInstance);
    }
}

void PackunPetit::exeTurn() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvPackunPetit::PackunPetitNrvLeftTurn::sInstance)) {
            MR::startBck(this, "TurnLeft", nullptr);
        }
        else {
            MR::startBck(this, "TurnRight", nullptr);
        }
    }

    f32 step = getNerveStep();
    step = (10.0f - step);
    if (step < 2.0f) {
        step = 2.0f;
    }
    TVec3f* pos = MR::getPlayerPos();
    MR::turnDirectionToTargetUseGroundNormalDegree(this, &_94, *pos, step);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvThreat::sInstance);
    }
}

void PackunPetit::exeAttackStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }

    if (MR::isStep(this, 70)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvAttack::sInstance);
    }
}

void PackunPetit::exeHitWaitForAttack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Attack")) {
        MR::startBck(this, "Impact", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvHit::sInstance);
    }
}

void PackunPetit::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
    }

    if (MR::isBckStopped(this) && !tryTurn()) {
        selectNrvWait();
    }
}

void PackunPetit::exeTrampleDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Press", nullptr);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::invalidateHitSensors(this);
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void PackunPetit::exePunchDown() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::invalidateHitSensor(this, "attack");
        mBlownModel->makeActorAppeared();
        MR::copyJointPos(this, "Head", &mBlownModel->mPosition);
        MR::startBck(mBlownModel, "BlowHead", nullptr);
        MR::startBck(this, "Blow", nullptr);
        MR::startBlowHitSound(this);
    }

    JMAVECScaleAdd(mGravity.toCVec(), mBlownModel->mVelocity.toCVec(), mBlownModel->mVelocity.toVec(), 2.5f);

    if (!MR::isHiddenModel(mBlownModel) && (MR::isStep(this, 20) || MR::checkStikeBallToMap(mBlownModel->mPosition, 50.0f))) {
        MR::emitEffect(mBlownModel, "Death");
        MR::appearStarPiece(this, mBlownModel->mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        MR::hideModel(mBlownModel);
    }

    if (MR::isStep(this, 30)) {
        kill();
    }
}

void PackunPetit::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonStart", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvSwoon::sInstance);
    }
}

void PackunPetit::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

    if (MR::isStep(this, 250)) {
        selectNrvWait();
    }
}

void PackunPetit::exeSwoonToThreat() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonToThreat", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvThreat::sInstance);
    }
}

void PackunPetit::exeSwoonOnEnd() {
    mStarPointerState->kill();
}

void PackunPetit::exeDPDSwoon() {
    if (MR::updateActorState(this, mStarPointerState)) {
        selectNrvWait();
    }
}

void PackunPetit::exeAttackBack() {
    if (MR::isBckStopped(this)) {
        if (!tryTurn()) {
            selectNrvWait();
        }
    }
}

void PackunPetit::exeAttack() {
    if (MR::isStep(this, 0xC)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvAttackBack::sInstance);
    }
}

void PackunPetit::exeNonActiveOnEnd() {
    MR::validateHitSensors(this);
    MR::onCalcAnim(this);
    MR::onCalcShadow(this, "JawA");
}

void PackunPetit::kill() {
    if (!MR::isDead(mBlownModel)) {
        if (!MR::isHiddenModel(mBlownModel)) {
            MR::emitEffect(mBlownModel, "Death");
            MR::appearStarPiece(this, mBlownModel->mPosition, 3, 10.0f, 40.0f, false);
            MR::startSound(mBlownModel, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }

        mBlownModel->kill();
    }
    else {
        if (!MR::isValidSwitchDead(this)) {
            MR::appearCoinPop(this, mPosition, 1);
        }
    }
    
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    LiveActor::kill();
}

void PackunPetit::calcAndSetBaseMtx() {
    TVec3f up;
    MR::calcUpVec(&up, this);
    TPos3f mtxUp;
    mtxUp.identity();
    MR::makeMtxUpFrontPos(&mtxUp, up, _94, mPosition);
    TVec3f mult;
    mult.multPS(mScale, mScaleController->_C);
    MR::setBaseScale(this, mult);
}

void PackunPetit::control() {
    mScaleController->update();
    tryDPDSwoon();
}

/*
void PackunPetit::attackSensor(HitSensor *a2, HitSensor *a3) {
    if (MR::isSensorPlayer(a3)) {
        bool isTrampleOrPunch = false;

        if (isNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance)) {
            isTrampleOrPunch = true;
        }

        
    }
}*/

bool PackunPetit::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool isTrampleOrPunch = false;

    if (isNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance)) {
        isTrampleOrPunch = true;
    }

    if (isTrampleOrPunch) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvSwoonStart::sInstance);
        return true;
    }
    else {
        bool isWaitOrHit = false;

        if (isNerve(&NrvPackunPetit::PackunPetitNrvHitWaitForAttack::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvHit::sInstance)) {
            isWaitOrHit = true;
        }

        if (isWaitOrHit) {
            return false;
        }
        else if (MR::isMsgInvincibleAttack(msg) || MR::isMsgFireBallAttack(msg)) {
            punchDown(a2, a3);
            return true;
        }
        else if (isNerve(&NrvPackunPetit::PackunPetitNrvAttack::sInstance)) {
            return false;
        }
        else if (MR::isMsgPlayerTrample(msg)) {
            MR::tryRumbleDefaultHit(this, 0);
            setNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance);
            return true;
        }
        else if (MR::isMsgPlayerHipDrop(msg)) {
            MR::tryRumbleDefaultHit(this, 0);
            setNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance);
            return true;
        }
        else if (MR::isMsgPlayerHitAll(msg)) {
            MR::stopSceneForDefaultHit(5);
            punchDown(a2, a3);
            return true;
        }

        return false;
    }
}

bool PackunPetit::receiveMsgEnemyAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool isTrampleOrPunch = false;

    if (isNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance)) {
        isTrampleOrPunch = true;
    }

    if (isTrampleOrPunch) {
        return false;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        punchDown(a2, a3);
        setNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance);
        return true;
    }
    else if (MR::isMsgToEnemyAttackTrample(msg)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance);
        return true;
    }

    return false;
}

bool PackunPetit::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool isSwoon = false;

    if (isNerve(&NrvPackunPetit::PackunPetitNrvSwoonStart::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvSwoonStart::sInstance)) {
        isSwoon = true;
    }

    if (!isSwoon) {
        return false;
    }

    if (!MR::isSensorPlayer(a2)) {
        return false;
    }

    if (MR::isMsgPlayerKick(msg)) {
        MR::stopSceneForDefaultHit(5);
        punchDown(a2, a3);
        return true;
    }

    return false;
}

void PackunPetit::initBlowModel() {
    mBlownModel = MR::createModelObjMapObjStrongLight("吹っ飛びモデル", "PackunPetitHead", nullptr);
    mBlownModel->initWithoutIter();
    MR::initShadowVolumeSphere(mBlownModel, 70.0f);
    MR::invalidateClipping(mBlownModel);
    MR::initLightCtrl(mBlownModel);
    mBlownModel->makeActorDead();
}

void PackunPetit::punchDown(HitSensor *a1, HitSensor *a2) {
    TVec3f v6;
    v6.subtract(a2->mPosition, a1->mPosition);
    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(v6);
    JMAVECScaleAdd(grav->toCVec(), v6.toCVec(), v6.toVec(), -dot);
    MR::normalize(&v6);

    TVec3f v5;
    v5.scale(20.0f, v6);
    JMAVECScaleAdd(mGravity.toCVec(), v5.toCVec(), v5.toVec(), -40.0f);
    mBlownModel->mVelocity.setInline(v5);
    setNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance);
}

void PackunPetit::selectNrvWait() {
    if (!MR::isNearPlayer(this, 1700.0f)) {
        setNerve(&NrvPackunPetit::PackunPetitNrvWait::sInstance);
    }
    else {
        if (isNerve(&NrvPackunPetit::PackunPetitNrvSwoon::sInstance)) {
            setNerve(&NrvPackunPetit::PackunPetitNrvSwoonToThreat::sInstance);
        }
        else {
            setNerve(&NrvPackunPetit::PackunPetitNrvThreat::sInstance);
        }
    }
}

bool PackunPetit::tryNonActive() {
    if (MR::isNearPlayerAnyTime(this, 5000.0f)) {
        return false;
    }

    setNerve(&NrvPackunPetit::PackunPetitNrvNonActive::sInstance);
    return true;
}

bool PackunPetit::tryTurn() {
    if (mDontTurn) {
        return false;
    }

    if (MR::isFaceToPlayerHorizontalDegree(this, _94, 90.0f)) {
        return false;
    }

    TVec3f* pos = MR::getPlayerPos();
    TVec3f playerDist;
    playerDist.subtract(*pos, mPosition);
    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);

    if (playerDist.dot(sideVec) > 0.0f) {
        setNerve(&NrvPackunPetit::PackunPetitNrvLeftTurn::sInstance);
    }
    else {
        setNerve(&NrvPackunPetit::PackunPetitNrvRightTurn::sInstance);
    }

    return true;
}

bool PackunPetit::tryDPDSwoon() {
    if (isNerve(&NrvPackunPetit::PackunPetitNrvDPDSwoon::sInstance)) {
        return false;
    }

    bool isSwoon = false;

    if (isNerve(&NrvPackunPetit::PackunPetitNrvSwoonStart::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvSwoon::sInstance)) {
        isSwoon = true;
    }

    if (isSwoon) {
        return false;
    }

    bool isTrampleOrPunch = false;

    if (isNerve(&NrvPackunPetit::PackunPetitNrvTrampleDown::sInstance) || isNerve(&NrvPackunPetit::PackunPetitNrvPunchDown::sInstance)) {
        isTrampleOrPunch = true;
    }

    if (isTrampleOrPunch) {
        return false;
    }

    if (!mStarPointerState->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvPackunPetit::PackunPetitNrvDPDSwoon::sInstance);
    return true;
}

PackunPetit::~PackunPetit() {

}

namespace NrvPackunPetit {
    INIT_NERVE(PackunPetitNrvNonActive);
    INIT_NERVE(PackunPetitNrvWait);
    INIT_NERVE(PackunPetitNrvFind);
    INIT_NERVE(PackunPetitNrvThreat);
    INIT_NERVE(PackunPetitNrvLeftTurn);
    INIT_NERVE(PackunPetitNrvRightTurn);
    INIT_NERVE(PackunPetitNrvAttackStart);
    INIT_NERVE(PackunPetitNrvAttack);
    INIT_NERVE(PackunPetitNrvAttackBack);
    INIT_NERVE(PackunPetitNrvHitWaitForAttack);
    INIT_NERVE(PackunPetitNrvHit);
    INIT_NERVE(PackunPetitNrvTrampleDown);
    INIT_NERVE(PackunPetitNrvPunchDown);
    INIT_NERVE(PackunPetitNrvSwoonStart);
    INIT_NERVE(PackunPetitNrvSwoon);
    INIT_NERVE(PackunPetitNrvSwoonToThreat);
    INIT_NERVE(PackunPetitNrvDPDSwoon);
};