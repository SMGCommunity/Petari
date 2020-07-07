#include "Camera/DotCamParams.h"
#include "smg.h"

DotCamReaderInBin::DotCamReaderInBin(const void *pData)
    : mVersion(0)
{
    JMapInfo mInfo();
    mIter.mInfo = nullptr;
    mIter.mPos = -1;
    init(pData);
}

void DotCamReaderInBin::nextToChunk()
{
    if (mIter.isValid())
    {
        mIter.mPos++;
    }
}

bool DotCamReaderInBin::getValueInt(const char *pName, s32 *out)
{
    return mIter.getValue<s32>(pName, out);
}

bool DotCamReaderInBin::getValueFloat(const char *pName, f32 *out)
{
    return mIter.getValue<f32>(pName, out);
}

s32 DotCamReaderInBin::getValueString(const char *pName, const char **pOut)
{
    s32 curPos = mIter.mPos;
    const JMapInfo* info = mIter.mInfo;
    s32 pos = info->searchItemInfo(pName);

    if (pos < 0)
    {
        return 0;
    }
    else
    {
        return info->getValueFast(curPos, pos, pOut);
    }
}

s32 DotCamReaderInBin::getVersion() const
{
    return mVersion;
}