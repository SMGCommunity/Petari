#include "Game/Enemy/PackunPetit.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvPackunPetit {
    NEW_NERVE_ONEND(PackunPetitNrvNonActive, PackunPetit, NonActive, NonActive);
    NEW_NERVE(PackunPetitNrvWait, PackunPetit, Wait);
    NEW_NERVE(PackunPetitNrvFind, PackunPetit, Find);
    NEW_NERVE(PackunPetitNrvThreat, PackunPetit, Threat);
    NEW_NERVE(PackunPetitNrvLeftTurn, PackunPetit, Turn);
    NEW_NERVE(PackunPetitNrvRightTurn, PackunPetit, Turn);
    NEW_NERVE(PackunPetitNrvAttackStart, PackunPetit, AttackStart);
    NEW_NERVE(PackunPetitNrvAttack, PackunPetit, Attack);
    NEW_NERVE(PackunPetitNrvAttackBack, PackunPetit, AttackBack);
    NEW_NERVE(PackunPetitNrvHitWaitForAttack, PackunPetit, HitWaitForAttack);
    NEW_NERVE(PackunPetitNrvHit, PackunPetit, Hit);
    NEW_NERVE(PackunPetitNrvTrampleDown, PackunPetit, TrampleDown);
    NEW_NERVE(PackunPetitNrvPunchDown, PackunPetit, PunchDown);
    NEW_NERVE(PackunPetitNrvSwoonStart, PackunPetit, SwoonStart);
    NEW_NERVE(PackunPetitNrvSwoon, PackunPetit, Swoon);
    NEW_NERVE(PackunPetitNrvSwoonToThreat, PackunPetit, SwoonToThreat);
    NEW_NERVE_ONEND(PackunPetitNrvDPDSwoon, PackunPetit, DPDSwoon, DPDSwoon);
};  // namespace NrvPackunPetit

PackunPetit::PackunPetit(const char* pName)
    : LiveActor(pName), mScaleController(nullptr), mStarPointerState(nullptr), _94(0.0f, 0.0f, 1.0f), mBlownModel(nullptr), mDontTurn(false) {
}

void PackunPetit::init(const JMapInfoIter& rIter) {
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
    initNerve(GET_NERVE(PackunPetit, PackunPetitNrvWait));
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
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvWait));
    }
}

void PackunPetit::endNonActive() {
    MR::validateHitSensors(this);
    MR::onCalcAnim(this);
    MR::onCalcShadow(this, "JawA");
}

void PackunPetit::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckWithInterpole(this, "Wait", 60);
    }

    if (MR::isNearPlayer(this, 1700.0f)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvFind));
    } else {
        tryNonActive();
    }
}

void PackunPetit::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Suspect");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvThreat));
    }
}

void PackunPetit::exeThreat() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Threat");
    }

    MR::startLevelSound(this, "SE_EM_LV_PACKUNPETIT_THREAT");

    if (!mDontTurn) {
        if (tryTurn()) {
            return;
        }

        TVec3f* pos = MR::getPlayerPos();
        MR::turnDirectionToTargetUseGroundNormalDegree(this, &_94, *pos, 2.0f);
    }

    if (MR::isGreaterStep(this, 50) && MR::isNearPlayer(this, 1100.0f) && (MR::isFaceToPlayerHorizontalDegree(this, _94, 10.0f) || mDontTurn)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvAttackStart));
    } else if (!MR::isNearPlayer(this, 2000.0f)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvWait));
    }
}

void PackunPetit::exeTurn() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(PackunPetit, PackunPetitNrvLeftTurn))) {
            MR::startBck(this, "TurnLeft");
        } else {
            MR::startBck(this, "TurnRight");
        }
    }

    f32 step = (10.0f - getNerveStep());

    if (step < 2.0f) {
        step = 2.0f;
    }
    TVec3f* pos = MR::getPlayerPos();
    MR::turnDirectionToTargetUseGroundNormalDegree(this, &_94, *pos, step);
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvThreat));
    }
}

void PackunPetit::exeAttackStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack");
    }

    if (MR::isStep(this, 70)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvAttack));
    }
}

void PackunPetit::exeAttack() {
    if (MR::isStep(this, 12)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvAttackBack));
    }
}

void PackunPetit::exeAttackBack() {
    if (MR::isBckStopped(this)) {
        if (!tryTurn()) {
            selectNrvWait();
        }
    }
}

void PackunPetit::exeHitWaitForAttack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Attack")) {
        MR::startBck(this, "Impact");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvHit));
    }
}

void PackunPetit::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit");
    }

    if (MR::isBckStopped(this) && !tryTurn()) {
        selectNrvWait();
    }
}

void PackunPetit::exeTrampleDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Press");
        MR::startSound(this, "SE_EM_STOMPED_S");
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
        MR::startBck(mBlownModel, "BlowHead");
        MR::startBck(this, "Blow");
        MR::startBlowHitSound(this);
    }

    mBlownModel->mVelocity.scaleAdd(2.5f, mGravity, mBlownModel->mVelocity);

    if (!MR::isHiddenModel(mBlownModel) && (MR::isStep(this, 20) || MR::checkStrikeBallToMap(mBlownModel->mPosition, 50.0f))) {
        MR::emitEffect(mBlownModel, "Death");
        MR::appearStarPiece(this, mBlownModel->mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        MR::hideModel(mBlownModel);
    }

    if (MR::isStep(this, 30)) {
        kill();
    }
}

