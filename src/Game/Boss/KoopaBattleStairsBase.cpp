#include "Game/Boss/KoopaBattleStairsBase.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"

KoopaBattleStairsBase::KoopaBattleStairsBase(Koopa* pKoopa) : NerveExecutor("クッパ戦（階段の戦い）"), mKoopa(pKoopa), mStairsGroup() {
    mStairsGroup = new LiveActorGroup("階段保持", 128);
    mStairsGroup->initWithoutIter();
}

void KoopaBattleStairsBase::calcFireStartPos(TVec3f* pVec, const char* pDemoName) const {
    if (pDemoName) {
        KoopaFunction::setKoopaPos(mKoopa, pDemoName);
    }

    MR::copyJointPos(mKoopa, "Tongue2", pVec);
}

void KoopaBattleStairsBase::update() {
    updateNerve();
}

void KoopaBattleStairsBase::end() {
    for (int idx = 0; idx < mStairsGroup->mObjectCount; idx++) {
        mStairsGroup->getActor(idx)->makeActorDead();
    }

    KoopaFunction::killKoopaFireStairsAll(mKoopa);
}

bool KoopaBattleStairsBase::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver);
}

s32 KoopaBattleStairsBase::calcFireAttackStep(KoopaBattleMapStair* pBattleMapStair, f32 myFloat, s32 myInt, const TVec3f& rVec) const {
    TVec3f zeroVec = TVec3f(0.0f, 0.0f, 0.0f);

    s32 target = static_cast< s32 >(90.0f / myFloat) - static_cast< s32 >(pBattleMapStair->calcAndSetTargetPos(&zeroVec, rVec) / myFloat);
    pBattleMapStair->_A8 = target;

    return pBattleMapStair->mFireTimer - target - myInt;
}
