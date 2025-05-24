#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGadget/hashcode.hpp"

JMapInfo::JMapInfo() {
    mData = nullptr;
    mName = "Undifined";
}

JMapInfo::~JMapInfo() {

}

bool JMapInfo::attach(const void* pData) {
    if (pData == nullptr) {
        return false;
    }
    mData = static_cast<const JMapData*>(pData);
    return true;
}

void JMapInfo::setName(const char* pName) {
    mName = pName;
}

const char* JMapInfo::getName() const {
    return mName;
}

s32 JMapInfo::searchItemInfo(const char* pKey) const {
    if (!dataExists()) {
        return -1;
    }

    s32 nFields = getNumFields();
    u32 hash = JGadget::getHashCode(pKey);

    for (int i = 0; i < nFields; ++i) {
        if (mData->mItems[i].mHash == hash) {
            return i;
        }
    }
    
    return -1;
}

s32 JMapInfo::getValueType(const char* pItem) const {
    s32 itemId = searchItemInfo(pItem);

    if (itemId < 0) {
        return JMAP_VALUE_TYPE_NULL;
    }
    return mData->mItems[itemId].mType;
}

bool JMapInfo::getValueFast(int entryIndex, int itemIndex, const char** pValueOut) const {
    const JMapItem* item = &mData->mItems[itemIndex];
    const char* valuePtr = getEntryAddress(mData, mData->mDataOffset, entryIndex) + item->mOffsData;

    switch (item->mType) {
        case JMAP_VALUE_TYPE_STRING_PTR:
            const char* pStringTable = getEntryAddress(mData, mData->mDataOffset, getNumEntries());
            *pValueOut = pStringTable + *reinterpret_cast<const u32*>(valuePtr);
            break;
        default:
            *pValueOut = valuePtr;
            break;
    }

    return true;
}

bool JMapInfo::getValueFast(int entryIndex, int itemIndex, u32* pValueOut) const {
    const JMapItem* item = &mData->mItems[itemIndex];
    const char* valuePtr = getEntryAddress(mData, mData->mDataOffset, entryIndex) + item->mOffsData;

    u32 rawValue;
    switch (item->mType) {
        case JMAP_VALUE_TYPE_LONG:
        case JMAP_VALUE_TYPE_LONG_2:
            rawValue = *reinterpret_cast<const u32*>(valuePtr);
            break;
        case JMAP_VALUE_TYPE_SHORT:
            rawValue = *reinterpret_cast<const u16*>(valuePtr);
            break;
        case JMAP_VALUE_TYPE_BYTE:
            rawValue = *reinterpret_cast<const u8*>(valuePtr);
            break;
        default:
            return false;
    }

    *pValueOut = (rawValue & item->mMask) >> item->mShift;
    return true;
}

bool JMapInfo::getValueFast(int entryIndex, int itemIndex, s32* pValueOut) const {
    const JMapItem* item = &mData->mItems[itemIndex];
    if (item->mShift != 0) {
        goto FAIL;
    }
    const char* valuePtr = getEntryAddress(mData, mData->mDataOffset, entryIndex) + item->mOffsData;
    
    switch (item->mType) {
        case JMAP_VALUE_TYPE_LONG:
            if (item->mMask == 0xffffffff) {
                *pValueOut = *reinterpret_cast<const s32*>(valuePtr);
                break;
            }
            goto FAIL;
        case JMAP_VALUE_TYPE_SHORT:
            if (item->mMask == 0xffff) {
                *pValueOut = *reinterpret_cast<const s16*>(valuePtr);
                break;
            }
            goto FAIL;
        case JMAP_VALUE_TYPE_BYTE:
            if (item->mMask == 0xff) {
                *pValueOut = *reinterpret_cast<const s8*>(valuePtr);
                break;
            }
            goto FAIL;
        default:
            goto FAIL;
    }
    return true;
FAIL:
    return false;
}

JMapInfoIter MR::findJMapInfoElementNoCase(const JMapInfo* pInfo, const char* key, const char* searchValue, int startIndex) {
    int entryIndex = startIndex;
    const char* value;
    while (entryIndex < pInfo->getNumEntries()) {
        pInfo->getValue<const char*>(entryIndex, key, &value);
        if (MR::isEqualStringCase(value, searchValue)) {
            return JMapInfoIter(pInfo, entryIndex);
        }
        entryIndex++;
    }
    return pInfo->end();
}

JMapInfoIter JMapInfo::findElementBinary(const char* key, const char* value) const {
    int low = 0;
    int high = getNumEntries();

    const char* nextVal;
    while (low < high) {
        int mid = (low + high) / 2;
        getValue<const char*>(mid, key, &nextVal);
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
