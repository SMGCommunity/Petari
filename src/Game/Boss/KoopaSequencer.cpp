#include "Game/Boss/KoopaSequencer.hpp"
#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"

KoopaSequencer::KoopaSequencer(const char* pName, s32 vs)
    : NerveExecutor(pName), mKoopa(), mPowerUp(), _10(), mJumpToPlanet(), mLv(1), mVs(vs), mIsAngry() {
}

KoopaSequencer::~KoopaSequencer() {
}

void KoopaSequencer::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
    mKoopa = pKoopa;

    mPowerUp = new KoopaDemoPowerUp(mKoopa);
    mPowerUp->init();

    mJumpToPlanet = new KoopaDemoJumpToPlanet(mKoopa);
    mJumpToPlanet->init();
}

void KoopaSequencer::update() {
    updateNerve();
}

void KoopaSequencer::calcAndSetBaseMtx() {
    MR::calcAndSetBaseMtxFromGravityAndZAxis(mKoopa, mKoopa->mFront);
}

void KoopaSequencer::startBattleLevel2() {
    mLv = 2;
    mIsAngry = false;
}

void KoopaSequencer::startBattleLevel3() {
    mLv = 3;
    mIsAngry = false;
}
