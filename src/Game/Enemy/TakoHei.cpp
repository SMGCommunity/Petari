#include "Game/Enemy/TakoHei.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/TakoHeiInkHolder.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static TVec3f sInkShotOffset(0.0f, 20.0f, 0.0f);
    static const f32 sBaseScale = 1.0f;
    static const f32 sBodyHitRadius = 70.0f;
    static const f32 sBodyHitYOffset = 70.0f;
    static const f32 sAttackHitRadius = 50.0f;
    static const f32 sAttackHitYOffset = 70.0f;
    static const f32 sCollisionRadius = 90.0f;
    static const f32 sShadowRadius = 60.0f;
    static const f32 sPointerHitSize = 70.0f;
    // static const f32 sFrontVecBlendRate;
    // static const f32 sUpVecBlendRate;
    static const f32 sPushAccel = 5.0f;
    static const f32 sPushScaleYSpeed = 0.1f;
    static const s32 sNoPushTime = 20;
    static const f32 sPushScaleYMarioSpeed = 0.15f;
    static const f32 sPushMarioAccel = 12.0f;
    static const s32 sNoPushMarioTime = 60;
    // static const f32 sFallLimitDistance;
    static const f32 sNormalGroundFrec = 0.85f;
    static const f32 sNormalAirFrec = 0.99f;
    static const f32 sNormalGravityAccel = 1.5f;
    static const f32 sActiveDistance = 3000.0f;
    static const s32 sWaitTime = 120;
    static const s32 sWalkMoveStartTime = 10;
    static const s32 sWalkEndTime = 120;
    static const f32 sWalkAccel = 0.2f;
    static const f32 sWalkGoalDistance = 500.0f;
    static const f32 sWalkTurnDegree = 6.0f;
    static const f32 sWalkTerritoryRadius = 400.0f;
    static const s32 sFindTurnTime = 20;
    static const s32 sFindTime = 30;
    // static const f32 sFindDistance;
    static const f32 sFindTurnDegree = 6.0f;
    static const s32 sPursueTime = 300;
    static const f32 sPursueAccel = 0.5f;
    static const f32 sPursueTurnDegree = 6.0f;
    static const s32 sCoolDownTime = 60;
    static const f32 sAttackSignDegree = 8.0f;
    static const f32 sAttackDistanceNear = 650.0f;
    static const f32 sAttackDistanceFar = 750.0f;
    static const s32 sForceFarAttackTime = 250;
    static const f32 sSpurtPowerH = 20.0f;
    static const f32 sSpurtPowerV = -15.0f;
    static const f32 sCheckFrontHeight = 40.0f;
    static const f32 sCheckFrontAttackSpace = 140.0f;
    static const f32 sBackAttackSpaceAccel = -0.1f;
    static const s32 sAttackSuccessTime = 60;
    static const f32 sAttackSuccessHitPower = 8.0f;
    static const s32 sSwoonTime = 180;
    static const s32 sRecoverTime = 20;
    static const f32 sSwoonGroundFrec = 0.85f;
    static const f32 sSwoonGravityAccel = 1.5f;
    static const s32 sPunchStopSceneTime = 2;
    static const s32 sPunchDownTime = 22;
    static const f32 sPunchHorizonPower = 20.0f;
    static const f32 sPunchVerticalPower = 35.0f;
    static const s32 sFlatDownTime = 20;
    static const s32 sHipDropDownTime = 40;
    static const s32 sPressDownTime = 180;
}  // namespace

namespace NrvTakoHei {
    NEW_NERVE(TakoHeiNrvNonActive, TakoHei, NonActive);
    NEW_NERVE(TakoHeiNrvWait, TakoHei, Wait);
    NEW_NERVE(TakoHeiNrvWalk, TakoHei, Walk);
    NEW_NERVE(TakoHeiNrvFindTurn, TakoHei, FindTurn);
    NEW_NERVE(TakoHeiNrvFind, TakoHei, Find);
    NEW_NERVE(TakoHeiNrvPursue, TakoHei, Pursue);
    NEW_NERVE(TakoHeiNrvCoolDown, TakoHei, CoolDown);
    NEW_NERVE(TakoHeiNrvAttackSign, TakoHei, AttackSign);
    NEW_NERVE(TakoHeiNrvAttack, TakoHei, Attack);
    NEW_NERVE(TakoHeiNrvAttackSuccess, TakoHei, AttackSuccess);
    NEW_NERVE_ONEND(TakoHeiNrvBindStarPointer, TakoHei, BindStarPointer, BindStarPointer);
    NEW_NERVE(TakoHeiNrvSwoon, TakoHei, Swoon);
    NEW_NERVE(TakoHeiNrvRecover, TakoHei, Recover);
    NEW_NERVE(TakoHeiNrvPressDown, TakoHei, PressDown);
    NEW_NERVE(TakoHeiNrvHipDropDown, TakoHei, HipDropDown);
    NEW_NERVE(TakoHeiNrvFlatDown, TakoHei, FlatDown);
    NEW_NERVE(TakoHeiNrvPunchDown, TakoHei, PunchDown);
}  // namespace NrvTakoHei

