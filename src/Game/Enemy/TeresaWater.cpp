#include "Game/Enemy/TeresaWater.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void TeresaWater_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const MR::ActorMoveParam sMoveParam = {0.5f, 0.0f, 0.9f, 1.5f};
    static const f32 sDefaultMoveLength = 800.0f;
    static const f32 sSideFriction = 0.5f;
    static const f32 sTargetRadius = 30.0f;
};  // namespace

namespace NrvTeresaWater {
    NEW_NERVE(TeresaWaterNrvMove, TeresaWater, Move);
    NEW_NERVE(TeresaWaterNrvHit, TeresaWater, Hit);
    NEW_NERVE(TeresaWaterNrvStun, TeresaWater, Stun);
    NEW_NERVE_ONEND(TeresaWaterNrvDPDSwoon, TeresaWater, DPDSwoon, DPDSwoon);
};  // namespace NrvTeresaWater

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
    f32 arg0 = ::sDefaultMoveLength;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    _AC.set(mPosition);
    _A0.scaleAdd(arg0, _94, _AC);
    _B8 = &_A0;
    MR::initShadowVolumeCylinder(this, 100.0f);
    initNerve(GET_NERVE(TeresaWater, TeresaWaterNrvMove));

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void TeresaWater::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    MR::moveAndTurnToTarget(this, &_94, *_B8, ::sMoveParam._0, ::sMoveParam._4, ::sMoveParam._8, ::sMoveParam._C);
    MR::attenuateVelocityExceptDirection(this, _94, ::sSideFriction);

    if (MR::isNear(this, *_B8, ::sTargetRadius)) {
        if (_B8 == &_A0) {
            _B8 = &_AC;
        } else {
            _B8 = &_A0;
        }
    }
}

void TeresaWater::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit");
        mVelocity.zero();
        MR::startSound(this, "SE_EV_TERESA_ATTACK_SUCCESS");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(TeresaWater, TeresaWaterNrvMove));
    }
}

void TeresaWater::exeStun() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Reaction");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(TeresaWater, TeresaWaterNrvMove));
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

    if (isNerve(GET_NERVE(TeresaWater, TeresaWaterNrvMove))) {
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

    MR::setBaseScale(this, _8C->_C * mScale);
}

void TeresaWater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(TeresaWater, TeresaWaterNrvHit))) {
        return;
    }

    if (!MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    if (!isNerve(GET_NERVE(TeresaWater, TeresaWaterNrvDPDSwoon)) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        setNerve(GET_NERVE(TeresaWater, TeresaWaterNrvHit));
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool TeresaWater::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(GET_NERVE(TeresaWater, TeresaWaterNrvStun));

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
    if (isNerve(GET_NERVE(TeresaWater, TeresaWaterNrvStun))) {
        return false;
    }

    if (isNerve(GET_NERVE(TeresaWater, TeresaWaterNrvDPDSwoon))) {
        return false;
    }

    if (!_90->tryStartPointBind()) {
        return false;
    }

    setNerve(GET_NERVE(TeresaWater, TeresaWaterNrvDPDSwoon));

    return true;
}

void TeresaWater::endDPDSwoon() {
    _90->kill();
}

void TeresaWater::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, _90, GET_NERVE(TeresaWater, TeresaWaterNrvMove));
}
