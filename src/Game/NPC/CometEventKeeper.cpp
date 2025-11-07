#include "Game/NPC/CometEventExecutorTimeLimit.hpp"
#include "Game/NPC/CometEventKeeper.hpp"
#include "Game/Screen/GalaxyCometScreenFilter.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"

struct GalaxyTimeLimitInfo {
    /* 0x0 */ const char* mGalaxyName;
    /* 0x4 */ s32 mScenarioNo;
    /* 0x8 */ u32 mTimeLimit;
};

namespace {
    static const GalaxyTimeLimitInfo sTimeLimitInfoTable[] = {
        {"EggStarGalaxy", 4, 240},
        {"StarDustGalaxy", 4, 240},
        {"SandClockGalaxy", 4, 270},
        {"CannonFleetGalaxy", 4, 360},
        {"StarDustGalaxy", 5, 120},
        {"PhantomGalaxy", 5, 60},
        {"CosmosGardenGalaxy", 5, 150},
        {"FactoryGalaxy", 5, 180},
        {"ReverseKingdomGalaxy", 5, 210},
    };
};

CometEventKeeper::CometEventKeeper() :
    mExecutorTimeLimit(nullptr),
    mScreenFilter(nullptr),
    mCometName(nullptr),
    mCometStateIndex(0)
{
    
}

void CometEventKeeper::init() {
    initCometStatus();

    if (isStartEvent("Red") || isStartEvent("Black")) {
        mExecutorTimeLimit = new CometEventExecutorTimeLimit(
            getTimeLimitFromTable(sTimeLimitInfoTable, sizeof(sTimeLimitInfoTable) / sizeof(*sTimeLimitInfoTable)) / 60);
        mExecutorTimeLimit->initWithoutIter();
        mExecutorTimeLimit->kill();
    }

    if (mCometName != nullptr) {
        mScreenFilter = new GalaxyCometScreenFilter();
        mScreenFilter->initWithoutIter();
        mScreenFilter->_20 = true;
        mScreenFilter->setCometType(mCometName);
    }
}

bool CometEventKeeper::isStartEvent(const char* pParam1) const {
    if (mCometName == nullptr) {
        return false;
    }

    return MR::isEqualString(mCometName, pParam1);
}

bool CometEventKeeper::isStartTimeLimitEvent() const {
    return mExecutorTimeLimit != nullptr;
}

void CometEventKeeper::startCometEventIfExist() {
    if (mExecutorTimeLimit == nullptr) {
        return;
    }

    mExecutorTimeLimit->appear();
}

void CometEventKeeper::endCometEvent() {
    if (mExecutorTimeLimit == nullptr) {
        return;
    }

    mExecutorTimeLimit->kill();
    mScreenFilter->_20 = false;
}

u32 CometEventKeeper::getTimeLimitFromTable(const GalaxyTimeLimitInfo* pTable, int size) {
    for (int i = 0; i < size; i++) {
        const GalaxyTimeLimitInfo* pInfo = &pTable[i];

        if (pInfo->mScenarioNo != MR::getCurrentScenarioNo()) {
            continue;
        }

        if (!MR::isEqualString(MR::getCurrentStageName(), pInfo->mGalaxyName)) {
            continue;
        }

        return pInfo->mTimeLimit * 60;
    }

    return 0;
}

void CometEventKeeper::initCometStatus() {
    mCometName = MR::makeCurrentGalaxyStatusAccessor().getCometName(MR::getCurrentScenarioNo());
    mCometStateIndex = MR::getGalaxyCometStateIndexInCurrentStage();
}
