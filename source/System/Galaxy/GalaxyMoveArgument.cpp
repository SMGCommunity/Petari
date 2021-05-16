#include "System/Galaxy/GalaxyMoveArgument.h"
#include "Util/SceneUtil.h"
#include "Util/StringUtil.h"

GalaxyMoveArgument::GalaxyMoveArgument(u32 p1, const char* pStage, s32 pScenarioNo, const JMapIdInfo* pInfo)
{
    _0 = p1;
    mStageName = pStage;
    mScenarioNo = pScenarioNo;
    _C = -1;

    mStartInfo.mScenarioNo = -1;
    mStartInfo.mZoneId = -1;

    if (pInfo != 0)
    {
        mStartInfo = *pInfo;
    }
    else
    {
        mStartInfo = *MR::getInitializeStartIdInfo();
    }
}

bool GalaxyMoveArgument::isEqualStage(const char* pStage) const
{
    if (mStageName == 0)
    {
        return false;
    }

    return MR::isEqualString(pStage, mStageName);
}

bool GalaxyMoveArgument::isEqualStageScenario(const char* pStage, s32 pScenarioNo) const
{
    bool isEqual = false;

    if (isEqualStage(pStage) && pScenarioNo == mScenarioNo)
    {
        isEqual = true;
    }

    return isEqual;
}