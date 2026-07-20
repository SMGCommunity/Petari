#include "Game/Enemy/Metbo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

void Metbo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 a = JGeometry::TUtil< f32 >::epsilon();
    (void)-1.0f;
}

namespace {
    // sSpeedMax
    static const f32 sReceivePushPower = 1.5f;
    static const s32 sStepForWait = 150;
    static const s32 sStepForWalkAround = 200;
    static const s32 sStepForChaseMin = 120;
    static const s32 sStepForChaseMax = 220;
    static const s32 sStepForSwoonWait = 150;
    static const s32 sStepForRest = 60;
    // sActiveRadius
    static const f32 sWalkAroundRadius = 300.0f;
    static const f32 sWalkAroundTurn = 20.0f;
    // sFindRadiusIn
    // sFindRadiusOut
    // sSpinHitStop
    // sSpinPower
    // sSpinPowerHRate
};  // namespace

namespace NrvMetbo {
    NEW_NERVE_ONEND(MetboNrvNonActive, Metbo, NonActive, NonActive);
    NEW_NERVE(MetboNrvWait, Metbo, Wait);
    NEW_NERVE(MetboNrvWalkAround, Metbo, WalkAround);
    NEW_NERVE(MetboNrvSearch, Metbo, Search);
    NEW_NERVE(MetboNrvChaseStart, Metbo, ChaseStart);
    NEW_NERVE_ONEND(MetboNrvChase, Metbo, Chase, Chase);
    NEW_NERVE(MetboNrvGuard, Metbo, Guard);
    NEW_NERVE(MetboNrvTrampleDown, Metbo, TrampleDown);
    NEW_NERVE(MetboNrvBlowDown, Metbo, BlowDown);
    NEW_NERVE(MetboNrvAttack, Metbo, Attack);
    NEW_NERVE(MetboNrvRest, Metbo, Rest);
    NEW_NERVE(MetboNrvSwoonStart, Metbo, SwoonStart);
    NEW_NERVE(MetboNrvSwoonLand, Metbo, SwoonLand);
    NEW_NERVE(MetboNrvSwoonSpin, Metbo, SwoonSpin);
    NEW_NERVE(MetboNrvSwoonWait, Metbo, SwoonWait);
    NEW_NERVE(MetboNrvRecover, Metbo, Recover);
    NEW_NERVE_ONEND(MetboNrvDPDSwoon, Metbo, DPDSwoon, DPDSwoon);
};  // namespace NrvMetbo

namespace {
    static const MR::ActorMoveParam sFallParam = {0.0f, 3.0f, 0.99f, 0.0f};
    static const MR::ActorMoveParam sDontMoveParam = {0.0f, 2.0f, 0.5f, 0.0f};
    static const MR::ActorMoveParam sSkateMoveParam = {0.0f, 2.0f, 0.98f, 0.0f};
    static const MR::ActorMoveParam sWalkAroundParam = {0.25f, 2.0f, 0.92f, 2.0f};
    static const MR::ActorMoveParam sSearchParam = {0.0f, 2.0f, 0.5f, 5.0f};
    static const MR::ActorMoveParam sChaseParam = {0.8f, 2.0f, 0.9f, 2.0f};
};  // namespace

Metbo::Metbo(const char* pName) : LiveActor(pName) {
    mSpinHitController = nullptr;
    mAnimScaleController = nullptr;
    mStarPointerBind = nullptr;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    _A4.x = 0.0f;
    _A4.y = 0.0f;
    _A4.z = 1.0f;
    _B0.x = 0.0f;
    _B0.y = 0.0f;
    _B0.z = 0.0f;
    mIsClipped = false;
}

Metbo::~Metbo() {
}