void PackunPetit::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonStart");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoon));
    }
}

void PackunPetit::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon");
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S");

    if (MR::isStep(this, 250)) {
        selectNrvWait();
    }
}

void PackunPetit::exeSwoonToThreat() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonToThreat");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvThreat));
    }
}

void PackunPetit::exeDPDSwoon() {
    if (MR::updateActorState(this, mStarPointerState)) {
        selectNrvWait();
    }
}

void PackunPetit::endDPDSwoon() {
    mStarPointerState->kill();
}

void PackunPetit::kill() {
    if (!MR::isDead(mBlownModel)) {
        if (!MR::isHiddenModel(mBlownModel)) {
            MR::emitEffect(mBlownModel, "Death");
            MR::appearStarPiece(this, mBlownModel->mPosition, 3, 10.0f, 40.0f, false);
            MR::startSound(mBlownModel, "SE_OJ_STAR_PIECE_BURST");
        }

        mBlownModel->kill();
    } else {
        if (!MR::isValidSwitchDead(this)) {
            MR::appearCoinPop(this, mPosition, 1);
        }
    }

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
}

void PackunPetit::calcAndSetBaseMtx() {
    TVec3f up;
    MR::calcUpVec(&up, this);
    TPos3f mtx;
    mtx.identity();
    MR::makeMtxUpFrontPos(&mtx, up, _94, mPosition);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void PackunPetit::control() {
    mScaleController->update();
    tryDPDSwoon();
}

void PackunPetit::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        bool isTrampleOrPunch =
            isNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));
        if (!isTrampleOrPunch && !isNerve(GET_NERVE(PackunPetit, PackunPetitNrvDPDSwoon)) && MR::isSensorEnemyAttack(pSender)) {
            if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender) && !MR::isPlayerHipDropFalling()) {
                MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
                setNerve(GET_NERVE(PackunPetit, PackunPetitNrvHitWaitForAttack));
                return;
            }
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
    }

    if (isNerve(GET_NERVE(PackunPetit, PackunPetitNrvDPDSwoon)) && MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (!MR::isSensorEnemy(pReceiver)) {
        return;
    }

    MR::sendMsgPush(pReceiver, pSender);
}

bool PackunPetit::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isTrampleOrPunch = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));

    if (isTrampleOrPunch) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoonStart));
        return true;
    }

    bool isWaitOrHit = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvHitWaitForAttack)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvHit));

    if (isWaitOrHit) {
        return false;
    }

    if (MR::isMsgInvincibleAttack(msg) || MR::isMsgFireBallAttack(msg)) {
        punchDown(pSender, pReceiver);
        return true;
    }

    if (isNerve(GET_NERVE(PackunPetit, PackunPetitNrvAttack))) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::tryRumbleDefaultHit(this, 0);
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown));
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumbleDefaultHit(this, 0);
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown));
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        MR::stopSceneForDefaultHit(5);
        punchDown(pSender, pReceiver);
        return true;
    }

    return false;
}

bool PackunPetit::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isTrampleOrPunch = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));

    if (isTrampleOrPunch) {
        return false;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        punchDown(pSender, pReceiver);
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));
        return true;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown));
        return true;
    }

    return false;
}

bool PackunPetit::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isSwoon = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoonStart)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoon));

    if (!isSwoon) {
        return false;
    }

    if (!MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (MR::isMsgPlayerKick(msg)) {
        MR::stopSceneForDefaultHit(5);
        punchDown(pSender, pReceiver);
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

void PackunPetit::punchDown(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f v6 = pReceiver->mPosition - pSender->mPosition;
    v6.orthogonalize(mGravity);
    MR::normalize(&v6);

    TVec3f v5;
    v5.scale(20.0f, v6);
    v5.scaleAdd(-40.0f, mGravity, v5);
    mBlownModel->mVelocity.set(v5);
    setNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));
}

void PackunPetit::selectNrvWait() {
    if (!MR::isNearPlayer(this, 1700.0f)) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvWait));
    } else if (isNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoon))) {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoonToThreat));
    } else {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvThreat));
    }
}

bool PackunPetit::tryNonActive() {
    if (MR::isNearPlayerAnyTime(this, 5000.0f)) {
        return false;
    }

    setNerve(GET_NERVE(PackunPetit, PackunPetitNrvNonActive));
    return true;
}

/*
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
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvLeftTurn));
    }
    else {
        setNerve(GET_NERVE(PackunPetit, PackunPetitNrvRightTurn));
    }

    return true;
}
*/

bool PackunPetit::tryDPDSwoon() {
    if (isNerve(GET_NERVE(PackunPetit, PackunPetitNrvDPDSwoon))) {
        return false;
    }

    bool isSwoon = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoonStart)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvSwoon));

    if (isSwoon) {
        return false;
    }

    bool isTrampleOrPunch = isNerve(GET_NERVE(PackunPetit, PackunPetitNrvTrampleDown)) || isNerve(GET_NERVE(PackunPetit, PackunPetitNrvPunchDown));

    if (isTrampleOrPunch) {
        return false;
    }

    if (!mStarPointerState->tryStartPointBind()) {
        return false;
    }

    setNerve(GET_NERVE(PackunPetit, PackunPetitNrvDPDSwoon));
    return true;
}

PackunPetit::~PackunPetit() {
}
