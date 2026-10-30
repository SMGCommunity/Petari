#include "Game/Util/MemoryUtil.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/Util/MutexHolder.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <JSystem/JAudio2/JASMutex.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <mem.h>

MEMAllocatorFunc MR::NewDeleteAllocator::sAllocatorFunc = {
    NewDeleteAllocator::alloc,
    NewDeleteAllocator::free,
};
MEMAllocator MR::NewDeleteAllocator::sAllocator = {&sAllocatorFunc};

MR::CurrentHeapRestorer::CurrentHeapRestorer(JKRHeap* pHeap) {
    _0 = JKRHeap::sCurrentHeap;
    OSLockMutex(&MR::MutexHolder< 1 >::sMutex);
    MR::becomeCurrentHeap(pHeap);
}

MR::CurrentHeapRestorer::~CurrentHeapRestorer() {
    MR::becomeCurrentHeap(_0);
    OSUnlockMutex(&MR::MutexHolder< 1 >::sMutex);
}

void* MR::NewDeleteAllocator::alloc(MEMAllocator* pAllocator, u32 size) {
    return new u8[size];
}

void MR::NewDeleteAllocator::free(MEMAllocator* pAllocator, void* pPtr) {
    delete static_cast< u8* >(pPtr);
}

MEMAllocator* MR::getHomeButtonLayoutAllocator() {
    JKRHeapAllocator< 0 >::sHeap = SingletonHolder< HeapMemoryWatcher >::get()->mHomeButtonLayoutHeap;

    return &JKRHeapAllocator< 0 >::sAllocator;
}

JKRHeap* MR::getCurrentHeap() {
    return JKRHeap::sCurrentHeap;
}

namespace {
    f32 getHeapFreeRate(JKRHeap* pHeap) {
        u32 heapSize = pHeap->mSize;

        return static_cast< f32 >(pHeap->getTotalFreeSize()) / heapSize;
    }
};  // namespace

JKRHeap* MR::getAproposHeapForSceneArchive(f32 heapFreeRateMax) {
    JKRHeap* pFileCacheHeap = SingletonHolder< HeapMemoryWatcher >::get()->mFileCacheHeap;

    if (pFileCacheHeap != nullptr) {
        if (::getHeapFreeRate(pFileCacheHeap) < heapFreeRateMax) {
            pFileCacheHeap = SingletonHolder< HeapMemoryWatcher >::get()->mSceneHeapGDDR;
        }
    }

    return pFileCacheHeap;
}

JKRExpHeap* MR::getStationedHeapNapa() {
    return SingletonHolder< HeapMemoryWatcher >::get()->mStationedHeapNapa;
}

JKRExpHeap* MR::getStationedHeapGDDR3() {
    return SingletonHolder< HeapMemoryWatcher >::get()->mStationedHeapGDDR;
}

JKRSolidHeap* MR::getSceneHeapNapa() {
    return SingletonHolder< HeapMemoryWatcher >::get()->mSceneHeapNapa;
}

JKRSolidHeap* MR::getSceneHeapGDDR3() {
    return SingletonHolder< HeapMemoryWatcher >::get()->mSceneHeapGDDR;
}

JKRHeap* MR::getHeapNapa(const JKRHeap* pHeap) {
    return SingletonHolder< HeapMemoryWatcher >::get()->getHeapNapa(pHeap);
}

JKRHeap* MR::getHeapGDDR3(const JKRHeap* pHeap) {
    return SingletonHolder< HeapMemoryWatcher >::get()->getHeapGDDR3(pHeap);
}

void MR::becomeCurrentHeap(JKRHeap* pHeap) {
    JASMutexLock lock(&MR::MutexHolder< 1 >::sMutex);
    pHeap->becomeCurrentHeap();
}

bool MR::isEqualCurrentHeap(JKRHeap* pHeap) {
    return JKRHeap::sCurrentHeap == pHeap;
}

void MR::adjustHeapSize(JKRExpHeap* pHeap, const char* pParam2) {
    pHeap->adjustSize();
}

