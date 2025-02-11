#include "Game/Util/JMapUtil.hpp"
#include "Game/Util.hpp"
#include "math_types.hpp"
#include <cstdio>
#include <cmath>

namespace {
    bool getJMapInfoRailArg(const JMapInfoIter &rIter, const char *pName, s32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = val;
            return true;
        }

        return false;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, s32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = val;
            return true;
        }

        return false;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, f32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = ::getJMapInfoArgNoInit(rIter, pName, &val);

        if (!hasValue) {
            return false;
        }

        *pOut = val;
        return true;
    } 

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, bool *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = true;
        }
        else {
            *pOut = false;
        }

        return true;
    }
};

namespace MR {
    bool isValidInfo(const JMapInfoIter &rIter) {
        return rIter.isValid();
    }

    bool isObjectName(const JMapInfoIter &rIter, const char *pName) {
        const char *objName = nullptr;
        if (MR::getObjectName(&objName, rIter)) {
            return MR::isEqualString(pName, objName);
        }

        return false;
    }

    inline bool getArgAndInit(const JMapInfoIter &rIter, const char *pName, s32 *pOut) {
        *pOut = -1;
        return ::getJMapInfoArgNoInit(rIter, pName, pOut);
    }

    inline bool getArgAndInit(const JMapInfoIter &rIter, const char *pName, f32 *pOut) {
        *pOut = -1.0f;
        return ::getJMapInfoArgNoInit(rIter, pName, pOut);
    }

    inline bool getArgAndInit(const JMapInfoIter &rIter, const char *pName, bool *pOut) {
        *pOut = false;
        return ::getJMapInfoArgNoInit(rIter, pName, pOut);
    }
};

namespace MR {
    bool getJMapInfoTrans(const JMapInfoIter &rIter, TVec3f *pOut) {
        if (!getJMapInfoTransLocal(rIter, pOut)) {
            return false;
        }

        if (isPlacementLocalStage()) {
            getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
        }

        return true;
    }

    bool getJMapInfoRotate(const JMapInfoIter &rIter, TVec3f *pOut) {
        if (!getJMapInfoRotateLocal(rIter, pOut)) {
            return false;
        }

        if (isPlacementLocalStage()) {
            TMtx34f rotateMtx;
            makeMtxRotate(rotateMtx.toMtxPtr(), *pOut);
            TMtx34f* placementMtx = getZonePlacementMtx(rIter);
            rotateMtx.concat(*placementMtx, rotateMtx);

            if (-0.001f <= rotateMtx.mMtx[2][0] - 1.0f) {
                pOut->x = std::atan2(-rotateMtx.mMtx[0][1], rotateMtx.mMtx[1][1]);
                pOut->y = -1.5707964f;
                pOut->z = 0.0f;
            }
            else if (rotateMtx.mMtx[2][0] + 1.0f <= 0.001f) {
                pOut->x = std::atan2(rotateMtx.mMtx[0][1], rotateMtx.mMtx[1][1]);
                pOut->y = 1.5707964f;
                pOut->z = 0.0f;
            }
            else {
                pOut->x = std::atan2(rotateMtx.mMtx[2][1], rotateMtx.mMtx[2][2]);
                pOut->z = std::atan2(rotateMtx.mMtx[1][0], rotateMtx.mMtx[0][0]);
                pOut->y = asin(-rotateMtx.mMtx[2][0]);
            }

            TVec3f stack_8;
            stack_8.setPS(*pOut);
            stack_8.x *= 57.29578f;
            stack_8.y *= 57.29578f;
            stack_8.z *= 57.29578f;
            pOut->setPS(stack_8);
        }
        
        return true;
    }

    bool getJMapInfoMatrixFromRT(const JMapInfoIter &rIter, TPos3f *pOut) {
        TVec3f trans;
        if (!getJMapInfoTrans(rIter, &trans)) {
            return false;
        }

        TVec3f rot;
        if (!getJMapInfoRotate(rIter, &rot)) {
            return false;
        }

        makeMtxTR(pOut->toMtxPtr(), trans, rot);
        return true;
    }

