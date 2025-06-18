#include "Game/Enemy/Kuribo.hpp"
#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Enemy/WalkerStateStagger.hpp"
#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateWander.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/Air.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/mtx.h"

namespace {
    class KuriboParam {
    public:
        KuriboParam();

        WalkerStateParam mStateParam;                       // 0x00
        WalkerStateStaggerParam mStaggerParam;              // 0x18
        WalkerStateFindPlayerParam mFindPlayerParam;        // 0x48
        WalkerStateChaseParam mChaseParam;                  // 0x54
        WalkerStateWanderParam mWanderParam;                // 0x68
    };

    KuriboParam::KuriboParam() {
        mStateParam._0 = 1.5f;
        mStateParam._4 = 0.99f;
        mStateParam._8 = 0.93f;
        mStateParam._C = 1000.0f;
        mStateParam._10 = 70.0f;
        mStateParam._14 = 30.0f;
        mWanderParam._8 = 0.2f;
        mWanderParam._0 = 120;
        mWanderParam._4 = 120;
        mWanderParam._C = 3.0f;
        mChaseParam._0 = 0.4f;
        mFindPlayerParam._8 = 5.0f;
        mFindPlayerParam._4 = 20.0f;
    }

    static KuriboParam sParam;
};

namespace NrvKuribo {
    NEW_NERVE_ONEND(KuriboNrvNonActive, Kuribo, NonActive, NonActive);
    NEW_NERVE(KuriboNrvAppearFromBox, Kuribo, AppearFromBox);
    NEW_NERVE(KuriboNrvBlow, Kuribo, Blow);
    NEW_NERVE(KuriboNrvBlowLand, Kuribo, BlowLand);
    NEW_NERVE(KuriboNrvWander, Kuribo, Wander);
    NEW_NERVE(KuriboNrvFindPlayer, Kuribo, FindPlayer);
    NEW_NERVE(KuriboNrvChase, Kuribo, Chase);
    NEW_NERVE(KuriboNrvAttackSuccess, Kuribo, AttackSuccess);
    NEW_NERVE_ONEND(KuriboNrvBindStarPointer, Kuribo, BindStarPointer, BindStarPointer);
    NEW_NERVE(KuriboNrvStagger, Kuribo, Stagger);
    NEW_NERVE(KuriboNrvFlatDown, Kuribo, FlatDown);
    NEW_NERVE(KuriboNrvHipDropDown, Kuribo, HipDropDown);
    NEW_NERVE(KuriboNrvPressDown, Kuribo, PressDown);
    NEW_NERVE(KuriboNrvBlowDown, Kuribo, BlowDown);
};

Kuribo::Kuribo(const char *pName) : LiveActor(pName), mScaleController(nullptr), mItemGenerator(nullptr), mStateWander(nullptr),
    mStateFindPlayer(nullptr), mBindStarPointer(nullptr), mStateStagger(nullptr), mStateChase(nullptr),
    _A8(0.0f, 0.0f, 0.0f, 1.0f), _B8(0, 0, 1), _C4(0), _C5(1) {
    
}

void Kuribo::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Kuribo", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_A8, &_B8, this);
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg0WithInit(rIter, &_C4);
    }

    if (!_C4) {
        MR::onCalcGravity(this);
        MR::calcGravity(this);
    }
    else {
        MR::offCalcGravity(this);
        TVec3f v8;
        MR::getUpVecFromQuat(&v8, _A8);
        mGravity.set<f32>(v8);
    }

    mScaleController = new AnimScaleController(nullptr);
    mItemGenerator = new ItemGenerator();
    f32 offsScale = mScale.y;
    TVec3f v7;
    v7.x = 0.0f;
    v7.y = 75.0f * offsScale;
    v7.z = 0.0f;
    MR::initStarPointerTarget(this, 70.0f * mScale.y, v7);
    initSound(4, false);
    if (MR::isEqualStageName("KoopaBattleVs2Galaxy")) {
        MR::setSeVersion(this, 1);
    }
    else {
        MR::setSeVersion(this, 0);
    }

    initBinder((70.0f * mScale.y), (70.0f * mScale.y), 0);
    initEffectKeeper(1, nullptr, false);
    initSensor();
    MR::initShadowVolumeSphere(this, 60.0f);
    initState();
    initNerve(&NrvKuribo::KuriboNrvWander::sInstance);
    initAppearState(rIter);
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    if (MR::isValidInfo(rIter)) {
        MR::setGroupClipping(this, rIter, 32);
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void Kuribo::initAfterPlacement() {
    MR::turnQuatUpToGravity(&_A8, _A8, this);
    MR::trySetMoveLimitCollision(this);
}

void Kuribo::initState() {
    mStateFindPlayer = new WalkerStateFindPlayer(this, &_B8, &sParam.mStateParam, &sParam.mFindPlayerParam);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    mStateStagger = new WalkerStateStagger(this, &_B8, &sParam.mStateParam, &sParam.mStaggerParam);
    mStateWander = new WalkerStateWander(this, &_B8, &sParam.mStateParam, &sParam.mWanderParam);
    mStateChase = new WalkerStateChase(this, &_B8, &sParam.mStateParam, &sParam.mChaseParam);
}

void Kuribo::initSensor() {
    f32 y_scale = mScale.y;
    initHitSensor(2);
    MR::addHitSensor(this, "body", 27, 8, 75.0f * y_scale, TVec3f(0.0f, 75.0f * y_scale, 0.0f));
    MR::addHitSensor(this, "attack", 22, 8, 45.0f * y_scale, TVec3f(0.0f, 75.0f * y_scale, 0.0f));
}

void Kuribo::initAppearState(const JMapInfoIter &rIter) {
    s32 state = -1;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg1NoInit(rIter, &state);
    }

    switch (state) {
        case -1:
            setNerve(&NrvKuribo::KuriboNrvWander::sInstance);
            break;
        case 0:
            setNerve(&NrvKuribo::KuriboNrvAppearFromBox::sInstance);
            _A8.getZDir(_B8);
            break;
    }
}

