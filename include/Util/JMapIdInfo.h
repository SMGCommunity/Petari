#pragma once

#include "JMap/JMapInfoIter.h"

class JMapIdInfo
{
public:
    JMapIdInfo() { }
    JMapIdInfo(s32 data, s32 zoneID)
    {
        mData = data;
        mZoneId = zoneID;
    }

    JMapIdInfo(s32, const JMapInfoIter &);
    JMapIdInfo(const JMapIdInfo &);

    void initialize(s32, const JMapInfoIter &);

    void operator=(const JMapIdInfo &);
    bool operator==(const JMapIdInfo &);

    s32 mData; // _0
    s32 mZoneId; // _4
};