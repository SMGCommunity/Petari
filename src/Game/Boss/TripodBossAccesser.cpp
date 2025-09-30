#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static s32 cJMapBoneIDToBoneIndexTableSize = 0x29;
    static s32 cJMapBoneIDToBoneIndexTable[0x29] = {
        0, 1, 2, 7, 8, 9, 0xE, 0xF, 0x10, 0x15, 3, 4, 0, 1, 5, 6, 2, -1, -1, -1,
        0xA, 0xB, 7, 8, 0xC, 0xD, 9, -1, -1, -1, 0x11, 0x12, 0xE, 0xF, 0x13, 0x14, 0x10, -1, -1, -1, 0x15
    };

    s32 convertBoneIDToIndex(s32 id) NO_INLINE {
        if (id < 0 || cJMapBoneIDToBoneIndexTableSize <= id) {
            return -1;
        }
        else {
            return cJMapBoneIDToBoneIndexTable[id];
        }
    }
};

TripodBossAccesser::TripodBossAccesser(const char *pName) : NameObj(pName) {
    mBoss = nullptr;
    mNumParts = 0;
}

void TripodBossAccesser::setTriPodBoss(TripodBoss *pBoss) {
    mBoss = pBoss;
}

void TripodBossAccesser::addTripodBossParts(LiveActor *pActor) {
    mParts[mNumParts] = pActor;
    mNumParts++;
}

void TripodBossAccesser::showTripodBossParts() {
    for (s32 i = 0; i < mNumParts; i++) {
        if (MR::isDead(mParts[i])) {
            mParts[i]->makeActorAppeared();   
        }
    }
}

void TripodBossAccesser::hideTripodBossParts() {
    for (s32 i = 0; i < mNumParts; i++) {
        if (!MR::isDead(mParts[i])) {
            mParts[i]->makeActorDead();   
        }
    }
}

void TripodBossAccesser::activeTripodBossParts() {
    for (s32 i = 0; i < mNumParts; i++) {
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

    for (s32 i = 0; i < mNumParts; i++) {
        if (mParts[i] == pObj) {
            return true;
        }
    }

    return false;
}

TripodBossAccesser* TripodBossAccesser::createSceneObj() {
    return (TripodBossAccesser*)MR::createSceneObj(SceneObj_TripodBossAccesser);
}

namespace MR {
    void addTripodBossParts(LiveActor *pActor) {
        ((TripodBossAccesser*)MR::createSceneObj(SceneObj_TripodBossAccesser))->addTripodBossParts(pActor);
        MR::joinToMovementOnOffGroup("三脚ボス部品", pActor, 0x100);
    }

    void addTripodBossPartsMovement(NameObj *pObj) {
        MR::joinToMovementOnOffGroup("三脚ボス部品", pObj, 0x100);
    }

    bool isTripoddBossParts(const NameObj *pObj) {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->isTripoddBossParts(pObj);
    }

    void showTripodBossParts() {
        ((TripodBossAccesser*)MR::createSceneObj(SceneObj_TripodBossAccesser))->showTripodBossParts();
    }

    void hideTripodBossParts() {
        ((TripodBossAccesser*)MR::createSceneObj(SceneObj_TripodBossAccesser))->hideTripodBossParts();
    }

    void activeTripodBossParts() {
        ((TripodBossAccesser*)MR::createSceneObj(SceneObj_TripodBossAccesser))->activeTripodBossParts();
    }

    void requestMovementTripodBossParts() {
        MR::onMovementOnOffGroup("三脚ボス部品");
    }

    bool isCreatedTripodBoss() {
        if (!MR::isExistSceneObj(SceneObj_TripodBossAccesser)) {
            return false;
        }

        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss != nullptr;
    }

    bool isStartedTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isStarted();
    }

    bool isBrokenTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isBroken();
    }

    bool isDemoTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isDemo();
    }

    bool isStartDemoTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isStartDemo();
    }

    bool isDamageDemoTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isDamageDemo();
    }

    bool isEndBreakDownDemoTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isEndBreakDownDemo();
    }

    bool isLeaveMarioNowTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isLeaveMarioNow();
    }

    bool isRideMarioTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isRideMario();
    }
    
    bool tryDamageDemoTripodBoss() {
        return MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->tryDamageDemo();
    }

    void requestEndDamageDemoTripodBoss() {
        MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->requestEndDamageDemo();
    }

    u32 getTripodBossGravityHostID() {
       return (u32)MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser);
    }

    void setTripodBossJointAttachBaseMatrix(const TPos3f &rPos, s32 id) {
        TripodBoss* boss = MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss;
        boss->setJointAttachBaseMatrix(rPos, convertBoneIDToIndex(id));
    }

    void requestStartTripodBossStepSequence(s32 seq) {
        MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->requestStartStepSequence(seq);
    }

    void addTripodBossStepPoint(TripodBossStepPoint *pPoint) {
        MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->addStepPoint(pPoint);
    }

    void getTripodBossJointMatrix(TPos3f *pPos, s32 id) {
        TripodBoss* boss = MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss;
        boss->getJointMatrix(pPos, convertBoneIDToIndex(id));
    }

    void getTripodBossAttachJointMatrix(TPos3f *pPos, s32 id) {
        TripodBoss* boss = MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss;
        boss->getJointAttachMatrix(pPos, convertBoneIDToIndex(id));
    }

    void concatTripodBossAttachJointMatrix(TPos3f *pPos, s32 id) {
        if (MR::isCreatedTripodBoss()) {
            TPos3f mtx;
            MR::getTripodBossAttachJointMatrix(&mtx, id);
            pPos->concat(mtx, *pPos);
        }
    }

    bool isSteppingTripodBossLegID(s32 id) {
        bool ret = !MR::getSceneObj<TripodBossAccesser*>(SceneObj_TripodBossAccesser)->mBoss->isStopLeg(id);
        return ret;
    }

    bool isSteppingTripodBossJointID(s32 id) {
        s32 partID = TripodBoss::getPartIDFromBoneID(convertBoneIDToIndex(id));
        bool ret = false;

        if (partID >= 0 && partID < 3) {
            ret = true;
        }

        if (ret) {
            return MR::isSteppingTripodBossLegID(partID);
        }
        
        return false;
    }
};

TripodBossAccesser::~TripodBossAccesser() {
    
}
