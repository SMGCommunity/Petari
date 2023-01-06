#include "Game/NPC/TalkNodeCtrl.h"
#include "Game/System/MessageHolder.h"
#include "Game/Util.h"
#include <cstdio>

void TalkMessageHistory::entry(u16 msgID) {
   mHistory[mCount++] = msgID;
}

bool TalkMessageHistory::search(u16 msgID) const {
    for (s32 i = 0; i < mCount; i++) {
        if (mHistory[i] == msgID) {
            return true;
        }
    }

    return false;
}

TalkNodeCtrl::TalkNodeCtrl() : _0(0), mCurrentNodeIdx(-1), mMessageInfo() {
    mHistory.mCount = 0;
    _38 = NULL;
    mCurrentNode = NULL;
    mNodeData = -1;
}

void TalkNodeCtrl::resetFlowNode() {
    if (mCurrentNode != _38) {
        mCurrentNode = _38;
        mFlowNode = _38;
        updateMessage();
    }
}

void TalkNodeCtrl::resetTempFlowNode() {
    if (mCurrentNode != mFlowNode) {
        mCurrentNode = mFlowNode;
        updateMessage();
    }
}

void TalkNodeCtrl::recordTempFlowNode() {
    mFlowNode = mCurrentNode;
}

void TalkNodeCtrl::forwardFlowNode() {
    TalkNode* node = mCurrentNode;

    if (node == NULL) {
        return;
    }

    if (node->mNodeType == 1) {
        u16 idx = node->mNextIdx;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getNode(idx);
    }
    else if (node->mNodeType == 3) {
        u16 idx = node->mNextIdx;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getBranchNode(idx);
    }

    updateMessage();
}

bool TalkNodeCtrl::isExistNextNode() const {
    return getNextNode() != NULL;
}

bool TalkNodeCtrl::isNextNodeMessage() const {
    TalkNode* next = getNextNode();

    if (next != NULL || next->mNodeType != 1) {
        return false;
    }

    return true;
}

bool TalkNodeCtrl::isCurrentNodeEvent() const {
    return getCurrentNodeEvent() != NULL;
}

TalkNode* TalkNodeCtrl::getNextNode() const {
    TalkNode* node = mCurrentNode;
    u16 idx;

    if (node == NULL) {
        return NULL;
    }

    if (node->mNodeType == 1) {
        idx = node->mNextIdx;
        if (idx != 0xFFFF) {
            return MessageSystem::getSceneMessageData()->getNode(idx);
        }
    }
    else if (node->mNodeType == 3) {
        idx = node->mIndex;
        MessageData* msg = MessageSystem::getSceneMessageData();

        if (msg->isValidBranchNode(idx)) {
            idx = node->mIndex;
            MessageData* msg = MessageSystem::getSceneMessageData();
            return msg->getBranchNode(idx);
        }
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getNextNodeBranch() const {
    TalkNode* node = getNextNode();
    
    if (node == NULL) {
        return NULL;
    }

    return node->mNodeType != 2 ? NULL : node;
}

TalkNode* TalkNodeCtrl::getCurrentNodeBranch() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 2) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getCurrentNodeMessage() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 1) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getCurrentNodeEvent() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 3) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getNextNodeEvent() const {
    TalkNode* node = getNextNode();
    
    if (node == NULL) {
        return NULL;
    }

    return node->mNodeType != 3 ? NULL : node;
}

void TalkNodeCtrl::updateMessage() {
    u16 idx;
    u8 groupID;
    TalkNode* cur = mCurrentNode;

    if (cur != NULL) {
        if (cur->mNodeType != 1) {
            mMessageInfo._0 = 0;
        } 
        else {
            idx = cur->mIndex;
            groupID = cur->mGroupID;
            MessageData* msg = MessageSystem::getSceneMessageData();
            msg->getMessage(&mMessageInfo, groupID, idx);
            mCurrentNodeIdx = cur->mIndex;

            TalkNode* nextBranchNode = getNextNodeBranch();

            if (nextBranchNode != NULL) {
                mNodeData = nextBranchNode->mIndex;
            }
            else {
                mNodeData = -1;
            }
        }
    }

    if (mMessageInfo.isEventTalk()) {
        if (mHistory.search(mCurrentNodeIdx)) {
            mMessageInfo.mTalkType = 0;
        }

        s8 msgId = mMessageInfo._B;

        if (msgId != -1) {
            if (MR::isOnMessageAlreadyRead(msgId)) {
                mMessageInfo.mTalkType = 0;
            }
        }
    }
}

void TalkNodeCtrl::readMessage() {
    if (mMessageInfo.isEventTalk()) {
        if (!mHistory.search(mCurrentNodeIdx)) {
            mHistory.entry(mCurrentNodeIdx);
        }

        mMessageInfo.mTalkType = 0;
    }

    if (mMessageInfo._B != -1) {
        MR::onMessageAlreadyRead(mMessageInfo._B);
    }
}

void TalkNodeCtrl::forwardCurrentBranchNode(bool storeCurrent) {
    TalkNode* cur = mCurrentNode;

    if (storeCurrent) {
        u16 group = cur->mNextGroup;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getBranchNode(group);
    }
    else {
        u16 group = cur->mNextGroup;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getBranchNode(group + 1);
    }

    updateMessage();
}

#ifdef NON_MATCHING
// refuses to load the msgID before the getCurrentPlacementZoneName() call
void TalkNodeCtrl::createFlowNode(TalkMessageCtrl *pMsgCtrl, const JMapInfoIter &rIter, const char *pName, ActorCameraInfo **pCameraInf) {
    char buf[0x100];
    s32 msgID = MR::getMessageID(rIter);
    snprintf(buf, 0x100, "%s_%s%03d", MR::getCurrentPlacementZoneName(), pName, msgID);
    createFlowNodeDirect(pMsgCtrl, rIter, buf, pCameraInf);
}
#endif
