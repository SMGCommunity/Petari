#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/ActiveActorList.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvKameck {
    NEW_NERVE(KameckNrvOpeningDemo, Kameck, OpeningDemo);
    NEW_NERVE_ONEND(KameckNrvDemoAppear, Kameck, DemoAppear, DemoAppear);
    NEW_NERVE(KameckNrvNonActive, Kameck, NonActive);
    NEW_NERVE(KameckNrvAppear, Kameck, Appear);
    NEW_NERVE(KameckNrvWait, Kameck, Wait);
    NEW_NERVE(KameckNrvHide, Kameck, Hide);
    NEW_NERVE(KameckNrvAttackWait, Kameck, AttackWait);
    NEW_NERVE(KameckNrvAttack, Kameck, Attack);
    NEW_NERVE(KameckNrvGuard, Kameck, Guard);
    NEW_NERVE(KameckNrvHit, Kameck, Hit);
    NEW_NERVE(KameckNrvMoveHide, Kameck, MoveHide);
    NEW_NERVE(KameckNrvMove, Kameck, Move);
    NEW_NERVE_ONEND(KameckNrvBindStarPointer, Kameck, BindStarPointer, BindStarPointer);
    NEW_NERVE(KameckNrvDown, Kameck, Down);
    NEW_NERVE(KameckNrvPressDown, Kameck, PressDown);
};  // namespace NrvKameck

Kameck::Kameck(const char* pName)
    : LiveActor(pName), mBeam(), _90(), _94(), _98(), _9C(), _A0(0.0f, 0.0f, 0.0f, 1.0f), _B0(0.0f, 0.0f, 1.0f), mBeamType(), mMoveStep(240),
      mRailCoord(), mRailNextPointCoord(), mActiveDistance(3000.0f) {
    _90 = new ActiveActorList(8);
    _94 = new SmallKameckBeamEventListener(this);
}

void Kameck::init(const JMapInfoIter& rIter) {
    initJMapParam(rIter);
    initModelManagerWithAnm("Kameck", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 70.0f);
    MR::makeQuatAndFrontFromRotate(&_A0, &_B0, this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 120.0f, TVec3f(0.0f, 40.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(70.0f, 0.0f, 0);
    MR::offBind(this);
    initBeam();
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTarget(this, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    _98 = new AnimScaleController(nullptr);
    _98->setParamTight();
    _9C = new WalkerStateBindStarPointer(this, _98);
    initSound(4, false);
    MR::addToAttributeGroupSearchTurtle(this);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToStartPos(this);
        MR::moveTransToCurrentRailPos(this);
    }
    initNerve(&NrvKameck::KameckNrvWait::sInstance);
    makeActorAppeared();
    MR::declareCoin(this, 1);
    initDemo(rIter);
}

void Kameck::initBeam() {
    MR::createKameckBeamHolder();
    switch (mBeamType) {
    case BeamType_None:
        break;
    case BeamType_Turtle:
        MR::createKameckBeamTurtleHolder();
        break;
    case BeamType_Fire:
    case BeamType_3:
    case BeamType_4:
        MR::createKameckFireBallHolder();
        break;
    }
}

void Kameck::initJMapParam(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::getJMapInfoArg3NoInit(rIter, &mActiveDistance);
        if (mBeamType == BeamType_Fire) {
            s32 objCastCount;
            MR::getJMapInfoArg0WithInit(rIter, &objCastCount);
            if (objCastCount >= 3) {
                mBeamType = BeamType_4;
            } else if (objCastCount >= 2) {
                mBeamType = BeamType_3;
            } else {
                mBeamType = BeamType_Fire;
            }
        }
    }
}

void Kameck::initDemo(const JMapInfoIter& rIter) {
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionFunctor(this, MR::Functor(this, &Kameck::startDemoAppear), "ザコカメック登場");
        MR::registerDemoActionFunctor(this, MR::Functor(this, &Kameck::killForce), "ザコカメ");
        makeActorDead();
    }
}

void Kameck::makeActorDead() {
    LiveActor::makeActorDead();
    resetBeam();
}

void Kameck::appear() {
    MR::moveCoordAndTransToRailStartPoint(this);
    LiveActor::appear();
    setNerve(&NrvKameck::KameckNrvAppear::sInstance);
}

