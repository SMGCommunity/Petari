#include "Karipon/System/ByamlFile.hpp"
#include "Karipon/System/ByamlHeader.hpp"
#include "revolution/types.h"

bool ByamlFile::attach(const u8* pData) {
    const ByamlHeader* pHeader = reinterpret_cast<const ByamlHeader*>(pData);

    if (pHeader->getTag() == 'BY' && (static_cast<s32>(pHeader->getVersion()) - 1) < 3) {
        mData = pData;
        return true;
    }

    mData = nullptr;
    return false;
}

ByamlStringTableIter ByamlFile::getHashKeyTable() const {
    s32 offset = mHeader->getHashKeyTableOffset();
    if (offset == 0) {
        return ByamlStringTableIter();
    }

    return ByamlStringTableIter(&mData[offset]);
}

ByamlStringTableIter ByamlFile::getStringTable() const {
    s32 offset = mHeader->getStringTableOffset();
    if (offset == 0) {
        return ByamlStringTableIter();
    }

    return ByamlStringTableIter(&mData[offset]);
}
