#include "Game/Enemy/Jellyfish.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvJellyfish {
    NEW_NERVE(JellyfishNrvWait, Jellyfish, Wait);
    NEW_NERVE(JellyfishNrvWaitWithRightTurn, Jellyfish, WaitWithRightTurn);
    NEW_NERVE(JellyfishNrvWaitWithLeftTurn, Jellyfish, WaitWithLeftTurn);
    NEW_NERVE(JellyfishNrvFind, Jellyfish, Find);
    NEW_NERVE(JellyfishNrvThreat, Jellyfish, Threat);
    NEW_NERVE(JellyfishNrvThreatWithRightTurn, Jellyfish, ThreatWithRightTurn);
    NEW_NERVE(JellyfishNrvThreatWithLeftTurn, Jellyfish, ThreatWithLeftTurn);
    NEW_NERVE(JellyfishNrvDeath, Jellyfish, Death);
    NEW_NERVE(JellyfishNrvAttack, Jellyfish, Attack);
    NEW_NERVE(JellyfishNrvRailGoal, Jellyfish, RailGoal);
    NEW_NERVE(JellyfishNrvDPDSwoon, Jellyfish, DPDSwoon);
};  // namespace NrvJellyfish

namespace {
    static Color8 sPointLightColor(255, 50, 202, 255);
};  // namespace

Jellyfish::Jellyfish(const char* pName)
    : LiveActor(pName), mController(), _94(), _98(0.0f, 0.0f, 1.0f), mIsConnectedRail(), _A8(5.0f), _AC(60), _B0(), _B4(0.0f, 0.0f, 0.0f) {
}

void Jellyfish::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    initModelManagerWithAnm("Jellyfish", nullptr, false);

    MR::connectToSceneEnemy(this);

    MR::addToAttributeGroupSearchTurtle(this);

    initHitSensor(1);
    TVec3f offset;
    offset.set(0.0f, 30.0f, 0.0f);
    MR::addHitSensor(this, "body", ATYPE_KILLER_TARGET_ENEMY, 8, 100.0f, offset);

    initBinder(130.0f, 0.0f, 0);
    MR::offBind(this);

    mController = new AnimScaleController(nullptr);

    initEffectKeeper(2, nullptr, false);

    MR::addEffectHitNormal(this, nullptr);

    initSound(4, false);

    MR::initShadowVolumeSphere(this, 100.0f);
    MR::setShadowDropLength(this, nullptr, 1900.0f);

    MR::declareCoin(this, 1);

    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

    mController = new AnimScaleController(nullptr);
    mBindStarPtr = new WalkerStateBindStarPointer(this, mController);

    if (MR::isConnectedWithRail(rIter)) {
        mIsConnectedRail = true;

        initRailRider(rIter);
        MR::initAndSetRailClipping(&_B4, this, 100.0f, 500.0f);
        MR::moveCoordToNearestPos(this, mPosition);

        MR::getJMapInfoArg0NoInit(rIter, &_A8);
        MR::getJMapInfoArg1NoInit(rIter, &_AC);

        s32 arg2 = 0;
        MR::getJMapInfoArg2NoInit(rIter, &arg2);
        _B0 = arg2 == 1;
    }

    MR::calcFrontVec(&_98, this);

    initNerve(GET_NERVE(Jellyfish, JellyfishNrvWait));

    makeActorAppeared();
}

void Jellyfish::kill() {
    MR::emitEffect(this, "Death");

    MR::appearCoinPop(this, mPosition, 1);

    LiveActor::kill();
}

void Jellyfish::control() {
    MR::requestPointLight(this, TVec3f(mPosition), ::sPointLightColor, 0.0998f, -1);

    mController->updateNerve();

    if (!isNerve(GET_NERVE(Jellyfish, JellyfishNrvDeath))) {
        if (mIsConnectedRail) {
            MR::moveCoordAndFollowTrans(this, _A8);
            if (MR::isRailReachedGoal(this) && !isNerve(GET_NERVE(Jellyfish, JellyfishNrvRailGoal))) {
                setNerve(GET_NERVE(Jellyfish, JellyfishNrvRailGoal));
                return;
            }
        }

        mVelocity.scale(MR::sin(_94 + 46), mGravity);

        _94++;
    }

    if (tryDPDSwoon()) {
    }
}

