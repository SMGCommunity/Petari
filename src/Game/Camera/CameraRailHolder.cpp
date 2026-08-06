#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

CameraRailHolder::CameraRailHolder(const char* pName) : NameObj(pName), mRails() {
    mRails = new ZoneRail[MR::getZoneNum()];

    for (s32 zoneId = 0; zoneId < MR::getZoneNum(); zoneId++) {
        for (s32 railId = 0; railId < MR::getPlacedRailNum(zoneId); railId++) {
            JMapInfoIter iter;
            const JMapInfo* info;
            if (!MR::getCameraRailInfoFromRailDataIndex(&iter, &info, railId, zoneId)) {
                continue;
            }

            s32 arg;
            if (MR::getJMapInfoRailArg0NoInit(iter, &arg) && arg >= 0) {
                mRails[zoneId].mNumRails++;
            }
        }

        if (mRails[zoneId].mNumRails == 0) {
            continue;
        }

        mRails[zoneId].mRails = new RailSet[mRails[zoneId].mNumRails];
        s32 railNum = 0;
        for (s32 railId = 0; railId < MR::getPlacedRailNum(zoneId); railId++) {
            JMapInfoIter iter;
            const JMapInfo* info;
            if (!MR::getCameraRailInfoFromRailDataIndex(&iter, &info, railId, zoneId)) {
                continue;
            }

            s32 arg;
            if (MR::getJMapInfoRailArg0NoInit(iter, &arg) && arg >= 0) {
                mRails[zoneId].mRails[railNum].mRailId = arg;
                s32 linkId;
                iter.getValue("l_id", &linkId);
                mRails[zoneId].mRails[railNum].mRailRider = new RailRider(linkId, zoneId);
                railNum++;
            }
        }

        for (s32 railId = 0; railId < mRails[zoneId].mNumRails - 1; railId++) {
            s32 sortId = railId;
            for (s32 j = railId + 1; j < mRails[zoneId].mNumRails; j++) {
                if (mRails[zoneId].mRails[sortId].mRailId > mRails[zoneId].mRails[j].mRailId) {
                    sortId = j;
                }
            }

            RailSet temp = mRails[zoneId].mRails[railId];
            mRails[zoneId].mRails[railId] = mRails[zoneId].mRails[sortId];
            mRails[zoneId].mRails[sortId] = temp;
        }
    }
}

RailRider* CameraRailHolder::getRider(s32 zoneId, s32 railId) const {
    s32 index = find(zoneId, railId);
    if (index >= 0) {
        return mRails[zoneId].mRails[index].mRailRider;
    }

    return nullptr;
}

s32 CameraRailHolder::find(s32 zoneId, s32 railId) const {
    s32 start = 0;
    s32 end = mRails[zoneId].mNumRails - 1;
    while (start <= end) {
        s32 idx = (start + end) / 2;
        if (railId == mRails[zoneId].mRails[idx].mRailId) {
            return idx;
        }
        if (railId > mRails[zoneId].mRails[idx].mRailId) {
            start = idx + 1;
        } else {
            end = idx - 1;
        }
    }
    return -1;
}
