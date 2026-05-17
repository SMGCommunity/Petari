#include "Game/NPC/PenguinRacerLeader.hpp"
#include "Game/NPC/NPCActorItem.hpp"

namespace NrvPenguinRacerLeader {
    NEW_NERVE(PenguinRacerLeaderNrvWait, PenguinRacerLeader, Wait);
    NEW_NERVE(PenguinRacerLeaderNrvPre, PenguinRacerLeader, Pre);
    NEW_NERVE(PenguinRacerLeaderNrvReady, PenguinRacerLeader, Ready);
    NEW_NERVE(PenguinRacerLeaderNrvPost, PenguinRacerLeader, Post);
    NEW_NERVE(PenguinRacerLeaderNrvTalk, PenguinRacerLeader, Talk);
    NEW_NERVE(PenguinRacerLeaderNrvTakeOutStar, PenguinRacerLeader, TakeOutStar);
}  // namespace NrvPenguinRacerLeader

PenguinRacerLeader::PenguinRacerLeader(const char* pName) : PenguinRacer(pName), mTakeOutStar(nullptr), mCameraInfo(nullptr) {
}

void PenguinRacerLeader::init(const JMapInfoIter& rIter) {
    MR::initDefaultPose(this, rIter);
    initModel();

    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);

    initSensor();

    initSound(8, false);
    initShadow();
    initBind();
    initEffectKeeper(1, nullptr, false);
    MR::onCalcGravity(this);

    initColor(rIter);
    initEtc(rIter);

    mMsgCtrl = MR::createTalkCtrl(this, rIter, "PenguinRacerLeader", TVec3f(0.0f, 130.0f, 0.0f), nullptr);
    MR::onRootNodeAutomatic(mMsgCtrl);

    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &PenguinRacerLeader::branchFunc));
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &PenguinRacerLeader::eventFunc));

    initNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvWait::sInstance);

    if (mRaceDisabled == 0) {
        MR::declarePowerStar(this);
        RaceManagerFunction::entryRacerOthers(this);
        initRailRider(rIter);
        mTakeOutStar = new TakeOutStar(this, "TakeOutStar", "TakeOutStar", &NrvPenguinRacerLeader::PenguinRacerLeaderNrvTakeOutStar::sInstance);
    }

    MR::addToAttributeGroupSearchTurtle(this);
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    makeActorAppeared();

    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "会話");  // conversation

    NPCActorItem item("PenguinRacerLeader");
    MR::getNPCItemData(&item, 0);
    equipment(item, false);
}

bool PenguinRacerLeader::branchFunc(u32 state) {
    if (state == 0) {
        return MR::isStageStatePowerStarAppeared();
    }

    if (state == 1) {
        return RaceManagerFunction::getRaceRank() == 1;
    }

    return false;
}

bool PenguinRacerLeader::eventFunc(u32 state) {
    if (state == 0) {
        RaceManagerFunction::startRaceWithWipe();
    }

    if (state == 1) {
        if (mTakeOutStar->takeOut()) {
            MR::endMultiActorCamera(this, mCameraInfo, "会話", false, -1);
            return true;
        }
        return false;
    }

    return true;
}

void PenguinRacerLeader::exeWait() {
    if (!tryReaction() && MR::tryTalkNearPlayerAndStartTalkAction(this) && mRaceDisabled == 0) {
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", -1);
        setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvPre::sInstance);
    }
}

void PenguinRacerLeader::exePre() {
    if (!MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        return;
    }

    if (MR::tryTalkSelectLeft(mMsgCtrl)) {
        RaceManagerFunction::startRaceWithWipe();
        setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvReady::sInstance);
    } else {
        MR::endMultiActorCamera(this, mCameraInfo, "会話", false, -1);
        setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvWait::sInstance);
    }
}

void PenguinRacerLeader::exeReady() {
}

void PenguinRacerLeader::exePost() {
    if (!MR::tryTalkForceAtEndAndStartTalkAction(this)) {
        return;
    }

    MR::endMultiActorCamera(this, mCameraInfo, "会話", true, -1);

    if (RaceManagerFunction::getRaceRank() == 1) {
        setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvTalk::sInstance);
    } else {
        MR::forceKillPlayerByGroundRace();
        setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvReady::sInstance);
    }
}

void PenguinRacerLeader::exeTalk() {
    if (!tryReaction()) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}

void PenguinRacerLeader::exeTakeOutStar() {
}

void PenguinRacerLeader::resetRacer(const RaceManager* pRaceManager) {
    PenguinRacer::resetRacer(pRaceManager);
    turnToPlayer(180.0f);
    calcAndSetBaseMtx();
    MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", 0);
    setNerve(&NrvPenguinRacerLeader::PenguinRacerLeaderNrvPost::sInstance);
}

void PenguinRacerLeader::exitRacer() {
    PenguinRacer::exitRacer();
}
