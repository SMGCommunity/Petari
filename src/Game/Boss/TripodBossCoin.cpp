#include "Game/Boss/TripodBossCoin.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/MapObj/Coin.hpp"

namespace NrvTripodBossCoin {
    NEW_NERVE(TripodBossCoinNrvNonActive, TripodBossCoin, NonActive);
    NEW_NERVE(TripodBossCoinNrvActive, TripodBossCoin, Active);
    NEW_NERVE(TripodBossCoinNrvEnd, TripodBossCoin, End);
};  // namespace NrvTripodBossCoin

TripodBossCoin::TripodBossCoin(const char* pName) : NameObj(pName), mCoin(nullptr), mSpine(nullptr), _44(-1) {
    _14.identity();
}

void TripodBossCoin::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoMatrixFromRT(rIter, &_14);
    MR::connectToSceneMapObjDecorationMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &_44);
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
    TPos3f pos;
    pos.setInline(_14);
    MR::concatTripodBossAttachJointMatrix(&pos, _44);
    TVec3f coinPos;
    f32 z = pos.mMtx[2][3];
    f32 y = pos.mMtx[1][3];
    f32 x = pos.mMtx[0][3];
    coinPos.set< f32 >(x, y, z);
    TVec3f* newPos = &mCoin->mPosition;
    newPos->x = coinPos.x;
    newPos->y = coinPos.y;
    newPos->z = coinPos.z;

    if (mSpine->mStep == 0) {
        mCoin->appearControlPose();
    }

    if (MR::isBrokenTripodBoss() || MR::isDead(mCoin)) {
        mCoin->kill();
        mSpine->setNerve(&NrvTripodBossCoin::TripodBossCoinNrvEnd::sInstance);
    }
}

void TripodBossCoin::exeEnd() {}
