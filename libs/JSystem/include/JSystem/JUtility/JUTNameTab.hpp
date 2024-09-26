#pragma once

#include <revolution.h>

struct ResNTAB {
    u16 _0;
    u16 _2;
    u16 mEntries[0x2];  // 0x4
};

class JUTNameTab {
public:
    JUTNameTab();
    JUTNameTab(const ResNTAB *);

    virtual ~JUTNameTab();

    void setResource(const ResNTAB *);
    u16 getIndex(const char *) const;
    const char* getName(u16) const;
    u32 calcKeyCode(const char *) const;

    const ResNTAB* mResource;   // 0x4
    u8* _8;
    u16 _C;
    u16 _E;
};