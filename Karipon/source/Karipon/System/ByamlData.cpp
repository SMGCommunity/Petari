#include "Karipon/System/ByamlData.hpp"

const ByamlHashPair* ByamlHashIter::findPair(s32 key) const {
    if (!mData) {
        return nullptr;
    }

    const ByamlHashPair* pPairTable = getPairTable();

    s32 lowerBound = 0;
    s32 upperBound = getSize();

    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pPair = &pPairTable[avg];
        s32 result = key - pPair->getKey();
        if (result == 0) {
            return pPair;
        }

        if (result > 0) {
            lowerBound = avg + 1;
        } else {
            upperBound = avg;
        }
    }

    return 0;
}

bool ByamlHashIter::getDataByIndex(ByamlData* pData, s32 index) const {
    if (!mData) {
        return false;
    }

    if (static_cast<s32>(getSize()) < 1) {
        return false;
    }

    pData->set(&getPairTable()[index]);
    return true;
}

bool ByamlHashIter::getDataByKey(ByamlData* pData, s32 key) const {
    if (!mData) {
        return false;
    }

    if (static_cast<s32>(getSize()) < 1) {
        return false;
    }

    const ByamlHashPair* pPairTable = getPairTable();
    const ByamlHashPair* pPair = nullptr;
    s32 lowerBound = 0;
    s32 upperBound = getSize();

    if (lowerBound >= upperBound) {
        return false;
    }

    while (true) {
        s32 avg = (lowerBound + upperBound) / 2;
        pPair = &pPairTable[avg];
        s32 result = key - pPair->getKey();
        if (result == 0) {
            break;
        }

        if (result > 0) {
            lowerBound = avg + 1;
        } else {
            upperBound = avg;
        }

        if (lowerBound >= upperBound) {
            return false;
        }
    }

    if (pPair == nullptr) {
        return false;
    }

    pData->set(pPair);
    return true;
}

const ByamlHashPair* ByamlHashIter::getPairByIndex(s32 index) const {
    if (index < 0) {
        return nullptr;
    }

    if (static_cast<s32>(getSize()) <= index) {
        return nullptr;
    }

    return &getPairTable()[index];
}

const ByamlHashPair* ByamlHashIter::getPairTable() const {
    if (!mData) {
        return nullptr;
    }

    return reinterpret_cast<const ByamlHashPair*>(mData + 4);
}

u32 ByamlHashIter::getSize() const {
    if (!mData) {
        return 0;
    }

    u32 val = *reinterpret_cast<const u32*>(mData);
    return val & 0xFFFFFF;
}

bool ByamlArrayIter::getDataByIndex(ByamlData* pData, s32 index) const {
    if (index < 0) {
        return false;
    }

    if (static_cast<s32>(getSize()) <= index) {
        return false;
    }

    pData->set(static_cast<ByamlDataType>(getTypeTable()[index]), getDataTable()[index]);
    return true;
}

const u32* ByamlArrayIter::getDataTable() const {
    return reinterpret_cast<const u32*>(getOffsetData((getSize() + 7) & 0xFFFFFFFC));
}
