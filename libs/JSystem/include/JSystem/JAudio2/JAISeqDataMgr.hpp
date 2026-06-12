#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include <stdint.h>

struct JAISeqData {
    JAISeqData(const void* _addr, u32 _size) {
        addr = (void*)_addr;
        size = _size;
    }

    void set(const void* _addr, u32 _size) {
        addr = (void*)_addr;
        size = _size;
    }

    /* 0x00 */ void* addr;
    /* 0x04 */ u32 size;
};

struct JAISeqDataRegion {
    bool intersects(const JAISeqData& seqData) const {
        if ((uintptr_t)addr + size < (uintptr_t)seqData.addr) {
            return false;
        }
        if ((uintptr_t)seqData.addr + seqData.size < (uintptr_t)addr) {
            return false;
        }
        return true;
    }

    /* 0x00 */ u8* addr;
    /* 0x04 */ u32 size;
};

struct JAISeqDataUser {
    virtual ~JAISeqDataUser();
    virtual bool isUsingSeqData(JAISeqDataRegion const&) = 0;
    virtual int releaseSeqData(JAISeqDataRegion const&) = 0;
};

struct JAISeqDataMgr {
    enum SeqDataReturnValue { SeqDataReturnValue_0 = 0, SeqDataReturnValue_1 = 1, SeqDataReturnValue_2 = 2 };
    virtual ~JAISeqDataMgr();
    virtual SeqDataReturnValue getSeqData(JAISoundID, JAISeqData*) = 0;
    virtual int releaseSeqData() = 0;
    virtual bool setSeqDataUser(JAISeqDataUser*) = 0;
};
