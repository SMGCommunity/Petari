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

    s32 _0;
    s32 _4;
    s32 _8;
};
