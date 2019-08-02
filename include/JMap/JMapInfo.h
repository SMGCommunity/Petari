#pragma once

#include "bcsv.h"
#include "types.h"

class JMapInfo
{
public:
    JMapInfo();
    ~JMapInfo();

    bool attach(const void *);
    void setName(const char *);
    const char* getName() const;
    s32 searchItemInfo(const char *) const;
    char getValueType(const char *) const;

    const BCSVHeader* mMap;
    const char* mName;
};