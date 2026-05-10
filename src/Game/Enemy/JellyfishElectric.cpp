#include "Game/Enemy/JellyfishElectric.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace NrvJellyfishElectric {
    NEW_NERVE(JellyfishElectricNrvWait, JellyfishElectric, Wait);
    NEW_NERVE(JellyfishElectricNrvWaitWithRightTurn, JellyfishElectric, WaitWithRightTurn);
    NEW_NERVE(JellyfishElectricNrvWaitWithLeftTurn, JellyfishElectric, WaitWithLeftTurn);
    NEW_NERVE(JellyfishElectricNrvDamage, JellyfishElectric, Damage);
    NEW_NERVE(JellyfishElectricNrvDeath, JellyfishElectric, Death);
    NEW_NERVE(JellyfishElectricNrvAttack, JellyfishElectric, Attack);
    NEW_NERVE(JellyfishElectricNrvRailGoal, JellyfishElectric, RailGoal);
    NEW_NERVE_ONEND(JellyfishElectricNrvDPDSwoon, JellyfishElectric, DPDSwoon, DPDSwoon);

};  // namespace NrvJellyfishElectric

namespace {
    static Color8 sPointLightColor(0x9C, 0xFF, 0xFF, 0xFF);
};

JellyfishElectric::JellyfishElectric(const char* pName) : LiveActor(pName) {
    mController = nullptr;
    mBindStarPtr = nullptr;
    _94 = 0;
    _98.set(0.0f, 0.0f, 1.0f);
    _A4 = false;
    mIsConnectedRail = 0;
    _A8 = 5.0f;
    _AC = 60;
    _B0 = 0;
    _B4.set(0.0f, 0.0f, 0.0f);
}

void JellyfishElectric::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JellyfishElectric", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::addToAttributeGroupSearchTurtle(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.set(0.0f, -60.0f, 0.0f);
    MR::addHitSensorEnemy(this, "body", 8, 350.0f, sensorOffs);
    initBinder(400.0f, 0.0f, 0);
    MR::offBind(this);
    mController = new AnimScaleController(nullptr);
    mController->setParamTight();
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    mController = new AnimScaleController(nullptr);
    mBindStarPtr = new WalkerStateBindStarPointer(this, mController);
    MR::initShadowVolumeSphere(this, 230.0f);
    MR::declareCoin(this, 1);

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
    MR::startBrk(this, "Wait");
    initNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
    makeActorAppeared();
}

void JellyfishElectric::kill() {
    MR::appearCoinPop(this, mPosition, 1);
    LiveActor::kill();
}

void JellyfishElectric::control() {
    Color8 clr = sPointLightColor;
    MR::requestPointLight(this, TVec3f(mPosition), clr, 0.0998f, -1);
    MR::changeShowModelFlagSyncNearClipping(this, 700.0f);
    mController->updateNerve();

    if (!tryDPDSwoon() && !isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDeath::sInstance)) {
        if (mIsConnectedRail) {
            MR::moveCoordAndFollowTrans(this, _A8);
            if (MR::isRailReachedGoal(this)) {
                if (!isNerve(&NrvJellyfishElectric::JellyfishElectricNrvRailGoal::sInstance)) {
                    setNerve(&NrvJellyfishElectric::JellyfishElectricNrvRailGoal::sInstance);
                    return;
                }
            }
        }

        mVelocity.scale(JMath::sSinCosTable.sinLapRad(_94 + 0x2D), mGravity);
        _94++;
    }
}

void JellyfishElectric::calcAndSetBaseMtx() {
    TVec3f scale;
    TVec3f v7 = -mGravity;
    TPos3f pos;
    MR::makeMtxFrontUpPos(&pos, _98, v7, mPosition);
    MR::setBaseTRMtx(this, pos);
    JMathInlineVEC::PSVECMultiply(&mController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

void JellyfishElectric::exeWait() {
    if (MR::isFirstStep(this)) {
        if (!_A4 && !MR::isBckPlaying(this, "Wait")) {
            MR::startAllAnim(this, "Wait");
        } else {
            if (_A4 && !MR::isBckPlaying(this, "DangerWait")) {
                MR::startAllAnim(this, "DangerWait");
                MR::startBas(this, "DangerBrk", false, 0.0f, 0.0f);
            }
        }
    }

    selectNerveAfterWait();
}

void JellyfishElectric::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_JELLYELEC_DAMAGE", -1, -1);
        MR::startAllAnim(this, "Damage");
    }

    if (MR::isBckStopped(this)) {
        _A4 = 1;
        MR::startAllAnim(this, "DangerWait");
        MR::startBas(this, "DangerBrk", false, 0.0f, 0.0f);
        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
    }
}

