#include "Game/Enemy/TeresaWater.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

namespace NrvTeresaWater {
    NEW_NERVE(TeresaWaterNrvMove, TeresaWater, Move);
    NEW_NERVE(TeresaWaterNrvHit, TeresaWater, Hit);
    NEW_NERVE(TeresaWaterNrvStun, TeresaWater, Stun);
    NEW_NERVE_ONEND(TeresaWaterNrvDPDSwoon, TeresaWater, DPDSwoon, DPDSwoon);
};  // namespace NrvTeresaWater

namespace {
    const TeresaWaterMoveParam sMoveParam = {0.5f, 0.0f, 0.899f, 1.5f};
};

TeresaWater::TeresaWater(const char* pName) : LiveActor(pName), _8C(), _90() {
    _94.set(0.0f, 0.0f, 1.0f);
    _A0.set(0.0f, 0.0f, 0.0f);
    _AC.set(0.0f, 0.0f, 0.0f);
    _B8 = nullptr;
}

void TeresaWater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TeresaWater", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f sensorOffset;
    sensorOffset.x = 0.0f;
    sensorOffset.y = 0.0f;
    sensorOffset.z = 0.0f;
    MR::addHitSensorAtJointEnemySimple(this, "body", "SklRoot", 8, 60.0f, sensorOffset);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    TVec3f ptrPos;
    ptrPos.set(0.0f, 0.0f, 0.0f);
    MR::initStarPointerTargetAtJoint(this, "SklRoot", 100.0f, ptrPos);

    _8C = new AnimScaleController(nullptr);
    _90 = new WalkerStateBindStarPointer(this, _8C);
    MR::declareCoin(this, 1);
    MR::calcFrontVec(&_94, this);
    f32 arg0 = 800.0f;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    _AC.set(mPosition);
    _A0.scaleAdd(arg0, _94, _AC);
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

    MR::moveAndTurnToTarget(this, &_94, *_B8, sMoveParam._0, sMoveParam._4, sMoveParam._8, sMoveParam._C);
    MR::attenuateVelocityExceptDirection(this, _94, 0.5f);

    if (MR::isNear(this, *_B8, 30.0f)) {
        if (_B8 == &_A0) {
            _B8 = &_AC;
        } else {
            _B8 = &_A0;
        }
    }
}

void TeresaWater::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
        mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
        MR::startSound(this, "SE_EV_TERESA_ATTACK_SUCCESS");
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
    MR::startSound(this, "SE_EM_TERESA_DEAD");
    MR::startSound(this, "SE_EV_TERESA_DEAD");
    LiveActor::kill();
}

void TeresaWater::control() {
    _8C->updateNerve();

    if (isNerve(&NrvTeresaWater::TeresaWaterNrvMove::sInstance)) {
        MR::startLevelSound(this, "SE_EM_LV_TERESA_MOVE");
    }

    if (tryDPDSwoon()) {
        return;
    }
}

void TeresaWater::calcAndSetBaseMtx() {
    TVec3f up;
    MR::calcUpVec(&up, this);
    up.negate();
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, up, _94);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale = _8C->_C * mScale;
    MR::setBaseScale(this, scale);
}

void TeresaWater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvTeresaWater::TeresaWaterNrvHit::sInstance) && MR::isSensorPlayerOrRide(pReceiver)) {
        if (!isNerve(&NrvTeresaWater::TeresaWaterNrvDPDSwoon::sInstance) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            setNerve(&NrvTeresaWater::TeresaWaterNrvHit::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool TeresaWater::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvTeresaWater::TeresaWaterNrvStun::sInstance);
        return true;
    } else if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    } else if (MR::isMsgSearchlightAttack(msg)) {
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

    if (!_90->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvTeresaWater::TeresaWaterNrvDPDSwoon::sInstance);
    return true;
}

void TeresaWater::endDPDSwoon() {
    _90->kill();
}

void TeresaWater::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, _90, &NrvTeresaWater::TeresaWaterNrvMove::sInstance);
}

TeresaWater::~TeresaWater() {
}
