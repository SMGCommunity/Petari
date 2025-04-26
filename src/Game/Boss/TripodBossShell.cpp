#include "Game/Boss/TripodBossShell.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvTripodBossShell {
    NEW_NERVE(TripodBossShellNrvNonActive, TripodBossShell, NonActive);
    NEW_NERVE(TripodBossShellNrvWait, TripodBossShell, Wait);
    NEW_NERVE(TripodBossShellNrvBreak, TripodBossShell, Break);
};

TripodBossShell::~TripodBossShell() {

}

TripodBossShell::TripodBossShell(const char *pName) : TripodBossFixPartsBase(pName) {
    mBreakModel = nullptr;
}

void TripodBossShell::init(const JMapInfoIter &rIter) {
    TripodBossFixPartsBase::init(rIter);
    initModelManagerWithAnm("TripodBossShell", nullptr, false);
    MR::connectToScene(this, 0x23, 0xB, 0x1F, -1);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 0x10, 900.0f, TVec3f(0.0, 300.0f, 0.0f));
    MR::addHitSensor(this, "killer_terget", 0x52, 8, 900.0f * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "TripodBossShell", getSensor("killer_terget"), nullptr);
    initSound(4, false);
    mBreakModel = MR::createModelObjMapObjStrongLight("壊れモデル", "TripodBossShellBreak", getBaseMtx());
    mBreakModel->initWithoutIter();
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
    MR::addTripodBossPartsMovement(mBreakModel);
    initNerve(&NrvTripodBossShell::TripodBossShellNrvNonActive::sInstance);
    MR::invalidateCollisionParts(this);
    getSensor("body")->invalidate();
    MR::useStageSwitchWriteDead(this, rIter);
    makeActorDead();
}

void TripodBossShell::kill() {
    LiveActor::kill();
    mBreakModel->kill();
}

bool TripodBossShell::receiveMsgEnemyAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (a3 == getSensor("killer_terget") && !isNerve(&NrvTripodBossShell::TripodBossShellNrvBreak::sInstance)) {
        setNerve(&NrvTripodBossShell::TripodBossShellNrvBreak::sInstance);
        return true;
    }

    return false;
}

void TripodBossShell::activateTripodBoss() {
    if (isNerve(&NrvTripodBossShell::TripodBossShellNrvNonActive::sInstance)) {
        MR::onCalcAnim(this);
        MR::validateCollisionParts(this);
        setNerve(&NrvTripodBossShell::TripodBossShellNrvWait::sInstance);
    }
}

// https://decomp.me/scratch/okSIK
void TripodBossShell::exeBreak() {
    updateTripodMatrix();
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_TRIPOD_CORE_BREAK", -1, -1);
        MR::offEntryDrawBuffer(this);
        getSensor("killer_terget")->invalidate();
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);
        mBreakModel->appear();
        MR::requestMovementOn(mBreakModel);
        MR::startBck(mBreakModel, "Break", nullptr);
        TVec3f v16(*MR::getPlayerPos());
        JGeometry::subInternal(&v16.x, &mPosition.x, &v16.x);
        f32 v11;
        MR::separateScalarAndDirection(&v11, &v16, v16);
        if (v11 <= 1000.0f) {
            TVec3f v15;
            MR::calcUpVec(&v15, this);
            if (v16.dot(v15) > 0.0f) {
                TVec3f v12(v16);
                v12.x *= 40.0f;
                v12.y *= 40.0f;
                v12.z *= 40.0f;
                MR::forceJumpPlayer(v12);
            }
        }
    }

    if (MR::isStep(this, 4)) {
        MR::stopScene(30);
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        kill();
    }
}

void TripodBossShell::exeWait() {
    updateTripodMatrix();
}

void TripodBossShell::exeNonActive() {
    
}