void Kameck::kill() {
    LiveActor::kill();
    MR::appearCoinPop(this, mPosition, 1);
    MR::emitEffect(this, "Death");
}

void Kameck::control() {
    _98->updateNerve();
    MR::blendQuatUpFront(&_A0, -mGravity, _B0, 0.04f, 0.2f);
    _90->removeDeadActor();
}

void Kameck::startClipped() {
    LiveActor::startClipped();
    tryNonActive();
}

void Kameck::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A0);
    MR::setBaseScale(this, _98->_C * mScale);
}

void Kameck::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && !MR::isPlayerHipDropFalling() && MR::isSensorEnemyAttack(pSender)) {
        requestAttack(pSender, pReceiver);
    }
}

bool Kameck::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceReflect(msg)) {
        return requestGuard(pSender, pReceiver);
    }
    if (MR::isMsgJetTurtleAttack(msg)) {
        return requestDown(pSender, pReceiver);
    }
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestDown(pSender, pReceiver);
    }
    if (MR::isMsgPlayerTrample(msg)) {
        return requestPressDown();
    }
    if (MR::isMsgPlayerHipDrop(msg)) {
        return requestPressDown();
    }
    return false;
}

bool Kameck::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        return requestDown(pSender, pReceiver);
    }
    return false;
}

bool Kameck::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();
        return true;
    }
    return false;
}

