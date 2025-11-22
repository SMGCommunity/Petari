#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/MessageHolder.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

#define SYSTEMMESSAGE_ARC "/Memory/SystemMessage.arc"
#define MESSAGE_ARC "/MessageData/Message.arc"

namespace {
    u8* getBlock(u32 magic, u8* pData) {
        u32 numBlocks = *reinterpret_cast< u32* >(pData + 0xc);
        pData += 0x20;
        for (u32 i = 0; i < numBlocks; i++) {
            u32 blockMagic = *reinterpret_cast< u32* >(pData);
            if (blockMagic == magic) {
                return pData;
            }
            u32 blockSize = *reinterpret_cast< u32* >(pData + 4);
            pData += blockSize;
        }
        return nullptr;
    }
} // namespace

bool MessageData::getMessageDirect(TalkMessageInfo* pMessageInfo, const char* pMessage) const {
    s32 messageIndex = findMessageIndex(pMessage);
    if (messageIndex >= 0 && messageIndex < mInfoBlock->mItemCount) {
        getMessage(pMessageInfo, 0, messageIndex);
        return true;
    }
    return false;
}

bool MessageData::getMessage(TalkMessageInfo* pMessageInfo, u16, u16 infoToolIndex) const {
    u8* pInfoTool = getMessageInfoTool(infoToolIndex);
    pMessageInfo->_0 = reinterpret_cast< u8* >(mDataBlock + 1) + *reinterpret_cast< u32* >(pInfoTool);
    pMessageInfo->_4 = *reinterpret_cast< u16* >(pInfoTool + 4);
    pMessageInfo->_6 = *(pInfoTool + 6);
    pMessageInfo->mCameraType = *(pInfoTool + 7);
    pMessageInfo->mTalkType = *(pInfoTool + 8);
    pMessageInfo->_A = *(pInfoTool + 0xa);
    pMessageInfo->_B = *(pInfoTool + 0xb);
    pMessageInfo->mBalloonType = *(pInfoTool + 9);
    return true;
}

TalkNode* MessageData::findNode(const char* pMessage) const {
    s32 messageIndex = findMessageIndex(pMessage);

    for (int i = 0; i < mFlowBlock->mNodeCount; i++) {
        TalkNode* pNode = reinterpret_cast< TalkNode* >(mFlowBlock + 1) + i;
        if (pNode->mNodeType == 1 && pNode->mIndex == messageIndex) {
            return pNode;
        }
    }
    return nullptr;
}

TalkNode* MessageData::getNode(u32 index) const {
    return reinterpret_cast< TalkNode* >(mFlowBlock + 1) + index;
}

TalkNode* MessageData::getBranchNode(u32 index) const {
    return reinterpret_cast< TalkNode* >(mFlowBlock + 1) + _14[index];
}

bool MessageData::isValidBranchNode(u32 index) const {
    return _14[index] != 0xffff;
}

u8* MessageData::getMessageInfoTool(int index) const {
    return reinterpret_cast< u8* >(mInfoBlock + 1) + mInfoBlock->mItemSize * index;
}

MessageHolder::MessageHolder() {
    mSystemMessage = nullptr;
    mGameMessage = nullptr;
    mSceneData = nullptr;
}

void MessageHolder::initSceneData() {
    mSceneData = mGameMessage;
}

void MessageHolder::destroySceneData() {
    mSceneData = nullptr;
}

void MessageSystem::getSystemMessageDirect(TalkMessageInfo* pMessageInfo, const char* pMessage) {
    MR::getGameSystemObjHolder()->mMsgHolder->mSystemMessage->getMessageDirect(pMessageInfo, pMessage);
}

void MessageSystem::getGameMessageDirect(TalkMessageInfo* pMessageInfo, const char* pMessage) {
    MR::getGameSystemObjHolder()->mMsgHolder->mGameMessage->getMessageDirect(pMessageInfo, pMessage);
}

void MessageSystem::getLayoutMessageDirect(TalkMessageInfo* pMessageInfo, const char* pMessage) {
    MR::getGameSystemObjHolder()->mMsgHolder->mGameMessage->getMessageDirect(pMessageInfo, pMessage);
}

MessageData* MessageSystem::getSceneMessageData() {
    return MR::getGameSystemObjHolder()->mMsgHolder->mSceneData;
}

MessageData::MessageData(const char* pArchiveName) {
    mIDTable = nullptr;
    mInfoBlock = nullptr;
    mDataBlock = nullptr;
    _C = 0;
    mFlowBlock = nullptr;
    _14 = nullptr;
    _18 = nullptr;
    mFLI1Block = nullptr;

    JKRArchive* pArchive = nullptr;
    JKRHeap*    pHeap = nullptr;
    MR::getMountedArchiveAndHeap(pArchiveName, &pArchive, &pHeap);

    u8* msgData = (u8*)pArchive->getResource("Message.bmg");
    u8* mapData = (u8*)pArchive->getResource(QUESTIONMARK_MAGIC, "MessageId.tbl");

    mIDTable = new JMapInfo();
    mIDTable->attach(mapData);

    mInfoBlock = (MessageInfoBlock*)getBlock('INF1', msgData);
    mDataBlock = (MessageDataBlock*)getBlock('DAT1', msgData);
    mFlowBlock = (MessageFlowBlock*)getBlock('FLW1', msgData);
    if (mFlowBlock) {
        _14 = reinterpret_cast< u16* >(reinterpret_cast< TalkNode* >(mFlowBlock + 1) + mFlowBlock->mNodeCount);
        _18 = reinterpret_cast< u8* >(_14 + mFlowBlock->_A);
    }
    mFLI1Block = (MessageFLI1Block*)getBlock('FLI1', msgData);
}

inline JMapInfoIter end(const JMapInfo* pInfo) {
    return JMapInfoIter(pInfo, pInfo->getNumEntries());
}

s32 MessageData::findMessageIndex(const char* pMessage) const {
    JMapInfoIter iter = mIDTable->findElementBinary("MessageId", pMessage);

    // This *should* be mIDTable->end(), but I have had trouble
    // getting the compiler to inline that (see comment in JMapInfo.hpp)
    if (iter == end(mIDTable)) {
        return -1;
    }

    s32 messageIndex = -1;
    s32 itemIndex = iter.mInfo->searchItemInfo("Index");
    if (itemIndex >= 0) {
        iter.mInfo->getValueFast(iter.mIndex, itemIndex, &messageIndex);
    }
    return messageIndex;
}

void MessageHolder::initSystemData() {
    mSystemMessage = new MessageData(SYSTEMMESSAGE_ARC);
}

void MessageHolder::initGameData() {
    MR::mountArchive(MESSAGE_ARC, 0);
    mGameMessage = new MessageData(MESSAGE_ARC);
}
