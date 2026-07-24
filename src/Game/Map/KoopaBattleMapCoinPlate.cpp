#include "Game/Map/KoopaBattleMapCoinPlate.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sSideSpeed = 30.0f;
    static const f32 sHopSpeed = 30.0f;
};  // namespace

KoopaBattleMapCoinPlate::~KoopaBattleMapCoinPlate() {
}

KoopaBattleMapCoinPlate::KoopaBattleMapCoinPlate(const char* pName) : LiveActor(pName) {
    mBaseMtx.identity();
}

void KoopaBattleMapCoinPlate::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}

void KoopaBattleMapCoinPlate::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::calcGravity(this);

    MR::makeMtxUpNoSupportPos(&mBaseMtx, -mGravity, mPosition);

    initModelManagerWithAnm("KoopaPlateCoin", nullptr, false);

    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::initCollisionParts(this, "KoopaPlateCoin", MR::addHitSensor(this, "Attack", ATYPE_KOOPA_COIN_PLATE, 8, 250.0f, TVec3f(0.0f, -150.0f, 0.0f)),
                           mBaseMtx);
    initEffectKeeper(1, nullptr, false);

    MR::addEffectHitNormal(this, "Hit");
    MR::setEffectBaseScale(this, "Hit", 2.5f);

    initSound(4, false);

    MR::setClippingFarMax(this);

    MR::declareCoin(this, 1);

    makeActorAppeared();
}

bool KoopaBattleMapCoinPlate::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg != ACTMES_KOOPA_HIP_DROP_ATTACK) {
        return false;
    }

    MR::emitEffectHit(this, MR::getSensorHost(pSender)->mPosition, "Hit");

    MR::startSound(this, "SE_OJ_KOOPA_PLATE_COIN_BRK");

    MR::calcGravity(this);

    TVec3f randomVec;
    MR::calcRandomVec(&randomVec, -::sSideSpeed, ::sSideSpeed);
    MR::vecKillElement(randomVec, mGravity, &randomVec);

    randomVec.add(mGravity * -::sHopSpeed);
    MR::appearCoinToVelocity(this, mPosition, randomVec, 1);

    MR::emitEffect(this, "Break");

    kill();

    return true;
}