void Jellyfish::calcAndSetBaseMtx() {
    TPos3f baseMtx;
    MR::makeMtxFrontUpPos(&baseMtx, _98, -mGravity, mPosition);
    MR::setBaseTRMtx(this, baseMtx);
    TVec3f baseScale(mController->_C * mScale);
    MR::setBaseScale(this, baseScale);
}

void Jellyfish::exeWait() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Wait")) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Wait");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELLYFISH_WAIT");
    selectNerveAfterWait();
}

void Jellyfish::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SearchOn", nullptr);
        MR::startBrk(this, "SearchOn");

        MR::startSound(this, "SE_EM_JELLYFISH_FIND");
    }

    faceToMario();

    if (MR::isBckStopped(this)) {
        selectNerveThreat();
    }
}

void Jellyfish::exeThreat() {
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "SearchWait", nullptr);
        MR::startBrk(this, "SearchWait");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELLYFISH_WAIT");

    faceToMario();

    if (selectNerveThreat()) {
        return;
    }
}

void Jellyfish::exeDeath() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);

        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);

        MR::stopScene(5);

        MR::startBck(this, "Death", nullptr);
        MR::startBrk(this, "Death");

        MR::startBlowHitSound(this);

        MR::startSound(this, "SE_EM_JELLYFISH_HIT_PUNCH");
    }

    if (MR::isStep(this, 30) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_JELLYFISH_DEAD");
        kill();
    }
}

void Jellyfish::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startBrk(this, "Attack");

        MR::startSound(this, "SE_EM_JELLYFISH_ATTACK");
    }

    if (MR::isBckStopped(this)) {
        if (!MR::isNearPlayer(this, 1500.0f)) {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvWait));
        } else {
            selectNerveThreat();
        }
    }
}

void Jellyfish::exeRailGoal() {
    if (MR::isStep(this, _AC)) {
        if (_B0) {
            MR::moveCoordAndTransToRailPoint(this, 0);
            _98.set(MR::getRailDirection(this));
        } else {
            MR::reverseRailDirection(this);
        }

        setNerve(GET_NERVE(Jellyfish, JellyfishNrvWait));
    }
}

void Jellyfish::exeThreatWithLeftTurn() {
    threatTurn();
}

void Jellyfish::exeThreatWithRightTurn() {
    threatTurn();
}

void Jellyfish::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mBindStarPtr, GET_NERVE(Jellyfish, JellyfishNrvWait));
}

void Jellyfish::exeWaitWithLeftTurn() {
    MR::rotateVecDegree(&_98, mGravity, -0.2f);
    selectNerveAfterWait();
}

void Jellyfish::exeWaitWithRightTurn() {
    MR::rotateVecDegree(&_98, mGravity, 0.2f);
    selectNerveAfterWait();
}

void Jellyfish::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvDeath))) {
        return;
    }

    if (MR::isSensorEnemy(pSender) && MR::isSensorPlayer(pReceiver)) {
        if (!isNerve(GET_NERVE(Jellyfish, JellyfishNrvDPDSwoon)) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvAttack));
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else if ((MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver)) && MR::isSensorEnemy(pSender)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Jellyfish::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        mController->startHitReaction();
        return true;
    } else if (MR::isMsgPlayerHitAll(msg)) {
        knockOut(pSender, pReceiver);
        return true;
    }

    return false;
}

bool Jellyfish::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && !isNerve(GET_NERVE(Jellyfish, JellyfishNrvDeath))) {
        knockOut(pSender, pReceiver);
        return true;
    }

    return false;
}

void Jellyfish::threatTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SearchRotate", nullptr);
        MR::startBrk(this, "SearchRotate");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELLYFISH_WAIT");

    faceToMario();

    if (MR::isBckStopped(this)) {
        selectNerveThreat();
    }
}

