#pragma once

#include <revolution.h>

#include "Game/Util/JMapInfo.h"

class JMapInfoIter {
public:
    template<typename T>
    bool getValue(const char *, T *) const;

    bool isValid() const;

    JMapInfo* mInfo; // _0
    u32 _4;
};