void MR::copyMemory(void* pDst, const void* pSrc, u32 size) {
    u8* pDstBytes = static_cast< u8* >(pDst);
    const u8* pSrcBytes = static_cast< const u8* >(pSrc);
    u32 srcAlignment = reinterpret_cast< u32 >(pSrc) & 3;
    u32 dstAlignment = reinterpret_cast< u32 >(pDst) & 3;

    if (srcAlignment == dstAlignment && (size & 0xF) == 0) {
        const u32* pSrcWords = reinterpret_cast< const u32* >(pSrcBytes);
        u32* pDstWords = reinterpret_cast< u32* >(pDstBytes);

        for (u32 count = size / 16; count != 0; count--) {
            u32 word3, word2, word1, word0;
            word0 = *pSrcWords++;
            word1 = *pSrcWords++;
            word2 = *pSrcWords++;
            word3 = *pSrcWords++;
            *pDstWords++ = word0;
            *pDstWords++ = word1;
            *pDstWords++ = word2;
            *pDstWords++ = word3;
        }
    } else if (srcAlignment == dstAlignment && size >= 16) {
        if (srcAlignment != 0) {
            for (u8 count = 4 - srcAlignment; count != 0; count--) {
                *pDstBytes++ = *pSrcBytes++;
                size--;
            }
        }

        while (size >= 4) {
            *reinterpret_cast< u32* >(pDstBytes) = *reinterpret_cast< const u32* >(pSrcBytes);
            size -= 4;
            pSrcBytes += 4;
            pDstBytes += 4;
        }

        if (size != 0) {
            for (; size != 0; size--) {
                *pDstBytes++ = *pSrcBytes++;
            }
        }
    } else {
        for (; size != 0; size--) {
            *pDstBytes++ = *pSrcBytes++;
        }
    }
}

void MR::fillMemory(void* pDst, u8 ch, u32 size) {
    if (ch == 0) {
        zeroMemory(pDst, size);
    } else {
        memset(pDst, ch, size);
    }
}

void MR::zeroMemory(void* pDst, u32 size) {
    u8* pDstBytes = static_cast< u8* >(pDst);
    u32 alignment = reinterpret_cast< u32 >(pDst) & 3;

    if ((reinterpret_cast< u32 >(pDst) & 0x1F) == 0 && (size & 0x1F) == 0) {
        DCZeroRange(pDst, size);
    } else if (alignment == 0 && (size & 0xF) == 0) {
        for (u32 count = size / 16; count != 0; count--) {
            u32* pWords = reinterpret_cast< u32* >(pDstBytes);
            pWords[0] = 0;
            pWords[1] = 0;
            pWords[2] = 0;
            pWords[3] = 0;
            pDstBytes += 16;
        }
    } else if (size >= 16) {
        if (alignment != 0) {
            for (u8 count = 4 - alignment; count != 0; count--) {
                *pDstBytes++ = 0;
                size--;
            }
        }

        while (size >= 4) {
            *reinterpret_cast< u32* >(pDstBytes) = 0;
            pDstBytes += 4;
            size -= 4;
        }

        if (size != 0) {
            for (; size != 0; size--) {
                *pDstBytes++ = 0;
            }
        }
    } else {
        for (; size != 0; size--) {
            *pDstBytes++ = 0;
        }
    }
}

u32 MR::calcCheckSum(const void* pPtr, u32 size) {
    u16 sum;
    u16 invSum;

    invSum = 0;
    sum = 0;

    const u16* p = static_cast< const u16* >(pPtr);
    u32 checkSize = size / sizeof(u16);

    for (int i = 0; i < checkSize; i++, p++) {
        sum += *p;
        invSum += ~(*p);
    }

    return (sum << 16) | invSum;
}

void* MR::allocFromWPadHeap(u32 size) {
    return SingletonHolder< HeapMemoryWatcher >::get()->mWPadHeap->alloc(size, 0);
}

u8 MR::freeFromWPadHeap(void* pPtr) {
    SingletonHolder< HeapMemoryWatcher >::get()->mWPadHeap->free(pPtr);

    return 1;
}
