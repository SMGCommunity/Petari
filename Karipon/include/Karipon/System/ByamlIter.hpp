#pragma once

#include "Karipon/System/ByamlData.hpp"
#include "revolution/types.h"

class ByamlContainerHeader;
class ByamlData;
class ByamlFile;
class ByamlHeader;

class ByamlIter {
public:
    ByamlIter() : mFile(nullptr), mRootNode(nullptr) {}
    ByamlIter(const ByamlFile* pFile);
    ByamlIter(const ByamlFile* pFile, const u8* pRoot) : mFile(pFile), mRootNode(pRoot) {}

    bool isValid() const { return mFile != nullptr; }
    bool isTypeHash() const;
    bool isTypeArray() const;
    bool isTypeContainer() const;
    bool isExistKey(const char* pKey) const;
    s32 getKeyIndex(const char* pKey) const;
    s32 getSize() const;

    ByamlIter getIterByIndex(s32 index) const;
    bool getByamlDataByIndex(ByamlData* pData, s32 index) const;

    ByamlIter getIterByKey(const char* pKey) const;
    bool getByamlDataByKey(ByamlData* pData, const char* pKey) const;

    template <typename T>
    bool tryGetValueByIndex(T* pValue, s32 index) const {
        ByamlData data;

        if (!getByamlDataByIndex(&data, index)) {
            return false;
        }

        return tryConvertValue<T>(pValue, &data);
    }

    template <typename T>
    bool tryGetValueByKey(T* pValue, const char* pKey) const {
        ByamlData data;

        if (!getByamlDataByKey(&data, pKey)) {
            return false;
        }

        return tryConvertValue<T>(pValue, &data);
    }

    template <typename T>
    bool tryConvertValue(T* pValue, const ByamlData* pData) const;

    bool operator==(const ByamlIter& rOther) const { return mFile == rOther.mFile && mRootNode == rOther.mRootNode; }
    bool operator!=(const ByamlIter& rOther) const { return !(*this == rOther); }

    const ByamlFile* mFile;

    union {
        const u8* mRootNode;
        const ByamlContainerHeader* mContainingHeader;
    };
};
