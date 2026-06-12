#include "JSystem/JAudio2/JAUStdSoundInfo.hpp"
#include "JSystem/JAudio2/JAISe.hpp"
#include "JSystem/JAudio2/JAISeq.hpp"
#include "JSystem/JAudio2/JAISoundChild.hpp"
#include "JSystem/JAudio2/JAIStream.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JAudio2/JAUSoundTable.hpp"
#include "revolution/dvd.h"

u16 JAUStdSoundInfo::getBgmSeqResourceID(JAISoundID soundID) const {
    void* data = JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    if (data != nullptr) {
        switch (typeID & 0xF0) {
        case DATA_BGM:
            return ((JAUSoundTableBgm*)data)->mResourceId;
        default:
            break;
        }
    }
    return -1;
}

int JAUStdSoundInfo::getSoundType(JAISoundID soundID) const {
    switch (soundID.getSectionID()) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return -1;
    }
}

int JAUStdSoundInfo::getCategory(JAISoundID soundID) const {
    return soundID.getGroupID();
}

u16 JAUStdSoundInfo::getAudibleSw(JAISoundID soundID) const {
    void* data = JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    if (data != nullptr) {
        switch (typeID & 0xF0) {
        case DATA_SE:
            return ((JAUSoundTableSe*)data)->mAudibleSw;
        default:
            break;
        }
    }
    return -1;
}

u32 JAUStdSoundInfo::getPriority(JAISoundID soundID) const {
    JAUSoundTableItem* data = JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    if (data != nullptr) {
        if ((typeID & 0x40) != 0) {  // BGM, SE, or STREAM
            return data->mPriority;
        }
    }
    return 0;
}

void JAUStdSoundInfo::getSeInfo(JAISoundID soundID, JAISe* pSe) const {
    getSoundInfo_(soundID, pSe);
}

void JAUStdSoundInfo::getSeqInfo(JAISoundID soundID, JAISeq* pSeq) const {
    getSoundInfo_(soundID, pSeq);
}

void JAUStdSoundInfo::getStreamInfo(JAISoundID soundID, JAIStream* pStream) const {
    getSoundInfo_(soundID, pStream);

    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    switch (typeID & 0xF0) {
    case DATA_STREAM:
        break;
    default:
        return;
    }

    JAUSoundTableStream* data = (JAUSoundTableStream*)JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);

    u16 bits = data->mChannelCtrl;
    int numChild = pStream->getNumChild();
    for (int idx = 0; idx < numChild && bits != 0; idx++, bits >>= 2) {
        u16 ctrl = bits & 0b11;
        if (ctrl == 0) {
            continue;
        }

        JAISoundChild* child = pStream->getChild(idx);
        if (child == nullptr) {
            continue;
        }

        switch (ctrl) {
        case 1:
            child->mMove.mParams.mPan = 0.5f;
            break;
        case 2:
            child->mMove.mParams.mPan = 0.0f;
            break;
        case 3:
            child->mMove.mParams.mPan = 1.0f;
            break;
        }
    }
}

void JAUStdSoundInfo::getSoundInfo_(JAISoundID soundID, JAISound* pSound) const {
    JAUSoundTableItem* data = JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);
    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    if (data != nullptr) {
        if ((typeID & 0x40) != 0) {  // BGM, SE, or STREAM
            pSound->mParams.mProperty.mVolume = data->mVolume * (1.0f / 255.0f);
        }
    }
}

const char* JAUStdSoundInfo::getStreamFilePath(JAISoundID soundID) {
    u8 typeID = JASGlobalInstance< JAUSoundTable >::getInstance()->getTypeID(soundID);
    switch (typeID & 0xF0) {
    case DATA_STREAM: {
        JAUSoundTableStream* data = (JAUSoundTableStream*)JASGlobalInstance< JAUSoundTable >::getInstance()->getData(soundID);
        return (const char*)JASGlobalInstance< JAUSoundTable >::getInstance()->mTable.mData + (u32)data->mStreamFileNameOffset;
    }
    default:
        break;
    }
    return nullptr;
}

s32 JAUStdSoundInfo::getStreamFileEntry(JAISoundID soundID) {
    const char* path = getStreamFilePath(soundID);
    if (path == nullptr) {
        return -1;
    }
    return DVDConvertPathToEntrynum(path);
}