void TakoHei_FORCE_MATCH_SDATA2() {
    (void) 1.0f;
    (void) 0.0f;
    (void) 0.5f;
    (void) -1.0f;
    (void) 2.0f;
}

TakoHei::TakoHei(const char* pName)
    : LiveActor(pName), _8C(0), _90(0), _94(0.0f, 0.0f, 0.0f, 1.0f), _A4(0, 0, 0), _B0(0, 0, 1), _BC(0, 0, 0), _C8(0, 0, 1), _D4(0, 0, 0), _E0(0),
      _E4(0), _E8(0), _EC(0), _ED(0) {
    MR::createTakoHeiInkHolder();
}

void TakoHei::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mScale.mult(::sBaseScale);
    initModelManagerWithAnm("TakoHei", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    _D4 = mPosition;
    MR::makeQuatAndFrontFromRotate(&_94, &_B0, this);
    MR::addToAttributeGroupSearchTurtle(this);
    initBinder(::sCollisionRadius * mScale.y, ::sCollisionRadius * mScale.y, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initSensor();
    MR::initShadowVolumeSphere(this, ::sShadowRadius);
    initNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
    _8C = new AnimScaleController(nullptr);
    _90 = new WalkerStateBindStarPointer(this, _8C);

    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    makeActorAppeared();
}

void TakoHei::initAfterPlacement() {
    MR::turnQuatUpToGravity(&_94, _94, this);
    _ED = MR::trySetMoveLimitCollision(this);
}

void TakoHei::initSensor() {
    const f32 scaleX = mScale.x;
    initHitSensor(2);
    MR::addHitSensorEnemyAttack(this, "attack", 8, ::sAttackHitRadius * scaleX, TVec3f(0.0f, ::sAttackHitYOffset * scaleX, 0.0f));
    MR::addHitSensorEnemy(this, "body", 8, ::sBodyHitRadius * scaleX, TVec3f(0.0f, ::sBodyHitYOffset * scaleX, 0.0f));
    MR::initStarPointerTarget(this, ::sPointerHitSize * mScale.y, TVec3f(0.0f, ::sPointerHitSize * mScale.y, 0.0f));
}

void TakoHei::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");

    switch (_E0) {
    case 1:
        MR::appearCoinPop(this, mPosition, 1);
        break;
    case 2:
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        break;
    }
}

void TakoHei::control() {
    if (isNerve(&NrvTakoHei::TakoHeiNrvNonActive::sInstance)) {
        return;
    }

    _8C->updateNerve();
    if (_E4 != 0) {
        _E4--;
    }

    updatePose();
    tryPressed();
}

void TakoHei::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _94);
    MR::setBaseScale(this, _8C->_C.mult(mScale));
}

void TakoHei::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender)) {
        if (MR::isSensorPlayer(pReceiver) && canAttack()) {
            if (!MR::isPlayerHipDropFalling() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                requestAttackSuccess(pSender, pReceiver);
            } else {
                MR::sendMsgPush(pReceiver, pSender);
            }
        }
    } else if ((MR::isSensorPlayer(pReceiver) && !canAttack()) || MR::isSensorEnemy(pReceiver)) {
        if (MR::sendMsgPush(pReceiver, pSender) && isPushMovable()) {
            TVec3f sensorDirection;
            MR::calcSensorDirectionNormalize(&sensorDirection, pReceiver, pSender);
            if (mVelocity.dot(sensorDirection) < 0.0f) {
                JMAVECScaleAdd(sensorDirection, mVelocity, mVelocity, -sensorDirection.dot(mVelocity));
            }
        }
    }
}

