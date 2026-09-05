#include "Game/Boss/TripodBossCoin.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/Coin.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTripodBossCoin {
    NEW_NERVE(TripodBossCoinNrvNonActive, TripodBossCoin, NonActive);
    NEW_NERVE(TripodBossCoinNrvActive, TripodBossCoin, Active);
    NEW_NERVE(TripodBossCoinNrvEnd, TripodBossCoin, End);
};  // namespace NrvTripodBossCoin

TripodBossCoin::TripodBossCoin(const char* pName) : NameObj(pName), mCoin(), mSpine(), mJointID(-1) {
    _14.identity();
}

void TripodBossCoin::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoMatrixFromRT(rIter, &_14);
    MR::connectToSceneMapObjDecorationMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mJointID);

    mCoin = static_cast< Coin* >(MR::createCoin(this, "コイン(三脚ボス用)"));
    mCoin->initWithoutIter();

    mSpine = new Spine(this, &NrvTripodBossCoin::TripodBossCoinNrvNonActive::sInstance);

    MR::addTripodBossPartsMovement(this);
}

void TripodBossCoin::movement() {
    mSpine->update();
}

void TripodBossCoin::exeNonActive() {
    if (MR::isStartedTripodBoss() && !MR::isStartDemoTripodBoss()) {
        mSpine->setNerve(&NrvTripodBossCoin::TripodBossCoinNrvActive::sInstance);
    }
}

void TripodBossCoin::exeActive() {
    TPos3f mtx;
    mtx.set(_14);

    MR::concatTripodBossAttachJointMatrix(&mtx, mJointID);

    TVec3f coinTrans;
    mtx.getTrans(coinTrans);

    mCoin->mPosition.set(coinTrans);

    if (mSpine->mStep == 0) {
        mCoin->appearControlPose();
    }

    if (MR::isBrokenTripodBoss() || MR::isDead(mCoin)) {
        mCoin->kill();
        mSpine->setNerve(&NrvTripodBossCoin::TripodBossCoinNrvEnd::sInstance);
    }
}

void TripodBossCoin::exeEnd() {
}
