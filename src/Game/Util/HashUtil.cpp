#pragma once

#include "Game/Util/HashUtil.hpp"
#include "Game/util/MathUtil.hpp"
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

bool HashSortTable::add(const char *pName, u32 a2, bool Skip) {
    u32 hash = MR::getHashCode(pName);

    if (Skip) {
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

void HashSortTable::sort(){
    if(mCurrentLength == 0){
        return;
    }
    s32 originalIndices[0x400];
    MR::sortSmall(mCurrentLength,mHashCodes,&(originalIndices[0]));
    u32 swapArray[0x400];
    for(int i = 0; i < mCurrentLength; i++){
        swapArray[i] = _8[originalIndices[i]];
    }
    for(int i = 0; i < mCurrentLength; i++){
        _8[i] = swapArray[i];
    }
    
    u32 counter2 = 0;
    u32 counter;
    for(counter = 0; counter < 0x100; counter++){
        u32 upperByte = mHashCodes[counter2] >>24;
        if(upperByte > counter){
            _C[counter] = 0xFFFF;
            _10[counter] = 0;
        }else if(upperByte == counter){
            _C[counter] = counter2;
            counter2++;
            u32 counter3 = 1;
            while(counter2 < mCurrentLength){
                u32 upperByte = mHashCodes[counter2] >> 24;
                if(upperByte != counter){
                    break;
                }
                counter3++;
                counter2++;
            }
            _10[counter] = counter3;
            if(counter2 == mCurrentLength){
                counter++;
                break;
            }
        }
    }
    for(u32 i = counter; i < 0x100; i++){
        _C[i] = 0xFFFF;
        _10[i] = 0;
    }
    mHasBeenSorted = true;
}

s32 HashSortTable::search(u32 a1, u32 *a2) {
    u8 upperByte = a1 >> 24;
    if(a2 != nullptr){
        *a2 = 0;
    }
    u32 count = _10[upperByte];
    if(count == 0){
        return 0;
    }

    u32* thing = &mHashCodes[_C[upperByte]];
    for(int i = 0; i < count; i++){
        if(*thing == a1){
            if(a2 != nullptr){
                *a2 = _8[_C[upperByte]+i];
            }
            return 1;
        }
        if(*thing > a1){
            return 0;
        }
        thing++;
    }
    return 0;
}

s32 HashSortTable::search(const char *pStr, u32 *a2) {
    return search(MR::getHashCode(pStr), a2);
}

s32 HashSortTable::search(const char *pStr, const char *a2, u32 *a3) {
    u32 hashCode = MR::getHashCode(pStr) + MR::getHashCode(a2);
    return search(hashCode, a3);
}

void HashSortTable::swap(const char *pStr1, const char *pStr2){
    u32 hashCode = MR::getHashCode(pStr1);
    for(int i = 0; i < mCurrentLength; i++){
        if (mHashCodes[i] == hashCode){
            mHashCodes[i] = MR::getHashCode(pStr2);
            return;
        }
    }
    // does not increment mCurrentLength?
    mHashCodes[mCurrentLength] = hashCode;
}

namespace MR {
    u32 getHashCode(const char *pStr) {
        u32 val;
        for (val = 0; *pStr != 0; pStr++) {
            val = *pStr + (val * 31);
        }

        return val;
    }

    u32 getHashCodeLower(const char *pStr){
        u32 val;
        for (val = 0; *pStr != 0; pStr++) {
            val = tolower(*pStr) + (val * 31);
        }
        
        return val;
    }
};
