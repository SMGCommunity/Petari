#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <locale.h>

// needed here for inlining reasons
inline int tolower(int c) {
    return ((c < 0) || (c >= 0x100)) ? c : (int)(_current_locale.ctype_cmpt_ptr->lower_map_ptr[c]);
}

HashSortTable::HashSortTable(u32 numMax) {
    mHashCodes = new u32[numMax];
    mValues = new u32[numMax];
    mBucketStartIndices = new u16[0x100];
    mBucketSizes = new u16[0x100];
    mTableSize = 0;
    mTableMaxSize = numMax;
    mlsSorted = false;
}

bool HashSortTable::add(const char* pName, u32 value, bool isValidSkip) {
    u32 hash = MR::getHashCode(pName);

    if (isValidSkip) {
        return addOrSkip(hash, value);
    } else {
        return add(hash, value);
    }
}

bool HashSortTable::add(u32 hash, u32 value) {
    mHashCodes[mTableSize] = hash;
    mValues[mTableSize] = value;
    mTableSize++;

    return true;
}

bool HashSortTable::addOrSkip(u32 hash, u32 value) {
    for (u32 i = 0; i < mTableSize; i++) {
        if (hash == mHashCodes[i]) {
            return false;
        }
    }

    mHashCodes[mTableSize] = hash;
    mValues[mTableSize] = value;
    mTableSize++;

    return true;
}

void HashSortTable::sort() {
    if (mTableSize == 0) {
        return;
    }

    s32 originalIndices[0x400];
    /*
    Once this function is complete, originalIndices[0] will contain the index of the lowest hash code in mHashCodeArray,
    originalIndices[1] will contain the index of the second-lowest hash code, and so on.
    */
    MR::sortSmall(mTableSize, mHashCodes, originalIndices);
    u32 swapArray[0x400];

    /*
    The values in mValues will be transferred here (in the order indicated in originalIndices)
    before being copied back into mValues.
    */
    for (int i = 0; i < mTableSize; i++) {
        swapArray[i] = mValues[originalIndices[i]];
    }

    for (int i = 0; i < mTableSize; i++) {
        mValues[i] = swapArray[i];
    }

    u32 i = 0;  //< Index in mHashCodes
    u32 currentByteValue;

    // Cycle through every possible byte (0x00 through 0xFF)
    for (currentByteValue = 0; currentByteValue < 0x100; currentByteValue++) {
        u32 upperByte = mHashCodes[i] >> 24;

        /*
        If the first hash code we encounter has an upper byte higher than currentByteValue,
        there is no hash code with an upper byte equal to currentByteValue.
        */
        if (upperByte > currentByteValue) {
            // No hash codes with this upper byte
            mBucketStartIndices[currentByteValue] = 0xFFFF;
            mBucketSizes[currentByteValue] = 0;
        } else if (upperByte == currentByteValue) {
            mBucketStartIndices[currentByteValue] = i;
            i++;
            u32 bucketSize = 1;

            /*
            Keep advancing through mHashCodes until we find a hash code whose upper byte is greater than currentByteValue.
            We'll keep track of how many hash codes we ended up finding with an upper byte equal to currentByteValue.
            Then, we can move on to the next currentByteValue.
            */
            while (i < mTableSize) {
                u32 upperByte = mHashCodes[i] >> 24;

                if (upperByte != currentByteValue) {
                    break;
                }

                bucketSize++;
                i++;
            }

            mBucketSizes[currentByteValue] = bucketSize;

            if (i == mTableSize) {
                currentByteValue++;
                break;
            }
        }
    }

    /*
    If we've gotten all the way to the end of mHashCodes and haven't gotten through all possible bytes,
    then there are zero hash codes whose upper bytes match any of the remaining possible bytes.
    */
    for (u32 i = currentByteValue; i < 0x100; i++) {
        mBucketStartIndices[i] = -1;
        mBucketSizes[i] = 0;
    }

    mlsSorted = true;
}

bool HashSortTable::search(u32 hashCode, u32* pDst) {
    u8 upperByte = hashCode >> 24;

    if (pDst != nullptr) {
        *pDst = 0;
    }

    u32 count = mBucketSizes[upperByte];

    if (count == 0) {
        return false;
    }

    u32* thing = &mHashCodes[mBucketStartIndices[upperByte]];

    for (int i = 0; i < count; i++) {
        if (*thing == hashCode) {
            if (pDst != nullptr) {
                *pDst = mValues[mBucketStartIndices[upperByte] + i];
            }

            return true;
        }

        if (*thing > hashCode) {
            return false;
        }

        thing++;
    }

    return false;
}

bool HashSortTable::search(const char* pName, u32* pDst) {
    u32 hash = MR::getHashCode(pName);

    return search(hash, pDst);
}

bool HashSortTable::search(const char* pName1, const char* pName2, u32* pDst) {
    u32 hash = MR::getHashCode(pName1) + MR::getHashCode(pName2);

    return search(hash, pDst);
}

void HashSortTable::swap(const char* pNameOld, const char* pNameNew) {
    u32 hash = MR::getHashCode(pNameOld);

    for (int i = 0; i < mTableSize; i++) {
        if (hash == mHashCodes[i]) {
            mHashCodes[i] = MR::getHashCode(pNameNew);
            return;
        }
    }

    mHashCodes[mTableSize] = hash;
}

namespace MR {
    u32 getHashCode(const char* pStr) {
        u32 hash;

        for (hash = 0; *pStr != '\0'; pStr++) {
            hash = *pStr + hash * 31;
        }

        return hash;
    }

    u32 getHashCodeLower(const char* pStr) {
        u32 hash;

        for (hash = 0; *pStr != '\0'; pStr++) {
            hash = tolower(*pStr) + hash * 31;
        }

        return hash;
    }
};  // namespace MR
