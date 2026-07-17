#include "Game/MapObj/TreasureSpot.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sNumCoin = 1;
    static const f32 sEnableSpoutLength = 2000.0f;
};  // namespace

namespace NrvTreasureSpot {
    NEW_NERVE(TreasureSpotNrvWait, TreasureSpot, Wait);
    NEW_NERVE(TreasureSpotNrvEnd, TreasureSpot, End);
    NEW_NERVE(TreasureSpotNrvSpout, TreasureSpot, Spout);
};  // namespace NrvTreasureSpot

TreasureSpot::TreasureSpot(const char* pName) : MapObjActor(pName), mIsCoinFlower() {
}

void TreasureSpot::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    info.setupHitSensorParam(4, 80.0f, TVec3f(0.0f, 3.0f, 0.0f));
    info.setupNerve(&NrvTreasureSpot::TreasureSpotNrvWait::sInstance);
    info.setupSound(4);
    MapObjActor::initialize(rIter, info);
    mIsCoinFlower = isObjectName("CoinFlower");
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::declareCoin(this, ::sNumCoin);
}

void TreasureSpot::exeWait() {
}

void TreasureSpot::exeEnd() {
}

void TreasureSpot::exeSpout() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffectAll(this);
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);
        MR::appearCoinPopToDirection(this, mPosition, upVec, ::sNumCoin);

        if (mIsCoinFlower) {
            MR::startBck(this, "Bloom", nullptr);
            MR::startSound(this, "SE_OJ_COIN_FLOWER_BLOOM");
        } else {
            setNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance);
            kill();
            return;
        }
    }

    if (mIsCoinFlower) {
        if (MR::isBckStopped(this)) {
            setNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance);
        }
    }
}

void TreasureSpot::control() {
    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance)) {
        return;
    }

    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance)) {
        return;
    }

    switchEmitGlow();
}

bool TreasureSpot::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance)) {
        return false;
    }

    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance)) {
        return false;
    }

    if (!MR::isNearPlayerAnyTime(this, ::sEnableSpoutLength)) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance);
        return true;
    }

    return false;
}

void TreasureSpot::switchEmitGlow() {
    if (!MR::isNearPlayerAnyTime(this, ::sEnableSpoutLength)) {
        MR::deleteEffect(this, "Glow");
    } else if (MR::isNearPlayerAnyTime(this, ::sEnableSpoutLength)) {
        if (!MR::isEffectValid(this, "Glow")) {
            MR::emitEffect(this, "Glow");
        }
    }
}
