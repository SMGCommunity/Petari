#pragma once

#include <revolution/types.h>

class SpkTable;

struct CSSoundNameHashData {
    /* 0x00 */ const char* mName;
    /* 0x04 */ u32 mSoundID;
    /* 0x08 */ s32 mHash;
};

class CSSoundNameConverter {
public:
    /// @brief Creates a new `CSSoundNameConverter`.
    CSSoundNameConverter();

    void init() NO_INLINE;
    void initDataTable(const SpkTable*);
    void makeDataTable(const SpkTable*);
    u32 getSoundID(const char*) const;

    /* 0x00 */ int mNumSounds;
    /* 0x04 */ CSSoundNameHashData* mSoundHashes;
};
