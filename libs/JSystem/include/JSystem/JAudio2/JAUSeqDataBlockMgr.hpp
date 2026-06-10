#pragma once

#include "JSystem/JAudio2/JAISeqDataMgr.hpp"
#include "JSystem/JSupport/JSUList.hpp"

struct JAISeqData;
struct JAISeqDataUser;
class JKRArchive;

struct JAUSeqDataBlock {
    JAUSeqDataBlock();

    /* 0x00 */ JSULink< JAUSeqDataBlock > mLink;
    /* 0x10 */ JAISoundID mSoundID;
    /* 0x14 */ JAISeqDataRegion region;
    /* 0x1C */ int field_0x1c;
};

class JAUSeqDataBlocks : public JSUList< JAUSeqDataBlock > {
public:
    u8* getSeqData(JAISoundID);
    JSULink< JAUSeqDataBlock >* seekFreeBlock(u32);
    bool append(JSULink< JAUSeqDataBlock >*);
    bool remove(JSULink< JAUSeqDataBlock >*);
    bool hasFailedBlock(JAISoundID);
};

class JAUDynamicSeqDataBlocks {
public:
    JAUDynamicSeqDataBlocks();
    void setSeqDataArchive(JKRArchive*);
    s32 getSeqData(JAISoundID, JAISeqDataUser*, JAISeqData*, bool);
    bool appendDynamicSeqDataBlock(JAUSeqDataBlock*);
    bool loadDynamicSeq(JAISoundID, bool, JAISeqDataUser*);
    u32 releaseIdleDynamicSeqDataBlock(JAISeqDataUser*);
    JAUSeqDataBlock* releaseIdleDynamicSeqDataBlock_(JAISeqDataUser*, u32);
    void rearrangeLoadingSeqs_();

    JAUSeqDataBlocks const& getFreeBlocks() const {
        return mFreeBlocks;
    }
    JAUSeqDataBlocks const& getLoadedBlocks() const {
        return mLoadedBlocks;
    }
    JKRArchive* getSeqDataArchive() {
        return mSeqDataArchive;
    }

    /* 0x00 */ JAUSeqDataBlocks mFreeBlocks;
    /* 0x0C */ JAUSeqDataBlocks field_0xc;
    /* 0x18 */ JAUSeqDataBlocks mLoadedBlocks;
    /* 0x24 */ JKRArchive* mSeqDataArchive;
};
