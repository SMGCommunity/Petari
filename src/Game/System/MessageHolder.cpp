#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/MessageHolder.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

#define SYSTEMMESSAGE_ARC "/Memory/SystemMessage.arc"
#define MESSAGE_ARC "/MessageData/Message.arc"

namespace {
    u8* getBlock(u32, u8*);
}

MessageData* MessageSystem::getSceneMessageData() {
    return MR::getGameSystemObjHolder()->mMsgHolder->mSceneData;
}

MessageData::MessageData(const char* pArchiveName) {
    mIDTable = 0;
    mInfoBlock = 0;
    mDataBlock = 0;
    _C = 0;
    mFlowBlock = 0;
    _14 = 0;
    _18 = 0;
    mFLI1Block = 0;

    JKRArchive* pArchive = 0;
    JKRHeap* pHeap = 0;
    MR::getMountedArchiveAndHeap(pArchiveName, &pArchive, &pHeap);

    u8* msgData = (u8*)pArchive->getResource("Message.bmg");
    u8* mapData = (u8*)pArchive->getResource(QUESTIONMARK_MAGIC, "MessageId.tbl");

    mIDTable = new JMapInfo();
    mIDTable->attach(mapData);

    mInfoBlock = getBlock(INF1_MAGIC, msgData);
    mDataBlock = getBlock(DAT1_MAGIC, msgData);
    mFlowBlock = getBlock(FLW1_MAGIC, msgData);
    if (mFlowBlock) {
        _14 = mFlowBlock + *(u16*)(mFlowBlock + 8) * 8 + 0x10;
        _18 = _14 + *(u16*)(mFlowBlock + 0xa) * 2;
    }
    mFLI1Block = getBlock(FLI1_MAGIC, msgData);
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
