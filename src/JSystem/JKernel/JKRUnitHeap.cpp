#include "JSystem/JKernel/JKRUnitHeap.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include <new>

static const u8 bitTable[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

JKRUnitHeap* JKRUnitHeap::create(u32 unitSize, u32 size, u32 alignment, JKRHeap* parent, bool errorFlag) {
    if (alignment == 0) {
        alignment = 4;
    }
    if (alignment > unitSize) {
        unitSize = alignment;
    }
    u32 alignedUnitSize = ALIGN_NEXT(unitSize, alignment);

    if (parent == nullptr) {
        parent = sRootHeap;
    }
    if (size == 0xFFFFFFFF) {
        size = parent->getMaxAllocatableSize(8);
    }
    if (size < sizeof(JKRUnitHeap)) {
        return nullptr;
    }

    u8* memory = static_cast< u8* >(JKRHeap::alloc(size, 0, parent));
    if (memory == nullptr) {
        return nullptr;
    }

    u8* bat = memory + sizeof(JKRUnitHeap);
    u32 unitCount = ALIGN_NEXT(((size - sizeof(JKRUnitHeap)) * 8) / (alignedUnitSize * 8 + 1), 8);
    if (unitCount == 0) {
        JKRHeap::free(memory, nullptr);
        return nullptr;
    }

    u8* memoryEnd = memory + size;
    u8* units = reinterpret_cast< u8* >(ALIGN_NEXT(reinterpret_cast< u32 >(bat + ALIGN_NEXT(unitCount / 8, 4)), alignment));
    u32 availableSize = memoryEnd - units;
    unitCount = availableSize / alignedUnitSize;
    JKRUnitHeap* heap = new (memory) JKRUnitHeap(bat, units, alignedUnitSize, unitCount, availableSize, alignment, parent, errorFlag);
    if (heap == nullptr) {
        JKRHeap::free(memory, nullptr);
    }
    return heap;
}

void JKRUnitHeap::do_destroy() {
    JKRHeap* parent = getParent();
    if (parent != nullptr) {
        this->~JKRUnitHeap();
        JKRHeap::free(this, parent);
    }
}

JKRUnitHeap::JKRUnitHeap(u8* bat, u8* units, u32 unitSize, u32 unitCount, u32 availableSize, u32 alignment, JKRHeap* parent, bool errorFlag)
    : JKRHeap(units, unitSize * unitCount, parent, errorFlag) {
    mUnitSize = unitSize;
    mUnitCount = unitCount;
    mAlignment = alignment;
    mBat = bat;
    mUnits = units;
    mAllocMode = 1;
    clearBatArea();
    mTotalFreeSize = mUnitSize * mUnitCount;
}

JKRUnitHeap::~JKRUnitHeap() {
    dispose();
}

void JKRUnitHeap::clearBatArea() {
    u8* bat = mBat;
    for (int i = 0; i < static_cast< s32 >(mUnitCount) / 8; ++i) {
        *bat++ = 0;
    }

    if (mUnitCount & 7) {
        *bat = 0;
        for (int bit = mUnitCount & 7; bit < 8; ++bit) {
            *bat |= bitTable[bit];
        }
        ++bat;
    }

    while (reinterpret_cast< u32 >(bat) & 3) {
        *bat++ = 0xFF;
    }
}

s32 JKRUnitHeap::find1FreeBlock(int direction) {
    if (direction >= 0) {
        u32 wordCount = (mUnitCount + 31) / 32;
        u32 word;
        u32* words = reinterpret_cast< u32* >(mBat);
        for (u32 index = 0; index < wordCount; ++index, ++words) {
            word = *words;
            if (word != 0xFFFFFFFF) {
                for (int bit = 0; bit < 32; ++bit) {
                    if (!(word & (0x80000000 >> bit))) {
                        return index * 32 + bit;
                    }
                }
            }
        }
    } else {
        u32 word;
        u32* words = reinterpret_cast< u32* >(mBat + ALIGN_NEXT(mUnitCount, 32) / 8) - 1;
        for (int index = (mUnitCount + 31) / 32 - 1; index >= 0; --index, --words) {
            word = *words;
            if (word != 0xFFFFFFFF) {
                for (int bit = 31; bit >= 0; --bit) {
                    if (!(word & (0x80000000 >> bit))) {
                        return index * 32 + bit;
                    }
                }
            }
        }
    }
    return -1;
}

s32 JKRUnitHeap::findFreeBlock(int idx, u32 a2) {
    if (a2 == 1) {
        return find1FreeBlock(idx);
    }

    if (idx >= 0) {
        return findFreeBlock_fromHead(a2);
    }

    return findFreeBlock_fromTail(a2);
}

s32 JKRUnitHeap::findFreeBlock_fromHead(u32 count) {
    u32* begin = reinterpret_cast< u32* >(mBat);
    bool inFreeRun = false;
    s32 runStart = -1;
    s32 runLength = 0xFFFF;
    s32 bestStart = -1;
    s32 bestLength = count == 0 ? 0 : 0xFFFF;
    u32* end = begin + (mUnitCount + 31) / 32;

    for (u32* words = begin; words <= end; ++words) {
        u32 word = words != end ? *words : 0xFFFFFFFF;
        if (word == 0) {
            if (!inFreeRun) {
                runStart = (words - reinterpret_cast< u32* >(mBat)) * 32;
                runLength = 0;
                inFreeRun = true;
            }
            runLength += 32;
            continue;
        }
        if (word == 0xFFFFFFFF && !inFreeRun) {
            continue;
        }

        for (int bit = 0; bit < 32; ++bit) {
            if (!(word & (0x80000000 >> bit))) {
                if (!inFreeRun) {
                    runStart = (words - reinterpret_cast< u32* >(mBat)) * 32 + bit;
                    runLength = 0;
                    inFreeRun = true;
                }
                ++runLength;
            } else if (inFreeRun) {
                if (count == 0) {
                    if (runLength > bestLength) {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                } else if (runLength == count) {
                    return runStart;
                } else if (runLength > count) {
                    if (mAllocMode == 1) {
                        return runStart;
                    }
                    if (runLength < bestLength) {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                }
                inFreeRun = false;
            }
        }
    }
    if (count == 0) {
        bestStart = bestLength;
    }
    return bestStart;
}

s32 JKRUnitHeap::findFreeBlock_fromTail(u32 count) {
    u32* begin = reinterpret_cast< u32* >(mBat);
    u32* end = begin - 1;
    bool inFreeRun = false;
    s32 runStart = -1;
    s32 runLength = 0xFFFF;
    s32 bestStart = -1;
    s32 bestLength = 0xFFFF;

    for (u32* words = begin + (mUnitCount + 31) / 32 - 1; words >= end; --words) {
        u32 word = words != end ? *words : 0xFFFFFFFF;
        if (word == 0) {
            if (!inFreeRun) {
                runStart = (words - reinterpret_cast< u32* >(mBat)) * 32 + 31;
                runLength = 0;
                inFreeRun = true;
            }
            runLength += 32;
            continue;
        }
        if (word == 0xFFFFFFFF && !inFreeRun) {
            continue;
        }

        for (int bit = 31; bit >= 0; --bit) {
            if (!(word & (0x80000000 >> bit))) {
                if (!inFreeRun) {
                    runStart = (words - reinterpret_cast< u32* >(mBat)) * 32 + bit;
                    runLength = 0;
                    inFreeRun = true;
                }
                ++runLength;
            } else if (inFreeRun) {
                if (runLength == count) {
                    return runStart - runLength + 1;
                } else if (runLength > count) {
                    if (mAllocMode == 1) {
                        return runStart - count + 1;
                    }
                    if (runLength < bestLength) {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                }
                inFreeRun = false;
            }
        }
    }
    return bestStart == -1 ? -1 : bestStart - count + 1;
}

void JKRUnitHeap::do_freeAll() {
    clearBatArea();
    mTotalFreeSize = mUnitSize * mUnitCount;
}

void JKRUnitHeap::do_freeTail() {
    return;
}

void JKRUnitHeap::do_fillFreeArea() {
    return;
}

s32 JKRUnitHeap::do_resize(void*, u32) {
    return -1;
}

s32 JKRUnitHeap::do_getSize(void* pAddr) {
    s32 idx = addressToIndex(pAddr);

    if (static_cast< u8 >(isUnitUsed(idx))) {
        return mUnitSize;
    }

    return 0;
}

s32 JKRUnitHeap::do_getFreeSize() {
    return mUnitSize * findFreeBlock_fromHead(0);
}

void* JKRUnitHeap::do_getMaxFreeBlock() {
    return indexToAddress(findFreeBlock_fromHead(0));
}

s32 JKRUnitHeap::do_getTotalFreeSize() {
    return mTotalFreeSize;
}

void* JKRUnitHeap::indexToAddress(int idx) {
    if (idx < 0 || idx >= mUnitCount) {
        return nullptr;
    }

    return &mUnits[mUnitSize * idx];
}

s32 JKRUnitHeap::addressToIndex(void* address) {
    s32 offset = static_cast< u8* >(address) - mUnits;
    if (offset < 0) {
        return -1;
    }
    u32 index = offset / mUnitSize;
    if (offset != index * mUnitSize) {
        return -1;
    }
    return index < mUnitCount ? index : -1;
}

BOOL JKRUnitHeap::isUnitUsed(int index) const {
    if (index < 0 || index >= mUnitCount) {
        return false;
    }
    return (bitTable[index & 7] & mBat[index / 8]) ? 1 : 0;
}

void JKRUnitHeap::setUnitUsed(int index) {
    if (index < 0 || index >= mUnitCount) {
        return;
    }
    mBat[index / 8] |= bitTable[index & 7];
}

bool JKRUnitHeap::check() {
    u32 freeSize = 0;
    int byteCount = (static_cast< s32 >(mUnitCount) + 7) / 8;
    u32 bits;
    u8* bat = mBat;
    for (int index = 0; index < byteCount; ++index, ++bat) {
        bits = *bat;
        if (bits == 0xFF) {
            continue;
        }
        if (bits == 0) {
            freeSize += mUnitSize * 8;
        } else {
            for (int bit = 0; bit < 8; ++bit) {
                if (!(bits & bitTable[bit])) {
                    freeSize += mUnitSize;
                }
            }
        }
    }

    if (freeSize != mTotalFreeSize) {
        JUTWarningConsole_f("check: bad total memory block size (%08X, %08X)\n", mTotalFreeSize, freeSize);
    }
    return true;
}

bool JKRUnitHeap::dump() {
    bool result = check();
    lock();
    u32 usedSize = mSize - mTotalFreeSize;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used\n", usedSize, mSize, static_cast< f32 >(usedSize) / static_cast< f32 >(mSize) * 100.0f);
    unlock();
    return result;
}

void JKRUnitHeap::state_register(TState* state, u32 id) const {
    setState_u32ID_(state, id);
    setState_uUsedSize_(state, mSize - const_cast< JKRUnitHeap* >(this)->getTotalFreeSize());
    u32 checkCode = 0;
    for (u32* word = reinterpret_cast< u32* >(mBat); word < reinterpret_cast< u32* >(mUnits); ++word) {
        checkCode += *word;
    }
    setState_u32CheckCode_(state, checkCode);
}

bool JKRUnitHeap::state_compare(const TState& first, const TState& second) const {
    bool result = true;
    if (first.getCheckCode() != second.getCheckCode()) {
        result = false;
    }
    if (first.getUsedSize() != second.getUsedSize()) {
        result = false;
    }
    return result;
}

u32 JKRUnitHeap::getHeapType() {
    return 'UNIT';
}