bool Jellyfish::faceToMario() {
    TVec3f v13;
    v13.sub(*MR::getPlayerPos(), mPosition);
    MR::normalizeOrZero(&v13);
    TVec3f v12;
    MR::calcSideVec(&v12, this);

    if (!MR::isNearZero(v13)) {
        MR::turnVecToVecCosOnPlane(&_98, v13, v12, MR::cos(0.5f));
        TVec3f v10;
        v10.negate(mGravity);
        TVec3f v11;
        MR::turnVecToPlane(&v11, _98, v10);
        MR::clampVecAngleDeg(&_98, v11, 30.0f);
    }

    if (!isNerve(GET_NERVE(Jellyfish, JellyfishNrvThreat))) {
        f32 frameMax = MR::getBckFrameMax(this, "SearchRotate");
        f32 v7 = (1.0f - (frameMax / getNerveStep()));
        f32 v8;
        if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvThreatWithLeftTurn))) {
            v8 = 1.0f;
        } else {
            v8 = -1.0f;
        }

        TVec3f v9;
        v9.negate(mGravity);
        MR::rotateVecDegree(&_98, v9, (v8 * (1.5f * v7)));
    }

    return true;
}

void Jellyfish::knockOut(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f toReceiverDir;
    MR::normalize(pReceiver->mPosition - pSender->mPosition, &toReceiverDir);

    mVelocity.scale(50.0f, toReceiverDir);

    _98.negate(toReceiverDir);

    setNerve(GET_NERVE(Jellyfish, JellyfishNrvDeath));
}

bool Jellyfish::selectNerveAfterWait() {
    if (MR::isNearPlayer(this, 1000.0f)) {
        setNerve(GET_NERVE(Jellyfish, JellyfishNrvFind));
        return true;
    }

    if (mIsConnectedRail && _B0) {
        return false;
    }

    if (MR::isStep(this, 280)) {
        s32 randomVal;
        if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvWait))) {
            randomVal = MR::getRandom(0l, 3l);
        } else {
            randomVal = 0;
        }

        if (randomVal == 0) {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvWait));
        } else if (randomVal == 1) {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvWaitWithRightTurn));
        } else {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvWaitWithLeftTurn));
        }

        return true;
    }

    return false;
}

bool Jellyfish::selectNerveThreat() {
    if (!MR::isNearPlayer(this, 1500.0f)) {
        setNerve(GET_NERVE(Jellyfish, JellyfishNrvWait));
        return true;
    }

    TVec3f vec2C;
    vec2C.sub(*MR::getPlayerPos(), mPosition);
    MR::normalizeOrZero(&vec2C);

    TVec3f vec38;
    MR::turnVecToPlane(&vec38, vec2C, mGravity);

    TVec3f vec44;
    MR::turnVecToPlane(&vec44, _98, mGravity);

    if (vec44.angle(vec38) * PI_180 < 35.0f) {
        if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvThreat))) {
            return false;
        }

        setNerve(GET_NERVE(Jellyfish, JellyfishNrvThreat));
    } else {
        TVec3f vec50;
        vec50.sub(*MR::getPlayerPos(), mPosition);

        TVec3f sideVec;
        MR::calcSideVec(&sideVec, this);

        if (vec50.dot(sideVec) > 0.0f) {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvThreatWithLeftTurn));
        } else {
            setNerve(GET_NERVE(Jellyfish, JellyfishNrvThreatWithRightTurn));
        }
    }

    return true;
}

bool Jellyfish::tryDPDSwoon() {
    if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvDPDSwoon))) {
        return false;
    }

    if (isNerve(GET_NERVE(Jellyfish, JellyfishNrvDeath))) {
        return false;
    }

    if (!mBindStarPtr->tryStartPointBind()) {
    return false;
    }

        setNerve(GET_NERVE(Jellyfish, JellyfishNrvDPDSwoon));
        return true;
}