void Metbo::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Metbo", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(100.0f, 100.0f, 0);
    MR::initLightCtrl(this);
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);

    if (MR::isEqualStageName("KoopaBattleVs2Galaxy")) {
        MR::setSeVersion(this, 1);
    } else {
        MR::setSeVersion(this, 0);
    }

    MR::initShadowVolumeSphere(this, 80.0f);
    s32 arg;
    mIsClipped = MR::getJMapInfoClippingGroupID(rIter, &arg);
    MR::setGroupClipping(this, rIter, 32);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    MR::onCalcGravity(this);
    MR::declareCoin(this, 1);
    MR::declareStarPiece(this, 3);
    TVec3f offset;
    MR::initStarPointerTarget(this, 100.0f, offset.makeZeroVec());
    mAnimScaleController = new AnimScaleController(nullptr);
    mAnimScaleController->setParamTight();
    mStarPointerBind = new WalkerStateBindStarPointer(this, mAnimScaleController);
    MR::calcFrontVec(&_A4, this);
    mSpinHitController = new SpinHitController(this, 20, 20, 4, 0.2f, 40.0f, 20.0f, 3, false);
    _98.set(mPosition);
    initNerve(&NrvMetbo::MetboNrvWait::sInstance);
    makeActorAppeared();
}

void Metbo::exeNonActive() {
    if (MR::isFirstStep(this)) {
        mVelocity.z = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.x = 0.0f;
        if (!mIsClipped) {
            MR::offBind(this);
            MR::offCalcShadow(this, nullptr);
            MR::offCalcGravity(this);
            MR::offCalcAnim(this);
        }
        MR::invalidateHitSensors(this);
    }

    if (mIsClipped) {
        moveOrFall(::sDontMoveParam, nullptr);
    }

    if (MR::isNearPlayerAnyTime(this, 3000.0f)) {
        setNerve(&NrvMetbo::MetboNrvWait::sInstance);
    }
}

void Metbo::endNonActive() {
    MR::onBind(this);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcAnim(this);
    MR::onCalcGravity(this);
    MR::validateHitSensors(this);
}

void Metbo::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if ((!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) && !trySearch() && !tryNonActive() && MR::isStep(this, ::sStepForWait)) {
        setNerve(&NrvMetbo::MetboNrvWalkAround::sInstance);
    }
}

void Metbo::exeWalkAround() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
    }

    bool isNear = MR::isNear(this, _98, ::sWalkAroundRadius) == false;
    if (isNear || MR::isBindedWallOfMoveLimit(this) || MR::isFallNextMove(mPosition, _A4, mGravity, 200.0f, 100.0f, 300.0f, nullptr)) {
        _B0.sub(_98, mPosition);
    } else if (!(getNerveStep() % 10)) {
        TVec3f upVec;
        f32 angle;
        MR::calcUpVec(&upVec, this);
        if (MR::getRandom(0l, 2l)) {
            angle = -1.0f;
        } else {
            angle = 1.0f;
        }
        MR::rotateVecDegree(&_B0, upVec, (::sWalkAroundTurn * angle));
    }

    TVec3f v;
    JMathInlineVEC::PSVECAdd(_B0, mPosition, v);
    moveOrFall(::sWalkAroundParam, &v);
    if (!trySearch() && MR::isStep(this, ::sStepForWalkAround)) {
        if (MR::getRandom(0l, 2l)) {
            setNerve(&NrvMetbo::MetboNrvWait::sInstance);
        } else {
            setNerve(&NrvMetbo::MetboNrvWalkAround::sInstance);
        }
    }
}

void Metbo::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Search", nullptr);
        MR::startSoundSeVer(this, "SE_EM_METBO_FIND");
    }

    moveOrFall(::sSearchParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvChaseStart::sInstance);
    }
}

void Metbo::exeChaseStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RunStart", nullptr);
    }

    if (MR::isBindedGround(this)) {
        MR::startLevelSoundSeVer(this, "SE_EM_LV_KURIBO_DASH1");
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvChase::sInstance);
    }
}