bool TakoHei::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemyAttack(pReceiver)) {
        if ((MR::isSensorPlayer(pSender) && !canAttack()) || MR::isSensorEnemy(pSender)) {
            if (isPushMovable()) {
                MR::startBtp(this, "BlinkTwice");
                TVec3f sensorDirection;
                MR::calcSensorDirectionNormalize(&sensorDirection, pSender, pReceiver);
                mVelocity += sensorDirection * (MR::isSensorPlayer(pSender) ? ::sPushMarioAccel : ::sPushAccel);
                _8C->startAndAddScaleVelocityY(MR::isSensorPlayer(pSender) ? ::sPushScaleYMarioSpeed : ::sPushScaleYSpeed);
                _E4 = MR::isSensorPlayer(pSender) ? ::sNoPushMarioTime : ::sNoPushTime;
            }
            return true;
        }
    }

    return false;
}

bool TakoHei::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return requestSwoon();
    }

    if (MR::isMsgInvincibleAttack(msg)) {
        return requestPunch(pReceiver, pSender);
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        return requestHipDropDown();
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return requestFlatDown();
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(pReceiver, pSender);
    }

    return false;
}

bool TakoHei::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        return requestFlatDown();
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        return requestPunch(pReceiver, pSender);
    }

    return false;
}

bool TakoHei::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerKick(msg) && isEnableKick() && requestPunch(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool TakoHei::requestPressDown() {
    if (!isDown()) {
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvTakoHei::TakoHeiNrvPressDown::sInstance);
        _E0 = 1;
        return true;
    }

    return false;
}

bool TakoHei::requestHipDropDown() {
    if (!isDown()) {
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvTakoHei::TakoHeiNrvHipDropDown::sInstance);
        _E0 = 1;
        return true;
    }

    return false;
}

bool TakoHei::requestFlatDown() {
    if (!isDown()) {
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvTakoHei::TakoHeiNrvFlatDown::sInstance);
        _E0 = 1;
        return true;
    }

    return false;
}

