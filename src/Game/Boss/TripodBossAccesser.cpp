#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static s32 cJMapBoneIDToBoneIndexTable[] = {0,  1, 2, 7,  8,  9, 14, 15, 16, 21, 3,  4,  0,  1,  5,  6,  2,  -1, -1, -1, 10,
                                                11, 7, 8, 12, 13, 9, -1, -1, -1, 17, 18, 14, 15, 19, 20, 16, -1, -1, -1, 21};
    static s32 cJMapBoneIDToBoneIndexTableSize = ARRAY_SIZE(cJMapBoneIDToBoneIndexTable);

    s32 convertBoneIDToIndex(s32 id) NO_INLINE {
        if (id < 0 || cJMapBoneIDToBoneIndexTableSize <= id) {
            return -1;
        }

        return cJMapBoneIDToBoneIndexTable[id];
    }
};  // namespace

TripodBossAccesser::TripodBossAccesser(const char* pName) : NameObj(pName), mBoss(), mPartsNum() {
}

TripodBoss* TripodBossAccesser::getTriPodBoss() const {
    return mBoss;
}

void TripodBossAccesser::setTriPodBoss(TripodBoss* pBoss) {
    mBoss = pBoss;
}

void TripodBossAccesser::addTripodBossParts(LiveActor* pActor) {
    mParts[mPartsNum] = pActor;
    mPartsNum++;
}

void TripodBossAccesser::showTripodBossParts() {
    for (s32 i = 0; i < mPartsNum; i++) {
        if (MR::isDead(mParts[i])) {
            mParts[i]->makeActorAppeared();
        }
    }
}

void TripodBossAccesser::hideTripodBossParts() {
    for (s32 i = 0; i < mPartsNum; i++) {
        if (!MR::isDead(mParts[i])) {
            mParts[i]->makeActorDead();
        }
    }
}

void TripodBossAccesser::activeTripodBossParts() {
    for (s32 i = 0; i < mPartsNum; i++) {
        if (MR::isDead(mParts[i])) {
            mParts[i]->makeActorAppeared();
        }

        MR::sendSimpleMsgToActor(ACTMES_TRIPODBOSS_STARTED, mParts[i]);
    }
}

bool TripodBossAccesser::isTripoddBossParts(const NameObj* pObj) const {
    if (pObj == this || pObj == mBoss) {
        return true;
    }

    for (s32 i = 0; i < mPartsNum; i++) {
        if (mParts[i] == pObj) {
            return true;
        }
    }

    return false;
}

TripodBossAccesser* TripodBossAccesser::createSceneObj() {
    return static_cast< TripodBossAccesser* >(MR::createSceneObj(SceneObj_TripodBossAccesser));
}

TripodBossAccesser* MR::getTripodBossAccesser() {
    return MR::getSceneObj< TripodBossAccesser >(SceneObj_TripodBossAccesser);
}

void MR::addTripodBossParts(LiveActor* pActor) {
    TripodBossAccesser::createSceneObj()->addTripodBossParts(pActor);
    MR::joinToMovementOnOffGroup("三脚ボス部品", pActor, 256);
}

void MR::addTripodBossPartsMovement(NameObj* pObj) {
    MR::joinToMovementOnOffGroup("三脚ボス部品", pObj, 256);
}

bool MR::isTripoddBossParts(const NameObj* pObj) {
    return MR::getSceneObj< TripodBossAccesser >(SceneObj_TripodBossAccesser)->isTripoddBossParts(pObj);
}

void MR::showTripodBossParts() {
    TripodBossAccesser::createSceneObj()->showTripodBossParts();
}

void MR::hideTripodBossParts() {
    TripodBossAccesser::createSceneObj()->hideTripodBossParts();
}

void MR::activeTripodBossParts() {
    TripodBossAccesser::createSceneObj()->activeTripodBossParts();
}

void MR::requestMovementTripodBossParts() {
    MR::onMovementOnOffGroup("三脚ボス部品");
}

bool MR::isCreatedTripodBoss() {
    if (!MR::isExistSceneObj(SceneObj_TripodBossAccesser)) {
        return false;
    }

    return getTripodBossAccesser()->getTriPodBoss() != nullptr;
}

bool MR::isStartedTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isStarted();
}

bool MR::isBrokenTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isBroken();
}

bool MR::isDemoTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isDemo();
}

bool MR::isStartDemoTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isStartDemo();
}

bool MR::isDamageDemoTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isDamageDemo();
}

bool MR::isEndBreakDownDemoTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isEndBreakDownDemo();
}

bool MR::isLeaveMarioNowTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isLeaveMarioNow();
}

bool MR::isRideMarioTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->isRideMario();
}

bool MR::tryDamageDemoTripodBoss() {
    return getTripodBossAccesser()->getTriPodBoss()->tryDamageDemo();
}

void MR::requestEndDamageDemoTripodBoss() {
    getTripodBossAccesser()->getTriPodBoss()->requestEndDamageDemo();
}

u32 MR::getTripodBossGravityHostID() {
    return reinterpret_cast< u32 >(getTripodBossAccesser());
}

void MR::setTripodBossJointAttachBaseMatrix(const TPos3f& rPos, s32 id) {
    getTripodBossAccesser()->getTriPodBoss()->setJointAttachBaseMatrix(rPos, ::convertBoneIDToIndex(id));
}

void MR::requestStartTripodBossStepSequence(s32 seq) {
    getTripodBossAccesser()->getTriPodBoss()->requestStartStepSequence(seq);
}

void MR::addTripodBossStepPoint(TripodBossStepPoint* pPoint) {
    getTripodBossAccesser()->getTriPodBoss()->addStepPoint(pPoint);
}

void MR::getTripodBossJointMatrix(TPos3f* pMtx, s32 id) {
    getTripodBossAccesser()->getTriPodBoss()->getJointMatrix(pMtx, ::convertBoneIDToIndex(id));
}

void MR::getTripodBossAttachJointMatrix(TPos3f* pMtx, s32 id) {
    getTripodBossAccesser()->getTriPodBoss()->getJointAttachMatrix(pMtx, ::convertBoneIDToIndex(id));
}

void MR::concatTripodBossAttachJointMatrix(TPos3f* pMtx, s32 id) {
    if (MR::isCreatedTripodBoss()) {
        TPos3f mtx;
        MR::getTripodBossAttachJointMatrix(&mtx, id);
        pMtx->concat(mtx, *pMtx);
    }
}

bool MR::isSteppingTripodBossLegID(s32 id) {
    return !getTripodBossAccesser()->getTriPodBoss()->isStopLeg(id);
}

bool MR::isSteppingTripodBossJointID(s32 id) {
    s32 partID = TripodBoss::getPartIDFromBoneID(::convertBoneIDToIndex(id));
    bool isValidPartID = partID >= TripodBoss::PART_ID_LEFT_LEG && partID < TripodBoss::PART_ID_MAX;

    if (isValidPartID) {
        return MR::isSteppingTripodBossLegID(partID);
    }

    return false;
}