void Metbo::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
    }

    if (MR::isBindedGround(this)) {
        MR::startLevelSoundSeVer(this, "SE_EM_LV_KURIBO_DASH1");
    }

    moveOrFall(::sChaseParam, nullptr);
    if (MR::isFallNextMove(mPosition, _A4, mGravity, 200.0f, 100.0f, 300.0f, nullptr)) {
        setNerve(&NrvMetbo::MetboNrvRest::sInstance);
        return;
    }

    if (MR::isLessStep(this, ::sStepForChaseMin)) {
        return;
    }

    bool isNear = MR::isNearPlayer(this, 1200.0f) == false;
    if (isNear || MR::isBindedWallOfMoveLimit(this) || MR::isStep(this, ::sStepForChaseMax) || !MR::getRandom(0l, 50l)) {
        setNerve(&NrvMetbo::MetboNrvRest::sInstance);
    }
}

void Metbo::endChase() {
    _98.set(mPosition);
}

void Metbo::exeGuard() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Guard", nullptr);
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvWait::sInstance);
    }
}

void Metbo::exeTrampleDown() {
    if (MR::isFirstStep(this)) {
        MR::startSoundSeVer(this, "SE_EM_STOMPED_S");
        MR::startBck(this, "Press", nullptr);
        MR::invalidateHitSensors(this);
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        MR::appearCoinPop(this, mPosition, 1);
        kill();
    }
}

void Metbo::exeBlowDown() {
    if (MR::isFirstStep(this)) {
        MR::startBlowHitSound(this);
        MR::startBck(this, "Blow", nullptr);
        MR::invalidateHitSensors(this);
    }
    mSpinHitController->execute(this);
}

void Metbo::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvRest::sInstance);
    }
}

void Metbo::exeRest() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isStep(this, ::sStepForRest)) {
        setNerve(&NrvMetbo::MetboNrvWait::sInstance);
    }
}

void Metbo::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonStart", nullptr);
        MR::startSoundSeVer(this, "SE_EM_CRASH_S");
    } else {
        MR::moveAndTurnToPlayer(this, &_A4, ::sFallParam._0, ::sFallParam._4, ::sFallParam._8, ::sFallParam._C);
        if (MR::isBindedGround(this)) {
            setNerve(&NrvMetbo::MetboNrvSwoonLand::sInstance);
        }
    }
}

void Metbo::exeSwoonLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonLand", nullptr);
    }

    MR::startLevelSoundSeVer(this, "SE_EM_LV_METBO_STAGGER");
    moveOrFall(::sSkateMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvSwoonSpin::sInstance);
    }
}

void Metbo::exeSwoonSpin() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonSpin", nullptr);
    }

    MR::startLevelSoundSeVer(this, "SE_EM_LV_METBO_STAGGER");
    moveOrFall(::sSkateMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvSwoonWait::sInstance);
    }
}

void Metbo::exeSwoonWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwoonWait", nullptr);
    }

    MR::startLevelSoundSeVer(this, "SE_EM_LV_METBO_SWOON");
    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isStep(this, ::sStepForSwoonWait)) {
        setNerve(&NrvMetbo::MetboNrvRecover::sInstance);
    }
}

void Metbo::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover", nullptr);
        MR::startSoundSeVer(this, "SE_EM_METBO_SWOON_RECOVER");
    }

    moveOrFall(::sDontMoveParam, nullptr);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvMetbo::MetboNrvWait::sInstance);
    }
}

void Metbo::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mStarPointerBind, &NrvMetbo::MetboNrvWait::sInstance);
}

void Metbo::endDPDSwoon() {
    mStarPointerBind->kill();
}

void Metbo::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void Metbo::kill() {
    MR::startSoundSeVer(this, "SE_EM_EXPLODE_S");
    MR::emitEffect(this, "Death");

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    LiveActor::kill();
}

