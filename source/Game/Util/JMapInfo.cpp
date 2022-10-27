#include "Game/Util/JMapInfo.h"
#include "JSystem/JGadget/hashcode.h"

JMapInfo::JMapInfo() {
    mData = NULL;
    mName = "Undifined";
}

JMapInfo::~JMapInfo() {

}

bool JMapInfo::attach(const void *pData) {
    if (pData == NULL) {
        return false;
    }
    else {
        mData = static_cast<const JMapData*>(pData);
        return true;
    }
}

void JMapInfo::setName(const char *pName) {
    mName = pName;
}

const char* JMapInfo::getName() const {
    return mName;
}

// s32 JMapInfo::searchItemInfo(const char *pItem) const

s32 JMapInfo::getValueType(const char *pItem) const {
    s32 itemId = searchItemInfo(pItem);

    if (itemId < 0) {
        return JMAP_VALUE_TYPE_NULL;
    }
    else {
        return static_cast<const JMapItem*>(&mData->mItems)[itemId].mType;
    }
}
