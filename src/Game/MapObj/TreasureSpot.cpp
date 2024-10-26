#include "Game/MapObj/TreasureSpot.hpp"
#include "Game/Util.hpp"

TreasureSpot::TreasureSpot(const char *pName) : MapObjActor(pName) {
    mIsCoinFlower = false;
}

void TreasureSpot::init(const JMapInfoIter &rIter) {
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
    MR::declareCoin(this, 1);
}

void TreasureSpot::exeSpout() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffectAll(this);
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);
        MR::appearCoinPopToDirection(this, mPosition, upVec, 1);

        if (mIsCoinFlower) {
            MR::startBck(this, "Bloom", 0);
            MR::startSound(this, "SE_OJ_COIN_FLOWER_BLOOM", -1, -1);
        }
        else {
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
    if (!isNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance) && !isNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance)) {
        switchEmitGlow();
    }
}

bool TreasureSpot::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance)) {
        return 0;
    }

    if (isNerve(&NrvTreasureSpot::TreasureSpotNrvEnd::sInstance)) {
        return 0;
    }

    if (!MR::isNearPlayerAnyTime(this, 2000.0f)) {
        return 0;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return 1;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance);
        return 1;
    }

    return 0;
}

void TreasureSpot::switchEmitGlow() {
    if (!MR::isNearPlayerAnyTime(this, 2000.0f)) {
        MR::deleteEffect(this, "Glow");
    }
    else {
        if (MR::isNearPlayerAnyTime(this, 2000.0f)) {
            if (!MR::isEffectValid(this, "Glow")) {
                MR::emitEffect(this, "Glow");
            }
        }
    }
}

TreasureSpot::~TreasureSpot() {

}

namespace NrvTreasureSpot {
    INIT_NERVE(TreasureSpotNrvWait);
    INIT_NERVE(TreasureSpotNrvEnd);
    INIT_NERVE(TreasureSpotNrvSpout);

    void TreasureSpotNrvSpout::execute(Spine *pSpine) const {
        TreasureSpot* spot = reinterpret_cast<TreasureSpot*>(pSpine->mExecutor);
        spot->exeSpout();
    }

    void TreasureSpotNrvEnd::execute(Spine *pSpine) const {

    }

    void TreasureSpotNrvWait::execute(Spine *pSpine) const {
        
    }
};