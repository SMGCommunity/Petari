#pragma once

#include "Game/Util/JMapInfo.h"

namespace {
    static const char* sFollowInfoTagName[3] = {
        "MapParts_ID",
        "Obj_ID",
        "ChildObjId"
    };

    static const char* sInfoNameTable[3] = {
        "mappartsinfo",
        "objinfo",
        "childobjinfo"
    };
};

class JMapLinkInfo {
public:
    JMapLinkInfo(const JMapInfoIter &, bool);

    void invalidate();
    void setLinkedInfo(const JMapInfoIter &);
    void setLinkInfo(const JMapInfoIter &);

    s32 _0;
    s32 _4;
    s32 _8;
};