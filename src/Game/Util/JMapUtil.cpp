#include "Game/Util/JMapUtil.hpp"
#include "Game/Util.hpp"
#include <cstdio>

bool MR::isValidInfo(const JMapInfoIter& rIter) {
    return rIter.isValid();
}

bool MR::isObjectName(const JMapInfoIter& rIter, const char* pName) {
    const char* objName = nullptr;

    if (getObjectName(&objName, rIter)) {
        return isEqualString(pName, objName);
    }

    return false;
}

namespace {
    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pName, s32* pOut) NO_INLINE {
        s32 arg;

        if (!rIter.getValue(pName, &arg)) {
            return false;
        }

        if (arg != -1) {
            *pOut = arg;

            return true;
        }

        return false;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pName, f32* pOut) NO_INLINE {
        s32 arg;

        if (!getJMapInfoArgNoInit(rIter, pName, &arg)) {
            return false;
        }

        *pOut = arg;

        return true;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter& rIter, const char* pName, bool* pOut) NO_INLINE {
        s32 arg;

        if (!getJMapInfoArgNoInit(rIter, pName, &arg)) {
            return false;
        }

        if (arg != -1) {
            *pOut = true;
        } else {
            *pOut = false;
        }

        return true;
    }

    bool getJMapInfoArgWithInit(const JMapInfoIter& rIter, const char* pName, s32* pOut) {
        *pOut = -1;

        return getJMapInfoArgNoInit(rIter, pName, pOut);
    }

    bool getJMapInfoArgWithInit(const JMapInfoIter& rIter, const char* pName, f32* pOut) {
        *pOut = -1.0f;

        return getJMapInfoArgNoInit(rIter, pName, pOut);
    }

    bool getJMapInfoArgWithInit(const JMapInfoIter& rIter, const char* pName, bool* pOut) {
        *pOut = false;

        return getJMapInfoArgNoInit(rIter, pName, pOut);
    }
};  // namespace

bool MR::getJMapInfoTrans(const JMapInfoIter& rIter, TVec3f* pOut) {
    if (!getJMapInfoTransLocal(rIter, pOut)) {
        return false;
    }

    if (isPlacementLocalStage()) {
        getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
    }

    return true;
}

bool MR::getJMapInfoRotate(const JMapInfoIter& rIter, TVec3f* pOut) {
    if (!getJMapInfoRotateLocal(rIter, pOut)) {
        return false;
    }

    if (isPlacementLocalStage()) {
        TMtx34f rotateMtx;
        makeMtxRotate(rotateMtx.toMtxPtr(), *pOut);
        rotateMtx.concat(*getZonePlacementMtx(rIter), rotateMtx);

        // TODO: getEuler but for std?
        if (-0.001f <= rotateMtx.mMtx[2][0] - 1.0f) {
            pOut->x = std::atan2(-rotateMtx.mMtx[0][1], rotateMtx.mMtx[1][1]);
            pOut->y = -1.5707964f;
            pOut->z = 0.0f;
        } else if (rotateMtx.mMtx[2][0] + 1.0f <= 0.001f) {
            pOut->x = std::atan2(rotateMtx.mMtx[0][1], rotateMtx.mMtx[1][1]);
            pOut->y = 1.5707964f;
            pOut->z = 0.0f;
        } else {
            pOut->x = std::atan2(rotateMtx.mMtx[2][1], rotateMtx.mMtx[2][2]);
            pOut->z = std::atan2(rotateMtx.mMtx[1][0], rotateMtx.mMtx[0][0]);
            pOut->y = ::asin(-rotateMtx.mMtx[2][0]);
        }

        *pOut = *pOut * _180_PI;
    }

    return true;
}

bool MR::getJMapInfoMatrixFromRT(const JMapInfoIter& rIter, TPos3f* pOut) {
    TVec3f trans;
    if (!getJMapInfoTrans(rIter, &trans)) {
        return false;
    }

    TVec3f rotate;
    if (!getJMapInfoRotate(rIter, &rotate)) {
        return false;
    }

    makeMtxTR(pOut->toMtxPtr(), trans, rotate);

    return true;
}

bool MR::getJMapInfoArg0WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg0WithInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg0WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg1WithInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg1WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg2WithInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg2WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg3WithInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg3WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg4WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg4", pOut);
}

bool MR::getJMapInfoArg7WithInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg7", pOut);
}

bool MR::getJMapInfoArg0NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg0NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg0NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg0", pOut);
}

bool MR::getJMapInfoArg1NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg1NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg1NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg2NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg2NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg2NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg3NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg3NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg3NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg4NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
}

bool MR::getJMapInfoArg4NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
}

bool MR::getJMapInfoArg4NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg4", pOut);
}

bool MR::getJMapInfoArg5NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
}

bool MR::getJMapInfoArg5NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
}

bool MR::getJMapInfoArg5NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg5", pOut);
}