bool Kameck::requestAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableAttack()) {
        if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            if (isEnableAttack()) {
                resetBeam();
                setNerve(&NrvKameck::KameckNrvHit::sInstance);
                return true;
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else {
        if (isNerve(&NrvKameck::KameckNrvBindStarPointer::sInstance)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
    return false;
}

bool Kameck::requestDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableDown()) {
        resetBeam();
        MR::onBind(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::setVelocityBlowAttack(this, pSender, pReceiver, 25.0f, 25.0f, 4);
        setNerve(&NrvKameck::KameckNrvDown::sInstance);
        return true;
    }
    return false;
}

bool Kameck::requestGuard(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableGurad()) {
        // Inline, probably
        TVec3f vec2;
        TVec3f vec = -pSender->mHost->mVelocity;
        TVec3f* pGravity = &mGravity;
        vec2.scaleAdd(-pGravity->dot(vec), *pGravity, vec);
        if (!MR::normalizeOrZero(&vec2)) {
            _B0.set(vec2);
        }
        resetBeam();
        MR::invalidateClipping(this);
        setNerve(&NrvKameck::KameckNrvGuard::sInstance);
        return true;
    }
    return false;
}

bool Kameck::requestPressDown() {
    if (isEnableDown()) {
        resetBeam();
        MR::onBind(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        setNerve(&NrvKameck::KameckNrvPressDown::sInstance);
        return true;
    }
    return false;
}

void Kameck::startDemoAppear() {
    makeActorAppeared();
    MR::invalidateHitSensors(this);
    MR::invalidateClipping(this);
    setNerve(&NrvKameck::KameckNrvDemoAppear::sInstance);
}

void Kameck::killForce() {
    MR::emitEffect(this, "Death");
    resetBeam();
    _90->killAll();
    makeActorDead();
}

void Kameck::makeActorDeadForce() {
    resetBeam();
    _90->killAll();
    makeActorDead();
}

void Kameck::hitBeam(s32 num) {
    if (MR::isDead(this)) {
        return;
    }
    switch (num) {
    case 1:
        MR::startSound(this, "SE_EV_KAMECK_ATK_SUCCESS");
        break;
    case 2:
    case 3:
    case 4:
        MR::startSound(this, "SE_EV_KAMECK_ATK_SUCCESS");
        break;
    }
}

bool Kameck::tryOpeningDemo() {
    if (MR::isStageStateScenarioOpeningCamera()) {
        setNerve(&NrvKameck::KameckNrvOpeningDemo::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryOpeningDemoEnd() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        setNonActive();
        return true;
    }
    return false;
}

bool Kameck::tryNonActive() {
    if (isNerve(&NrvKameck::KameckNrvWait::sInstance) && canNonActive()) {
        setNonActive();
        return true;
    }
    return false;
}

void Kameck::setNonActive() {
    MR::hideModel(this);
    MR::invalidateHitSensors(this);
    MR::offCalcGravity(this);
    MR::offCalcShadow(this, nullptr);
    MR::validateClipping(this);
    MR::deleteEffectAll(this);
    setNerve(&NrvKameck::KameckNrvNonActive::sInstance);
}

bool Kameck::tryActive() {
    if (!canNonActive()) {
        setNerve(&NrvKameck::KameckNrvAppear::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryAppearEnd() {
    if (MR::isBckStopped(this)) {
        if (!tryAttackWait()) {
            setNerve(&NrvKameck::KameckNrvWait::sInstance);
        }
        return true;
    }
    return false;
}

bool Kameck::tryAttackWait() {
    if (MR::isNearPlayer(this, 2000.0f) && !_90->isFull()) {
        mBeam = MR::startFollowKameckBeam(mBeamType, MR::getJointMtx(this, "Wand"), 0.6f, TVec3f(0.0f, 110.0f, 0.0f), _94);
        if (mBeam != nullptr) {
            setNerve(&NrvKameck::KameckNrvAttackWait::sInstance);
            return true;
        }
    }
    return false;
}

bool Kameck::tryAttack() {
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvKameck::KameckNrvAttack::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryAttackEnd() {
    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvKameck::KameckNrvMoveHide::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryHitEnd() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvKameck::KameckNrvMoveHide::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryMove() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvKameck::KameckNrvMove::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryMoveEnd() {
    if (MR::isGreaterStep(this, mMoveStep)) {
        setNerve(&NrvKameck::KameckNrvAppear::sInstance);
        return true;
    }
    return false;
}

bool Kameck::tryPointBind() {
    if (_9C->tryStartPointBind()) {
        resetBeam();
        setNerve(&NrvKameck::KameckNrvBindStarPointer::sInstance);
        return true;
    }
    return false;
}

void Kameck::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::validateClipping(this);
    }
    if (tryOpeningDemoEnd()) {
        return;
    }
}

void Kameck::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::startSound(this, "SE_EM_KAMECK_SMOKE");
        MR::startSound(this, "SE_EM_KAMECK_APPEAR");
    }
    MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), 0.98f);
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void Kameck::endDemoAppear() {
    if (isNerve(&NrvKameck::KameckNrvDemoAppear::sInstance)) {
        setNerve(&NrvKameck::KameckNrvWait::sInstance);
    }
}

void Kameck::exeNonActive() {
    if (tryOpeningDemo() || tryActive()) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
    }
}

void Kameck::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::startSound(this, "SE_EM_KAMECK_SMOKE");
        MR::startSound(this, "SE_EM_KAMECK_APPEAR");
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::invalidateClipping(this);
    }
    if (MR::isNearPlayer(this, 2000.0f)) {
        MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), 0.98f);
    }
    if (!tryPointBind() && tryAppearEnd()) {
        return;
    }
}

void Kameck::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::validateClipping(this);
        MR::validateHitSensors(this);
    }
    if (!tryPointBind() && !tryOpeningDemo() && tryAttackWait()) {
        return;
    }
}

void Kameck::exeAttackWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "AttackWait", nullptr);
        MR::invalidateClipping(this);
    }
    MR::startLevelSound(this, "SE_EM_LV_KAMECK_STAFF_TURN");
    MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), 0.98f);
    if (!tryPointBind() && tryAttack()) {
        return;
    }
}

void Kameck::exeGuard() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Guard", nullptr);
    }
    if (!tryPointBind() && MR::isGreaterStep(this, 50)) {
        setNerve(&NrvKameck::KameckNrvMoveHide::sInstance);
    }
}

void Kameck::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EM_KAMECK_STAFF_SWING");
        MR::startSound(this, "SE_EV_KAMECK_STAFF_SWING");
    }
    if (MR::isStep(this, 9)) {
        mBeam->requestShootToPlayerCenter(12.0f);
        _90->addActor(mBeam);
        mBeam = 0;
    }
    if (!tryPointBind() && tryAttackEnd()) {
        return;
    }
}

void Kameck::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
    }
    if (!tryPointBind() && tryHitEnd()) {
        return;
    }
}