void Metbo::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _A4);
    MR::blendMtx(getBaseMtx(), mtx, 0.3f, mtx);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale = mAnimScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void Metbo::control() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInDarkMatter(mPosition) || MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        kill();
        return;
    }

    if (!isNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance)) {
        if (!isNrvDown()) {
            MR::restrictVelocity(this, 30.0f);
        }
    }
    mAnimScaleController->updateNerve();
    tryDPDSwoon();
}

void Metbo::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNrvDown()) {
        if (MR::isSensorPlayer(pReceiver) && MR::isSensorEnemyAttack(pSender)) {
            if (isNrvNormal() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
                setNerve(&NrvMetbo::MetboNrvAttack::sInstance);
            } else {
                MR::sendMsgPush(pReceiver, pSender);
            }
        }

        if (!MR::isSensorPlayer(pReceiver) && MR::isSensorEnemy(pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool Metbo::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNrvDown()) {
        return false;
    }

    if ((MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender))) {
        TVec3f v;
        v.sub(mPosition, pSender->mHost->mPosition);
        MR::normalizeOrZero(&v);
        mVelocity.scaleAdd(::sReceivePushPower, v, mVelocity);
        return true;
    }

    return false;
}

bool Metbo::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    } else if (MR::isMsgStarPieceReflect(msg)) {
        MR::startSoundSeVer(this, "SE_EM_METBO_TRAMPLE");
        if (!isNrvSwoon() && !isNerve(&NrvMetbo::MetboNrvRecover::sInstance)) {
            setNerve(&NrvMetbo::MetboNrvGuard::sInstance);
        }
        return true;
    } else {
        if (isNrvDown()) {
            return false;
        } else if (MR::isMsgPlayerHipDrop(msg)) {
            MR::startSoundSeVer(this, "SE_EM_METBO_TRAMPLE");
            if (isNerve(&NrvMetbo::MetboNrvRecover::sInstance)) {
                return false;
            } else if (isNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance)) {
                return false;
            } else {
                if (isNrvSwoon()) {
                    setNerve(&NrvMetbo::MetboNrvTrampleDown::sInstance);
                } else {
                    setNerve(&NrvMetbo::MetboNrvGuard::sInstance);
                    MR::tryRumbleDefaultHit(this, 0);
                    MR::sendMsgEnemyAttackCounterHipDrop(pSender, pReceiver);
                }
                return true;
            }
        } else if (MR::isMsgPlayerTrample(msg)) {
            MR::startSoundSeVer(this, "SE_EM_METBO_TRAMPLE");
            if (isNerve(&NrvMetbo::MetboNrvRecover::sInstance)) {
                return true;
            } else if (isNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance)) {
                return false;
            } else {
                MR::tryRumbleDefaultHit(this, 0);
                if (isNrvSwoon()) {
                    setNerve(&NrvMetbo::MetboNrvTrampleDown::sInstance);
                } else {
                    setNerve(&NrvMetbo::MetboNrvGuard::sInstance);
                }
                return true;
            }
        } else if (MR::isMsgPlayerSpinAttack(msg)) {
            if (isNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance)) {
                return false;
            } else {
                MR::stopScene(3);
                MR::startSoundSeVer(this, "SE_EM_METBO_SPIN_HIT");
                MR::startBlowHitSound(this);
                startSwoon(false);
                return true;
            }
        } else if (MR::isMsgInvincibleAttack(msg)) {
            mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
            setNerve(&NrvMetbo::MetboNrvBlowDown::sInstance);
            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool Metbo::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNrvDown()) {
        return false;
    } else if (MR::isMsgExplosionAttack(msg)) {
        mSpinHitController->startWithoutStopScene(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(&NrvMetbo::MetboNrvBlowDown::sInstance);
        return true;
    }

    return false;
}

bool Metbo::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    return false;
}

