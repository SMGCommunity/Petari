#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

JMapIdInfo::JMapIdInfo(s32 id, const JMapInfoIter& rIter) {
    mZoneID = -1;
    initialize(id, rIter);
}

JMapIdInfo::JMapIdInfo(s32 id, s32 zoneId) {
    mID = id;
    mZoneID = zoneId;
}

void JMapIdInfo::initialize(s32 id, const JMapInfoIter& rIter) {
    mID = id;
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
