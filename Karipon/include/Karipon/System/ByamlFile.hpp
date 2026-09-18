#pragma once

#include "Karipon/System/ByamlIter.hpp"
#include "revolution/types.h"

class ByamlHeader;
class ByamlStringTableIter;

class ByamlFile {
public:
    ByamlFile() : mData(nullptr) {}
    ByamlFile(const u8* pData) { attach(pData); }

    bool attach(const u8* pData);
    ByamlStringTableIter getHashKeyTable() const;
    ByamlStringTableIter getStringTable() const;
    ByamlIter getRootIter() const {
        return ByamlIter(this);
    }

    union {
        const u8* mData;
        const ByamlHeader* mHeader;
    };
};
