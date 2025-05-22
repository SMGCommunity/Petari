#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGadget/hashcode.hpp"

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
        if ((&mData->mItems + i)->mHash == hash) {
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
    else {
        return static_cast<const JMapItem*>(&mData->mItems)[itemId].mType;
    }
}