void Metbo::moveOrFall(const MR::ActorMoveParam& a1, const TVec3f* a2) {
    TVec3f v;
    v = a2 ? *a2 : *MR::getPlayerPos();
    if (MR::isBindedGround(this)) {
        MR::moveAndTurnToTarget(this, &_A4, v, a1._0, a1._4, a1._8, a1._C);
    } else {
        MR::moveAndTurnToTarget(this, &_A4, v, ::sFallParam._0, ::sFallParam._4, ::sFallParam._8, ::sFallParam._C);
    }
}

void Metbo::startSwoon(bool isFromPlayer) {
    TVec3f v;
    v.sub(mPosition, *MR::getPlayerPos());
    v.setLength(0.4f);
    JMathInlineVEC::PSVECAdd(v, -mGravity, v);
    MR::normalize(&v);
    mVelocity.scale(45.0f, v);

    if (isFromPlayer) {
        MR::turnDirectionFromPlayerDegree(this, &_A4, 180.0f);
    } else {
        MR::turnDirectionToPlayerDegree(this, &_A4, 180.0f);
    }
    setNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance);
}

bool Metbo::tryNonActive() {
    bool isNearPlayer = MR::isNearPlayerAnyTime(this, 3000.0f) == false;
    if (isNearPlayer && MR::isBindedGround(this)) {
        setNerve(&NrvMetbo::MetboNrvNonActive::sInstance);
        return true;
    }
    return false;
}

bool Metbo::trySearch() {
    bool isNear = MR::isNearPlayer(this, 800.0f) == false;
    if (isNear) {
        return false;
    }

    TVec3f* sensorPos = &getSensor("body")->mPosition;
    TVec3f v3;
    v3.sub(*MR::getPlayerCenterPos(), *sensorPos);
    if (MR::isExistMapCollision(*sensorPos, v3)) {
        return false;
    }

    if (MR::isExistMoveLimitCollision(*sensorPos, v3)) {
        return false;
    }
    TVec3f v4;
    v4.sub(*MR::getPlayerPos(), mPosition);
    MR::normalize(&v4);

    if (MR::isFallNextMove(mPosition, v4, mGravity, 200.0f, 100.0f, 300.0f, nullptr)) {
        return false;
    }

    setNerve(&NrvMetbo::MetboNrvSearch::sInstance);
    return true;
}

bool Metbo::tryDPDSwoon() {
    if (isNerve(&NrvMetbo::MetboNrvDPDSwoon::sInstance)) {
        return false;
    }

    if (isNerve(&NrvMetbo::MetboNrvRecover::sInstance)) {
        return false;
    }

    if (isNrvSwoon()) {
        return false;
    }

    if (isNrvDown()) {
        return false;
    }

    if (!mStarPointerBind->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvMetbo::MetboNrvDPDSwoon::sInstance);
    return true;
}

bool Metbo::isNrvNormal() const {
    return isNerve(&NrvMetbo::MetboNrvWait::sInstance) || isNerve(&NrvMetbo::MetboNrvWalkAround::sInstance) ||
           isNerve(&NrvMetbo::MetboNrvSearch::sInstance) || isNerve(&NrvMetbo::MetboNrvChaseStart::sInstance) ||
           isNerve(&NrvMetbo::MetboNrvChase::sInstance);
}

inline bool Metbo::isNrvDown() const {
    return isNerve(&NrvMetbo::MetboNrvTrampleDown::sInstance) || isNerve(&NrvMetbo::MetboNrvBlowDown::sInstance);
}

inline bool Metbo::isNrvSwoon() const {
    return isNerve(&NrvMetbo::MetboNrvSwoonStart::sInstance) || isNerve(&NrvMetbo::MetboNrvSwoonLand::sInstance) ||
           isNerve(&NrvMetbo::MetboNrvSwoonSpin::sInstance) || isNerve(&NrvMetbo::MetboNrvSwoonWait::sInstance);
}

inline TVec3f makeZeroVec() {
    TVec3f v;
    v.set(0.0f, 0.0f, 0.0f);
    return v;
}