bool MR::getJMapInfoArg6NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
}

bool MR::getJMapInfoArg6NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
}

bool MR::getJMapInfoArg6NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg6", pOut);
}

bool MR::getJMapInfoArg7NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
}

bool MR::getJMapInfoArg7NoInit(const JMapInfoIter& rIter, f32* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
}

bool MR::getJMapInfoArg7NoInit(const JMapInfoIter& rIter, bool* pOut) {
    return ::getJMapInfoArgNoInit(rIter, "Obj_arg7", pOut);
}

bool MR::isEqualObjectName(const JMapInfoIter& rIter, const char* pOtherName) {
    const char* objName;
    getObjectName(&objName, rIter);

    return isEqualStringCase(objName, pOtherName);
}

s32 MR::getDemoGroupID(const JMapInfoIter& rIter) {
    s32 groupID = -1;
    rIter.getValue< s32 >("DemoGroupId", &groupID);

    return groupID;
}

s32 MR::getDemoGroupLinkID(const JMapInfoIter& rIter) {
    s32 linkID = -1;
    rIter.getValue< s32 >("l_id", &linkID);

    return linkID;
}

namespace {
    bool getJMapInfoRailArg(const JMapInfoIter& rIter, const char* pName, s32* pOut) NO_INLINE {
        s32 arg;

        if (!rIter.getValue< s32 >(pName, &arg)) {
            return false;
        }

        if (arg != -1) {
            *pOut = arg;
            return true;
        }

        return false;
    }
};  // namespace

bool MR::getJMapInfoRailArg0NoInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoRailArg(rIter, "path_arg0", pOut);
}

bool MR::getRailId(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "CommonPath_ID", pOut);
}

bool MR::getObjectName(const char** pName, const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    if (rIter.getValue< const char* >("type", pName)) {
        return true;
    }

    return rIter.getValue< const char* >("name", pName);
}

bool MR::isExistJMapArg(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;

    return rIter.getValue< s32 >("Obj_arg0", &arg);
}

bool MR::getJMapInfoShapeIdWithInit(const JMapInfoIter& rIter, s32* pShapeID) {
    return rIter.getValue< s32 >("ShapeModelNo", pShapeID);
}

bool MR::getJMapInfoTransLocal(const JMapInfoIter& rIter, TVec3f* pOut) {
    if (!MR::getValue< f32 >(rIter, "pos_x", &pOut->x)) {
        return false;
    }

    if (!MR::getValue< f32 >(rIter, "pos_y", &pOut->y)) {
        return false;
    }

    return MR::getValue< f32 >(rIter, "pos_z", &pOut->z);
}

bool MR::getJMapInfoRotateLocal(const JMapInfoIter& rIter, TVec3f* pOut) {
    if (!MR::getValue< f32 >(rIter, "dir_x", &pOut->x)) {
        return false;
    }

    if (!MR::getValue< f32 >(rIter, "dir_y", &pOut->y)) {
        return false;
    }

    return MR::getValue< f32 >(rIter, "dir_z", &pOut->z);
}

bool MR::getJMapInfoScale(const JMapInfoIter& rIter, TVec3f* pOut) {
    if (!MR::getValue< f32 >(rIter, "scale_x", &pOut->x)) {
        return false;
    }

    if (!MR::getValue< f32 >(rIter, "scale_y", &pOut->y)) {
        return false;
    }

    return MR::getValue< f32 >(rIter, "scale_z", &pOut->z);
}

bool MR::getJMapInfoV3f(const JMapInfoIter& rIter, const char* pName, TVec3f* pOut) {
    char str[32];
    sprintf(str, "%sX", pName);

    if (!MR::getValue< f32 >(rIter, str, &pOut->x)) {
        return false;
    }

    sprintf(str, "%sY", pName);

    if (!MR::getValue< f32 >(rIter, str, &pOut->y)) {
        return false;
    }

    sprintf(str, "%sZ", pName);

    return MR::getValue< f32 >(rIter, str, &pOut->z);
}

bool MR::getJMapInfoArg1WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg1", pOut);
}

bool MR::getJMapInfoArg2WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg2", pOut);
}

bool MR::getJMapInfoArg3WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg3", pOut);
}

bool MR::getJMapInfoArg4WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg4", pOut);
}

bool MR::getJMapInfoArg5WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg5", pOut);
}

bool MR::getJMapInfoArg6WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg6", pOut);
}

bool MR::getJMapInfoArg7WithInit(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "Obj_arg7", pOut);
}

bool MR::getJMapInfoFollowID(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "FollowId", pOut);
}

bool MR::getJMapInfoGroupID(const JMapInfoIter& rIter, s32* pOut) {
    if (::getJMapInfoArgWithInit(rIter, "GroupId", pOut)) {
        return true;
    }

    return MR::getJMapInfoClippingGroupID(rIter, pOut);
}

