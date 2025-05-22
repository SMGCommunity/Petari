#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGadget/hashcode.hpp"
#include <cstring>

JMapInfo::JMapInfo() {
    mData = nullptr;
    mName = "Undifined";
}

JMapInfo::~JMapInfo() {

}

bool JMapInfo::attach(const void *pData) {
    if (pData == nullptr) {
        return false;
    }
    mData = static_cast<const JMapData*>(pData);
    return true;
}

void JMapInfo::setName(const char *pName) {
    mName = pName;
}

const char* JMapInfo::getName() const {
    return mName;
}

s32 JMapInfo::searchItemInfo(const char *key) const {
    if (!mData) {
        return -1;
    }

    s32 nFields = mData ? mData->mNumFields : 0;
    u32 hash = JGadget::getHashCode(key);

    for (int i = 0; i < nFields; ++i) {
        if ((&mData->mItems)[i].mHash == hash) {
            return i;
        }
    }
    
    return -1;
}

s32 JMapInfo::getValueType(const char *pItem) const {
    s32 itemId = searchItemInfo(pItem);

    if (itemId < 0) {
        return JMAP_VALUE_TYPE_NULL;
    }
    return (&mData->mItems)[itemId].mType;
}

JMapInfoIter JMapInfo::findElementBinary(const char *key, const char *value) const {
    int low = 0;
    int high = mData ? mData->mNumEntries : 0;

    const char *nextVal;
    while (low < high) {
        int mid = (low + high) / 2;
        getValue<const char *>(mid, key, &nextVal);
        int comparison = strcmp(nextVal, value);
        if (comparison == 0) {
            return JMapInfoIter(this, mid);
        }
        else if (comparison < 0) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }
    return end();
}
