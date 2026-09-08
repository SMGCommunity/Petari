#pragma once

#include <revolution/types.h>

/// @brief Stores archive names of NameObjs.
class NameObjArchiveListCollector {
public:
    /// @brief Creates a new `NameObjArchiveListCollector`.
    NameObjArchiveListCollector();

    void addArchive(const char*);
    const char* getArchive(s32) const;

    s32 getArchiveNum() const {
        return mArchiveNum;
    }

private:
    /* 0x000 */ char mArchive[32][64];
    /* 0x800 */ s32 mArchiveNum;
};