bool TakoHei::requestSwoon() {
    if (!isDown()) {
        setNerve(&NrvTakoHei::TakoHeiNrvSwoon::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::requestPunch(HitSensor* pSensor1, HitSensor* pSensor2) {
    if (!isDown()) {
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
        MR::addVelocitySeparateHV(this, pSensor2, pSensor1, ::sPunchHorizonPower, ::sPunchVerticalPower);
        setNerve(&NrvTakoHei::TakoHeiNrvPunchDown::sInstance);
        _E0 = 2;
        return true;
    }

    return false;
}

bool TakoHei::requestAttackSuccess(HitSensor* pSensor1, HitSensor* pSensor2) {
    if (canAttack()) {
        MR::addVelocityMoveToDirection(this, pSensor1->mPosition - pSensor2->mPosition, ::sAttackSuccessHitPower);
        setNerve(&NrvTakoHei::TakoHeiNrvAttackSuccess::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryActive() {
    if (MR::isNearPlayerAnyTime(this, ::sActiveDistance)) {
        setNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryNonActive() {
    bool isNotNearPlayer = MR::isNearPlayerAnyTime(this, ::sActiveDistance) ? false : true;
    if (isNotNearPlayer && MR::isBindedGround(this)) {
        MR::zeroVelocity(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvTakoHei::TakoHeiNrvNonActive::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryWalk() {
    if (MR::isGreaterStep(this, ::sWaitTime)) {
        setNerve(&NrvTakoHei::TakoHeiNrvWalk::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryWalkEnd() {
    TVec3f scaleAdd;
    TVec3f diff(_BC - mPosition);
    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(diff);
    JMAVECScaleAdd(grav, diff, scaleAdd, -dot);

    if (MR::isGreaterStep(this, ::sWalkEndTime) || scaleAdd.squared() < ::sWalkTerritoryRadius) {
        setNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
        return true;
    }

    if (isFallNextMove()) {
        MR::zeroVelocity(this);
        setNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryFindTurn() {
    if (isInSightMario()) {
        setNerve(&NrvTakoHei::TakoHeiNrvFindTurn::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryFind() {
    if (MR::isGreaterStep(this, ::sFindTurnTime)) {
        setNerve(&NrvTakoHei::TakoHeiNrvFind::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryPursue() {
    if (MR::isGreaterStep(this, ::sFindTime)) {
        setNerve(&NrvTakoHei::TakoHeiNrvPursue::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryPursueEnd() {
    if (MR::isGreaterStep(this, ::sPursueTime)) {
        setNerve(&NrvTakoHei::TakoHeiNrvCoolDown::sInstance);
        return true;
    }

    if (isFallNextMove()) {
        MR::zeroVelocity(this);
        setNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryAttack() {
    f32 distanceToPlayer = MR::calcDistanceToPlayer(this);
    if (::sAttackDistanceNear <= distanceToPlayer && (distanceToPlayer <= ::sAttackDistanceFar || MR::isGreaterStep(this, ::sForceFarAttackTime))) {
        setNerve(&NrvTakoHei::TakoHeiNrvAttackSign::sInstance);
        return true;
    }

    return false;
}

bool TakoHei::tryCoolDownEnd() {
    if (MR::isGreaterStep(this, ::sCoolDownTime)) {
        if (isInSightMario()) {
            setNerve(&NrvTakoHei::TakoHeiNrvPursue::sInstance);
        } else {
            setNerve((&NrvTakoHei::TakoHeiNrvWait::sInstance));
        }
        return true;
    }

    return false;
}

bool TakoHei::tryAttackSuccessEnd() {
    if (MR::isGreaterStep(this, ::sAttackSuccessTime)) {
        setNerve((&NrvTakoHei::TakoHeiNrvWait::sInstance));
        return true;
    }

    return false;
}

bool TakoHei::tryPointBind() {
    if (_90->tryStartPointBind()) {
        setNerve((&NrvTakoHei::TakoHeiNrvBindStarPointer::sInstance));
        return true;
    }

    return false;
}

bool TakoHei::tryRecover() {
    if (MR::isGreaterStep(this, ::sSwoonTime)) {
        setNerve((&NrvTakoHei::TakoHeiNrvRecover::sInstance));
        return true;
    }

    return false;
}

bool TakoHei::tryRecoverEnd() {
    if (MR::isGreaterStep(this, ::sRecoverTime)) {
        setNerve((&NrvTakoHei::TakoHeiNrvWait::sInstance));
        return true;
    }

    return false;
}

bool TakoHei::tryPressed() {
    if (MR::isPressedRoofAndGround(this) && requestPressDown()) {
        MR::zeroVelocity(this);
        MR::offBind(this);
        return true;
    }

    return false;
}

void TakoHei::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::startBtp(this, "Open");
        updatePose();
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::offCalcGravity(this);
    }

    MR::zeroVelocity(this);

    if (tryActive() || tryPointBind()) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
    }
}

void TakoHei::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBtp(this, "Open");
    }

    if (_E4 == 1) {
        MR::startBtp(this, "Open");
    }

    if (getNerveStep() % 60 == 59) {
        MR::startBtp(this, "Blink");
    }

    updateNormalVelocity();

    if (tryPointBind() || tryFindTurn() || tryWalk() || tryNonActive()) {
        // needed to get unused comparison to generate
        return;
    }
}

void TakoHei::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
        MR::startBtp(this, "Open");
        decideNextTargetPos();
    }

    if (_E4 == 1) {
        MR::startBtp(this, "Open");
    }

    if (getNerveStep() % 60 == 59) {
        MR::startBtp(this, "Blink");
    }

    MR::turnDirectionToTargetDegree(this, &_B0, _BC, ::sWalkTurnDegree);

    if (MR::isGreaterStep(this, ::sWalkMoveStartTime)) {
        MR::addVelocityMoveToTarget(this, _BC, ::sWalkAccel);
    }

    updateNormalVelocity();

    if (tryPointBind() || tryFindTurn() || tryWalkEnd()) {
        return;
    }
}

void TakoHei::exeFindTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Turn", nullptr);
        MR::startBtp(this, "Open");
        MR::startSound(this, "SE_EM_TAKOHEI_FIND");
    }

    if (_E4 == 1) {
        MR::startBtp(this, "Open");
    }

    MR::turnDirectionToTargetDegree(this, &_B0, *MR::getPlayerPos(), ::sFindTurnDegree);

    updateNormalVelocity();

    if (tryPointBind() || tryFind()) {
        return;
    }
}

void TakoHei::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Search", nullptr);
        MR::startBtp(this, "Search");
    }

    updateNormalVelocity();

    if (tryPointBind() || tryPursue()) {
        return;
    }
}

void TakoHei::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::startBtp(this, "Blink");
    }

    if (_E4 == 1) {
        MR::startBtp(this, "Blink");
    }

    bool isFarPlayer = false;

    if (MR::isNearPlayer(this, 700.0f)) {
        MR::addVelocityAwayFromTarget(this, *MR::getPlayerPos(), ::sPursueAccel);
    } else {
        MR::addVelocityMoveToTarget(this, *MR::getPlayerPos(), ::sPursueAccel);
        isFarPlayer = true;
    }

    MR::turnDirectionToTargetDegree(this, &_B0, *MR::getPlayerPos(), ::sPursueTurnDegree);

    updateNormalVelocity();

    if (isFarPlayer) {
        if (checkFront()) {
            setNerve(&NrvTakoHei::TakoHeiNrvAttackSign::sInstance);
            return;
        }
    }

    if (tryAttack() || tryPointBind() || tryPursueEnd()) {
        return;
    }
}

void TakoHei::exeCoolDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "CoolDown", nullptr);
        MR::startBtp(this, "Cry");
    }

    if (_E4 == 1) {
        MR::startBtp(this, "Cry");
    }

    MR::turnDirectionToTargetDegree(this, &_B0, *MR::getPlayerPos(), 0.0f);

    updateNormalVelocity();

    if (tryPointBind() || tryCoolDownEnd()) {
        return;
    }
}

void TakoHei::exeAttackSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShotStart", nullptr);
        MR::startBtp(this, "Angry");
    }

    MR::startLevelSound(this, "SE_EM_TAKOHEI_LV_PRE_SPIT_OUT");

    MR::turnDirectionToTargetDegree(this, &_B0, *MR::getPlayerPos(), ::sAttackSignDegree);

    if (checkFront()) {
        MR::addVelocity(this, _B0 * ::sBackAttackSpaceAccel);
    }

    updateNormalVelocity();

    if (tryPointBind()) {
        return;
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTakoHei::TakoHeiNrvAttack::sInstance);
    }
}

void TakoHei::exeAttack() {
    if (MR::isFirstStep(this)) {
        TPos3f mouthMtx = MR::getJointMtx(this, "Mouth");
        TVec3f mult;
        mouthMtx.mult(::sInkShotOffset, mult);
        MR::spurtTakoHeiInk(mult, _B0 * ::sSpurtPowerH + mGravity * ::sSpurtPowerV);

        MR::startBck(this, "Shot", nullptr);
        MR::startSound(this, "SE_EM_TAKOHEI_SPIT_OUT");
    }

    updateNormalVelocity();

    if (tryPointBind()) {
        return;
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance);
    }
}

void TakoHei::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startBtp(this, "Angry");
    }

    updateNormalVelocity();

    if (tryPointBind() || tryAttackSuccessEnd()) {
        return;
    }
}

void TakoHei::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, _90, &NrvTakoHei::TakoHeiNrvWait::sInstance);
}

void TakoHei::endBindStarPointer() {
    _90->kill();
}

void TakoHei::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "StunStart", nullptr);
        MR::startBtp(this, "Cry");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Stun", nullptr);
    }

    if (MR::isBckPlaying(this, "Stun")) {
        MR::startLevelSound(this, "SE_EM_LV_SWOON_S");
    }

    updateSwoonVelocity();

    if (tryRecover()) {
        return;
    }
}

