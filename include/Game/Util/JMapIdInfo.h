#pragma once

#include <revolution.h>
#include "Game/Util/JMapInfo.h"

class JMapIdInfo {
public:
    inline JMapIdInfo() {

    }

    JMapIdInfo(s32, s32);
    JMapIdInfo(s32, const JMapInfoIter &);
    JMapIdInfo(const JMapIdInfo &);

    void initalize(s32, const JMapInfoIter &);

    inline bool operator==(const JMapIdInfo &rOther) const {
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
    JMapIdInfo& createJMapIdInfoFromClippingGroupId(const JMapInfoIter &);
};
