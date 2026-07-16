#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

JMapIdInfo::JMapIdInfo(s32 a1, const JMapInfoIter& rIter) {
    mZoneID = -1;
    initialize(a1, rIter);
}

JMapIdInfo::JMapIdInfo(s32 a1, s32 zoneId) {
    _0 = a1;
    mZoneID = zoneId;
}

void JMapIdInfo::initialize(s32 a1, const JMapInfoIter& rIter) {
    _0 = a1;
    mZoneID = MR::getPlacedZoneId(rIter);
}

namespace MR {
    JMapIdInfo createJMapIdInfoFromGroupId(const JMapInfoIter& rIter) {
        s32 groupId = -1;
        MR::getJMapInfoGroupID(rIter, &groupId);
        return JMapIdInfo(groupId, rIter);
    }

    JMapIdInfo createJMapIdInfoFromClippingGroupId(const JMapInfoIter& rIter) {
        s32 groupId = -1;
        MR::getJMapInfoClippingGroupID(rIter, &groupId);
        return JMapIdInfo(groupId, rIter);
    }
};  // namespace MR