void TakoHei::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "StunEnd", nullptr);
        MR::startBtp(this, "BlinkTwice");
        MR::startSound(this, "SE_EV_TAKOHEI_RECOVER");
    }

    updateNormalVelocity();

    if (tryRecoverEnd()) {
        return;
    }
}

void TakoHei::exePressDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HipDropDown", nullptr);
        MR::startBtp(this, "Cry");
        MR::startSound(this, "SE_EM_TAKOHEI_STOMPED_L");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, ::sPressDownTime)) {
        kill();
    }
}

void TakoHei::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HipDropDown", nullptr);
        MR::startBtp(this, "Cry");
        MR::startSound(this, "SE_EM_TAKOHEI_STOMPED_L");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, ::sHipDropDownTime)) {
        kill();
    }
}

void TakoHei::exeFlatDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FlatDown", nullptr);
        MR::startBtp(this, "Cry");
        MR::startSound(this, "SE_EM_TAKOHEI_STOMPED");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, ::sFlatDownTime)) {
        kill();
    }
}

void TakoHei::exePunchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PunchDown", nullptr);
        MR::startBtp(this, "Cry");
        MR::turnDirectionToTarget(this, &_B0, *MR::getPlayerPos(), -1.0f);
        MR::stopScene(::sPunchStopSceneTime);
        MR::startSound(this, "SE_EM_TAKOHEI_HITPUNCH");
        MR::startBlowHitSound(this);
    }

    updateNormalVelocity();

    if (MR::isGreaterStep(this, ::sPunchDownTime) || (MR::isBinded(this) && MR::isGreaterStep(this, 5))) {
        kill();
    }
}

