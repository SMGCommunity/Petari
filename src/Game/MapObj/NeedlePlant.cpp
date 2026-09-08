#include "Game/MapObj/NeedlePlant.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    // static const f32 sBreakSensorSize = _;
    static const f32 sAttackSensorSize = 70.0f;
};  // namespace

namespace NrvNeedlePlant {
    NEW_NERVE(NeedlePlantNrvWait, NeedlePlant, Wait);
    NEW_NERVE(NeedlePlantNrvShake, NeedlePlant, Shake);
};  // namespace NrvNeedlePlant

NeedlePlant::NeedlePlant(const char* pName) : MapObjActor(pName), mObjArg0(-1) {
}

void NeedlePlant::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupAffectedScale();
    info.setupHitSensor();
    info.setupHitSensorParam(4, 180.0f, TVec3f(0.0f, mScale.x * 150.0f, 0.0f));
    info.setupShadow(nullptr);
    info.setupNerve(&NrvNeedlePlant::NeedlePlantNrvWait::sInstance);
    initialize(rIter, info);
    initEffectKeeper(1, "NeedlePlant", false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, 0);
    MR::initStarPointerTarget(this, mScale.x * 150.0f, TVec3f(0.0f, mScale.x * 150.0f, 0.0f));
    MR::getJMapInfoArg0NoInit(rIter, &mObjArg0);

    if (mObjArg0 == -1) {
        MR::declareCoin(this, 1);
    } else if (mObjArg0 == 0) {
        MR::declareStarPiece(this, 3);
    }
}

void NeedlePlant::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, mObjectName);
    }
}

void NeedlePlant::exeShake() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        MR::startSound(this, "SE_OJ_LEAVES_SWING");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvNeedlePlant::NeedlePlantNrvWait::sInstance);
    }
}

void NeedlePlant::kill() {
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_NEEDLE_PLANT_BREAK");

    if (mObjArg0 == 0) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
    } else if (mObjArg0 == -1) {
        MR::appearCoinPop(this, mPosition, 1);
    }

    MapObjActor::kill();
}

void NeedlePlant::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    // TODO: Possible inline. https://decomp.me/scratch/zwlUm
    if (MR::calcDistance(pSender, pReceiver, nullptr) > pReceiver->mRadius + mScale.x * ::sAttackSensorSize) {
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
        setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);
    } else if (MR::isSensorPlayerOrRide(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        f32 radius = getSensor(nullptr)->mRadius;
        getSensor(nullptr)->mRadius = ::sAttackSensorSize * mScale.x;
        MR::sendMsgPush(pReceiver, pSender);
        getSensor(nullptr)->mRadius = radius;
    }
}

bool NeedlePlant::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance)) {
        return false;
    }

    if (!MR::isSensorEnemy(pSender)) {
        return false;
    }

    setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);

    return true;
}

bool NeedlePlant::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);

        return true;
    }

    if (MR::isMsgInvincibleAttack(msg)) {
        kill();

        return true;
    }

    return false;
}

bool NeedlePlant::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    kill();

    return true;
}

void NeedlePlant::connectToScene(const MapObjActorInitInfo&) {
    MR::connectToSceneNoShadowedMapObj(this);
}
