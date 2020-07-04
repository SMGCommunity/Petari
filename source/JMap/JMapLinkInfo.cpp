#include "JMap/JMapLinkInfo.h"
#include "MR/SceneUtil.h"
#include "MR/StringUtil.h"

JMapLinkInfo::JMapLinkInfo(const JMapInfoIter &rIter, bool isLinked)
    : mID(-1), mZoneID(-1), _8(-1)
{
    if (isLinked)
    {
        setLinkedInfo(rIter);
    }
    else
    {
        setLinkInfo(rIter);
    } 
}

bool JMapLinkInfo::isValid() const
{
    bool ret = false;

    if (mID >= 0 && mZoneID >= 0 && _8 >= 0)
    {
       ret = true;
    }

    return ret;
}

void JMapLinkInfo::invalidate()
{
    mID = -1;
    mZoneID = -1;
    _8 = -1;
}