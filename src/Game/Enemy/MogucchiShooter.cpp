#include "Game/Enemy/MogucchiShooter.hpp"
#include "Game/Enemy/WaterBazooka.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvMogucchiShooter {
    NEW_NERVE(MogucchiShooterNrvWait, MogucchiShooter, Wait);
    NEW_NERVE(MogucchiShooterNrvShot, MogucchiShooter, Shot);
    NEW_NERVE(MogucchiShooterNrvTire, MogucchiShooter, Tire);
    NEW_NERVE(MogucchiShooterNrvDeathPanic, MogucchiShooter, DeathPanic);
    NEW_NERVE(MogucchiShooterNrvExplosion, MogucchiShooter, Explosion);
    NEW_NERVE(MogucchiShooterNrvLaugh, MogucchiShooter, Laugh);
    NEW_NERVE(MogucchiShooterNrvShock, MogucchiShooter, Shock);
    NEW_NERVE(MogucchiShooterNrvPanic, MogucchiShooter, Panic);
    NEW_NERVE(MogucchiShooterNrvHitShock, MogucchiShooter, HitShock);
    NEW_NERVE(MogucchiShooterNrvHitPanic, MogucchiShooter, HitPanic);
    NEW_NERVE(MogucchiShooterNrvAnger, MogucchiShooter, Anger);
    NEW_NERVE(MogucchiShooterNrvStormStart, MogucchiShooter, StormStart);
    NEW_NERVE(MogucchiShooterNrvStorm, MogucchiShooter, Storm);

}  // namespace NrvMogucchiShooter

namespace {
    static f32 sAngerBlurOffset = 100.0f;
    static f32 sExplosionBlurOffset = 100.0f;
}  // namespace

MogucchiShooter::MogucchiShooter(LiveActor* pActor, const char* pName)
    : PartsModel(pActor, pName, "MogucchiShooter", nullptr, MR::DrawBufferType_Enemy, false), mFront(0.0f, 0.0f, 0.0f) {}

void MogucchiShooter::init(const JMapInfoIter& rIter) {
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(1, false);
    MR::calcFrontVec(&mFront, this);
    MR::createCenterScreenBlur();
    MR::startBrk(this, "Normal");
    initNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    makeActorAppeared();
}

void MogucchiShooter::panicDeath() {
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvDeathPanic::sInstance);
}

void MogucchiShooter::explosion() {
    resetDirection();
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvExplosion::sInstance);
}

void MogucchiShooter::hitShock() {
    resetDirection();
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvHitShock::sInstance);
}

void MogucchiShooter::anger() {
    resetDirection();
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvAnger::sInstance);
}

void MogucchiShooter::stormStart() {
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvStormStart::sInstance);
}

void MogucchiShooter::storm() {
    setNerve(&NrvMogucchiShooter::MogucchiShooterNrvStorm::sInstance);
}

bool MogucchiShooter::isLaughed() const {
    return isNerve(&NrvMogucchiShooter::MogucchiShooterNrvLaugh::sInstance);
}

void MogucchiShooter::exeWait() {
    WaterBazooka* bazooka = reinterpret_cast< WaterBazooka* >(mHost);

    if (MR::isFirstStep(this)) {
        if (bazooka->isBazookaPinch()) {
            MR::startAction(this, "WaitPinch");
        } else {
            MR::startAction(this, "Wait");
        }
        return;
    }

    faceToMario();

    if (MR::isPlayerInBind()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvLaugh::sInstance);
        return;
    }

    if (bazooka->isPanic()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvShock::sInstance);
        return;
    }

    if (bazooka->isTired()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvTire::sInstance);
        return;
    }

    if (bazooka->isFirstShoot()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvShot::sInstance);
    }
}

void MogucchiShooter::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shot");
        return;
    }

    if (MR::isPlayerInBind()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvLaugh::sInstance);
        return;
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeTire() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Tire");
    }

    WaterBazooka* bazooka = reinterpret_cast< WaterBazooka* >(mHost);

    if (bazooka->isPanic()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvShock::sInstance);
        return;
    }

    if (!bazooka->isTired()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeDeathPanic() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Panic");
        MR::startBrk(this, "CoolDown");
    }
}

void MogucchiShooter::exeExplosion() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startCenterScreenBlur(50, sExplosionBlurOffset, 0x50, 5, 30);
    }

    if (MR::isActionEnd(this)) {
        kill();
    }
}

void MogucchiShooter::exeLaugh() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Laugh");
    }

    if (MR::isStep(this, 120)) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeShock() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shock");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvPanic::sInstance);
    }
}

void MogucchiShooter::exePanic() {
    WaterBazooka* bazooka = reinterpret_cast< WaterBazooka* >(mHost);

    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Panic");
        if (bazooka->isBazookaLifeOut()) {
            MR::setBckRate(this, 2.0f);
        }
    }

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PANIC", -1, -1, -1);
    faceToMario();

    if (!bazooka->isPanic() && MR::isOnGroundPlayer() && !bazooka->isBazookaLifeOut()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeHitShock() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shock");
    }

    faceToMario();

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvHitPanic::sInstance);
    }
}

void MogucchiShooter::exeHitPanic() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Panic");
    }

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PANIC", -1, -1, -1);

    if (!MR::isDemoActive()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeAnger() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Angry");
    }

    if (MR::isStep(this, 40)) {
        MR::startCenterScreenBlur(60, sAngerBlurOffset, 0x50, 5, 30);
    }

    faceToMario();

    if (!MR::isDemoActive()) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::exeStormStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SpinAttackStart");
    }
    faceToMario();
}

void MogucchiShooter::exeStorm() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SpinAttack");
    }

    faceToMario();

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogucchiShooter::MogucchiShooterNrvWait::sInstance);
    }
}

void MogucchiShooter::calcAndSetBaseMtx() {
    if (!mCalcOwnMtx) {
        PartsModel::calcAndSetBaseMtx();
        return;
    }

    TPos3f posMtx;
    TPos3f copyMtx;
    posMtx.identity();
    JMath::gekko_ps_copy12(&copyMtx, mFixedPos->_1C);

    TVec3f up;
    copyMtx.getYDirInline(up);

    mFixedPos->_1C.getTransInline(mPosition);
    MR::makeMtxUpFrontPos(&posMtx, up, mFront, mPosition);
    MR::setBaseTRMtx(this, posMtx);
}

void MogucchiShooter::resetDirection() {
    TPos3f mtx;
    JMath::gekko_ps_copy12(&mtx, MR::getJointMtx(mHost, "Cockpit"));
    mtx.getZDir(mFront);
}

void MogucchiShooter::faceToMario() {
    if (!MR::isPlayerHidden() && !MR::isStageStateScenarioOpeningCamera() && !MR::isDemoActive()) {
        TPos3f mtx;
        JMath::gekko_ps_copy12(&mtx, mFixedPos->_1C);
        MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), 1.0f);
        TVec3f front;
        mtx.getZDir(front);
        MR::clampVecAngleDeg(&mFront, front, 30.0f);
    }
}
