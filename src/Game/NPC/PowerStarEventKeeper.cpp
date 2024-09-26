#include "Game/NPC/PowerStarEventKeeper.hpp"
#include "Game/Util.hpp"

PowerStarEventKeeper::PowerStarEventKeeper() {
    _0 = 1;
    mPowerStarNum = 1;
    mInfoTbl = nullptr;
    mPowerStarNum = MR::getCurrentStagePowerStarNum();
    initStarInfoTable();
}

/*
void PowerStarEventKeeper::initStarInfoTable() {
    mInfoTbl = new PowerStarInfo*[mPowerStarNum * 2];

    for (s32 i = 0; i < mPowerStarNum; i++) {
        PowerStarInfo* inf = mInfoTbl[(i - 1) + _0];
        inf->_0 = 0;
        inf->_1 = 0;
        inf->_2 = 0;
        inf->_3 = 0;
        inf->_4 = nullptr;
    }

    for (s32 i = _0; i < mPowerStarNum; i++) {
        if (isExistPowerStarAtScenario(i)) {
            mInfoTbl[(i * 8) - 1]->_0 = 1;
        }

        if (!MR::getAppearPowerStarObjName(i)) {
            mInfoTbl[(i - 1) * 8]->_1 = 1;
        }
    }
}
*/

// PowerStarEventKeeper::declareStar

bool PowerStarEventKeeper::isGreen(const char *pName, s32 starNum) const {
    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starNum)) {
        return false;
    }

    return MR::isPowerStarGreenInCurrentStage(starNum);
}

bool PowerStarEventKeeper::isRed(const char *pName, s32 starNum) const {
    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starNum)) {
        return false;
    }

    return MR::isPowerStarRedInCurrentStage(starNum);
}

bool PowerStarEventKeeper::isGrand(const char *pName, s32 starNum) const {
    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starNum)) {
        return false;
    }

    return MR::isGrandStarInCurrentStage(starNum);
}

// PowerStarEventKeeper::initStarInfoTableAfterPlacement
// PowerStarEventKeeper::requestAppearPowerStar
// PowerStarEventKeeper::isExistPowerStarAtScenario
// PowerStarEventKeeper::isCorrectStarIdWithBBS

bool PowerStarEventKeeper::isSuccess(const char *pName, s32 starNum) const {
    if (starNum < 0) {
        starNum = findStarID(pName);
    }

    if (!isCorrectStarIdWithBBS(pName, "ゾーン不明", starNum)) {
        return false;
    }

    return MR::hasPowerStarInCurrentStage(starNum);
}