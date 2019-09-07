#include "JMap/JMapInfoIter.h"
#include "arch/printf.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/StringUtil.h"

namespace MR
{
    bool isValidInfo(const JMapInfoIter &iter)
    {
        return iter.isValid();
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg0", out);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg0", out);
    }

    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg0", out);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg1", out);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg1", out);
    }

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg1", out);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg2", out);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg2", out);
    }

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg2", out);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg3", out);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg3", out);
    }

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg3", out);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg4", out);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg4", out);
    }

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg4", out);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg5", out);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg5", out);
    }

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg5", out);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg6", out);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg6", out);
    }

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg6", out);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg7", out);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, f32 *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg7", out);
    }

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, bool *out)
    {
        return getJMapInfoArgNoInit(iter, "Obj_arg7", out);
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

    s32 getJMapInfoRailArg(const JMapInfoIter &iter, const char *railParam, s32 *out)
    {
        s32 ret;
        bool success = iter.getValue<s32>(railParam, &ret);

        if (!success)
            return 0;

        if (ret != -1)
        {
            *out = ret;
            return 1;
        }

        return 0;
    }

    s32 getJMapInfoRailArg0NoInit(const JMapInfoIter &iter, s32 *out)
    {
        return getJMapInfoRailArg(iter, "path_arg0", out);
    }

    s32 getRailId(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "CommonPath_ID", out);
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

    bool getJMapInfoShapeIdWithInit(const JMapInfoIter &iter, s32 *out)
    {
        return iter.getValue<s32>("ShapeModelNo", out);
    }

    bool getJMapInfoTransLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *out)
    {
        bool ret = iter.getValue<f32>("pos_x", &out->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("pos_y", &out->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("pos_z", &out->z);
    }

    bool getJMapInfoRotateLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *out)
    {
        bool ret = iter.getValue<f32>("dir_x", &out->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("dir_y", &out->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("dir_z", &out->z);
    }

    bool getJMapInfoScaleLocal(const JMapInfoIter &iter, JGeometry::TVec3<f32> *out)
    {
        bool ret = iter.getValue<f32>("scale_x", &out->x);

        if (!ret)
            return 0;

        ret = iter.getValue<f32>("scale_y", &out->y);

        if (!ret)
            return 0;

        return iter.getValue<f32>("scale_z", &out->z);
    }

    bool getJMapInfoV3f(const JMapInfoIter &iter, const char *identifier, JGeometry::TVec3<f32> *out)
    {
        char formattedStr;
        sprintf(&formattedStr, "%sX", identifier);

        bool ret = iter.getValue<f32>(&formattedStr, &out->x);

        if (!ret)
            return 0;

        sprintf(&formattedStr, "%sY", identifier);

        ret = iter.getValue<f32>(&formattedStr, &out->y);

        if (!ret)
            return 0;

        sprintf(&formattedStr, "%sZ", identifier);

        return iter.getValue<f32>(&formattedStr, &out->z);
    }

    bool getJMapInfoArg1WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg1", out);
    }

    bool getJMapInfoArg2WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg2", out);
    }

    bool getJMapInfoArg3WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg3", out);
    }

    bool getJMapInfoArg4WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg4", out);
    }

    bool getJMapInfoArg5WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg5", out);
    }

    bool getJMapInfoArg6WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg6", out);
    }

    bool getJMapInfoArg7WithInit(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "Obj_arg7", out);
    }

    bool getJMapInfoFollowID(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;
        return getJMapInfoArgNoInit(iter, "FollowId", out);
    }

    bool getJMapInfoGroupID(const JMapInfoIter &iter, s32 *out)
    {
        *out = -1;

        bool ret = getJMapInfoArgNoInit(iter, "GroupId", out);

        if (ret)
            return 1;

        return getJMapInfoClippingGroupID(iter, out);
    }
};