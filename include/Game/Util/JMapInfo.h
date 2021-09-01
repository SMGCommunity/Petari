#pragma once

#include <revolution.h>

class JMapInfo {
public:
    JMapInfo();
    ~JMapInfo();

    void attach(const void *);
    s32 searchItemInfo(const char *) const;


    const void* _0;
    const char* mName; // _4
};