void JellyfishElectric::exeDeath() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::invalidateHitSensors(this);
        MR::stopScene(5);
        MR::startAllAnim(this, "Death");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELYELEC_PRE_DEAD", -1, -1, -1);
    if (MR::isStep(this, 30) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_JELLYELEC_DEAD", -1, -1);
        kill();
    }
}

void JellyfishElectric::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Attack");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELYELEC_ATTACK", -1, -1, -1);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
    }
}

void JellyfishElectric::exeRailGoal() {
    if (MR::isStep(this, _AC)) {
        if (_B0) {
            MR::moveCoordAndTransToRailPoint(this, 0);
            const TVec3f& dir = MR::getRailDirection(this);
            _98.x = dir.x;
            _98.y = dir.y;
            _98.z = dir.z;
            _A4 = 0;
        } else {
            MR::reverseRailDirection(this);
        }

        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
    }
}

void JellyfishElectric::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }

    MR::updateActorStateAndNextNerve(this, mBindStarPtr, &NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
}

void JellyfishElectric::exeWaitWithLeftTurn() {
    waitTurn();
    selectNerveAfterWait();
}

void JellyfishElectric::exeWaitWithRightTurn() {
    waitTurn();
    selectNerveAfterWait();
}

void JellyfishElectric::endDPDSwoon() {
    mBindStarPtr->kill();
}

void JellyfishElectric::waitTurn() {
    f32 turnDirection;
    f32 turnDecay = (1.0f - (getNerveStep() / 280.0f));

    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvWaitWithLeftTurn::sInstance)) {
        turnDirection = 1.0f;
    } else {
        turnDirection = -1.0f;
    }

    MR::rotateVecDegree(&_98, -mGravity, (turnDirection * (0.2f * turnDecay)));
}

void JellyfishElectric::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pSender) && MR::isSensorPlayer(pReceiver)) {
        tryToAttackElectric(pReceiver, pSender);
    } else if (MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver)) {
        if (MR::isSensorEnemy(pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool JellyfishElectric::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        mController->startHitReaction();
        return true;
    } else if (MR::isMsgJetTurtleAttack(msg)) {
        knockOut();
        return true;
    } else if (MR::isMsgInvincibleAttack(msg)) {
        _A4 = 1;
        knockOut();
        return true;
    } else {
        if (MR::isMsgPlayerSpinAttack(msg)) {
            return tryToAttackElectric(pSender, pReceiver);
        }
    }

    return false;
}

bool JellyfishElectric::receiveMsgEnemyAttack(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isMsgExplosionAttack(msg) && !isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDeath::sInstance)) {
        knockOut();
        return true;
    }

    return false;
}

void JellyfishElectric::knockOut() {
    if (!_A4) {
        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvDamage::sInstance);
    } else {
        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvDeath::sInstance);
    }
}

bool JellyfishElectric::tryToAttackElectric(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDPDSwoon::sInstance) && MR::sendMsgEnemyAttackElectric(pSender, pReceiver)) {
        MR::emitEffectHitBetweenSensors(this, pReceiver, pSender, 0.0f, nullptr);
        setNerve(&NrvJellyfishElectric::JellyfishElectricNrvAttack::sInstance);
    }

    MR::sendMsgPush(pSender, pReceiver);
    return true;
}

bool JellyfishElectric::tryDPDSwoon() {
    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDPDSwoon::sInstance)) {
        return false;
    }

    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDamage::sInstance)) {
        return false;
    }

    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvDeath::sInstance)) {
        return false;
    }

    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvAttack::sInstance)) {
        return false;
    }

    if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvRailGoal::sInstance)) {
        return false;
    }

    if (!mBindStarPtr->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvJellyfishElectric::JellyfishElectricNrvDPDSwoon::sInstance);
    return true;
}

bool JellyfishElectric::selectNerveAfterWait() {
    if (MR::isStep(this, 280)) {
        s32 rand;

        if (isNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance)) {
            rand = MR::getRandom(0l, 3l);
        } else {
            rand = 0;
        }

        if (rand == 0) {
            setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
        } else if (rand == 1) {
            setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWaitWithRightTurn::sInstance);
        } else {
            setNerve(&NrvJellyfishElectric::JellyfishElectricNrvWaitWithLeftTurn::sInstance);
        }

        return true;
    }

    return false;
}

JellyfishElectric::~JellyfishElectric() {
}
