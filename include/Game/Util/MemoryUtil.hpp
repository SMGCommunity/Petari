#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"
#include <revolution/mem.h>

class JKRExpHeap;
class JKRSolidHeap;

namespace MR {
    class CurrentHeapRestorer {
    public:
        CurrentHeapRestorer(JKRHeap* pHeap);

        ~CurrentHeapRestorer();

    private:
        /* 0x00 */ JKRHeap* _0;
    };

    class NewDeleteAllocator {
    public:
        static void* alloc(MEMAllocator* pAllocator, u32 size);
        static void free(MEMAllocator* pAllocator, void* pPtr);

        static MEMAllocatorFunc sAllocatorFunc;
        static MEMAllocator sAllocator;
    };

    MEMAllocator* getHomeButtonLayoutAllocator();
    JKRHeap* getCurrentHeap();
    f32 getHeapFreeRatio(JKRHeap* pHeap);
    JKRHeap* getAproposHeapForSceneArchive(f32 maxFreeSizeRate);
    JKRExpHeap* getStationedHeapNapa();
    JKRExpHeap* getStationedHeapGDDR3();
    JKRSolidHeap* getSceneHeapNapa();
    JKRSolidHeap* getSceneHeapGDDR3();
    JKRHeap* getHeapNapa(const JKRHeap* pHeap);
    JKRHeap* getHeapGDDR3(const JKRHeap* pHeap);
    void becomeCurrentHeap(JKRHeap* pHeap) NO_INLINE;
    bool isEqualCurrentHeap(JKRHeap* pHeap);
    void adjustHeapSize(JKRExpHeap* pHeap, const char* pParam2);

    /// @brief Copies `size` bytes from `pSrc` to `pDst`.
    /// @param pDst The pointer to the memory to copy to.
    /// @param pSrc The pointer to the memory to copy from.
    /// @param size The number of bytes to copy.
    void copyMemory(void* pDst, const void* pSrc, u32 size);

    /// @brief Fills `size` bytes at `pDst` with `ch`.
    /// @param pDst The pointer to the memory to fill.
    /// @param ch The value to copy.
    /// @param size The number of bytes to fill.
    void fillMemory(void* pDst, u8 ch, u32 size);

    /// @brief Fills `size` bytes at `pDst` with `0`.
    /// @param pDst The pointer to the memory to fill.
    /// @param size The number of bytes to fill.
    void zeroMemory(void* pDst, u32 size);

    /// @brief Computes a checksum from the given block of memory.
    /// @param pPtr The pointer to the memory to process.
    /// @param size The number of bytes to process.
    /// @return The computed checksum.
    u32 calcCheckSum(const void* pPtr, u32 size);

    void* allocFromWPadHeap(u32 size);
    u8 freeFromWPadHeap(void* pPtr);

    template < int N >
    class JKRHeapAllocator {
    public:
        static void* alloc(MEMAllocator* pAllocator, u32 size);
        static void free(MEMAllocator* pAllocator, void* pPtr);

        static MEMAllocator sAllocator;
        static MEMAllocatorFunc sAllocatorFunc;
        static JKRHeap* sHeap;
    };

    template < int N >
    void* JKRHeapAllocator< N >::alloc(MEMAllocator* pAllocator, u32 size) {
        return JKRHeapAllocator< N >::sHeap->alloc(size, 0);
    }

    template < int N >
    void JKRHeapAllocator< N >::free(MEMAllocator* pAllocator, void* pPtr) {
        JKRHeapAllocator< N >::sHeap->free(pPtr);
    }

    template < int N >
    MEMAllocator JKRHeapAllocator< N >::sAllocator = {&sAllocatorFunc, nullptr, 4, 0};

    template < int N >
    MEMAllocatorFunc JKRHeapAllocator< N >::sAllocatorFunc = {
        JKRHeapAllocator::alloc,
        JKRHeapAllocator::free,
    };

    template < int N >
    JKRHeap* JKRHeapAllocator< N >::sHeap;
};  // namespace MR
