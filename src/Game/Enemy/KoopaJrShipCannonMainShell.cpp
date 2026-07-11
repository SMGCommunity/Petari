#include "Game/Enemy/KoopaJrShipCannonMainShell.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sTmpScale = 1.2f;
    static const f32 sAttackSensorRadius = 75.0f;
    static const f32 sShadowRadius = 60.0f;
    static const f32 sBinderRadius = 75.0f;
    static const s32 sLifeTime = 300;
    static const f32 sCameraShakeDistance = 1000.0f;
    static const s32 sWallHitInvalidTime = 60;
};  // namespace

namespace NrvKoopaJrShipCannonMainShell {
    NEW_NERVE(HostTypeWait, KoopaJrShipCannonMainShell, Wait);
    NEW_NERVE(HostTypeFly, KoopaJrShipCannonMainShell, Fly);
};  // namespace NrvKoopaJrShipCannonMainShell

KoopaJrShipCannonMainShell::KoopaJrShipCannonMainShell(const char* pName) : CannonShellBase(pName) {
    f32 one = 1.0f;  // This makes Data match
    _8C.set< f32 >(0.0f, 0.0f, 0.0f, one);
}

CannonShellBase::~CannonShellBase() {
}

void KoopaJrShipCannonMainShell::init(const JMapInfoIter& rIter) {
    mScale.setAll< f32 >(getBaseScale());

    initModelManagerWithAnm("KoopaJrShipCannonMainShell", nullptr, false);

    MR::startBtk(this, "KoopaJrShipCannonMainShell");

    initSound(4, false);
    initEffectKeeper(0, "KoopaJrShipCannonShell", false);
    initHitSensor(1);

    MR::addHitSensorEnemyAttack(this, "attack", 8, ::sAttackSensorRadius * getBaseScale(), TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(::sBinderRadius * getBaseScale(), 0.0f, 0);
    initNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance);

    MR::connectToSceneEnemy(this);
    MR::invalidateClipping(this);
    MR::initShadowVolumeSphere(this, ::sShadowRadius * getBaseScale());
    MR::offCalcGravity(this);
    makeActorDead();
}

void KoopaJrShipCannonMainShell::kill() {
    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;
    MR::deleteEffect(this, "LocusSmoke");
    MR::deleteEffect(this, "LocusFire");
    LiveActor::kill();
}

void KoopaJrShipCannonMainShell::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void KoopaJrShipCannonMainShell::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemyAttack(pSender)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender)) {
        explosion();
        return;
    }

    if (!MR::isSensorEnemy(pReceiver)) {
        return;
    }

    bool isFly = isNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance) && MR::isGreaterEqualStep(this, 0);

    if (!isFly) {
        return;
    }

    if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
        return;
    }
}

bool KoopaJrShipCannonMainShell::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg)) {
        MR::sendMsgEnemyAttack(pReceiver, pSender);
        return true;
    }

    return MR::isMsgStarPieceReflect(msg);
}

void KoopaJrShipCannonMainShell::launch(const TVec3f& rStartPos, const TVec3f& rVelocity) {
    appear();

    mPosition.set< f32 >(rStartPos);
    TVec3f dir;
    dir = rVelocity;
    MR::normalize(&dir);

    TPos3f mtx;
    MR::makeMtxFrontUp(&mtx, dir, mGravity.invertOperatorInternal());
    mtx.getQuat(_8C);
    mVelocity.set< f32 >(rVelocity);
    setNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance);
}

f32 KoopaJrShipCannonMainShell::getBaseScale() const {
    return ::sTmpScale;
}

void KoopaJrShipCannonMainShell::explosion() {
    MR::startRumbleWithShakeCameraWeak(this, "強", "中", ::sCameraShakeDistance, ::sCameraShakeDistance * 2);
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_BM_KOOPAJR_SHIP_BREAK_FIRE");
    kill();
}

void KoopaJrShipCannonMainShell::exeWait() {
}

void KoopaJrShipCannonMainShell::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "LocusSmoke");
        MR::emitEffect(this, "LocusFire");
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_FLY_FIRE");

    if (MR::isGreaterStep(this, ::sWallHitInvalidTime) && MR::isBindedWall(this)) {
        explosion();
    } else if (MR::isStep(this, ::sLifeTime)) {
        kill();
    }
}

namespace CannonShellUtil {
    void registerKoopaJrShipCannonMainShell(CannonShellHolder* pHolder, int shellNum) {
        for (s32 i = 0; i < shellNum; i++) {
            KoopaJrShipCannonMainShell* pShell = new KoopaJrShipCannonMainShell("メイン砲弾");
            pShell->initWithoutIter();
            pShell->makeActorDead();
            pHolder->registerCannonShell(pShell);
        }
    }
};  // namespace CannonShellUtil
