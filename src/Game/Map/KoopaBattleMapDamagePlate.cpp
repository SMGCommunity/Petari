#include "Game/Map/KoopaBattleMapDamagePlate.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void FORCE_MATCH_KOOPABATTLEMAPDAMAGEPLATE_SDATA2() {
    (void)0.0f;
    (void)3.0f;
    (void)-150.0f;
    (void)250.0f;
}

namespace {
    const static s32 sBreakStep = 180;
    const static s32 sBreakStepToAttack = 30;
};  // namespace

namespace NrvKoopaBattleMapDamagePlate {
    NEW_NERVE(KoopaBattleMapDamagePlateNrvWait, KoopaBattleMapDamagePlate, Wait);
    NEW_NERVE(KoopaBattleMapDamagePlateNrvBreak, KoopaBattleMapDamagePlate, Break);
    NEW_NERVE(KoopaBattleMapDamagePlateNrvRecover, KoopaBattleMapDamagePlate, Recover);
};  // namespace NrvKoopaBattleMapDamagePlate

KoopaBattleMapDamagePlate::KoopaBattleMapDamagePlate(const char* pName) : LiveActor(pName) {
    mBaseMtx.identity();
}

void KoopaBattleMapDamagePlate::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::calcGravity(this);

    MR::makeMtxUpNoSupportPos(&mBaseMtx, -mGravity, mPosition);

    initModelManagerWithAnm("KoopaPlateDamage", nullptr, false);

    MR::startBtk(this, "KoopaPlateDamage");
    MR::startBrk(this, "Revival");
    MR::setBrkFrame(this, MR::getBrkFrameMax(this));

    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addHitSensor(this, "Attack", ATYPE_KOOPA_DAMAGE_PLATE, 8, 250.0f, TVec3f(0.0f, -150.0f, 0.0f));
    MR::initCollisionParts(this, "KoopaPlateDamage", getSensor("Attack"), mBaseMtx);
    initEffectKeeper(1, nullptr, false);

    MR::addEffectHitNormal(this, "Hit");
    MR::setEffectBaseScale(this, "Hit", 3.0f);

    initSound(4, false);
    initNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvWait));

    MR::setClippingFarMax(this);

    MR::registerDemoSimpleCastAll(this);

    makeActorAppeared();
}

void KoopaBattleMapDamagePlate::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Break");
        MR::emitEffect(this, "Break");
        MR::emitEffect(this, "LavaBubble");
    }

    if (MR::isStep(this, ::sBreakStep)) {
        setNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvRecover));
    }
}

void KoopaBattleMapDamagePlate::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Revival");

        MR::emitEffect(this, "Recover");
        MR::deleteEffect(this, "LavaBubble");

        MR::startSound(this, "SE_OJ_KOOPA_PLATE_CLOSE");
    }

    if (MR::isBrkStopped(this)) {
        MR::validateClipping(this);
        MR::validateCollisionParts(this);

        setNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvWait));
    }
}

void KoopaBattleMapDamagePlate::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}

void KoopaBattleMapDamagePlate::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvBreak)) &&
        MR::isGreaterStep(this, ::sBreakStepToAttack) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        return;
    }
}

bool KoopaBattleMapDamagePlate::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_KOOPA_PLATE_DAMAGE) {
        return true;
    }

    if (!isNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvWait)) || msg != ACTMES_KOOPA_HIP_DROP_ATTACK) {
        return false;
    }

    MR::emitEffectHit(this, MR::getSensorHost(pSender)->mPosition, "Hit");

    MR::invalidateClipping(this);
    MR::invalidateCollisionParts(this);
    
    setNerve(GET_NERVE(KoopaBattleMapDamagePlate, KoopaBattleMapDamagePlateNrvBreak));
    
    return true;
}

KoopaBattleMapDamagePlate::~KoopaBattleMapDamagePlate() {
}

void KoopaBattleMapDamagePlate::exeWait() {
}
