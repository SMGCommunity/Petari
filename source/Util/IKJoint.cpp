#include "Util/IKJoint.h"

IKJoint::IKJoint()
{
    mRootBoneLength = 100.0f;
    mMiddleBoneLength = 100.0f;

    _0.identity();
    _30.identity();
    _60.identity();
}

void IKJoint::setRootBoneLength(f32 length)
{
    mRootBoneLength = length;
}

void IKJoint::setMiddleBoneLength(f32 length)
{
    mMiddleBoneLength = length;
}

// todo -- nonmatching
s32 IKJoint::checkReachIKTarget(f32 a1, f32 a2, f32 a3)
{
    f32 val = a2 + a3;

    if (a1 > val)
    {
        return 2;
    }

    if (a1 >= __fabs(a2 - a3))
    {
        return 1;
    }

    return 0;
}

// todo -- nonmatching
f32 IKJoint::calcIKRootAngleCosign(f32 a1, f32 a2, f32 a3)
{
    f32 angle, v11;

    s32 target = checkReachIKTarget(a1, a2, a3);

    if (target == 1)
    {
        angle = 1.0f;
    }
    else if (target == -1)
    {
        if (a2 >= a3)
        {
            angle = 1.0f;
        }
        else
        {
            angle = -1.0f;
        }
    }
    else
    {
        angle = -1.0f;

        v11 = ((((a3 * a3) - (a1 * a1)) - (a2 * a2)) / ((-2.0f * a2) * a1));

        if (v11 >= -1.0f)
        {
            angle = 1.0f;

            if (v11 <= 1.0f)
            {
                angle = ((((a3 * a3) - (a1 * a1)) - (a2 * a2)) / ((-2.0f * a2) * a1));
            }
        }
    }

    return angle;
}