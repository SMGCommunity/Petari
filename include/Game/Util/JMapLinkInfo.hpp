#pragma once

#include "Game/Util/JMapInfo.hpp"

class JMapLinkInfo {
public:
    JMapLinkInfo(const JMapInfoIter&, bool);

    void invalidate();
    void setLinkedInfo(const JMapInfoIter&);
    void setLinkInfo(const JMapInfoIter&);

    s32 _0;
    s32 _4;
    s32 _8;
};
