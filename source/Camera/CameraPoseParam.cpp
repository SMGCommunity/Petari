#include "Camera/CameraPoseParam.h"

CameraPoseParam::CameraPoseParam()
{
    mWatchUpVec.x = 0.0f;
    mWatchUpVec.y = 1.0f;
    mWatchUpVec.z = 0.0f;

    mWatchPos.x = 0.0f;
    mWatchPos.y = 0.0f;
    mWatchPos.z = 1.0f;

    mUpVector.x = 0.0f;
    mUpVector.y = 1.0f;
    mUpVector.z = 0.0f;

    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;

    mFovy = 40.0f;

    mGlobalOffset.x = 0.0f;
    mGlobalOffset.y = 0.0f;
    mGlobalOffset.z = 0.0f;

    mLocalOffset.x = 0.0f;
    mLocalOffset.y = 0.0f;
    mLocalOffset.z = 0.0f;

    mFrontOffset = 0.0f;
    mUpperOffset = 0.0f;
    mRoll = 0.0f;
}

void CameraPoseParam::copyFrom(const CameraPoseParam &otherParam)
{
    mWatchUpVec.set(otherParam.mWatchUpVec);
    mWatchPos.set(otherParam.mWatchPos);
    mUpVector.set(otherParam.mUpVector);
    mPosition.set(otherParam.mPosition);
    mGlobalOffset.set(otherParam.mGlobalOffset);
    f32 fovy = otherParam.mFovy;
    mFovy = fovy;
    mLocalOffset.set(otherParam.mLocalOffset);

    mFrontOffset = otherParam.mFrontOffset;
    mUpperOffset = otherParam.mUpperOffset;
    mRoll = otherParam.mRoll;
}