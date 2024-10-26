#include "Game/LiveActor/ActorCameraInfo.hpp"

ActorCameraInfo::ActorCameraInfo(const JMapInfoIter &rIter) {
    mCameraSetID = -1;
    mZoneID = 0;

    s32 cameraID = -1;
    bool ret = MR::getJMapInfoCameraSetID(rIter, &cameraID);

    if (!ret) {
        mCameraSetID = cameraID;
    }

    mZoneID = MR::getPlacedZoneId(rIter);
}

ActorCameraInfo::ActorCameraInfo(s32 cameraID, s32 zoneID) {
    mCameraSetID = cameraID;
    mZoneID = zoneID;
}
