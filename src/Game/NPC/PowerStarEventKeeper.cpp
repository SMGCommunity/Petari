#include "Game/NPC/PowerStarEventKeeper.hpp"
#include "Game/MapObj/PowerStarHolder.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/NPC/StageStateKeeper.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

PowerStarEventKeeper::PowerStarEventKeeper() : mStartScenario(1), mPowerStarNum(1), mInfoTbl() {
    mPowerStarNum = MR::getCurrentStagePowerStarNum();
    initStarInfoTable();
}

void PowerStarEventKeeper::initStarInfoTable() {
    mInfoTbl = new PowerStarInfo[mPowerStarNum];

    for (s32 starID = 0; starID < mPowerStarNum; starID++) {
        PowerStarInfo* pInfo = getStarInfo(starID + mStartScenario - 1);
        pInfo->_0 = false;
        pInfo->_1 = false;
        pInfo->_2 = false;
        pInfo->_3 = false;
        pInfo->mDemoName = nullptr;
    }

    for (s32 scenarioNo = mStartScenario; scenarioNo <= mPowerStarNum; scenarioNo++) {
        if (isExistPowerStarAtScenario(scenarioNo)) {
            getStarInfo(scenarioNo - 1)->_0 = true;
        }

        if (MR::getAppearPowerStarObjName(scenarioNo) == nullptr) {
            getStarInfo(scenarioNo - 1)->_1 = true;
        }
    }
}

s32 PowerStarEventKeeper::findStarID(const char* pName) const {
    if (MR::getAppearPowerStarObjName(MR::getCurrentScenarioNo()) != nullptr &&
        MR::isEqualString(pName, MR::getAppearPowerStarObjName(MR::getCurrentScenarioNo()))) {
        return MR::getCurrentScenarioNo();
    }

    for (s32 starID = mStartScenario; starID <= mPowerStarNum; starID++) {
        if (starID == MR::getCurrentScenarioNo()) {
            continue;
        }

        if (MR::getAppearPowerStarObjName(starID) == nullptr) {
            continue;
        }

        if (MR::isEqualString(pName, MR::getAppearPowerStarObjName(starID))) {
            return starID;
        }
    }

    return -1;
}

void PowerStarEventKeeper::declareStar(const char* pName1, const char* pName2, s32 starID, bool myBool) {
    if (starID < 0) {
        starID = findStarID(pName1);
    }

    if (!isCorrectStarIdWithBBS(pName1, pName2, starID)) {
        return;
    }

    PowerStarInfo* pInfo = getStarInfo(starID - 1);
    pInfo->mDemoName = pName1;
    pInfo->_2 = myBool;
}

bool PowerStarEventKeeper::isGreen(const char* pName, s32 starID) const {
    if (starID < 0) {
        starID = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starID)) {
        return false;
    }

    return MR::isPowerStarGreenInCurrentStage(starID);
}

bool PowerStarEventKeeper::isRed(const char* pName, s32 starID) const {
    if (starID < 0) {
        starID = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starID)) {
        return false;
    }

    return MR::isPowerStarRedInCurrentStage(starID);
}

bool PowerStarEventKeeper::isGrand(const char* pName, s32 starID) const {
    if (starID < 0) {
        starID = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starID)) {
        return false;
    }

    return MR::isGrandStarInCurrentStage(starID);
}

void PowerStarEventKeeper::initStarInfoTableAfterPlacement() {
    PowerStarInfo* pInfo;
    for (s32 powerStarNo = 0; powerStarNo < mPowerStarNum; powerStarNo++) {
        pInfo = getStarInfo(powerStarNo + mStartScenario - 1);

        if (!pInfo->_0) {
            continue;
        }

        if (pInfo->mDemoName != nullptr) {
            PowerStar* pPowerStar = PowerStarFunction::findPowerStar(mStartScenario + powerStarNo);
            pPowerStar->mIsInDemo = true;

            if (pInfo->_2) {
                pPowerStar->kill();
            }

            continue;
        }

        if (pInfo->_1) {
            PowerStarFunction::findPowerStar(mStartScenario + powerStarNo)->appear();
        }
    }
}

void PowerStarEventKeeper::requestAppearPowerStar(const char* pName, s32 starNum, const TVec3f* pPos, bool forceAppear, bool a2) NO_INLINE {
    StageStateKeeper* pStateKeeper = EventFunction::getStageStateKeeper();
    pStateKeeper->mIsPowerStarAppeared = true;

    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    getStarInfo(starNum - 1)->_3 = true;

    if (a2) {
        EventFunction::getPowerStarHolder()->appearPowerStarWithoutDemo(starNum);
    } else {
        EventFunction::getPowerStarHolder()->requestAppearPowerStar(starNum, pPos, forceAppear);
    }
}

bool PowerStarEventKeeper::isExistPowerStarAtScenario(int scenarioNo) const {
    GalaxyStatusAccessor accessor = MR::makeCurrentGalaxyStatusAccessor();
    return accessor.getActivePowerStarId(MR::getCurrentScenarioNo()) & calcStarID(scenarioNo);
}

bool PowerStarEventKeeper::isCorrectStarIdWithBBS(const char*, const char*, s32 scenarioNo) const {
    if (scenarioNo <= 0) {
        return false;
    }

    return isExistPowerStarAtScenario(scenarioNo);
}

bool PowerStarEventKeeper::isSuccess(const char* pName, s32 starNum) const {
    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starNum)) {
        return false;
    }

    return MR::hasPowerStarInCurrentStage(starNum);
}
