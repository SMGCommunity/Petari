#pragma once

#include <JSystem/JAudio2/JAIStreamDataMgr.hpp>
#include <JSystem/JAudio2/JASAramStream.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JAudio2/JAUStreamFileTable.hpp>
#include <bitset>

template < size_t N >
class JAUStreamAramMgrBase_ : public JAIStreamAramMgr {
public:
    JAUStreamAramMgrBase_() {
        for (int i = 0; i < N; i++) {
            mBits.reset(i);
        }
        mSize = 0;
    }

    virtual void* newStreamAram(u32*) = 0;
    virtual bool deleteStreamAram(u32) = 0;

    void alloc(JASHeap* pHeap, u32 size) {  // TODO name better?
        for (int i = 0; i < N; i++) {
            if (!mHeaps[i].alloc(pHeap, size)) {
                break;
            }
            mSize++;
        }
    }

    /* 0x04 */ std::bitset< N > mBits;
    /* 0x08 */ JASHeap mHeaps[N];
    /* 0x90 */ u32 mSize;
};

template < size_t N >
class JAUStreamStaticAramMgr_ : public JAUStreamAramMgrBase_< N > {
public:
    virtual void* newStreamAram(u32*);
    virtual bool deleteStreamAram(u32);
};

class JAUStreamStaticAramMgr : public JAUStreamStaticAramMgr_< 2 > {
public:
    virtual void* newStreamAram(u32*);
    virtual bool deleteStreamAram(u32);
};
