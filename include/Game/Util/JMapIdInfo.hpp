#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <revolution.h>

class JMapIdInfo {
public:
    inline JMapIdInfo() {
    }

    JMapIdInfo(s32 id, s32 zoneId);
    JMapIdInfo(s32 id, const JMapInfoIter& rIter);

    void initialize(s32 id, const JMapInfoIter& rIter);

    void operator=(const JMapIdInfo& rOther) NO_INLINE {
        mID = rOther.mID;
        mZoneID = rOther.mZoneID;
    }

    inline bool operator==(const JMapIdInfo& rOther) const {
        bool ret = false;

        if (mID == rOther.mID && mZoneID == rOther.mZoneID) {
            ret = true;
        }

        return ret;
    }

    /* 0x0 */ s32 mID;
    /* 0x4 */ s32 mZoneID;
};

namespace MR {
    JMapIdInfo createJMapIdInfoFromGroupId(const JMapInfoIter& rIter);
    JMapIdInfo createJMapIdInfoFromClippingGroupId(const JMapInfoIter& rIter);
};  // namespace MR
