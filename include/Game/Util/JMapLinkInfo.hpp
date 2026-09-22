#pragma once

#include "Game/Util/JMapInfo.hpp"

class JMapLinkInfo {
public:
    JMapLinkInfo(const JMapInfoIter&, bool);

    bool operator==(const JMapLinkInfo& rOther) const {
        if (!isValid() || !rOther.isValid()) {
            return false;
        }

        return _0 == rOther._0 && _4 == rOther._4 && _8 == rOther._8;
    }

    void invalidate();
    bool isValid() const;
    void setLinkedInfo(const JMapInfoIter&);
    void setLinkInfo(const JMapInfoIter&);

    /* 0x00 */ s32 _0;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 _8;
};
