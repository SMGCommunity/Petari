#pragma once

#include "revolution.h"

/// @brief Stores archive names of NameObjs.
class NameObjArchiveListCollector {
public:
    NameObjArchiveListCollector();

    void addArchive(const char *);
    const char* getArchive(s32) const;

    char mArchiveNames[0x20][0x40];     // _0
    s32 mCount;                         // _800
};