#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkNodeCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

#ifdef NON_MATCHING
// one liner regswap with mNodeCtrl->mCurrentNode
void TalkMessageCtrl::createMessage(const JMapInfoIter &rIter, const char *pName) {
    mNodeCtrl = new TalkNodeCtrl();
    mNodeCtrl->createFlowNode(this, rIter, pName, &mCameraInfo);
    
    if (mNodeCtrl->mCurrentNode != nullptr) {
        mAlreadyDoneFlags = MR::setupAlreadyDoneFlag(((MessageNode*)mNodeCtrl->mCurrentNode)->mMessage, rIter, &_3C);
    }
    else {
        _3C = 1;
    }
}
#endif

void TalkMessageCtrl::createMessageDirect(const JMapInfoIter &rIter, const char *pName) {
    mNodeCtrl = new TalkNodeCtrl();
    mNodeCtrl->createFlowNode(this, rIter, pName, &mCameraInfo);
    _3C = 1;
}


// ...

void TalkMessageCtrl::rootNodePst() {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    if (!ctrl->isExistNextNode()) {
        ctrl->resetFlowNode();
    }
    else {
        ctrl->forwardFlowNode();
    }

    ctrl->recordTempFlowNode();
}

void TalkMessageCtrl::rootNodeSel(bool a1) {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    ctrl->forwardFlowNode();

    if (a1) {
        ctrl->forwardCurrentBranchNode(true);
    }
    else {
        ctrl->forwardCurrentBranchNode(false);
    }

    ctrl->recordTempFlowNode();
}

void TalkMessageCtrl::registerBranchFunc(const TalkMessageFuncBase &rFunc) {
    mBranchFunc = rFunc.clone();
}

void TalkMessageCtrl::registerEventFunc(const TalkMessageFuncBase &rFunc) {
    mEventFunc = rFunc.clone();
}

void TalkMessageCtrl::registerAnimeFunc(const TalkMessageFuncBase &rFunc) {
    mAnimeFunc = rFunc.clone();
}

void TalkMessageCtrl::registerKillFunc(const TalkMessageFuncBase &rFunc) {
    mKillFunc = rFunc.clone();
}

void TalkMessageCtrl::readMessage() {
    if (mIsOnReadNodeAuto) {
        mNodeCtrl->readMessage();
    }
    
    if (!_3C) {
        MR::updateAlreadyDoneFlag(mAlreadyDoneFlags, 1);
    }
}

bool TalkMessageCtrl::isSelectYesNo() const {
    TalkNode* node = mNodeCtrl->getNextNodeBranch();

    if (node == nullptr || node->mIndex != 0) {
        return false;
    }

    switch (node->mNextIdx) {
        case 16:
            return false;
        case 14:
            return false;
    }

    return true;
}

u32 TalkMessageCtrl::getMessageID() const {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    if (ctrl->getCurrentNodeMessage() != nullptr) {
        return ctrl->getCurrentNodeMessage()->mIndex;
    }
    else {
        return ctrl->mCurrentNodeIdx;
    }
}

// TalkMessageCtrl::requestTalk
// TalkMessageCtrl::requestTalkForce
// TalkMessageCtrl::startTalk
// TalkMessageCtrl::startTalkForce
// TalkMessageCtrl::startTalkForcePuppetable
// TalkMessageCtrl::startTalkForceWithoutDemo
// TalkMessageCtrl::startTalkForceWithoutDemoPuppetable
// TalkMessageCtrl::endTalk
// TalkMessageCtrl::updateBalloonPos

bool TalkMessageCtrl::isNearPlayer(const TalkMessageCtrl *pCtrl) {
    if (pCtrl == nullptr) {
        return true;
    }

    TVec3f* playerPos = MR::getPlayerPos();
    return mHostActor->getPosPtr()->squared(*playerPos) < pCtrl->mHostActor->getPosPtr()->squared(*playerPos);
}

void TalkMessageCtrl::startCamera(s32 a1) {
    TalkMessageInfo* inf = &mNodeCtrl->mMessageInfo;
    if (inf->isCameraNormal()) {
        MR::startNPCTalkCamera(this, mHostActor->getBaseMtx(), 1.0f, a1);
    }
    else if (inf->isCameraEvent()) {
        if (inf->_4) {
            mCameraInfo->mCameraSetID = inf->_4;
            MR::startMultiActorCameraNoTarget(mHostActor, mCameraInfo, "会話", a1);
        }
    }
}

const char* TalkMessageCtrl::getBranchID() const {
    TalkNode* node = mNodeCtrl->getNextNodeBranch();

    if (node == nullptr || node->mIndex != 0 ) {
        return nullptr;
    }

    const char* result = nullptr;

    switch (node->mNextIdx) {
        case 0:
            result = "PenguinRace";
            break;
        case 1:
            result = "SwimmingSchool";
            break;
        case 2:
            result = "PenguinRace";
            break;
        case 3:
            result = "BombTimeAttackLv1";
            break;
        case 4:
            result = "PhantomTeresaRacer";
            break;
        case 5:
            result = "BombTimeAttackLv2";
            break;
        case 6:
            result = "TrialSurfingCoach";
            break;
        case 7:
            result = "TrialSurfingHowTo";
            break;
        case 8:
            result = "DeathPromenadeTeresaRacer";
            break;
        case 9:
            result = "RosettaFinalBattle";
            break;
        case 10:
            result = "CometTico";
            break;
        case 11:
            result = "TransformTico";
            break;
        case 12:
            result = "ChallengeSurfingCoach";
            break;
        case 13:
            result = "TicoShopExchange";
            break;
        case 14:
            result = "TicoShopWhich";
            break;
        case 15:
            result = "KinopioPurple";
            break;
        case 16:
            result = "CometTicoTell";
            break;
        case 17:
            result = "TrialTamakoroHowTo";
            break;
        case 18:
            result = "KnockOnTheDoor";
            break;
        case 19:
            result = "LedPattern";
            break;
    }

    return result;
}