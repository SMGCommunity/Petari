#include "Game/Enemy/TeresaWater.hpp"

namespace NrvTeresaWater {
    NEW_NERVE(TeresaWaterNrvMove, TeresaWater, Move);
    NEW_NERVE(TeresaWaterNrvHit, TeresaWater, Hit);
    NEW_NERVE(TeresaWaterNrvStun, TeresaWater, Stun);
    NEW_NERVE_ONEND(TeresaWaterNrvDPDSwoon, TeresaWater, DPDSwoon, DPDSwoon);
}  // namespace NrvTeresaWater

namespace {
    const MR::ActorMoveParam sMoveParam = {0.5f, 0.0f, 0.9f, 1.5f};
}

TeresaWater::TeresaWater(const char* pName)
    : LiveActor(pName), mScaleController(nullptr), mBindStarPointer(nullptr), _90(0.0f, 0.0f, 1.0f), _A0(0.0f, 0.0f), _AC(0.0f, 0.0f), _B8(nullptr) {
}  // I feel like the def of _90 is wrong
TeresaWater::~TeresaWater() {
}

void TeresaWater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TeresaWater", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f null1;
    null1.x = 0.0f;
    null1.y = 0.0f;
    null1.z = 0.0f;
    MR::addHitSensorAtJointEnemySimple(this, "body", "SklRoot", ATYPE_RIDE_START, 60.0f, null1);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    TVec3f null2;
    null2.x = 0.0f;
    null2.y = 0.0f;
    null2.z = 0.0f;
    MR::initStarPointerTargetAtJoint(this, "SklRoot", 100.0f, null2);
    mScaleController = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    MR::declareCoin(this, 1);
    MR::calcFrontVec(&_90, this);
    f32 v8 = 800.0f;
    MR::getJMapInfoArg0NoInit(rIter, &v8);
    _AC.x = mPosition.x;
    _AC.y = mPosition.y;
    _AC.z = mPosition.z;
    JMAVECScaleAdd(_90, &_AC, &_A0, v8);
    _B8 = &_A0;
    MR::initShadowVolumeCylinder(this, 100.0f);
    initNerve(&NrvTeresaWater::TeresaWaterNrvMove::sInstance);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void TeresaWater::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
    MR::moveAndTurnToTarget(this, &_90, *_B8, sMoveParam._0, sMoveParam._4, sMoveParam._8, sMoveParam._C);
    MR::attenuateVelocityExceptDirection(this, _90, 0.5f);
    if (MR::isNear(this, *_B8, 30.0f)) {
        if (_B8 == _A0) {
            _B8 = &_AC;
            return;
        }
        _B8 = &_A0;
    }
}

void TeresaWater::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
        mVelocity.z = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.x = 0.0f;
        MR::startSound(this, "SE_EV_TERESA_ATTACK_SUCCESS", -1, -1);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvTeresaWater::TeresaWaterNrvMove::sInstance);
    }
}

void TeresaWater::exeStun() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Reaction", nullptr);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvTeresaWater::TeresaWaterNrvMove::sInstance);
    }
}

void TeresaWater::kill() {
    MR::appearCoinPop(this, mPosition, 1);
    MR::emitEffect(this, "DeathWater");
    MR::startSound(this, "SE_EM_TERESA_DEAD", -1, -1);
    MR::startSound(this, "SE_EV_TERESA_DEAD", -1, -1);
    LiveActor::kill();
}

void TeresaWater::control() {
    mScaleController->updateNerve();
    if (isNerve(&NrvTeresaWater::TeresaWaterNrvMove::sInstance)) {
        MR::startLevelSound(this, "SE_EM_LV_TERESA_MOVE", -1, -1, -1);
    }
    tryDPDSwoon();
}

void TeresaWater::calcAndSetBaseMtx() {
    TVec3f johnK;
    MR::calcUpVec(&johnK, this);
    JMathInlineVEC::PSVECNegate(johnK, johnK);
    TPos3f johnL;
    MR::calcMtxFromGravityAndZAxis(&johnL, this, johnK, _90);
    MR::setBaseTRMtx(this, johnL);
    TVec3f sc;
    JMathInlineVEC::PSVECMultiply(mScaleController->_C, mScale, sc);
    MR::setBaseScale(this, sc);
}

void TeresaWater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvTeresaWater::TeresaWaterNrvHit::sInstance) && !MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }
    if (isNerve(&NrvTeresaWater::TeresaWaterNrvDPDSwoon::sInstance) || MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        setNerve(&NrvTeresaWater::TeresaWaterNrvHit::sInstance);
        return;
    }
    MR::sendMsgPush(pReceiver, pSender);
}

bool TeresaWater::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvTeresaWater::TeresaWaterNrvStun::sInstance);
        return true;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgSearchlightAttack(msg)) {
        kill();
        return true;
    }
    return false;
}

bool TeresaWater::tryDPDSwoon() {
    if (isNerve(&NrvTeresaWater::TeresaWaterNrvStun::sInstance)) {
        return false;
    }
    if (isNerve(&NrvTeresaWater::TeresaWaterNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }
    setNerve(&NrvTeresaWater::TeresaWaterNrvDPDSwoon::sInstance);
    return true;
}

void TeresaWater::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvTeresaWater::TeresaWaterNrvMove::sInstance);
}

void TeresaWater::endDPDSwoon() {
    mBindStarPointer->kill();
}
