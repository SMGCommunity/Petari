#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <locale.h>

//needed here for inlining reasons
inline int tolower(int c) {
    return ((c < 0) || (c >= 0x100)) ? c : (int) (_current_locale.ctype_cmpt_ptr->lower_map_ptr[c]);
}

HashSortTable::HashSortTable(u32 cnt) {
    mHashCodes = new u32[cnt];
    _8 = new u32[cnt];
    _C = new u16[0x100];
    _10 = new u16[0x100];
    mCurrentLength = 0;
    mMaxLength = cnt;
    mHasBeenSorted = false;
}

bool HashSortTable::add(const char *pName, u32 a2, bool isValidSkip) {
    u32 hash = MR::getHashCode(pName);

    if (isValidSkip) {
        return addOrSkip(hash, a2);
    }
    else {
        return add(hash, a2);
    }
}

bool HashSortTable::add(u32 a1, u32 a2) {
    mHashCodes[mCurrentLength] = a1;
    _8[mCurrentLength] = a2;
    mCurrentLength++;

    return true;
}

bool HashSortTable::addOrSkip(u32 a1, u32 a2) {
    for (u32 i = 0; i < mCurrentLength; i++) {
       if (a1 == mHashCodes[i]) {
           return false;
       }
    }

    mHashCodes[mCurrentLength] = a1;
    _8[mCurrentLength] = a2;
    mCurrentLength++;

    return true;
}

void HashSortTable::sort() {
    if (mCurrentLength == 0) {
        return;
    }

    s32 originalIndices[0x400];
    MR::sortSmall(mCurrentLength, mHashCodes, originalIndices);
    u32 swapArray[0x400];

    for (int i = 0; i < mCurrentLength; i++) {
        swapArray[i] = _8[originalIndices[i]];
    }

    for (int i = 0; i < mCurrentLength; i++) {
        _8[i] = swapArray[i];
    }

    u32 counter2 = 0;
    u32 counter;

    for (counter = 0; counter < 0x100; counter++) {
        u32 upperByte = mHashCodes[counter2] >> 24;

        if (upperByte > counter) {
            _C[counter] = 0xFFFF;
            _10[counter] = 0;
        }
        else if (upperByte == counter) {
            _C[counter] = counter2;
            counter2++;
            u32 counter3 = 1;

            while (counter2 < mCurrentLength) {
                u32 upperByte = mHashCodes[counter2] >> 24;

                if (upperByte != counter) {
                    break;
                }

                counter3++;
                counter2++;
            }

            _10[counter] = counter3;

            if (counter2 == mCurrentLength) {
                counter++;
                break;
            }
        }
    }

    for (u32 i = counter; i < 0x100; i++) {
        _C[i] = 0xFFFF;
        _10[i] = 0;
    }

    mHasBeenSorted = true;
}

bool HashSortTable::search(u32 a1, u32 *a2) {
    u8 upperByte = a1 >> 24;

    if (a2 != nullptr) {
        *a2 = 0;
    }

    u32 count = _10[upperByte];

    if (count == 0) {
        return false;
    }

    u32* thing = &mHashCodes[_C[upperByte]];

    for (int i = 0; i < count; i++) {
        if (*thing == a1) {
            if (a2 != nullptr) {
                *a2 = _8[_C[upperByte] + i];
            }

            return true;
        }

        if (*thing > a1) {
            return false;
        }

        thing++;
    }

    return false;
}

bool HashSortTable::search(const char *a1, u32 *a2) {
    u32 hash = MR::getHashCode(a1);

    return search(hash, a2);
}

bool HashSortTable::search(const char *a1, const char *a2, u32 *a3) {
    u32 hash = MR::getHashCode(a1) + MR::getHashCode(a2);

    return search(hash, a3);
}

namespace MR {
    u32 getHashCode(const char *pStr) {
        u32 hash;

        for (hash = 0; *pStr != '\0'; pStr++) {
            hash = *pStr + hash * 31;
        }

        return hash;
    }

    u32 getHashCodeLower(const char *pStr) {
        u32 hash;

        for (hash = 0; *pStr != '\0'; pStr++) {
            hash = tolower(*pStr) + hash * 31;
        }

        return hash;
    }
};
