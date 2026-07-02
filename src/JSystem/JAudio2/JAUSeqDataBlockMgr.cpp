#include "JSystem/JAudio2/JAUSeqDataBlockMgr.hpp"
#include "JSystem/JAudio2/JASResArcLoader.hpp"
#include "JSystem/JAudio2/JAUSoundInfo.hpp"
#include <revolution/types.h>

JAUSeqDataBlock::JAUSeqDataBlock() : mLink(this) {
}

u8* JAUSeqDataBlocks::getSeqData(JAISoundID param_0) {
    if (param_0.isAnonymous()) {
        return nullptr;
    }
    JSULink< JAUSeqDataBlock >* link;
    for (link = getFirst(); link != nullptr; link = link->getNext()) {
        if ((u32)link->getObject()->mSoundID == (u32)param_0) {
            return link->getObject()->region.addr;
        }
    }
    return nullptr;
}

JSULink< JAUSeqDataBlock >* JAUSeqDataBlocks::seekFreeBlock(u32 size) {
    u32 currentBlockSize = 0xffffffff;
    JSULink< JAUSeqDataBlock >* rv = nullptr;
    for (JSULink< JAUSeqDataBlock >* link = getFirst(); link != nullptr; link = link->getNext()) {
        if (link->getObject()->mSoundID.isAnonymous()) {
            u32 blockSize = link->getObject()->region.size;
            if (blockSize >= size && blockSize < currentBlockSize) {
                rv = link;
                currentBlockSize = blockSize;
            }
        }
    }
    if (rv != nullptr) {
        return rv;
    }
    return nullptr;
}

bool JAUSeqDataBlocks::append(JSULink< JAUSeqDataBlock >* param_0) {
    return JSUList::append(param_0);
}

bool JAUSeqDataBlocks::remove(JSULink< JAUSeqDataBlock >* param_0) {
    return JSUList::remove(param_0);
}

bool JAUSeqDataBlocks::hasFailedBlock(JAISoundID param_0) {
    JSULink< JAUSeqDataBlock >* link;
    for (link = getFirst(); link != nullptr; link = link->getNext()) {
        if (link->getObject()->mSoundID == param_0) {
            link->getObject()->mSoundID.setAnonymous();
            return true;
        }
    }
    return false;
}

JAUDynamicSeqDataBlocks::JAUDynamicSeqDataBlocks() {
    mSeqDataArchive = nullptr;
}

void JAUDynamicSeqDataBlocks::setSeqDataArchive(JKRArchive* param_0) {
    mSeqDataArchive = param_0;
}

s32 JAUDynamicSeqDataBlocks::getSeqData(JAISoundID param_0, JAISeqDataUser* param_1, JAISeqData* param_2, bool param_3) {
    rearrangeLoadingSeqs_();
    if (mFreeBlocks.hasFailedBlock(param_0)) {
        return JAISeqDataMgr::SeqDataReturnValue_0;
    }

    if (field_0xc.getSeqData(param_0)) {
        return JAISeqDataMgr::SeqDataReturnValue_1;
    }

    u8* seqData = mLoadedBlocks.getSeqData(param_0);
    if (seqData != nullptr) {
        param_2->data = seqData;
        param_2->offset = 0;
        return JAISeqDataMgr::SeqDataReturnValue_2;
    }

    return loadDynamicSeq(param_0, param_3, param_1) ? JAISeqDataMgr::SeqDataReturnValue_1 : JAISeqDataMgr::SeqDataReturnValue_0;
}

bool JAUDynamicSeqDataBlocks::appendDynamicSeqDataBlock(JAUSeqDataBlock* seqDataBlock) {
    rearrangeLoadingSeqs_();
    bool result;
    if (seqDataBlock->mSoundID.isAnonymous()) {
        result = mFreeBlocks.append(&seqDataBlock->mLink);
    } else {
        result = mLoadedBlocks.append(&seqDataBlock->mLink);
    }
    return 1;
}

static void JAUDynamicSeqDataBlocks_receiveLoaded_(u32 param_0, uintptr_t param_1) {
    JSULink< JAUSeqDataBlock >* link = (JSULink< JAUSeqDataBlock >*)param_1;
    JAUSeqDataBlock* seqDataBlock = link->getObject();
    if (param_0 != 0) {
        seqDataBlock->field_0x1c = 2;
    } else {
        seqDataBlock->field_0x1c = 0;
    }
}

