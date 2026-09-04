#include "Game/Boss/TripodBossShell.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sHitSensorRadius = 900.0f;
    static const s32 sBreakStopStep = 4;
    static const s32 sBreakStopFrame = 30;
    static const f32 sBreakStopDistance = 1000.0f;
    // static const f32 sStartEventForceJumpRange = _;
    static const f32 sStartEventForceJumpPlayerPower = 40.0f;
};  // namespace

namespace NrvTripodBossShell {
    NEW_NERVE(TripodBossShellNrvNonActive, TripodBossShell, NonActive);
    NEW_NERVE(TripodBossShellNrvWait, TripodBossShell, Wait);
    NEW_NERVE(TripodBossShellNrvBreak, TripodBossShell, Break);
};  // namespace NrvTripodBossShell

TripodBossShell::~TripodBossShell() {
}

TripodBossShell::TripodBossShell(const char* pName) : TripodBossFixPartsBase(pName), mBreakModel() {
}

void TripodBossShell::init(const JMapInfoIter& rIter) {
    TripodBossFixPartsBase::init(rIter);
    initModelManagerWithAnm("TripodBossShell", nullptr, false);
    MR::connectToScene(this, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_TripodBoss, -1);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 16, ::sHitSensorRadius, TVec3f(0.0f, 300.0f, 0.0f));
    MR::addHitSensor(this, "killer_terget", ATYPE_BREAKABLE_CAGE, 8, ::sHitSensorRadius * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
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

bool TripodBossShell::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("killer_terget") && !isNerve(&NrvTripodBossShell::TripodBossShellNrvBreak::sInstance)) {
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

void TripodBossShell::exeNonActive() {
}

void TripodBossShell::exeWait() {
    updateTripodMatrix();
}

void TripodBossShell::exeBreak() {
    updateTripodMatrix();

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_TRIPOD_CORE_BREAK");
        MR::offEntryDrawBuffer(this);
        getSensor("killer_terget")->invalidate();
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);
        mBreakModel->appear();
        MR::requestMovementOn(mBreakModel);
        MR::startBck(mBreakModel, "Break", nullptr);

        f32 playerDistance;
        TVec3f toPlayerDir = *MR::getPlayerPos() - mPosition;
        MR::separateScalarAndDirection(&playerDistance, &toPlayerDir, toPlayerDir);

        if (playerDistance <= ::sBreakStopDistance) {
            TVec3f up;
            MR::calcUpVec(&up, this);

            if (toPlayerDir.dot(up) > 0.0f) {
                MR::forceJumpPlayer(toPlayerDir * ::sStartEventForceJumpPlayerPower);
            }
        }
    }

    if (MR::isStep(this, ::sBreakStopStep)) {
        MR::stopScene(::sBreakStopFrame);

        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        kill();
    }
}
