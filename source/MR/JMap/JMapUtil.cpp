#include "JMap/JMapInfoIter.h"
#include "arch/printf.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/MtxUtil.h"
#include "MR/SceneUtil.h"
#include "MR/StringUtil.h"

namespace MR
{
    bool isValidInfo(const JMapInfoIter &iter)
    {
        return iter.isValid();
    }

    bool isObjectName(const JMapInfoIter &iter, const char *pObjName)
    {
        const char* objName = 0;
        bool ret = MR::getObjectName(&objName, iter);

        if (ret)
        {
            return MR::isEqualString(pObjName, objName);
        }

        return 0;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &iter, const char *pArgName, s32 *pOut)
    {
        s32 out;
        bool ret = iter.getValue<s32>(pArgName, &out);
    
        if (!ret)
        {
            return 0;
        }

        if (out != -1)
        {
            *pOut = out;
            return 1;
        }

        return 0;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &iter, const char *pArgName, f32 *pOut)
    {
        s32 out;
        bool ret = MR::getJMapInfoArgNoInit(iter, pArgName, &out);

        if (!ret)
        {
            return 0;
        }

        *pOut = out;
        return 1;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &iter, const char *pArgName, bool *pOut)
    {
        s32 out;
        bool ret = MR::getJMapInfoArgNoInit(iter, pArgName, &out);

        if (!ret)
        {
            return 0;
        }

        if (out != -1)
        {
            *pOut = 1;
        }
        else
        {
            *pOut = 0;
        }

        return 1;
    }

    bool getJMapInfoTrans(const JMapInfoIter &iter, JGeometry::TVec3<f32> *pOut)
    {
        bool ret = MR::getJMapInfoTransLocal(iter, pOut);

        if (!ret)
        {
            return 0;
        }

        if (MR::isPlacementLocalStage())
        {
            JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> const>* mtx = MR::getZonePlacementMtx(iter);
            mtx->mult(*pOut, *pOut);
        }

        return 1;
    }

    bool getJMapInfoRotate(const JMapInfoIter &iter, JGeometry::TVec3<f32> *pOut)
    {
        bool ret = MR::getJMapInfoRotateLocal(iter, pOut);

        if (!ret)
        {
            return 0;
        }

        if (MR::isPlacementLocalStage())
        {
            /* some type mismatching here -- todo, fix */
        }

        return 1;
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg0", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg7", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, f32 *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg7", pOut);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, bool *pOut)
    {
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg7", pOut);
    }

    bool isEqualObjectName(const JMapInfoIter &iter, const char *name)
    {
        const char* in;
        MR::getObjectName(&in, iter);
        return MR::isEqualStringCase(in, name);
    }

    s32 getDemoGroupID(const JMapInfoIter &iter)
    {
        s32 ret = -1;
        iter.getValue<s32>("DemoGroupId", &ret);
        return ret;
    }

    s32 getDemoGroupLinkID(const JMapInfoIter &iter)
    {
        s32 ret = -1;
        iter.getValue<s32>("l_id", &ret);
        return ret;
    }

    s32 getJMapInfoRailArg(const JMapInfoIter &iter, const char *railParam, s32 *pOut)
    {
        s32 ret;
        bool success = iter.getValue<s32>(railParam, &ret);

        if (!success)
            return 0;

        if (ret != -1)
        {
            *pOut = ret;
            return 1;
        }

        return 0;
    }

    s32 getJMapInfoRailArg0NoInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return getJMapInfoRailArg(iter, "path_arg0", pOut);
    }

    s32 getRailId(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "CommonPath_ID", pOut);
    }

    bool getObjectName(const char **name, const JMapInfoIter &iter)
    {
        if (!iter.isValid())
            return 0;

        bool ret = iter.getValue<const char *>("type", name);

        if (ret)
            return 1;

        return iter.getValue<const char *>("name", name);
    }

    bool isExistJMapArg(const JMapInfoIter &iter)
    {
        if (!iter.isValid())
            return 0;

        s32 temp;
        return iter.getValue<s32>("Obj_arg0", &temp);
    }

    bool getJMapInfoShapeIdWithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        return iter.getValue<s32>("ShapeModelNo", pOut);
    }

    bool getJMapInfoTransLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *pOut)
    {
        bool ret = iter.getValue<f32>("pos_x", &pOut->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("pos_y", &pOut->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("pos_z", &pOut->z);
    }

    bool getJMapInfoRotateLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *pOut)
    {
        bool ret = iter.getValue<f32>("dir_x", &pOut->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("dir_y", &pOut->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("dir_z", &pOut->z);
    }

    bool getJMapInfoScaleLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *pOut)
    {
        bool ret = iter.getValue<f32>("scale_x", &pOut->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("scale_y", &pOut->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("scale_z", &pOut->z);
    }

    bool getJMapInfoV3f(const JMapInfoIter &iter, const char *identifier, JGeometry::TVec3<f32> *pOut)
    {
        char formattedStr;
        sprintf(&formattedStr, "%sX", identifier);

        bool ret = iter.getValue<f32>(&formattedStr, &pOut->x);

        if (!ret)
            return 0;

        sprintf(&formattedStr, "%sY", identifier);

        ret = iter.getValue<f32>(&formattedStr, &pOut->y);

        if (!ret)
            return 0;

        sprintf(&formattedStr, "%sZ", identifier);

        return iter.getValue<f32>(&formattedStr, &pOut->z);
    }

    bool getJMapInfoArg1WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg1", pOut);
    }

    bool getJMapInfoArg2WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg2", pOut);
    }

    bool getJMapInfoArg3WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg3", pOut);
    }

    bool getJMapInfoArg4WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg4", pOut);
    }

    bool getJMapInfoArg5WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg5", pOut);
    }

    bool getJMapInfoArg6WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg6", pOut);
    }

    bool getJMapInfoArg7WithInit(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "Obj_arg7", pOut);
    }

    bool getJMapInfoFollowID(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "FollowId", pOut);
    }

    bool getJMapInfoGroupID(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;

        bool ret = MR::getJMapInfoArgNoInit(iter, "GroupId", pOut);

        if (ret)
            return 1;

        return getJMapInfoClippingGroupID(iter, pOut);
    }

    bool getJMapInfoClippingGroupID(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "ClippingGroupID", pOut);
    }

    bool getJMapInfoDemoGroupID(const JMapInfoIter &iter, s32 *pOut)
    {
        *pOut = -1;
        return MR::getJMapInfoArgNoInit(iter, "DemoGroupID", pOut);
    }

    bool getJMapInfoLinkID(const JMapInfoIter &iter, s32 *pOut)
    {
        return iter.getValue<s32>("l_id", pOut);
    }

    bool isConnectedWithRail(const JMapInfoIter &iter)
    {
        if (!iter.isValid())
            return 0;

        s32 out = -1;
        bool ret = MR::getJMapInfoArgNoInit(iter, "CommonPath_ID", &out);

       if (!ret)
            return 0;
        else
            return out;
    }
};