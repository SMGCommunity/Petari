#include "Game/Enemy/KoopaJrShipCannonShell.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    static const f32 sBodySensorRadius = 75.0f;
    static const f32 sAttackSensorRadius = 60.0f;
    static const f32 sShadowRadius = 60.0f;
    static const f32 sBinderRadius = 75.0f;
    static const f32 sStarWandRadius = 150.0f;
    static const s32 sLifeTime = 360;
    static const f32 sCameraShakeDistance = 1000.0f;
    static const s32 sDownTime = 45;
    static const f32 sGravity = 1.0f;
    static const f32 sReboundRate = 0.55f;
    static const f32 sReboundMinSpeed = 2.0f;
    static const f32 sAirFrec = 0.99f;
    static const f32 sGroundFrec = 0.94999999f;
    static const s32 sFreezeFrame = 20;
    static const f32 sFreezeRumbleSpeed = 0.75f;
    static const f32 sFreezeRumbleWidth = 5.0f;
    static const s32 sWallHitInvalidTime = 10;
};  // namespace

namespace NrvKoopaJrShipCannonShell {
    NEW_NERVE(HostTypeWait, KoopaJrShipCannonShell, Wait);
    NEW_NERVE(HostTypeFly, KoopaJrShipCannonShell, Fly);
    NEW_NERVE(HostTypeDown, KoopaJrShipCannonShell, Down);
    NEW_NERVE(HostTypeHipDropDown, KoopaJrShipCannonShell, Down);
    NEW_NERVE(HostTypeFreeze, KoopaJrShipCannonShell, Freeze);
};  // namespace NrvKoopaJrShipCannonShell

KoopaJrShipCannonShell::KoopaJrShipCannonShell(const char* pName) : CannonShellBase(pName), _9C(gZeroVec), _A8(gZeroVec), _B4(0), _B8(true) {
    f32 one = 1.0f;  // This makes Data match
    _8C.set< f32 >(0.0f, 0.0f, 0.0f, one);
    mPosition.x = gZeroVec.x;
    mPosition.y = gZeroVec.y;
    mPosition.z = gZeroVec.z;
}

inline void KoopaJrShipCannonShell::initStarPointer() {
    TVec3f offset;
    offset.set(0.0f);
    MR::initStarPointerTarget(this, ::sStarWandRadius, offset);
}

void KoopaJrShipCannonShell::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("KoopaJrShipCannonShell", nullptr, false);
    MR::startBck(this, "KoopaJrShipCannonShell");
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, ::sBodySensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, ::sAttackSensorRadius * getBaseScale(), TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(::sBinderRadius * getBaseScale(), 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFly));

    MR::connectToSceneEnemy(this);
    MR::invalidateClipping(this);
    initStarPointer();
    MR::initShadowVolumeSphere(this, ::sShadowRadius * getBaseScale());
    MR::offCalcGravity(this);
    MR::declareCoin(this, 1);
    makeActorDead();
}

void KoopaJrShipCannonShell::kill() {
    mVelocity.zero();
    MR::deleteEffect(this, "LocusSmoke");
    LiveActor::kill();
}

void KoopaJrShipCannonShell::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void KoopaJrShipCannonShell::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemyAttack(pSender))
        return;

    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
        explosion();
        return;
    }

    if (MR::isSensorEnemy(pReceiver) && isStateEnableExplosion() && MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
        explosion();
        return;
    }

    if (MR::isSensorRide(pReceiver))
        if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender))
            return;
}

bool KoopaJrShipCannonShell::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pReceiver))
        return false;

    if (MR::isMsgPlayerTrample(msg)) {
        if (isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeDown)))
            return false;

        MR::tryRumblePad(this, "中", WPAD_CHAN0);
        setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeDown));
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumblePad(this, "中", WPAD_CHAN0);
        setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeHipDropDown));
        return true;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg))
        return true;

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::appearCoinPop(this, mPosition, 1);
        explosion();
        return true;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        explosion();
        return true;
    }

    return false;
}

bool KoopaJrShipCannonShell::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgExplosionAttack(msg))
        return false;

    if (!MR::isSensorEnemyAttack(pSender))
        return false;

    if (!isStateEnableExplosion())
        return false;

    explosion();
    return true;
}