bool MR::getJMapInfoClippingGroupID(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "ClippingGroupId", pOut);
}

bool MR::getJMapInfoDemoGroupID(const JMapInfoIter& rIter, s32* pOut) {
    return ::getJMapInfoArgWithInit(rIter, "DemoGroupId", pOut);
}

bool MR::getJMapInfoLinkID(const JMapInfoIter& rIter, s32* pOut) {
    return rIter.getValue< s32 >("l_id", pOut);
}

bool MR::isConnectedWithRail(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;

    if (::getJMapInfoArgWithInit(rIter, "CommonPath_ID", &arg) == false) {
        return false;
    }

    return arg != -1;
}

bool MR::isExistStageSwitchA(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;
    ::getJMapInfoArgWithInit(rIter, "SW_A", &arg);

    return arg != -1;
}

bool MR::isExistStageSwitchB(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;
    ::getJMapInfoArgWithInit(rIter, "SW_B", &arg);

    return arg != -1;
}

bool MR::isExistStageSwitchAppear(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;
    ::getJMapInfoArgWithInit(rIter, "SW_APPEAR", &arg);

    return arg != -1;
}

bool MR::isExistStageSwitchDead(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;
    ::getJMapInfoArgWithInit(rIter, "SW_DEAD", &arg);

    return arg != -1;
}

bool MR::isExistStageSwitchSleep(const JMapInfoIter& rIter) {
    if (!rIter.isValid()) {
        return false;
    }

    s32 arg;
    ::getJMapInfoArgWithInit(rIter, "SW_SLEEP", &arg);

    return arg != -1;
}

bool MR::getJMapInfoCameraSetID(const JMapInfoIter& rIter, s32* pOut) {
    *pOut = -1;

    if (!rIter.isValid()) {
        return false;
    }

    return ::getJMapInfoArgNoInit(rIter, "CameraSetId", pOut);
}

bool MR::getJMapInfoViewGroupID(const JMapInfoIter& rIter, s32* pOut) {
    *pOut = -1;

    if (!rIter.isValid()) {
        return false;
    }

    return ::getJMapInfoArgNoInit(rIter, "ViewGroupId", pOut);
}

bool MR::getJMapInfoMessageID(const JMapInfoIter& rIter, s32* pOut) {
    *pOut = -1;

    if (!rIter.isValid()) {
        return false;
    }

    return ::getJMapInfoArgNoInit(rIter, "MessageId", pOut);
}

s32 MR::getDemoCastID(const JMapInfoIter& rIter) {
    s32 castId = -1;
    rIter.getValue< s32 >("CastId", &castId);

    return castId;
}

const char* MR::getDemoName(const JMapInfoIter& rIter) {
    const char* demoName = nullptr;
    rIter.getValue< const char* >("DemoName", &demoName);

    return demoName;
}

const char* MR::getDemoSheetName(const JMapInfoIter& rIter) {
    const char* timeSheetName = nullptr;
    rIter.getValue< const char* >("TimeSheetName", &timeSheetName);
    return timeSheetName;
}

bool MR::getNextLinkRailID(const JMapInfoIter& rIter, s32* pOut) {
    return rIter.getValue< s32 >("Path_ID", pOut);
}

bool MR::isEqualRailUsage(const JMapInfoIter& rIter, const char* pUsage) {
    const char* usage = nullptr;
    rIter.getValue< const char* >("usage", &usage);

    return isEqualStringCase(usage, pUsage);
}

void MR::getRailPointPos0(const JMapInfoIter& rIter, TVec3f* pOut) {
    rIter.getValue< f32 >("pnt0_x", &pOut->x);
    rIter.getValue< f32 >("pnt0_y", &pOut->y);
    rIter.getValue< f32 >("pnt0_z", &pOut->z);

    if (isPlacementLocalStage()) {
        getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
    }
}

void MR::getRailPointPos1(const JMapInfoIter& rIter, TVec3f* pOut) {
    rIter.getValue< f32 >("pnt1_x", &pOut->x);
    rIter.getValue< f32 >("pnt1_y", &pOut->y);
    rIter.getValue< f32 >("pnt1_z", &pOut->z);

    if (isPlacementLocalStage()) {
        getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
    }
}

void MR::getRailPointPos2(const JMapInfoIter& rIter, TVec3f* pOut) {
    rIter.getValue< f32 >("pnt2_x", &pOut->x);
    rIter.getValue< f32 >("pnt2_y", &pOut->y);
    rIter.getValue< f32 >("pnt2_z", &pOut->z);

    if (isPlacementLocalStage()) {
        getZonePlacementMtx(rIter)->mult(*pOut, *pOut);
    }
}

bool MR::isLoopRailPathIter(const JMapInfoIter& rIter) {
    const char* closed = "";
    rIter.getValue< const char* >("closed", &closed);

    return isEqualString(closed, "CLOSE");
}
