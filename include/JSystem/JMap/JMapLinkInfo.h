#pragma once

#include "JSystem/JMap/JMapInfoIter.h"

namespace
{
    const char* sInfoNameTable[0x3] = 
    {
        "mappartsinfo",
        "objinfo",
        "childobjinfo"
    };

    const char* sFollowInfoTagName[0x3] = 
    {
        "MapParts_ID",
        "Obj_ID",
        "ChildObjId"
    };
};

class JMapLinkInfo
{
public:
    JMapLinkInfo(const JMapInfoIter &, bool);

    bool isValid() const;
    void invalidate();
    void setLinkedInfo(const JMapInfoIter &);
    void setLinkInfo(const JMapInfoIter &);

    s32 mID; // _0 (l_id of object)
    s32 mZoneID; // _4
    s32 _8;
};
