#include "System/Camera/CameraParamChunk.h"
#include "MR/MathUtil.h"
#include "string.h"

bool CameraParamChunk::isOnNoReset() const
{
    return mFlags & 0x1;
}

bool CameraParamChunk::isOnUseFovy() const
{
    return (mFlags >> 1) & 0x1;
}

bool CameraParamChunk::isLOfsErpOff() const
{
    return (mFlags >> 2) & 0x1;
}

bool CameraParamChunk::isAntiBlurOff() const
{
    return (mFlags >> 3) & 0x1;
}

bool CameraParamChunk::isCollisionOff() const
{
    return (mFlags >> 4) & 0x1;
}

bool CameraParamChunk::isSubjectiveCameraOff() const
{
    return (mFlags >> 5) & 0x1;
}

void CameraParamChunk::getVPanAxis(JGeometry::TVec3<f32> *out) const
{
    out->x = mVPanAxisX;
    out->y = mVPanAxisY;
    out->z = mVPanAxisZ;

    if (MR::isNearZero(*out, 0.001f))
    {
        out->set<f32>(0.0f, 1.0f, 0.0f);
    }

    MR::normalize(out);
}

void CameraParamChunk::setUseFovy(bool flag)
{
    mFlags = (mFlags & 0xFFFFFFFD);

    if (flag)
    {
        mFlags |= 0x2;
    }
}

void CameraParamChunk::setLOFsErpOff(bool flag)
{
    mFlags = (mFlags & 0xFFFFFFFB);

    if (flag)
    {
        mFlags |= 0x4;
    }
}

void CameraParamChunk::setCollisionOff(bool flag)
{
    mFlags = (mFlags & 0xFFFFFFEF);

    if (flag)
    {
        mFlags |= 0x10;
    }
}

/* todo some still invalid logic... */
void CameraParamChunk::arrangeCamTypeName(u32 type, const char **out)
{
    if (type < 0x30004)
    {
        if (type < 0x30006)
        {
            if (type < 0x30009)
            {
                if (!strcmp(*out, "CAM_TYPE_ICECUBE_PLANET"))
                {
                    *out = "CAM_TYPE_CUBE_PLANET";
                    return;
                }
            }

            if (!strcmp(*out, "CAM_TYPE_BEHIND_DEBUG"))
            {
                *out = "CAM_TYPE_SLIDER";
                return;
            }
            else
            {
                if (!strcmp(*out, "CAM_TYPE_INWARD_TOWER_TEST"))
                {
                    *out = "CAM_TYPE_INWARD_TOWER";
                    return;
                }
                else
                {
                    if (!strcmp(*out, "CAM_TYPE_EYE_FIXED_THERE_TEST"))
                    {
                        *out = "CAM_TYPE_EYEPOS_FIX_THERE";
                        return;
                    }

                    return;
                }
            }

            return;
        }

        if (!strcmp(*out, "CAM_TYPE_DONKETSU_TEST"))
        {
            return;
        }

        *out = "CAM_TYPE_BOSS_DONKETSU";
    }
}