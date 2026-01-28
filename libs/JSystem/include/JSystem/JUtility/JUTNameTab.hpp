#pragma once

#include <revolution.h>

struct ResNTAB {
    u16 mEntryNum;
    u16 _2;

    struct Entry {
        u16 mKeyCode;
        u16 mOffs;
    } mEntries[1];
};

class JUTNameTab {
public:
    JUTNameTab();
    JUTNameTab(const ResNTAB*);

    virtual ~JUTNameTab() {}

    void setResource(const ResNTAB*);
    s32 getIndex(const char*) const;
    const char* getName(u16) const;
    u16 calcKeyCode(const char*) const;

    const ResNTAB* mResource;  // 0x4
    const char* mStrData;      // 0x8
    u16 mNameNum;              // 0xC
    u16 _E;
};