void Kuribo::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    mItemGenerator->setTypeStarPeace(3);
}

void Kuribo::makeActorDead() {
    LiveActor::makeActorDead();
}

void Kuribo::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "DeathSmokeKuribo");
    MR::startSoundSeVer(this, "SE_EM_EXPLODE_S", -1, -1);
    if (_C5) {
        mItemGenerator->generate(this);
    }

    LiveActor::kill();
}

void Kuribo::control() {
    mScaleController->updateNerve();
    if (_C4) {
        MR::calcGravityOrZero(this);
    }

    MR::blendQuatFromGroundAndFront(&_A8, this, _B8, 0.05f, 0.5f);
    if (!isNerve(&NrvKuribo::KuriboNrvBindStarPointer::sInstance)) {
        TVec3f v3(0, 0, 0);
        if (MR::calcVelocityAreaOrRailMoveOnGround(&v3, this)) {
            TVec3f stack_8;
            stack_8.setPS(v3);
            stack_8.z *= 0.05f;
            stack_8.x *= 0.05f;
            stack_8.y *= 0.05f;
            MR::addVelocity(this, stack_8);
        }
    }

    if (!tryDead()) {
        if (tryPointBind()) {
            return;
        }
    }
}

void Kuribo::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A8);
    TVec3f scale;
    JMathInlineVEC::PSVECMultiply(&mScaleController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

void Kuribo::attackSensor(HitSensor *a2, HitSensor *a3) {
    if ((!MR::isSensorType(a2, 27) || (isEnableAttack() || !MR::isSensorPlayer(a3)) && !MR::isSensorEnemy(a3) || !isEnablePushMove() || !MR::sendMsgPushAndKillVelocityToTarget(this, a3, a2)) && isEnableAttack() && MR::isSensorPlayer(a3) && MR::isSensorEnemyAttack(a2)) {
        if (!MR::isPlayerHipDropFalling() && MR::sendMsgEnemyAttack(a3, a2)) {
            requestAttackSuccess();
        }
        else {
            MR::sendMsgPush(a3, a2);
        }
    }
}

bool Kuribo::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return requestStagger(a2, a3);
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return requestFlatDown(a2, a3);
    }

    if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgFireBallAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
        return requestBlowDown(a2, a3);
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        return requestHipDropDown(a2, a3);
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        if (!isEnableKick()) {
            return requestStagger(a2, a3);
        }
        return requestBlowDown(a2, a3);
    }

    return false;
}

bool Kuribo::receiveMsgEnemyAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgToEnemyAttackShockWave(msg)) {
        return requestStagger(a2, a3);
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        return requestPressDown();
    }
    
    if (MR::isMsgExplosionAttack(msg)) {
        return requestBlowDown(a2, a3);
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        return requestBlowDown(a2, a3);
    }

    return false;
}

bool Kuribo::receiveMsgPush(HitSensor *a2, HitSensor *a3) {
    if (MR::isSensorEnemyAttack(a3)) {
        return false;
    }

    if (MR::isSensorEnemy(a2) || (MR::isSensorRide(a2) || (!isEnableAttack() && MR::isSensorPlayer(a2)))) {
        if (isEnablePushMove()) {
            MR::addVelocityFromPush(this, 1.5f, a2, a3);
            return true;
        }
    }

    return false;
}

