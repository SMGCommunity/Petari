#pragma once

#include <JSystem/JAudio2/JAIStreamDataMgr.hpp>
#include <JSystem/JAudio2/JASAramStream.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JAudio2/JAUStreamFileTable.hpp>
#include <bitset>

template < int N >
class JAUStreamAramMgrBase_ : public JAIStreamAramMgr {
public:
    JAUStreamAramMgrBase_() {
        for (u32 i = 0; i < N; i++) {
            mBits.reset(i);
        }
        mSize = 0;
    }

    virtual ~JAUStreamAramMgrBase_() {
        for (int i = N - 1; i >= 0; i--) {
            if (mHeaps[i].mBase != nullptr) {
                mHeaps[i].free();
            }
        }
    }

    virtual void* newStreamAram(u32*) = 0;
    virtual bool deleteStreamAram(u32) = 0;

    void alloc(JASHeap* pHeap, u32 size) {
        for (int i = 0; i < N; i++) {
            if (!mHeaps[i].alloc(pHeap, size)) {
                break;
            }
            mSize = i + 1;
        }
    }

    /* 0x04 */ std::bitset< N > mBits;
    /* 0x08 */ JASHeap mHeaps[N];
    /* 0x90 */ u32 mSize;
};

template < int N >
class JAUStreamStaticAramMgr_ : public JAUStreamAramMgrBase_< N > {
public:
    typedef JAUStreamAramMgrBase_< N > Base;
    virtual void* newStreamAram(u32* pSize) {
        for (s32 i = 0; i < Base::mSize; i++) {
            if (!Base::mBits.test(i)) {
                Base::mBits.set(i, true);
                *pSize = Base::mHeaps[i].mSize;
                return Base::mHeaps[i].mBase;
            }
        }
        return nullptr;
    }

    virtual bool deleteStreamAram(u32 addr) {
        for (s32 i = 0; i < Base::mSize; i++) {
            if (Base::mBits.test(i) && addr == (u32)Base::mHeaps[i].mBase) {
                Base::mBits.reset(i);
                return true;
            }
        }
        return false;
    }
};

class JAUStreamStaticAramMgr : public JAUStreamStaticAramMgr_< 2 > {
public:
};