bool KoopaJrShipCannonShell::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender))
        return false;

    if (!MR::isMsgInvalidHit(msg))
        return false;

    explosion();
    return true;
}

void KoopaJrShipCannonShell::launch(const TVec3f& rStartPos, const TVec3f& rVelocity) {
    appear();

    mPosition.set(rStartPos);

    TVec3f vec(rVelocity);
    MR::normalize(&vec);

    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, vec);
    mtx.getQuat(_8C);
    mVelocity.set(rVelocity);
    MR::startSound(this, "SE_BM_KOOPAJR_SHIP_SHOOT_NORMAL");
    setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFly));
}

bool KoopaJrShipCannonShell::tryFreeze() {
    if (!_B8)
        return false;

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false))
        return false;

    _9C.set(mPosition);
    _A8.set(mVelocity);
    _B4 = 0;
    setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFreeze));
    return true;
}

bool KoopaJrShipCannonShell::isStateEnableExplosion() const {
    return (isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFly)) && MR::isGreaterEqualStep(this, 0)) ||
           isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFreeze)) || isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeDown)) ||
           isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeHipDropDown));
}

void KoopaJrShipCannonShell::explosion() {
    MR::startRumbleWithShakeCameraWeak(this, "強", "中", ::sCameraShakeDistance, ::sCameraShakeDistance * 2);
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_EM_KILLER_EXPLOSION");
    kill();
}

void KoopaJrShipCannonShell::misfire() {
    MR::shakeCameraWeak();
    MR::tryRumblePad(this, "弱", WPAD_CHAN0);
    MR::emitEffect(this, "MisFire");
    MR::startSound(this, "SE_EM_KILLER_MISS_FIRE");
    kill();
}

void KoopaJrShipCannonShell::exeWait() {
}

void KoopaJrShipCannonShell::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "LocusSmoke");
    }
    MR::startLevelSound(this, "SE_EM_LV_KILLER_FLY");

    if (tryFreeze())
        return;

    if (MR::isGreaterStep(this, ::sWallHitInvalidTime) && MR::isBinded(this)) {
        explosion();
        return;
    }
    if (MR::isStep(this, ::sLifeTime))
        kill();
}

void KoopaJrShipCannonShell::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::onCalcGravity(this);
        MR::startSound(this, "SE_EM_STOMPED_S");
        if (isNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeHipDropDown))) {
            MR::jumpPlayer(-mGravity);
        }
    }

    mVelocity.add(mGravity * ::sGravity);
    if (MR::reboundVelocityFromCollision(this, ::sReboundRate, ::sReboundMinSpeed, 1.0f))
        MR::deleteEffect(this, "LocusSmoke");

    mVelocity.scale(MR::isBindedGround(this) ? ::sGroundFrec : ::sAirFrec);

    if (MR::isStep(this, ::sDownTime))
        misfire();
}

#pragma push
#pragma opt_propagation off
void KoopaJrShipCannonShell::exeFreeze() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        if (!_B4) {
            MR::startDPDHitSound();
        }
    }

    _B4++;
    MR::startDPDFreezeLevelSound(this);
    TVec3f vec14;
    f32 scale = ::sFreezeRumbleWidth * MR::cosDegree(MR::repeatDegree(_B4 * ::sFreezeRumbleSpeed)) *
                static_cast< f32 >(::sFreezeFrame - getNerveStep()) / ::sFreezeFrame;

    vec14.set(MR::getCamXdir());
    vec14.scale(scale);
    editPosition()->add(_9C, vec14);

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFreeze));
        return;
    }

    if (MR::isStep(this, ::sFreezeFrame)) {
        mPosition.set(_9C);
        mVelocity.set(_A8);
        setNerve(GET_NERVE(KoopaJrShipCannonShell, HostTypeFly));
    }
}
#pragma pop

namespace CannonShellUtil {
    void registerKoopaJrShipCannonShell(CannonShellHolder* pHolder, int shellNum) {
        for (s32 i = 0; i < shellNum; i++) {
            KoopaJrShipCannonShell* pShell = new KoopaJrShipCannonShell("砲弾");
            pShell->initWithoutIter();
            pShell->makeActorDead();
            pHolder->registerCannonShell(pShell);
        }
    }
};  // namespace CannonShellUtil
