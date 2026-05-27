#include "Game/Enemy/Jellyfish.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

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
    static Color8 sPointLightColor(0xFF, 0x32, 0xCA, 0xFF);
};

Jellyfish::Jellyfish(const char* pName) : LiveActor(pName) {
    mController = nullptr;
    _94 = 0;
    _98.set(0.0f, 0.0f, 1.0f);
    mIsConnectedRail = false;
    _A8 = 5.0f;
    _AC = 60;
    _B0 = 0;
    _B4.set(0.0f, 0.0f, 0.0f);
}

void Jellyfish::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Jellyfish", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::addToAttributeGroupSearchTurtle(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.set(0.0f, 30.0f, 0.0f);
    MR::addHitSensor(this, "body", 0x1E, 8, 100.0f, sensorOffs);
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
    initNerve(&NrvJellyfish::JellyfishNrvWait::sInstance);
    makeActorAppeared();
}

void Jellyfish::kill() {
    MR::emitEffect(this, "Death");
    MR::appearCoinPop(this, mPosition, 1);
    LiveActor::kill();
}

void Jellyfish::control() {
    Color8 clr = sPointLightColor;
    MR::requestPointLight(this, TVec3f(mPosition), clr, 0.0998f, -1);
    mController->updateNerve();

    if (!isNerve(&NrvJellyfish::JellyfishNrvDeath::sInstance)) {
        if (mIsConnectedRail) {
            MR::moveCoordAndFollowTrans(this, _A8);
            if (MR::isRailReachedGoal(this)) {
                if (!isNerve(&NrvJellyfish::JellyfishNrvRailGoal::sInstance)) {
                    setNerve(&NrvJellyfish::JellyfishNrvRailGoal::sInstance);
                    return;
                }
            }
        }

        mVelocity.scale(JMath::sSinCosTable.sinLapRad(_94 + 0x2D), mGravity);
        _94++;
    }

    if (tryDPDSwoon()) {
    }
}

void Jellyfish::calcAndSetBaseMtx() {
    TVec3f v7;
    v7.negate(mGravity);
    TPos3f pos;
    MR::makeMtxFrontUpPos(&pos, _98, v7, mPosition);
    MR::setBaseTRMtx(this, pos);
    TVec3f scale;
    JMathInlineVEC::PSVECMultiply(&mController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

void Jellyfish::exeWait() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Wait")) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Wait");
    }

    MR::startLevelSound(this, "SE_EM_LV_JELYFISH_WAIT", -1, -1, -1);
    selectNerveAfterWait();
}

void Jellyfish::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SearchOn", nullptr);
        MR::startBrk(this, "SearchOn");
        MR::startSound(this, "SE_EM_JELYFISH_FIND", -1, -1);
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

    MR::startLevelSound(this, "SE_EM_LV_JELLYFISH_WAIT", -1, -1, -1);
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
        MR::startSound(this, "SE_EM_JELLYFISH_HIT_PUNCH", -1, -1);
    }

    if (MR::isStep(this, 30) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_JELLYFISH_DEAD", -1, -1);
        kill();
    }
}

void Jellyfish::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startBrk(this, "Attack");
        MR::startSound(this, "SE_EM_JELLYFISH_ATTACK", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        if (!MR::isNearPlayer(this, 1500.0f)) {
            setNerve(&NrvJellyfish::JellyfishNrvWait::sInstance);
        } else {
            selectNerveThreat();
        }
    }
}

void Jellyfish::exeRailGoal() {
    if (MR::isStep(this, _AC)) {
        if (_B0) {
            MR::moveCoordAndTransToRailPoint(this, 0);
            const TVec3f& railDir = MR::getRailDirection(this);
            _98.x = railDir.x;
            _98.y = railDir.y;
            _98.z = railDir.z;
        } else {
            MR::reverseRailDirection(this);
        }

        setNerve(&NrvJellyfish::JellyfishNrvWait::sInstance);
    }
}

void Jellyfish::exeThreatWithLeftTurn() {
    threatTurn();
}

void Jellyfish::exeThreatWithRightTurn() {
    threatTurn();
}

void Jellyfish::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mBindStarPtr, &NrvJellyfish::JellyfishNrvWait::sInstance);
}

void Jellyfish::exeWaitWithLeftTurn() {
    MR::rotateVecDegree(&_98, mGravity, -0.2f);
    selectNerveAfterWait();
}

void Jellyfish::exeWaitWithRightTurn() {
    MR::rotateVecDegree(&_98, mGravity, 0.2f);
    selectNerveAfterWait();
}

// Jellyfish::attackSensor

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
    if (MR::isMsgExplosionAttack(msg) && !isNerve(&NrvJellyfish::JellyfishNrvDeath::sInstance)) {
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

    MR::startLevelSound(this, "SE_EM_LV_JELLYFISH_WAIT", -1, -1, -1);
    faceToMario();

    if (MR::isBckStopped(this)) {
        selectNerveThreat();
    }
}

bool Jellyfish::faceToMario() {
    TVec3f* pos = &mPosition;
    TVec3f v13;
    JMathInlineVEC::PSVECSubtract(MR::getPlayerPos(), pos, &v13);
    MR::normalizeOrZero(&v13);
    TVec3f v12;
    MR::calcSideVec(&v12, this);

    if (!MR::isNearZero(v13)) {
        f32 v4 = MR::negateIfLessZero(0.5f);
        f32 cos = JMath::sSinCosTable.cosShort(v4);
        MR::turnVecToVecCosOnPlane(&_98, v13, v12, cos);
        TVec3f v10;
        v10.negate(mGravity);
        TVec3f v11;
        MR::turnVecToPlane(&v11, _98, v10);
        MR::clampVecAngleDeg(&_98, v11, 30.0f);
    }

    if (!isNerve(&NrvJellyfish::JellyfishNrvThreat::sInstance)) {
        f32 frameMax = MR::getBckFrameMax(this, "SearchRotate");
        f32 v7 = (1.0f - (frameMax / getNerveStep()));
        f32 v8;
        if (isNerve(&NrvJellyfish::JellyfishNrvThreatWithLeftTurn::sInstance)) {
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

void Jellyfish::knockOut(HitSensor* a2, HitSensor* a3) {
    TVec3f v6;
    TVec3f v5(a3->mPosition);
    JMathInlineVEC::PSVECSubtract(&v5, &a2->mPosition, v5);
    MR::normalize(v5, &v6);
    mVelocity.scale(50.0f, v6);
    _98.negate(v6);
    setNerve(&NrvJellyfish::JellyfishNrvDeath::sInstance);
}

// Jellyfish::selectNerveAfterWait
// Jellyfish::selectNerveThreat
// Jellyfish::tryDPDSwoon

Jellyfish::~Jellyfish() {
}
