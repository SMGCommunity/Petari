#include "Game/Enemy/KoopaJrShipCannonMainShell.hpp"

namespace NrvKoopaJrShipCannonMainShell {
    NEW_NERVE(HostTypeWait, KoopaJrShipCannonMainShell, Wait);
    NEW_NERVE(HostTypeFly, KoopaJrShipCannonMainShell, Fly);
}

KoopaJrShipCannonMainShell::KoopaJrShipCannonMainShell(const char* pName) : CannonShellBase(pName) {
    f32 one = 1.0f; // This makes Data match
    _8C.set< f32 >(0.0f, 0.0f, 0.0f, one);
}

void KoopaJrShipCannonMainShell::init(const JMapInfoIter& rIter) {
    mScale.setAll< f32 >(getBaseScale());

    initModelManagerWithAnm("KoopaJrShipCannonMainShell", nullptr, false);

    MR::startBtk(this, "KoopaJrShipCannonMainShell");

    initSound(4, false);
    initEffectKeeper(0, "KoopaJrShipCannonShell", false);
    initHitSensor(1);

    MR::addHitSensorEnemyAttack(this, "attack", 8, 75.0f * getBaseScale(), TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(75.0f * getBaseScale(), 0.0f, 0);
    initNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance);

    MR::connectToSceneEnemy(this);
    MR::invalidateClipping(this);
    MR::initShadowVolumeSphere(this, 60.0f * getBaseScale());
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
    if (!MR::isSensorEnemyAttack(pSender))
        return;

    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender)) {
        explosion();
        return;
    }

    if (!MR::isSensorEnemy(pReceiver))
        return;
    
    bool x = false;
    if (isNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance) && MR::isGreaterEqualStep(this, 0)) {
        x = true;
    }

    if (x)
        if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender))
            return;
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
    TVec3f* vec = &TVec3f();

    vec->setPS2(rVelocity);
    MR::normalize(vec);

    TPos3f mtx;
    MR::makeMtxFrontUp(&mtx, *vec, mGravity.invertOperatorInternal());
    mtx.getQuat(_8C);
    mVelocity.set< f32 >(rVelocity);
    setNerve(&NrvKoopaJrShipCannonMainShell::HostTypeFly::sInstance);
}


f32 KoopaJrShipCannonMainShell::getBaseScale() const {
    return 1.2f;
}

void KoopaJrShipCannonMainShell::explosion() {
    MR::startRumbleWithShakeCameraWeak(this, "強", "中", 1000.0f, 2000.0f);
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_BM_KOOPAJR_SHIP_BREAK_FIRE", -1, -1);
    kill();
}

void KoopaJrShipCannonMainShell::exeWait() { }

void KoopaJrShipCannonMainShell::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "LocusSmoke");
        MR::emitEffect(this, "LocusFire");
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_FLY_FIRE", -1, -1, -1);

    if (MR::isGreaterStep(this, 60) && MR::isBindedWall(this))
        explosion();
    else if (MR::isStep(this, 300))
        kill();
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
}