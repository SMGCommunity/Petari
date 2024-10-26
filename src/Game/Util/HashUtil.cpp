#pragma once

#include "Game/Util/HashUtil.hpp"

HashSortTable::HashSortTable(u32 cnt) {
    mHashCodes = new u32[cnt];
    _8 = new u32[cnt];
    _C = new u32[0x80];
    _10 = new u32[0x80];
    _14 = 0;
    _18 = cnt;
    _0 = 0;
}

bool HashSortTable::add(const char *pName, u32 a2, bool noSkip) {
    u32 hash = MR::getHashCode(pName);

    if (!noSkip) {
        return addOrSkip(hash, a2);
    }
    else {
        return add(hash, a2);
    }
}

bool HashSortTable::add(u32 a1, u32 a2) {
    mHashCodes[_14] = a1;
    _8[_14] = a2;
    _14++;
    return true;
}

bool HashSortTable::addOrSkip(u32 a1, u32 a2) {
    for (u32 i = 0; i < _14; i++) {
       if (a1 == mHashCodes[i]) {
           return false;
       } 
    }

    mHashCodes[_14] = a1;
    _8[_14] = a2;
    _14++;
    return true;
}

s32 HashSortTable::search(const char *pStr, u32 *a2) {
    return search(MR::getHashCode(pStr), a2);
}

s32 HashSortTable::search(const char *pStr, const char *a2, u32 *a3) {
    u32 hashCode = MR::getHashCode(pStr) + MR::getHashCode(a2);
    return search(hashCode, a3);
}

namespace MR {
    u32 getHashCode(const char *pStr) {
        u32 val;
        for (val = 0; *pStr != 0; pStr++) {
            val = *pStr + (val * 31);
        }

        return val;
    }

};