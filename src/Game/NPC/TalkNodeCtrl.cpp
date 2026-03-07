#include "Game/NPC/TalkNodeCtrl.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/Screen/MessageTagSkipTagProcessor.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/StringUtil.hpp"
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
    _38 = nullptr;
    mCurrentNode = nullptr;
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

    if (node == nullptr) {
        return;
    }

    if (node->mNodeType == 1) {
        u16 idx = node->mNextIdx;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getNode(idx);
    } else if (node->mNodeType == 3) {
        u16 idx = node->mIndex;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getBranchNode(idx);
    }

    updateMessage();
}

bool TalkNodeCtrl::isExistNextNode() const {
    return getNextNode() != nullptr;
}

bool TalkNodeCtrl::isNextNodeMessage() const {
    TalkNode* next = getNextNode();

    if (next == nullptr || next->mNodeType != 1) {
        return false;
    }

    return true;
}

bool TalkNodeCtrl::isCurrentNodeEvent() const {
    return getCurrentNodeEvent() != nullptr;
}

TalkNode* TalkNodeCtrl::getNextNode() const {
    TalkNode* node = mCurrentNode;
    u16 idx;

    if (node == nullptr) {
        return nullptr;
    }

    if (node->mNodeType == 1) {
        idx = node->mNextIdx;
        if (idx != 0xFFFF) {
            return MessageSystem::getSceneMessageData()->getNode(idx);
        }
    } else if (node->mNodeType == 3) {
        idx = node->mIndex;
        MessageData* msg = MessageSystem::getSceneMessageData();

        if (msg->isValidBranchNode(idx)) {
            idx = node->mIndex;
            MessageData* msg = MessageSystem::getSceneMessageData();
            return msg->getBranchNode(idx);
        }
    }

    return nullptr;
}

TalkNode* TalkNodeCtrl::getNextNodeBranch() const {
    TalkNode* node = getNextNode();

    if (node == nullptr) {
        return nullptr;
    }

    return node->mNodeType != 2 ? nullptr : node;
}

TalkNode* TalkNodeCtrl::getCurrentNodeBranch() const {
    if (mCurrentNode != nullptr && mCurrentNode->mNodeType == 2) {
        return mCurrentNode;
    }

    return nullptr;
}

TalkNode* TalkNodeCtrl::getCurrentNodeMessage() const {
    if (mCurrentNode != nullptr && mCurrentNode->mNodeType == 1) {
        return mCurrentNode;
    }

    return nullptr;
}

TalkNode* TalkNodeCtrl::getCurrentNodeEvent() const {
    if (mCurrentNode != nullptr && mCurrentNode->mNodeType == 3) {
        return mCurrentNode;
    }

    return nullptr;
}

TalkNode* TalkNodeCtrl::getNextNodeEvent() const {
    TalkNode* node = getNextNode();

    if (node == nullptr) {
        return nullptr;
    }

    return node->mNodeType != 3 ? nullptr : node;
}