bool Kuribo::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        mItemGenerator->setTypeNone();
        kill();
        return true;
    }
    else if (MR::isMsgPlayerKick(msg) && isEnableKick()) {
        return requestBlowDown(a2, a3);
    }

    return false;
}

bool Kuribo::requestDead() {
    if (isNerve(&NrvKuribo::KuriboNrvNonActive::sInstance) || isNerve(&NrvKuribo::KuriboNrvFlatDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvPressDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvBlowDown::sInstance)) {
        return false;
    }

    MR::deleteEffectAll(this);
    MR::clearHitSensors(this);
    MR::invalidateHitSensors(this);
    return true;
}

bool Kuribo::requestFlatDown(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorEnemyAttack(a1)) {
        return false;
    }

    if (!isEnableStamp()) {
        return false;
    }

    if (!requestDead()) {
        return false;
    }

    MR::startSoundSeVer(this, "SE_EM_STOMPED_S", -1, -1);
    if (isUpsideDown()) {
        MR::startAction(this, "FlatDownReverse");
    }
    else {
        MR::startAction(this, "FlatDown");
    }

    setNerve(&NrvKuribo::KuriboNrvFlatDown::sInstance);
    MR::offBind(this);
    mItemGenerator->setTypeCoin(1);
    return true;
}

bool Kuribo::requestHipDropDown(HitSensor *a1, HitSensor *a2) {
    if (!requestDead()) {
        return false;
    }

    if (MR::isSensorEnemyAttack(a2)) {
        return false;
    }

    MR::startSoundSeVer(this, "SE_EM_STOMPED_S", -1, -1);
    if (isUpsideDown()) {
        MR::startAction(this, "HipDropDownReverse");
    }
    else {
        MR::startAction(this, "HipDropDown");
    }

    setNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance);
    MR::offBind(this);
    mItemGenerator->setTypeCoin(1);
    return true;
}

bool Kuribo::requestPressDown() {
    if (!requestDead()) {
        return false;
    }

    MR::startSoundSeVer(this, "SE_EM_STOMPED_S", -1, -1);
    if (isUpsideDown()) {
        MR::startAction(this, "HipDropDownReverse");
    }
    else {
        MR::startAction(this, "HipDropDown");
    }

    setNerve(&NrvKuribo::KuriboNrvPressDown::sInstance);
    MR::zeroVelocity(this);
    MR::offBind(this);
    mItemGenerator->setTypeStarPeace(3);
    return true;
}

