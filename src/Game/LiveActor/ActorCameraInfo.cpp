#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

ActorCameraInfo::ActorCameraInfo(const JMapInfoIter& rIter) : mCameraSetID(-1), mZoneID(0) {
    s32 cameraSetID = -1;

    if (MR::getJMapInfoCameraSetID(rIter, &cameraSetID)) {
        mCameraSetID = cameraSetID;
    }

    mZoneID = MR::getPlacedZoneId(rIter);
}

ActorCameraInfo::ActorCameraInfo(s32 cameraSetID, s32 zoneID) : mCameraSetID(cameraSetID), mZoneID(zoneID) {}
