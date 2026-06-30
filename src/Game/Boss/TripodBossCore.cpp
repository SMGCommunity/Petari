#include "Game/Boss/TripodBossCore.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvTripodBossCore {
    NEW_NERVE(TripodBossCoreNrvNonActive, TripodBossCore, NonActive);
    NEW_NERVE(TripodBossCoreNrvWait, TripodBossCore, Wait);
    NEW_NERVE(TripodBossCoreNrvDamageDemo, TripodBossCore, DamageDemo);
    NEW_NERVE(TripodBossCoreNrvWarning, TripodBossCore, Warning);
    NEW_NERVE(TripodBossCoreNrvBreak, TripodBossCore, Break);
};  // namespace NrvTripodBossCore

TripodBossCore::TripodBossCore(const char* pName) : TripodBossFixPartsBase(pName) {
    mBreakModel = nullptr;
    mBloomModel = nullptr;
}

void TripodBossCore::init(const JMapInfoIter& rIter) {
    TripodBossFixPartsBase::init(rIter);
    initModelManagerWithAnm("TripodBossCore", nullptr, false);
    MR::connectToScene(this, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_TripodBoss, -1);
    initClippingSphere();
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_BREAKABLE_CAGE, 8, 300.0f * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "TripodBossCore", getSensor("body"), nullptr);
    initSound(4, false);
    mBreakModel = MR::createModelObjMapObjStrongLight("壊れモデル", "TripodBossCoreBreak", getBaseMtx());
    mBreakModel->initWithoutIter();
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
    MR::addTripodBossPartsMovement(mBreakModel);
    mBloomModel = MR::createModelObjBloomModel("ブルームモデル", "TripodBossCoreBloom", getBaseMtx());
    mBloomModel->initWithoutIter();
    MR::invalidateClipping(mBloomModel);
    mBloomModel->makeActorDead();
    MR::addTripodBossPartsMovement(mBloomModel);
    initNerve(&NrvTripodBossCore::TripodBossCoreNrvNonActive::sInstance);
    initEffectKeeper(0, "TripodBossCore", false);
    MR::setEffectHostMtx(this, "BlackSmoke", getBaseMtx());
    MR::useStageSwitchWriteDead(this, rIter);
    MR::invalidateCollisionParts(this);
    makeActorDead();
}

void TripodBossCore::kill() {
    LiveActor::kill();
    mBreakModel->kill();
}

bool TripodBossCore::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvTripodBossCore::TripodBossCoreNrvBreak::sInstance)) {
        setNerve(&NrvTripodBossCore::TripodBossCoreNrvBreak::sInstance);
        return true;
    }

    return false;
}

void TripodBossCore::activateTripodBoss() {
    if (isNerve(&NrvTripodBossCore::TripodBossCoreNrvNonActive::sInstance)) {
        MR::onCalcAnim(this);
        MR::validateCollisionParts(this);
        setNerve(&NrvTripodBossCore::TripodBossCoreNrvWait::sInstance);
        mBloomModel->makeActorAppeared();
    }
}

void TripodBossCore::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "Wait");
        MR::tryStartAllAnim(mBloomModel, "Wait");
    }

    updateTripodMatrix();
    MR::requestPointLight(this, mPosition, (GXColor){0x96, 0x96, 0x96, 0xFF}, 1.0f, -1);
    if (MR::isDamageDemoTripodBoss()) {
        setNerve(&NrvTripodBossCore::TripodBossCoreNrvDamageDemo::sInstance);
    }
}

void TripodBossCore::exeDamageDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "2ndDemo");
        MR::tryStartAllAnim(mBloomModel, "2ndDemo");
    }

    MR::requestPointLight(this, mPosition, (GXColor){0xFF, 0x96, 0x96, 0xFF}, 1.0f, -1);
    if (MR::isDamageDemoTripodBoss()) {
        setNerve(&NrvTripodBossCore::TripodBossCoreNrvWarning::sInstance);
    }
}

void TripodBossCore::exeWarning() {
    updateTripodMatrix();
    MR::requestPointLight(this, mPosition, (GXColor){0xFF, 0x96, 0x96, 0xFF}, 1.0f, -1);
}

void TripodBossCore::exeBreak() {
    updateTripodMatrix();
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_TRIPOD_CORE_BREAK");
        MR::emitEffect(this, "BlackSmoke");
        MR::hideModelAndOnCalcAnim(this);
        getSensor("body")->invalidate();
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);
        mBloomModel->kill();
        mBreakModel->appear();
        MR::startBck(mBreakModel, "Break", nullptr);
        MR::requestMovementOn(mBreakModel);
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(16);
        MR::shakeCameraNormal();
    }

    if (MR::isEndBreakDownDemoTripodBoss()) {
        kill();
        MR::deleteEffect(this, "BlackSmoke");
    }
}

void TripodBossCore::exeNonActive() {
}

TripodBossCore::~TripodBossCore() {
}
