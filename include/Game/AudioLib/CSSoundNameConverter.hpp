#pragma once

#include <revolution/types.h>

class SpkTable;

class CSSoundNameConverter {
public:
    /// @brief Creates a new `CSSoundNameConverter`.
    CSSoundNameConverter();

    void init();
    void initDataTable(const SpkTable*);
    void makeDataTable(const SpkTable*);
    s32 getSoundID(const char*) const;

private:
    /* 0x00 */ void* _0;
    /* 0x04 */ u32 _4;
};