void TalkNodeCtrl::updateMessage() {
    u16 idx;
    u8 groupID;
    TalkNode* cur = mCurrentNode;

    if (cur != nullptr) {
        if (cur->mNodeType != 1) {
            mMessageInfo._0 = 0;
        } else {
            idx = cur->mIndex;
            groupID = cur->mGroupID;
            MessageData* msg = MessageSystem::getSceneMessageData();
            msg->getMessage(&mMessageInfo, groupID, idx);
            mCurrentNodeIdx = cur->mIndex;

            TalkNode* nextBranchNode = getNextNodeBranch();

            if (nextBranchNode != nullptr) {
                mNodeData = nextBranchNode->mIndex;
            } else {
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
    } else {
        u16 group = cur->mNextGroup;
        MessageData* msg = MessageSystem::getSceneMessageData();
        mCurrentNode = msg->getBranchNode(group + 1);
    }

    updateMessage();
}

void TalkNodeCtrl::createFlowNode(TalkMessageCtrl* pMsgCtrl, const JMapInfoIter& rIter, const char* pName, ActorCameraInfo** pCameraInf) {
    char buf[0x100];
    s32 msgID = MR::getMessageID(rIter);
    const char* zoneName = MR::getCurrentPlacementZoneName();
    snprintf(buf, sizeof(buf), "%s_%s%03d", zoneName, pName, msgID);
    createFlowNodeDirect(pMsgCtrl, rIter, buf, pCameraInf);
}

// Inexplicable 0x130 bytes large stack
void TalkNodeCtrl::createFlowNodeDirect(TalkMessageCtrl* pMsgCtrl, const JMapInfoIter& rIter, const char* pName, ActorCameraInfo** pCameraInf) {
    TalkNode* node = MessageSystem::getSceneMessageData()->findNode(pName);
    _38 = node;
    mCurrentNode = node;
    mFlowNode = node;
    _0 = new char[strlen(pName) + 1];
    MR::copyString(_0, pName, strlen(pName) + 1);

    if (mCurrentNode == nullptr) {
        char* name = _0;
        MessageData* msgData = MessageSystem::getSceneMessageData();
        msgData->getMessageDirect(&mMessageInfo, name);
    } else {
        updateMessage();
    }

    if (mMessageInfo.isFlowTalk()) {
        forwardFlowNode();
        _38 = mCurrentNode;
        mFlowNode = mCurrentNode;
    }

    RecursiveHelper helper;
    helper.mIndex = 0;

    if (!MR::isValidInfo(rIter)) {
        *pCameraInf = new ActorCameraInfo(0, 0);
    } else {
        *pCameraInf = new ActorCameraInfo(rIter);
        initNodeRecursive(pMsgCtrl, rIter, *pCameraInf, &helper);
    }

    resetFlowNode();
}

const wchar_t* TalkNodeCtrl::getSubMessage() const {
    const TalkMessageInfo info = mMessageInfo;

    if (*((wchar_t*)info._0) != 0x1A) {
        return nullptr;
    }

    MessageEditorMessageTag messageTag = MessageEditorMessageTag((const wchar_t*)&info._0[2]);

    if (((char*)messageTag.mMessage)[1] != 8 || messageTag.mMessage[1] != 0) {
        return nullptr;
    }

    TalkMessageInfo info2 = TalkMessageInfo();
    u16 param = messageTag.getParam32(0);

    if (MessageSystem::getSceneMessageData()->getMessage(&info2, 0, param)) {
        return (const wchar_t*)info2._0;
    }

    return nullptr;
}

bool RecursiveHelper::hasNode(const TalkNode* pNode) const {
    for (int i = 0; i < mIndex; i++) {
        if (mStack[i] == pNode) {
            return true;
        }
    }

    return false;
}

void TalkNodeCtrl::initNodeRecursive(TalkMessageCtrl* pMsgCtrl, const JMapInfoIter& rIter, ActorCameraInfo* pCameraInf, RecursiveHelper* pHelper) {
    TalkNode* currentNode = mCurrentNode;

    if (pHelper->hasNode(currentNode)) {
        return;
    }

    pHelper->mStack[pHelper->mIndex++] = currentNode;

    if (mMessageInfo.isCameraEvent()) {
        pCameraInf->mCameraSetID = mMessageInfo.mCameraSetID;
        MR::initMultiActorCamera(pMsgCtrl->mHostActor, rIter, &pCameraInf, "会話");
    }

    TalkNode* nodeEvent = getCurrentNodeEvent();
    if (nodeEvent != nullptr) {
        if (nodeEvent->mGroupID == 5) {
            MR::needStageSwitchWriteA(pMsgCtrl->mHostActor, rIter);
        } else if (nodeEvent->mGroupID == 6) {
            MR::needStageSwitchWriteB(pMsgCtrl->mHostActor, rIter);
        }
    }

    TalkNode* nodeBranch = getCurrentNodeBranch();
    if (nodeBranch != nullptr) {
        if (nodeBranch->mIndex == 3) {
            MR::needStageSwitchReadA(pMsgCtrl->mHostActor, rIter);
        } else if (nodeBranch->mIndex == 4) {
            MR::needStageSwitchReadB(pMsgCtrl->mHostActor, rIter);
        }
    }

    TalkNode* nextNodeBranch = getNextNodeBranch();
    if (nextNodeBranch != nullptr || getCurrentNodeBranch() != nullptr) {
        if (getNextNodeBranch() != nullptr) {
            forwardFlowNode();
        }
        TalkNode* rememberNode = mCurrentNode;
        forwardCurrentBranchNode(true);
        initNodeRecursive(pMsgCtrl, rIter, pCameraInf, pHelper);
        mCurrentNode = rememberNode;
        forwardCurrentBranchNode(false);
        initNodeRecursive(pMsgCtrl, rIter, pCameraInf, pHelper);
        return;
    }

    TalkNode* nextNodeEvent = getNextNodeEvent();
    if (nextNodeEvent == nullptr && !isNextNodeMessage()) {
        return;
    }

    forwardFlowNode();
    initNodeRecursive(pMsgCtrl, rIter, pCameraInf, pHelper);
}
