#pragma once

#include <revolution.h>

struct ResNTAB
{
    u16 _0;
    char _4;
};

class JUTNameTab
{
public:
    JUTNameTab(const ResNTAB *);

    virtual ~JUTNameTab();

    void setResource(const ResNTAB *);
    s16 getIndex(const char *) const;
    const char* getName(u16) const;
    u16 calcKeyCode(const char *) const;

    const ResNTAB* mTab; // _4
    u32 _8;
    u16 mIdxCount; // _C
};