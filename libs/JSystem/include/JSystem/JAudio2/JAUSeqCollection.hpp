#pragma once

#include "JSystem/JAudio2/JAISeqDataMgr.hpp"

struct JAISeqData;
struct JAISeqDataRegion;

struct JAUSeqCollectionData {
    s8 field_0x0;
    s8 field_0x1;
    u16 field_0x2;
    u32 field_0x4;
    u32 field_0x8;
};

class JAUSeqCollection {
public:
    JAUSeqCollection();
    void init(void const*);
    bool getSeqData(int, int, JAISeqData*);
    bool getSeqDataRegion(JAISeqDataRegion*);

    bool isValid() const {
        return mCollectionData != nullptr;
    }

    const JAUSeqCollectionData* getCollectionData() const {
        return mCollectionData;
    }

    /* 0x00 */ u16 field_0x0;
    /* 0x04 */ const u32* field_0x4;
    /* 0x08 */ const JAUSeqCollectionData* mCollectionData;
    /* 0x0C */ int field_0xc;
};

class JAUSeqDataMgr_SeqCollection : public JAISeqDataMgr, public JAUSeqCollection {
public:
    JAUSeqDataMgr_SeqCollection();
    bool setSeqDataUser(JAISeqDataUser*);
    int releaseSeqData();
    SeqDataReturnValue getSeqData(JAISoundID, JAISeqData*);
    ~JAUSeqDataMgr_SeqCollection();

    const void* getResource() const {
        return field_0x4;
    }
    void init(const void* pData) {
        JAUSeqCollection::init(pData);
    }

    /* 0x14 */ JAISeqDataUser* user_;
};
