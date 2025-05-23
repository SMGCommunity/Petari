#include "Game/Enemy/Kuribo.hpp"
#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Enemy/WalkerStateStagger.hpp"
#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateWander.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/Air.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
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

Kuribo::~Kuribo() {
    
}