    bool getJMapInfoArg0WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return getArgAndInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0WithInit(const JMapInfoIter &rIter, f32 *pOut) {
        return getArgAndInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg1WithInit(const JMapInfoIter &rIter, f32 *pOut) {
        return getArgAndInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg1WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg2WithInit(const JMapInfoIter &rIter, f32 *pOut) {
        return getArgAndInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg2WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg3WithInit(const JMapInfoIter &rIter, f32 *pOut) {
        return getArgAndInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg3WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg4WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg7WithInit(const JMapInfoIter &rIter, bool *pOut) {
        return getArgAndInit(rIter, "Obj_arg7", pOut);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &rIter, s32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &rIter, f32 *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &rIter, bool *pOut) {
        return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
    }

    bool isEqualObjectName(const JMapInfoIter &rIter, const char *pOtherName) {
        const char* objName;
        getObjectName(&objName, rIter);
        return isEqualStringCase(objName, pOtherName);
    }

    s32 getDemoGroupID(const JMapInfoIter &rIter) {
        s32 groupID = -1;
        rIter.getValue<s32>("DemoGroupId", &groupID);
        return groupID;
    }

    s32 getDemoGroupLinkID(const JMapInfoIter &rIter) {
        s32 linkID = -1;
        rIter.getValue<s32>("l_id", &linkID);
        return linkID; 
    }

    bool getJMapInfoRailArg0NoInit(const JMapInfoIter &rIter, s32 *pRailArg0) { 
        return ::getJMapInfoRailArg(rIter, "path_arg0", pRailArg0);
    }

    bool getRailId(const JMapInfoIter &rIter, s32 *pRailID) {
        return getArgAndInit(rIter, "CommonPath_ID", pRailID); 
    } 

    bool getObjectName(const char **pName, const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        if (rIter.getValue<const char *>("type", pName)) {
            return true;
        }

        return rIter.getValue<const char*>("name", pName);
    }

    bool isExistJMapArg(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 val;
        return rIter.getValue<s32>("Obj_arg0", &val);
    }

    bool getJMapInfoShapeIdWithInit(const JMapInfoIter &rIter, s32 *pShapeID) {
        return rIter.getValue<s32>("ShapeModelNo", pShapeID);
    }


    bool getJMapInfoTransLocal(const JMapInfoIter &rIter, TVec3f *pOut) {
        if (!MR::getValue<f32>(rIter, "pos_x", &pOut->x)) {
            return false;
        }
        else if (!MR::getValue<f32>(rIter, "pos_y", &pOut->y)) {
            return false;
        }

        return MR::getValue<f32>(rIter, "pos_z", &pOut->z);
    }

    bool getJMapInfoRotateLocal(const JMapInfoIter &rIter, TVec3f *pOut) {
        if (!MR::getValue<f32>(rIter, "dir_x", &pOut->x)) {
            return false;
        }
        else if (!MR::getValue<f32>(rIter, "dir_y", &pOut->y)) {
            return false;
        }

        return MR::getValue<f32>(rIter, "dir_z", &pOut->z);
    }

    bool getJMapInfoScale(const JMapInfoIter &rIter, TVec3f *pOut) {
        if (!MR::getValue<f32>(rIter, "scale_x", &pOut->x)) {
            return false;
        }
        else if (!MR::getValue<f32>(rIter, "scale_y", &pOut->y)) {
            return false;
        }

        return MR::getValue<f32>(rIter, "scale_z", &pOut->z);
    }

    bool getJMapInfoV3f(const JMapInfoIter &rIter, const char *pName, TVec3f *pOut) {
        char str[0x20];
        sprintf(str, "%sX", pName);

        if (!MR::getValue<f32>(rIter, str, &pOut->x)) {
            return false;
        }

        sprintf(str, "%sY", pName);

        if (!MR::getValue<f32>(rIter, str, &pOut->y)) {
            return false;
        }

        sprintf(str, "%sZ", pName);
        return MR::getValue<f32>(rIter, str, &pOut->z);
    }

    bool getJMapInfoArg1WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg2WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg3WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg4WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg5WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg6WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg7WithInit(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "Obj_arg7", pOut);
    }

    bool getJMapInfoFollowID(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "FollowId", pOut);
    }

    bool getJMapInfoGroupID(const JMapInfoIter &rIter, s32 *pOut) {
        if (MR::getArgAndInit(rIter, "GroupId", pOut)) {
            return true;
        }

        return MR::getJMapInfoClippingGroupID(rIter, pOut);
    }

    bool getJMapInfoClippingGroupID(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "ClippingGroupId", pOut);
    }

    bool getJMapInfoDemoGroupID(const JMapInfoIter &rIter, s32 *pOut) {
        return MR::getArgAndInit(rIter, "DemoGroupId", pOut);
    }

    bool getJMapInfoLinkID(const JMapInfoIter &rIter, s32 *pOut) {
        return rIter.getValue<s32>("l_id", pOut);
    }

    bool isConnectedWithRail(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        return !MR::getArgAndInit(rIter, "CommonPath_ID", &id) ? false : id != -1;
    }

    bool isExistStageSwitchA(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        MR::getArgAndInit(rIter, "SW_A", &id);
        return id != -1;
    }

    bool isExistStageSwitchB(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        MR::getArgAndInit(rIter, "SW_B", &id);
        return id != -1;
    }

    bool isExistStageSwitchAppear(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        MR::getArgAndInit(rIter, "SW_APPEAR", &id);
        return id != -1;
    }

    bool isExistStageSwitchDead(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        MR::getArgAndInit(rIter, "SW_DEAD", &id);
        return id != -1;
    }

    bool isExistStageSwitchSleep(const JMapInfoIter &rIter) {
        if (!rIter.isValid()) {
            return false;
        }

        s32 id;
        MR::getArgAndInit(rIter, "SW_SLEEP", &id);
        return id != -1;
    }

    bool getJMapInfoCameraSetID(const JMapInfoIter &rIter, s32 *pOut) {
        *pOut = -1;
        
        if (!rIter.isValid()) {
            return false;
        }

        return ::getJMapInfoArgNoInit(rIter, "CameraSetId", pOut);
    }

    bool getJMapInfoViewGroupID(const JMapInfoIter &rIter, s32 *pOut) {
        *pOut = -1;
        
        if (!rIter.isValid()) {
            return false;
        }

        return ::getJMapInfoArgNoInit(rIter, "ViewGroupId", pOut);
    }

    bool getJMapInfoMessageID(const JMapInfoIter &rIter, s32 *pOut) {
        *pOut = -1;
        
        if (!rIter.isValid()) {
            return false;
        }

        return ::getJMapInfoArgNoInit(rIter, "MessageId", pOut);
    }

    s32 getDemoCastID(const JMapInfoIter &rIter) {
        s32 id = -1;
        rIter.getValue<s32>("CastId", &id);
        return id;
    }

    const char* getDemoName(const JMapInfoIter &rIter) {
        const char* name = nullptr;
        rIter.getValue<const char *>("DemoName", &name);
        return name;
    }

    const char* getDemoSheetName(const JMapInfoIter &rIter) {
        const char* name = nullptr;
        rIter.getValue<const char *>("TimeSheetName", &name);
        return name;
    }

    bool getNextLinkRailID(const JMapInfoIter &rIter, s32 *pOut) {
        return rIter.getValue<s32>("Path_ID", pOut);
    }

    bool isEqualRailUsage(const JMapInfoIter &rIter, const char *pUsage) {
        const char* str = nullptr;
        rIter.getValue<const char *>("usage", &str);
        return isEqualStringCase(str, pUsage);
    }

    void getRailPointPos0(const JMapInfoIter &rIter, TVec3f *pOut) {
        rIter.getValue<f32>("pnt0_x", &pOut->x);
        rIter.getValue<f32>("pnt0_y", &pOut->y);
        rIter.getValue<f32>("pnt0_z", &pOut->z);

        if (isPlacementLocalStage()) {
            getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
        }
    }

    void getRailPointPos1(const JMapInfoIter &rIter, TVec3f *pOut) {
        rIter.getValue<f32>("pnt1_x", &pOut->x);
        rIter.getValue<f32>("pnt1_y", &pOut->y);
        rIter.getValue<f32>("pnt1_z", &pOut->z);

        if (isPlacementLocalStage()) {
            getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
        }
    }

    void getRailPointPos2(const JMapInfoIter &rIter, TVec3f *pOut) {
        rIter.getValue<f32>("pnt2_x", &pOut->x);
        rIter.getValue<f32>("pnt2_y", &pOut->y);
        rIter.getValue<f32>("pnt2_z", &pOut->z);

        if (isPlacementLocalStage()) {
            getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
        }
    }

    bool isLoopRailPathIter(const JMapInfoIter &rIter) {
        const char* status = "";
        rIter.getValue<const char *>("closed", &status);
        return isEqualString(status, "CLOSE");
    }
}; 
