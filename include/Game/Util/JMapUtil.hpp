#pragma once

#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>

namespace MR {
    bool isValidInfo(const JMapInfoIter& rIter);
    bool isObjectName(const JMapInfoIter& rIter, const char* pName);
    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pFieldName, s32*);
    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pFieldName, f32*);
    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pFieldName, bool*);
    bool getJMapInfoTrans(const JMapInfoIter& rIter, TVec3f*);
    bool getJMapInfoRotate(const JMapInfoIter& rIter, TVec3f*);
    bool getJMapInfoMatrixFromRT(const JMapInfoIter& rIter, TPos3f*);
    bool getJMapInfoArg0WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg0WithInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg0WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg1WithInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg1WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg2WithInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg2WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg3WithInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg3WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg4WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg7WithInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg0NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg0NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg0NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg1NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg1NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg1NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg2NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg2NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg2NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg3NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg3NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg3NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg4NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg4NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg4NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg5NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg5NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg5NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg6NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg6NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg6NoInit(const JMapInfoIter& rIter, bool*);
    bool getJMapInfoArg7NoInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg7NoInit(const JMapInfoIter& rIter, f32*);
    bool getJMapInfoArg7NoInit(const JMapInfoIter& rIter, bool*);
    bool isEqualObjectName(const JMapInfoIter& rIter, const char*);
    s32 getDemoGroupID(const JMapInfoIter& rIter);
    s32 getDemoGroupLinkID(const JMapInfoIter& rIter);
    bool getJMapInfoRailArg(const JMapInfoIter& rIter, const char*, s32*);
    bool getJMapInfoRailArg0NoInit(const JMapInfoIter& rIter, s32*);
    bool getRailId(const JMapInfoIter& rIter, s32*);
    bool getObjectName(const char** pDest, const JMapInfoIter& rIter);
    bool isExistJMapArg(const JMapInfoIter& rIter);
    bool getJMapInfoShapeIdWithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoTransLocal(const JMapInfoIter& rIter, TVec3f*);
    bool getJMapInfoRotateLocal(const JMapInfoIter& rIter, TVec3f*);
    bool getJMapInfoScale(const JMapInfoIter& rIter, TVec3f*);
    bool getJMapInfoV3f(const JMapInfoIter& rIter, const char*, TVec3f*);
    bool getJMapInfoArg1WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg2WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg3WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg4WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg5WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg6WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoArg7WithInit(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoFollowID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoGroupID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoClippingGroupID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoDemoGroupID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoLinkID(const JMapInfoIter& rIter, s32*);
    bool isConnectedWithRail(const JMapInfoIter& rIter);
    bool isExistStageSwitchA(const JMapInfoIter& rIter);
    bool isExistStageSwitchB(const JMapInfoIter& rIter);
    bool isExistStageSwitchAppear(const JMapInfoIter& rIter);
    bool isExistStageSwitchDead(const JMapInfoIter& rIter);
    bool isExistStageSwitchSleep(const JMapInfoIter& rIter);
    bool getJMapInfoCameraSetID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoViewGroupID(const JMapInfoIter& rIter, s32*);
    bool getJMapInfoMessageID(const JMapInfoIter& rIter, s32*);
    s32 getDemoCastID(const JMapInfoIter& rIter);
    const char* getDemoName(const JMapInfoIter& rIter);
    const char* getDemoSheetName(const JMapInfoIter& rIter);
    bool getNextLinkRailID(const JMapInfoIter& rIter, s32*);
    bool isEqualRailUsage(const JMapInfoIter& rIter, const char*);
    void getRailPointPos0(const JMapInfoIter& rIter, TVec3f*);
    void getRailPointPos1(const JMapInfoIter& rIter, TVec3f*);
    void getRailPointPos2(const JMapInfoIter& rIter, TVec3f*);
    bool isLoopRailPathIter(const JMapInfoIter& rIter);

    template < typename T >
    inline bool getValue(const JMapInfoIter& rIter, const char* pName, T* pOut) {
        return rIter.getValue< T >(pName, pOut);
    }

    inline s32 getMessageID(const JMapInfoIter& rIter) {
        s32 msgId;
        getJMapInfoMessageID(rIter, &msgId);
        return msgId;
    }

    inline bool checkJMapDataEntries(const JMapInfoIter& rIter) {
        bool flag;
        bool ret;

        ret = false;
        flag = false;

        if (rIter.mInfo != nullptr && rIter.mIndex >= 0) {
            flag = true;
        }

        if (flag) {
            s32 numEntries;

            if (rIter.mInfo->mData != nullptr) {
                numEntries = rIter.mInfo->mData->mNumEntries;
            } else {
                numEntries = 0;
            }

            if (rIter.mIndex < numEntries) {
                ret = true;
            }
        }

        return ret;
    }
};  // namespace MR