void TakoHei::updatePose() {
    TVec3f* up;

    if (MR::isBindedGround(this)) {
        up = const_cast< TVec3f* >(MR::getGroundNormal(this));
    } else {
        up = &mGravity.invertOperatorInternal();
    }

    MR::blendQuatUpFront(&_94, *up, _B0, 0.1f, 0.2f);
}

void TakoHei::updateNormalVelocity() {
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, ::sNormalGroundFrec);
    } else {
        MR::addVelocityToGravity(this, ::sNormalGravityAccel);
        MR::attenuateVelocity(this, ::sNormalAirFrec);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void TakoHei::updateSwoonVelocity() {
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, ::sSwoonGroundFrec);
    } else {
        MR::addVelocityToGravity(this, ::sSwoonGravityAccel);
        MR::attenuateVelocity(this, ::sNormalAirFrec);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void TakoHei::decideNextTargetPos() {
    MR::getRandomVector(&_BC, ::sWalkGoalDistance);

    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(_BC);
    JMAVECScaleAdd(grav, _BC, _BC, -dot);

    _BC += _D4;
}

bool TakoHei::checkFront() const {
    TVec3f positionUpOffset;
    MR::calcPositionUpOffset(&positionUpOffset, this, ::sCheckFrontHeight);
    return MR::isExistMapCollision(positionUpOffset, _B0 * ::sCheckFrontAttackSpace);
}

bool TakoHei::canAttack() const {
    if (isNerve(&NrvTakoHei::TakoHeiNrvWait::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvWalk::sInstance) ||
        isNerve(&NrvTakoHei::TakoHeiNrvFindTurn::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvFind::sInstance) ||
        isNerve(&NrvTakoHei::TakoHeiNrvPursue::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvCoolDown::sInstance)) {
        return true;
    } else {
        return false;
    }
}

bool TakoHei::isPushMovable() const {
    if (_E4 > 0) {
        return false;
    }

    if (isDown()) {
        return false;
    }

    if (isNerve(&NrvTakoHei::TakoHeiNrvNonActive::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvAttackSuccess::sInstance) ||
        isNerve(&NrvTakoHei::TakoHeiNrvBindStarPointer::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvSwoon::sInstance)) {
        return false;
    } else {
        return true;
    }
}

bool TakoHei::isEnableKick() const {
    if (isNerve(&NrvTakoHei::TakoHeiNrvSwoon::sInstance)) {
        return true;
    } else {
        return false;
    }
}

bool TakoHei::isDown() const {
    if (isNerve(&NrvTakoHei::TakoHeiNrvHipDropDown::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvFlatDown::sInstance) ||
        isNerve(&NrvTakoHei::TakoHeiNrvPressDown::sInstance) || isNerve(&NrvTakoHei::TakoHeiNrvPunchDown::sInstance)) {
        return true;
    } else {
        return false;
    }
}

bool TakoHei::isFallNextMove() const {
    if (_ED) {
        return false;
    }

    if (MR::isOnGround(this)) {
        return MR::isFallNextMove(mPosition, mVelocity, mGravity, 150.0f, mScale.x * 90.0f * 2.0f, 150.0f, nullptr);
    } else {
        return false;
    }
}

bool TakoHei::isInSightMario() const {
    TVec3f dirToPlayer(*MR::getPlayerPos() - mPosition);
    f32 scalarToPlayer;
    MR::separateScalarAndDirection(&scalarToPlayer, &dirToPlayer, dirToPlayer);

    TVec3f quatZDir;

    _94.getZDir(quatZDir);

    if (scalarToPlayer < 1600.0f && dirToPlayer.dot(quatZDir) > 0.0f) {
        return true;
    } else {
        return false;
    }
}

TakoHei::~TakoHei() {};
