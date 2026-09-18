#include "Karipon/System/ByamlIter.hpp"
#include "Karipon/System/ByamlContainerHeader.hpp"
#include "Karipon/System/ByamlData.hpp"
#include "Karipon/System/ByamlHeader.hpp"
#include "Karipon/System/ByamlFile.hpp"
#include <cstring>

ByamlIter::ByamlIter(const ByamlFile* pFile) : mFile(pFile), mRootNode(nullptr) {
    u32 dataOffset = pFile->mHeader->getDataOffset();
    if (dataOffset == 0) {
        return;
    }

    mRootNode = &pFile->mData[dataOffset];
}

bool ByamlIter::isExistKey(const char* pKey) const {
    if (mRootNode == nullptr || *mRootNode != BYAML_TYPE_HASH) {
        return false;
    }

    s32 index = getKeyIndex(pKey);
    if (index < 0) {
        return false;
    }

    ByamlHashIter iter(mRootNode);
    return iter.findPair(index);
}

s32 ByamlIter::getKeyIndex(const char* pKey) const {
    ByamlStringTableIter hash = mFile->getHashKeyTable();
    if (!hash.isValidate()) {
        return -1;
    }

    return hash.findStringIndex(pKey);
}

s32 ByamlIter::getSize() const {
    if (mRootNode == nullptr) {
        return false;
    }

    if (mContainingHeader->getType() == BYAML_TYPE_ARRAY || mContainingHeader->getType() == BYAML_TYPE_HASH) {
        return mContainingHeader->getCount();
    }

    return 0;
}

ByamlIter ByamlIter::getIterByIndex(s32 index) const {
    ByamlData data;

    if (!getByamlDataByIndex(&data, index)) {
        return ByamlIter();
    }

    if (data.getType() != BYAML_TYPE_ARRAY && data.getType() != BYAML_TYPE_HASH) {
        if (data.getType() == BYAML_TYPE_NULL) {
            return ByamlIter(mFile, 0);
        }

        return ByamlIter();
    }

    return ByamlIter(mFile, &mFile->mData[data.getValue()]);
}

bool ByamlIter::getByamlDataByIndex(ByamlData* pData, s32 index) const {
    if (mRootNode == nullptr) {
        return false;
    }

    if (*mRootNode == BYAML_TYPE_ARRAY) {
        ByamlArrayIter iter(mRootNode);
        return iter.getDataByIndex(pData, index);
    }

    if (*mRootNode == BYAML_TYPE_HASH) {
        ByamlHashIter iter(mRootNode);
        return iter.getDataByIndex(pData, index);
    }

    return false;
}

ByamlIter ByamlIter::getIterByKey(const char* pKey) const {
    ByamlData data;

    if (!getByamlDataByKey(&data, pKey)) {
        return ByamlIter();
    }

    if (data.getType() != BYAML_TYPE_ARRAY && data.getType() != BYAML_TYPE_HASH) {
        if (data.getType() == BYAML_TYPE_NULL) {
            return ByamlIter(mFile, 0);
        }

        return ByamlIter();
    }

    return ByamlIter(mFile, &mFile->mData[data.getValue()]);
}

bool ByamlIter::getByamlDataByKey(ByamlData* pData, const char* pKey) const {
    if (mRootNode == nullptr || *mRootNode != BYAML_TYPE_HASH) {
        return false;
    }

    ByamlStringTableIter hashTable = mFile->getHashKeyTable();
    if (!hashTable.isValidate()) {
        return false;
    }

    ByamlHashIter iter(mRootNode);
    s32 lowerBound = 0;
    s32 upperBound = iter.getSize();

    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pPair = iter.getPairByIndex(avg);
        s32 result = strcmp(pKey, hashTable.getString(pPair->getKey()));
        if (result == 0) {
            pData->set(pPair);
            return true;
        }

        if (result > 0) {
            lowerBound = avg + 1;
        } else {
            upperBound = avg;
        }
    }

    return false;
}

template<>
bool ByamlIter::tryConvertValue(const char** pValue, const ByamlData* pData) const {
    if (pData->getType() != BYAML_TYPE_STRING) {
        return false;
    }

    ByamlStringTableIter stringTable = mFile->getStringTable();
    if (!stringTable.isValidate()) {
        return false;
    }

    *pValue = stringTable.getString(pData->getValue());
    return true;
}

template<>
bool ByamlIter::tryConvertValue(bool* pValue, const ByamlData* pData) const {
    if (pData->getType() != BYAML_TYPE_BOOL) {
        return false;
    }

    *pValue = pData->getValue() != 0;
    return true;
}

template<>
bool ByamlIter::tryConvertValue(s32* pValue, const ByamlData* pData) const {
    if (pData->getType() != BYAML_TYPE_INT) {
        return false;
    }

    *pValue = pData->getValue();
    return true;
}

template<>
bool ByamlIter::tryConvertValue(u32* pValue, const ByamlData* pData) const {
    s32 val = pData->getValue<s32>();

    if (pData->getType() == BYAML_TYPE_INT) {
        if (val >= 0) {
            *pValue = val;
            return true;
        }

        return false;
    }

    if (pData->getType() == BYAML_TYPE_UINT) {
        *pValue = val;
        return true;
    }

    return false;
}

template<>
bool ByamlIter::tryConvertValue(f32* pValue, const ByamlData* pData) const {
    if (pData->getType() != BYAML_TYPE_FLOAT) {
        return false;
    }

    *pValue = pData->getValue<f32>();
    return true;
}
