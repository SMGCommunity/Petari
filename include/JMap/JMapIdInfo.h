#pragma once

#include "JMap/JMapInfoIter.h"

class JMapIdInfo
{
public:
    JMapIdInfo(s64, const JMapInfoIter &);
    JMapIdInfo(s64, s64);
    JMapIdInfo(const JMapIdInfo &);

    void initialize(s64, const JMapInfoIter &);

    void operator=(const JMapIdInfo &);
    bool operator==(const JMapIdInfo &);

    s32 _0;
    s32 mZoneId; // _4
};