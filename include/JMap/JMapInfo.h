#pragma once

#include "bcsv.h"
#include "types.h"

class JMapInfo
{
public:
    JMapInfo();
    JMapInfo(u32 val) : _0(val) { }
    ~JMapInfo() { }

    bool attach(const void *);
    void setName(const char *);
    const char* getName() const;
    s32 searchItemInfo(const char *) const;
    char getValueType(const char *) const;
    s32 getValueFast(s32, s32, const char **) const;

    u32 _0;
    const char* mName; // _4
};