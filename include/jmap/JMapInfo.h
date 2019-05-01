#pragma once

#include "types.h"

class JMapInfo
{
public:
    JMapInfo();
    ~JMapInfo();

    bool attach(const void *);
    void setName(const char *);
    char* getName() const;
    s32 searchItemInfo(const char *) const;
    char getValueType(const char *) const;

    void* mMap;
    char* mName;
};