bool Kuribo::requestStagger(HitSensor *a1, HitSensor *a2) {
    if (isEnablePanch()) {
        mStateStagger->setPunchDirection(a1, a2);
        setNerve(&NrvKuribo::KuriboNrvStagger::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::requestBlowDown(HitSensor *a1, HitSensor *a2) {
    if (!isEnablePanch()) {
        return false;
    }

    MR::deleteEffectAll(this);
    MR::setVelocityBlowAttack(this, a1, a2, 36.0f, 30.0f, 4);
    setNerve(&NrvKuribo::KuriboNrvBlowDown::sInstance);
    mItemGenerator->setTypeStarPeace(3);
    return true;
}

bool Kuribo::requestAttackSuccess() {
    if (isEnableAttack()) {
        mScaleController->stopAndReset();
        MR::deleteEffectAll(this);
        setNerve(&NrvKuribo::KuriboNrvAttackSuccess::sInstance);
        return true;
    }
    return false;
}

void Kuribo::onNoGravitySupport() {
    _C4 = 1;
    MR::offCalcGravity(this);
}

void Kuribo::setUp(const TVec3f &a1, const TQuat4f &a2, const TVec3f &a3) {
    mPosition.set<f32>(a1);
    mVelocity.set<f32>(a3);
    _A8.x = a2.x;
    _A8.y = a2.y;
    _A8.z = a2.z;
    _A8.w = a2.w;
    _A8.getZDir(_B8);
}

void Kuribo::appearBlowed(const TVec3f &a1, const TQuat4f &a2, const TVec3f &a3) {
    setUp(a1, a2, a3);
    setNerve(&NrvKuribo::KuriboNrvBlow::sInstance);
    MR::startAction(this, "Blow");
    appear();
    if (_C4) {
        MR::calcGravityOrZero(this);
    }
}

void Kuribo::appearHipDropped(const TVec3f &a1, const TQuat4f &a2) {
    setUp(a1, a2, TVec3f(0.0f, 0.0f, 0.0f));
    appear();
    MR::startAction(this, "HipDropDown");
    setNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance);
    MR::offBind(this);
    mItemGenerator->setTypeCoin(1);
}

bool Kuribo::tryNonActive() {
    bool isNotNearPlayer =  !MR::isNearPlayerAnyTime(this, 3000.0f);
    if (isNotNearPlayer && MR::isBindedGround(this)) {
        MR::zeroVelocity(this);
        setNerve(&NrvKuribo::KuriboNrvNonActive::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::tryActive() {
    if (MR::isNearPlayerAnyTime(this, 3000.0f)) {
        setNerve(&NrvKuribo::KuriboNrvWander::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::tryEndBlow() {
    if (MR::isBindedGround(this) && MR::isGreaterStep(this, 10)) {
        MR::startAction(this, "Land");
        MR::onBind(this);
        setNerve(&NrvKuribo::KuriboNrvBlowLand::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::tryEndBlowLand() {
    if (MR::isGreaterStep(this, 15)) {
        setNerve(&NrvKuribo::KuriboNrvChase::sInstance);
        return true;
    }
    
    return false;
}

bool Kuribo::tryFind() {
    if (mStateFindPlayer->isInSightPlayer()) {
        setNerve(&NrvKuribo::KuriboNrvFindPlayer::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::tryPointBind() {
    if (isEnablePointBind() && mBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvKuribo::KuriboNrvBindStarPointer::sInstance);
        return true;
    }

    return false;
}

bool Kuribo::tryDead() {
    if (isEnableDead()) {
        if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isBindedGroundDamageFire(this) || MR::isInWater(mPosition)) {
            mItemGenerator->setTypeNone();
            kill();
            return true;
        }

        if (MR::isPressedRoofAndGround(this) && requestPressDown()) {
            return true;
        }
    }

    return false;
}

void Kuribo::exeWander() {
    MR::updateActorState(this, mStateWander);
    if (!tryFind()) {
        if (tryNonActive()) {
            return;
        }
    }
}

void Kuribo::exeFindPlayer() {
    if (!MR::updateActorStateAndNextNerve(this, mStateFindPlayer, &NrvKuribo::KuriboNrvChase::sInstance)) {
        if (mStateFindPlayer->isFindJumpBegin()) {
            MR::startSoundSeVer(this, "SE_EM_KURIBO_FIND", -1, -1);
        }
    }
}

void Kuribo::exeChase() {
    if (MR::updateActorStateAndNextNerve(this, mStateChase, &NrvKuribo::KuriboNrvWander::sInstance)) {
        mStateWander->setWanderCenter(mPosition);
    }

    if (mStateChase->isRunning()) {
        MR::startLevelSoundSeVer(this, "SE_EM_LV_KURIBO_DASH1", -1, -1, -1);
    }
}

void Kuribo::exeStagger() {
    if (!MR::updateActorStateAndNextNerve(this, mStateStagger, &NrvKuribo::KuriboNrvWander::sInstance)) {
        if (mStateStagger->isStaggerStart()) {
            MR::startSoundSeVer(this, "SE_EM_CRASH_S", -1, -1);
            MR::startBlowHitSound(this);
        }

        if (mStateStagger->isSwooning(15)) {
            MR::startLevelSoundSeVer(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
        }

        if (mStateStagger->isRecoverStart()) {
            MR::startSoundSeVer(this, "SE_EM_KURIBO_SWOON_RECOVER", -1, -1);
        }
    }
}

void Kuribo::exeNonActive() {
    if (MR::isFirstStep(this)) {
        calcAnim();
        MR::offBind(this);
        MR::offCalcGravity(this);
        MR::offCalcShadow(this, nullptr);
    }

    MR::zeroVelocity(this);
    if (tryActive()) {
        return;
    }
}

void Kuribo::exeAppearFromBox() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "AppearFromBox");
        TVec3f zDir;
        _A8.getZDir(zDir);
        MR::setVelocitySeparateHV(this, zDir, 7.0f, 30.0f);
        MR::offBind(this);
    }
    exeBlow();
}

void Kuribo::exeBlow() {
    calcPassiveMovement();
    if (MR::isStep(this, 10)) {
        MR::onBind(this);
    }

    if (tryEndBlow()) {
        return;
    }
}

void Kuribo::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hit");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &_B8, *MR::getPlayerPos(), 5.0f);
    calcPassiveMovement();
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvKuribo::KuriboNrvWander::sInstance);
    }
}

void Kuribo::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startSoundSeVer(this, "SE_EM_CRASH_S", -1, -1);
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 40)) {
        kill();
    }
}

void Kuribo::exeFlatDown() {
    if (MR::isFirstStep(this)) {
        MR::startSoundSeVer(this, "SE_EM_CRASH_S", -1, -1);
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 20)) {
        kill();
    }
}

void Kuribo::exePressDown() {
    if (MR::isFirstStep(this)) {
        MR::startSoundSeVer(this, "SE_EM_CRASH_S", -1, -1);
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 180)) {
        kill();
    }
}

void Kuribo::exeBlowDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Blow");
        MR::startSoundSeVer(this, "SE_EM_CRASH_S", -1, -1);
        MR::startBlowHitSound(this);
    }

    calcPassiveMovement();
    TVec3f invVelocity;
    JMathInlineVEC::PSVECNegate(&mVelocity, &invVelocity);
    MR::turnDirectionDegree(this, &_B8, invVelocity, 30.0f);
    if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}

