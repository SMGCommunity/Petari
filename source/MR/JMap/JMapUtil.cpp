#include "JMap/JMapInfoIter.h"
#include "MR/JMap/JMapUtil.h"

/*
// TODO -- this, for some reason, just won't invoke use of the stack
bool getJMapInfoArgNoInit(const JMapInfoIter &iter, const char *name, s32 *out)
{
    s32* val = out;
    bool valRes = iter.getValue<s32>(name, *val);

    bool ret;

    if (valRes == 0)
    {
        ret = 0;
    }
    else
    {
        if (*val != -1)
        {
            out = val;
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }

    return ret;
}*/

namespace MR
{
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
};