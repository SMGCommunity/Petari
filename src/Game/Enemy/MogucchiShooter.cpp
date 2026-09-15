#include "Game/Enemy/MogucchiShooter.hpp"
#include "Game/Enemy/WaterBazooka.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void MogucchiShooter_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace {
    static const s32 sStepForLaugh = 120;
    static const s32 sStepToAngerBlur = 40;
    static const s32 sAngerBlurTime = 60;
    volatile static const f32 sAngerBlurOffset = 100.0f;
    static const s32 sExplosionBlurTime = 50;
    volatile static const f32 sExplosionBlurOffset = 100.0f;
    static const f32 sTurnRate = 1.0f;
    static const f32 sLimitAngleOfTurn = 30.0f;
};  // namespace

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
};  // namespace NrvMogucchiShooter

MogucchiShooter::MogucchiShooter(LiveActor* pActor, const char* pName)
    : PartsModel(pActor, pName, "MogucchiShooter", nullptr, MR::DrawBufferType_Enemy, false), mFront(0.0f, 0.0f, 0.0f) {
}

void MogucchiShooter::init(const JMapInfoIter& rIter) {
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(1, false);
    MR::calcFrontVec(&mFront, this);
    MR::createCenterScreenBlur();
    MR::startBrk(this, "Normal");
    initNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    makeActorAppeared();
}

void MogucchiShooter::panicDeath() {
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvDeathPanic));
}

void MogucchiShooter::explosion() {
    resetDirection();
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvExplosion));
}

void MogucchiShooter::hitShock() {
    resetDirection();
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvHitShock));
}

void MogucchiShooter::anger() {
    resetDirection();
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvAnger));
}

void MogucchiShooter::stormStart() {
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvStormStart));
}

void MogucchiShooter::storm() {
    setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvStorm));
}

bool MogucchiShooter::isLaughed() const {
    return isNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvLaugh));
}

void MogucchiShooter::exeWait() {
    WaterBazooka* bazooka = static_cast< WaterBazooka* >(mHost);

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
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvLaugh));
        return;
    }

    if (bazooka->isPanic()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvShock));
        return;
    }

    if (bazooka->isTired()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvTire));
        return;
    }

    if (bazooka->isFirstShoot()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvShot));
    }
}

void MogucchiShooter::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shot");
        return;
    }

    if (MR::isPlayerInBind()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvLaugh));
        return;
    }

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    }
}

void MogucchiShooter::exeTire() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Tire");
    }

    WaterBazooka* bazooka = static_cast< WaterBazooka* >(mHost);

    if (bazooka->isPanic()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvShock));
    } else if (!bazooka->isTired()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
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
        MR::startCenterScreenBlur(::sExplosionBlurTime, ::sExplosionBlurOffset, 80, 5, 30);
    }

    if (MR::isActionEnd(this)) {
        kill();
    }
}

void MogucchiShooter::exeLaugh() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Laugh");
    }

    if (MR::isStep(this, ::sStepForLaugh)) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    }
}

void MogucchiShooter::exeShock() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shock");
    }

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvPanic));
    }
}

void MogucchiShooter::exePanic() {
    WaterBazooka* bazooka = static_cast< WaterBazooka* >(mHost);

    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Panic");

        if (bazooka->isBazookaLifeOut()) {
            MR::setBckRate(this, 2.0f);
        }
    }

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PANIC");
    faceToMario();

    if (!bazooka->isPanic() && MR::isOnGroundPlayer() && !bazooka->isBazookaLifeOut()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    }
}

void MogucchiShooter::exeHitShock() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shock");
    }

    faceToMario();

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvHitPanic));
    }
}

void MogucchiShooter::exeHitPanic() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Panic");
    }

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PANIC");

    if (!MR::isDemoActive()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    }
}

void MogucchiShooter::exeAnger() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Angry");
    }

    if (MR::isStep(this, ::sStepToAngerBlur)) {
        MR::startCenterScreenBlur(::sAngerBlurTime, ::sAngerBlurOffset, 80, 5, 30);
    }

    faceToMario();

    if (!MR::isDemoActive()) {
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
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
        setNerve(GET_NERVE(MogucchiShooter, MogucchiShooterNrvWait));
    }
}

void MogucchiShooter::calcAndSetBaseMtx() {
    if (!mIsCalcOwnMtx) {
        PartsModel::calcAndSetBaseMtx();
        return;
    }

    TPos3f posMtx;
    TPos3f copyMtx;
    posMtx.identity();
    copyMtx.setInline(mFixedPosition->mMtx);

    TVec3f up;
    copyMtx.getYDir(up);

    mFixedPosition->mMtx.getTrans(mPosition);
    MR::makeMtxUpFrontPos(&posMtx, up, mFront, mPosition);
    MR::setBaseTRMtx(this, posMtx);
}

void MogucchiShooter::resetDirection() {
    TPos3f mtx(MR::getJointMtx(mHost, "Cockpit"));
    mtx.getZDir(mFront);
}

void MogucchiShooter::faceToMario() {
    if (MR::isPlayerHidden()) {
        return;
    }

    if (MR::isStageStateScenarioOpeningCamera()) {
        return;
    }

    if (MR::isDemoActive()) {
        return;
    }

    TPos3f mtx;
    mtx.setInline(mFixedPosition->mMtx);
    MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), ::sTurnRate);
    TVec3f front;
    mtx.getZDir(front);
    MR::clampVecAngleDeg(&mFront, front, ::sLimitAngleOfTurn);
}
