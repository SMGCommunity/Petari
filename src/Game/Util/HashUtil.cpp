#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cctype>

#define BUCKET_SIZE_MAX 256

HashSortTable::HashSortTable(u32 numMax) {
    mHashArray = new u32[numMax];
    mValueArray = new u32[numMax];
    mBucketArray = new u16[BUCKET_SIZE_MAX];
    mBucketSizeArray = new u16[BUCKET_SIZE_MAX];
    mTableSize = 0;
    mTableSizeMax = numMax;
    mIsSorted = false;
}

bool HashSortTable::add(const char* pName, u32 value, bool isValidSkip) {
    u32 hash = MR::getHashCode(pName);

    if (isValidSkip) {
        return addOrSkip(hash, value);
    }

    return add(hash, value);
}

bool HashSortTable::add(u32 hash, u32 value) {
    mHashArray[mTableSize] = hash;
    mValueArray[mTableSize] = value;
    mTableSize++;

    return true;
}

bool HashSortTable::addOrSkip(u32 hash, u32 value) {
    for (u32 i = 0; i < mTableSize; i++) {
        if (hash == mHashArray[i]) {
            return false;
        }
    }

    mHashArray[mTableSize] = hash;
    mValueArray[mTableSize] = value;
    mTableSize++;

    return true;
}

void HashSortTable::sort() {
    if (mTableSize == 0) {
        return;
    }

    s32 hashIndexTable[1024];
    u32 valueSortTable[1024];

    MR::sortSmall(mTableSize, mHashArray, hashIndexTable);

    for (int i = 0; i < mTableSize; i++) {
        valueSortTable[i] = mValueArray[hashIndexTable[i]];
    }

    for (int i = 0; i < mTableSize; i++) {
        mValueArray[i] = valueSortTable[i];
    }

    u32 i = 0;
    u32 currentBucketId;

    for (currentBucketId = 0; currentBucketId < BUCKET_SIZE_MAX; currentBucketId++) {
        u32 bucketId = mHashArray[i] >> 24;

        if (bucketId > currentBucketId) {
            mBucketArray[currentBucketId] = -1;
            mBucketSizeArray[currentBucketId] = 0;
        } else if (bucketId == currentBucketId) {
            mBucketArray[currentBucketId] = i++;
            u32 bucketSize;

            for (bucketSize = 1; i < mTableSize; bucketSize++, i++) {
                u32 bucketId = mHashArray[i] >> 24;

                if (bucketId != currentBucketId) {
                    break;
                }
            }

            mBucketSizeArray[currentBucketId] = bucketSize;

            if (i == mTableSize) {
                currentBucketId++;
                break;
            }
        }
    }

    for (u32 i = currentBucketId; i < BUCKET_SIZE_MAX; i++) {
        mBucketArray[i] = -1;
        mBucketSizeArray[i] = 0;
    }

    mIsSorted = true;
}

bool HashSortTable::search(u32 hash, u32* pDst) {
    u8 bucketId = hash >> 24;

    if (pDst != nullptr) {
        *pDst = 0;
    }

    u32 bucketSize = mBucketSizeArray[bucketId];

    if (bucketSize == 0) {
        return false;
    }

    u32* bucket = &mHashArray[mBucketArray[bucketId]];

    for (int i = 0; i < bucketSize; i++) {
        if (*bucket == hash) {
            if (pDst != nullptr) {
                *pDst = mValueArray[mBucketArray[bucketId] + i];
            }

            return true;
        }

        if (*bucket > hash) {
            return false;
        }

        bucket++;
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

void HashSortTable::swap(const char* pOldName, const char* pNewName) {
    u32 oldHash = MR::getHashCode(pOldName);

    for (int i = 0; i < mTableSize; i++) {
        if (oldHash == mHashArray[i]) {
            mHashArray[i] = MR::getHashCode(pNewName);
            return;
        }
    }

    mHashArray[mTableSize] = oldHash;
}

u32 MR::getHashCode(const char* pStr) {
    u32 hash;

    for (hash = 0; *pStr != '\0'; pStr++) {
        hash = *pStr + hash * 31;
    }

    return hash;
}

u32 MR::getHashCodeLower(const char* pStr) {
    u32 hash;

    for (hash = 0; *pStr != '\0'; pStr++) {
        hash = tolower(*pStr) + hash * 31;
    }

    return hash;
}
