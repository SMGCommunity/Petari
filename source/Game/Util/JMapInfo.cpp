#include "Game/Util/JMapInfo.h"
#include "JSystem/JGadget/hashcode.h"

/*
s32 JMapInfo::searchItemInfo(const char *pName) const {
    if (mData - sizeof(JMapData) == 0) {
        return -1;
    }

    s32 num = mData->mNumData;
    u32 hash = JGadget::getHashCode(pName);

    for (s32 i = 0; i < num; i++) {
        if (hash == mData->mItems[i].mHash) {
            return i;
        }
    }
 
    return -1;
}*/