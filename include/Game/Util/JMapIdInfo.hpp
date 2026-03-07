#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <revolution.h>

class JMapIdInfo {
public:
    inline JMapIdInfo() {}

    JMapIdInfo(s32, s32);
    JMapIdInfo(s32, const JMapInfoIter&);
    JMapIdInfo(const JMapIdInfo& rInf) {
        _0 = rInf._0;
        mZoneID = rInf.mZoneID;
    }

    void initialize(s32, const JMapInfoIter&);

    void operator=(const JMapIdInfo& rhs) NO_INLINE {
        _0 = rhs._0;
        mZoneID = rhs.mZoneID;
    }

    inline bool operator==(const JMapIdInfo& rOther) const {
        bool ret = false;

        if (_0 == rOther._0 && mZoneID == rOther.mZoneID) {
            ret = true;
        }

        return ret;
    }

    s32 _0;
    s32 mZoneID;
};

namespace MR {
    JMapIdInfo& createJMapIdInfoFromClippingGroupId(const JMapInfoIter&);
};
