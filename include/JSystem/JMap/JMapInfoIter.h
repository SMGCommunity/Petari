#pragma once

#include <revolution.h>
#include "JSystem/JMap/JMapInfo.h"

class JMapInfoIter
{
public:
    inline JMapInfoIter() { }
    JMapInfoIter(const JMapInfo* info, int starting_position) : mInfo(info), mPos(starting_position)
    {}
    
    bool isValid() const;

    template<typename T>
    bool getValue(const char *, T *) const;

    const JMapInfo* mInfo; // _0
    s32 mPos; // _4
};

static char* unk_str = 0;
