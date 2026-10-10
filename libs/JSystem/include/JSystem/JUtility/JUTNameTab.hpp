#pragma once

#include <revolution.h>

struct ResNTAB {
    /* 0x0 */ u16 mEntryNum;
    /* 0x2 */ u16 _2;

    /* 0x4 */ struct Entry {
        u16 mKeyCode;
        u16 mOffs;
    } mEntries[1];
};

class JUTNameTab {
public:
    JUTNameTab();
    JUTNameTab(const ResNTAB*);

    virtual ~JUTNameTab() {
    }

    void setResource(const ResNTAB*);
    s32 getIndex(const char*) const;
    const char* getName(u16) const;
    u16 calcKeyCode(const char*) const;

    /* 0x4 */ const ResNTAB* mResource;
    /* 0x8 */ const char* mStrData;
    /* 0xC */ u16 mNameNum;
    /* 0xE */ u16 _E;
};
