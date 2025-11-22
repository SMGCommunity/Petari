#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

GalaxyMoveArgument::GalaxyMoveArgument(u32 moveType, const char* pStageName, s32 scenarioNo, const JMapIdInfo* pMapIdInfo)
    : mMoveType(moveType),
      mStageName(pStageName),
      mScenarioNo(scenarioNo),
      _C(-1) {
    mIDInfo._0 = -1;
    mIDInfo.mZoneID = -1;

    if (pMapIdInfo != nullptr) {
        mIDInfo = *pMapIdInfo;
    } else {
        mIDInfo = MR::getInitializeStartIdInfo();
    }
}

bool GalaxyMoveArgument::isEqualStage(const char* pStageName) const {
    if (mStageName == nullptr) {
        return false;
    }

    return MR::isEqualString(pStageName, mStageName);
}

bool GalaxyMoveArgument::isEqualStageScenario(const char* pStageName, int scenarioNo) const {
    return isEqualStage(pStageName) && scenarioNo == mScenarioNo;
}