void Kameck::exeMoveHide() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hide", nullptr);
        MR::startSound(this, "SE_EM_KAMECK_HIDE");
    }
    MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), 0.98f);
    if (!tryPointBind() && !tryMove()) {
        MR::startSound(this, "SE_EM_KAMECK_SMOKE");
    }
}

void Kameck::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Move", nullptr);
        MR::hideModelAndOnCalcAnim(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        if (MR::isExistRail(this)) {
            if (MR::isRailReachedGoal(this)) {
                MR::reverseRailDirection(this);
            }
            mRailCoord = MR::getRailCoord(this);
            mRailNextPointCoord = MR::getRailPointCoord(this, MR::getNextRailPointNo(this));
            mMoveStep = MR::abs(mRailNextPointCoord - mRailCoord) / 20.0f;
        }
    }
    MR::startLevelSound(this, "SE_EM_LV_KAMECK_WARP");
    if (MR::isExistRail(this)) {
        MR::setRailCoord(this, MR::calcNerveEaseInOutValue(this, mMoveStep, mRailCoord, mRailNextPointCoord));
        MR::moveTransToCurrentRailPos(this);
    }
    MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), 0.98f);
    if (tryMoveEnd()) {
        return;
    }
}

void Kameck::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", nullptr);
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EV_KAMECK_DEAD_BLOW");
    }
    MR::addVelocityToGravity(this, 1.0f);
    MR::attenuateVelocity(this, 0.99f);
    if (MR::isBckStopped(this) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        kill();
    }
}

void Kameck::exePressDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Press", nullptr);
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::startSound(this, "SE_EV_KAMECK_DEAD_STOMPED");
        MR::zeroVelocity(this);
    }
    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        kill();
    }
}

void Kameck::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, _9C, &NrvKameck::KameckNrvWait::sInstance);
}

void Kameck::endBindStarPointer() {
    _9C->kill();
}

void Kameck::exeHide() {
}

void Kameck::resetBeam() {
    if (mBeam != nullptr) {
        mBeam->kill();
        mBeam = nullptr;
    }
}

void Kameck::setBeamType(s32 type) {
    mBeamType = type;
}

bool Kameck::canNonActive() const {
    return !MR::isNearPlayerAnyTime(this, mActiveDistance);
}

bool Kameck::isEnableAttack() const {
    if (!(!isNerve(&NrvKameck::KameckNrvWait::sInstance) && !isNerve(&NrvKameck::KameckNrvAppear::sInstance) &&
          !isNerve(&NrvKameck::KameckNrvAttackWait::sInstance) && !isNerve(&NrvKameck::KameckNrvAttack::sInstance) &&
          !isNerve(&NrvKameck::KameckNrvMoveHide::sInstance))) {
        return true;
    }
    return false;
}

bool Kameck::isEnableGurad() const {
    if (!(!isNerve(&NrvKameck::KameckNrvWait::sInstance) && !isNerve(&NrvKameck::KameckNrvAppear::sInstance) &&
          !isNerve(&NrvKameck::KameckNrvAttackWait::sInstance) && !isNerve(&NrvKameck::KameckNrvAttack::sInstance) &&
          !isNerve(&NrvKameck::KameckNrvGuard::sInstance) && !isNerve(&NrvKameck::KameckNrvMoveHide::sInstance))) {
        return true;
    }
    return false;
}

bool Kameck::isEnableDown() const {
    if (MR::isDead(this)) {
        return false;
    }
    if (!(!isNerve(&NrvKameck::KameckNrvNonActive::sInstance) && !isNerve(&NrvKameck::KameckNrvDown::sInstance) &&
          !isNerve(&NrvKameck::KameckNrvPressDown::sInstance))) {
        return false;
    }
    return true;
}

namespace MR {
    NameObj* createFireBallBeamKameck(const char* pName) {
        Kameck* kmck = new Kameck(pName);
        kmck->mBeamType = Kameck::BeamType_Fire;
        return kmck;
    }

    NameObj* createTurtleBeamKameck(const char* pName) {
        Kameck* kmck = new Kameck(pName);
        kmck->mBeamType = Kameck::BeamType_Turtle;
        return kmck;
    }
};  // namespace MR

void SmallKameckBeamEventListener::hitBeam(s32 num) {
    mHost->hitBeam(num);
}