void Kuribo::calcPassiveMovement() {
    if (!MR::isOnGround(this)) {
        MR::addVelocityToGravity(this, 1.5f);
    }

    f32 vel;

    if (MR::isOnGround(this)) {
        vel = 0.93f;
    }
    else {
        vel = 0.99f;
    }

    MR::attenuateVelocity(this, vel);
    if (MR::isBindedWall(this)) {
        MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.2f, 0.69f);
    }
}

bool Kuribo::isEnableKick() const {
    if (isNerve(&NrvKuribo::KuriboNrvStagger::sInstance)) {
        return mStateStagger->isEnableKick();
    }

    return false;
}

bool Kuribo::isUpsideDown() const {
    if (isNerve(&NrvKuribo::KuriboNrvStagger::sInstance)) {
        return mStateStagger->isUpsideDown();
    }

    return false;
}

bool Kuribo::isEnableDead() const {
    if (isNerve(&NrvKuribo::KuriboNrvWander::sInstance) || isNerve(&NrvKuribo::KuriboNrvFindPlayer::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvChase::sInstance) || isNerve(&NrvKuribo::KuriboNrvStagger::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvBindStarPointer::sInstance) || isNerve(&NrvKuribo::KuriboNrvNonActive::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvAttackSuccess::sInstance)) {
        return true;
    }

    return false;
}

bool Kuribo::isEnablePointBind() const {
    if (isNerve(&NrvKuribo::KuriboNrvWander::sInstance) || isNerve(&NrvKuribo::KuriboNrvFindPlayer::sInstance) || isNerve(&NrvKuribo::KuriboNrvChase::sInstance) || isNerve(&NrvKuribo::KuriboNrvAttackSuccess::sInstance)) {
        return true;
    }

    return false;
}

bool Kuribo::isEnableAttack() const {
    if (isNerve(&NrvKuribo::KuriboNrvWander::sInstance) || isNerve(&NrvKuribo::KuriboNrvFindPlayer::sInstance) || isNerve(&NrvKuribo::KuriboNrvChase::sInstance)) {
        return true;
    }

    return false;
}

bool Kuribo::isEnableStamp() const {
    if (isNerve(&NrvKuribo::KuriboNrvNonActive::sInstance) || isNerve(&NrvKuribo::KuriboNrvFlatDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvPressDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvBlowDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvBlow::sInstance)) {
        return false;
    }

    return true;
}

bool Kuribo::isEnablePanch() const {
    if (isNerve(&NrvKuribo::KuriboNrvFlatDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvPressDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvBlowDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvBlow::sInstance)) {
        return false;
    }

    return true;
}

bool Kuribo::isEnablePushMove() const {
    if (isNerve(&NrvKuribo::KuriboNrvFlatDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvHipDropDown::sInstance) || 
        isNerve(&NrvKuribo::KuriboNrvPressDown::sInstance) || isNerve(&NrvKuribo::KuriboNrvBlowDown::sInstance)) {
        return false;
    }

    return true;
}

void Kuribo::exeOnEndBindStarPointer() {
    mBindStarPointer->kill();
}

void Kuribo::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvKuribo::KuriboNrvWander::sInstance);
}

void Kuribo::exeBlowLand() {
    calcPassiveMovement();
    tryEndBlowLand();
}

void Kuribo::exeOnEndNonActive() {
    MR::onBind(this);
    MR::onCalcGravity(this);
    MR::onCalcShadow(this, nullptr);
}

namespace MR {
    LiveActor* createNoItemKuriboActor(const char *pName) {
        Kuribo* kuribo = new Kuribo(pName);
        kuribo->_C5 = 0;
        return kuribo;
    }
};

Kuribo::~Kuribo() {
    
}
