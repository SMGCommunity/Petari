#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

GalaxyMoveArgument::GalaxyMoveArgument(u32 moveType, const char *pStageName, s32 scenario, const JMapIdInfo *pMapIdInfo) {
    mMoveType = moveType;
    mStageName = pStageName;
    mScenario = scenario;
    _C = -1;
    mIDInfo._0 = -1;
    mIDInfo.mZoneID = -1;

    if (pMapIdInfo) {
        mIDInfo = *pMapIdInfo;
    }
    else {
        mIDInfo = MR::getInitializeStartIdInfo();
    }
}

bool GalaxyMoveArgument::isEqualStage(char const *pOtherName) const {
    if (mStageName == nullptr)
        return false;
    
    return MR::isEqualString(pOtherName, mStageName);
}

bool GalaxyMoveArgument::isEqualStageScenario(char const *pOtherName, int otherScenario) const {
    return isEqualStage(pOtherName) && otherScenario == mScenario;
}