bool JAUDynamicSeqDataBlocks::loadDynamicSeq(JAISoundID param_0, bool param_1, JAISeqDataUser* param_2) {
    {
        if (mSeqDataArchive == nullptr) {
            return false;
        }
        JAUSoundInfo* soundInfo = JASGlobalInstance< JAUSoundInfo >::getInstance();
        if (soundInfo == nullptr) {
            return false;
        }
        u16 resourceId = soundInfo->getBgmSeqResourceID(param_0);
        u32 resSize = JASResArcLoader::getResSize(mSeqDataArchive, resourceId);
        JSULink< JAUSeqDataBlock >* link = mFreeBlocks.seekFreeBlock(resSize);
        if (link == nullptr) {
            if (param_1) {
                link = &releaseIdleDynamicSeqDataBlock_(param_2, resSize)->mLink;
                if (link == nullptr) {
                    return false;
                }
            } else {
                return false;
            }
        }
        mFreeBlocks.remove(link);
        link->getObject()->mSoundID = param_0;
        link->getObject()->field_0x1c = 1;
        field_0xc.append(link);
        JASResArcLoader::loadResourceAsync(mSeqDataArchive, resourceId, link->getObject()->region.addr, link->getObject()->region.size,
                                           JAUDynamicSeqDataBlocks_receiveLoaded_, (uintptr_t)link);
    }
    return true;
}

u32 JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock(JAISeqDataUser* param_0) {
    rearrangeLoadingSeqs_();
    u32 size = 0;
    JSULink< JAUSeqDataBlock >* nextLink;
    for (JSULink< JAUSeqDataBlock >* link = mLoadedBlocks.getFirst(); link != nullptr;) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == nullptr || !param_0->isUsingSeqData(seqDataBlock->region)) {
            mLoadedBlocks.remove(link);
            JAUSeqDataBlock* seqDataBlock = link->getObject();
            seqDataBlock->mSoundID.setAnonymous();
            if (size < seqDataBlock->region.size) {
                size = seqDataBlock->region.size;
            }
            mFreeBlocks.append(link);
        }
        link = nextLink;
    }
    return size;
}

JAUSeqDataBlock* JAUDynamicSeqDataBlocks::releaseIdleDynamicSeqDataBlock_(JAISeqDataUser* param_0, u32 param_1) {
    rearrangeLoadingSeqs_();
    u32 minSize = 0xffffffff;
    JSULink< JAUSeqDataBlock >* foundBlock = nullptr;
    JSULink< JAUSeqDataBlock >* link = mLoadedBlocks.getFirst();
    JSULink< JAUSeqDataBlock >* nextLink;
    while (link != nullptr) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (param_0 == nullptr || !param_0->isUsingSeqData(seqDataBlock->region)) {
            u32 size = seqDataBlock->region.size;
            if (size >= param_1 && size < minSize) {
                foundBlock = link;
                minSize = size;
            }
        }
        link = nextLink;
    }
    if (foundBlock != nullptr) {
        mLoadedBlocks.remove(foundBlock);
        JAUSeqDataBlock* seqDataBlock = foundBlock->getObject();
        seqDataBlock->mSoundID.setAnonymous();
        mFreeBlocks.append(foundBlock);
        return foundBlock->getObject();
    }
    return nullptr;
}

void JAUDynamicSeqDataBlocks::rearrangeLoadingSeqs_() {
    JSULink< JAUSeqDataBlock >* link = field_0xc.getFirst();
    JSULink< JAUSeqDataBlock >* nextLink;
    while (link != nullptr) {
        nextLink = link->getNext();
        JAUSeqDataBlock* seqDataBlock = link->getObject();
        if (seqDataBlock->field_0x1c == 0) {
            field_0xc.remove(link);
            mFreeBlocks.append(link);
        } else if (seqDataBlock->field_0x1c == 2) {
            field_0xc.remove(link);
            mLoadedBlocks.append(link);
        }
        link = nextLink;
    }
}
