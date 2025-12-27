#include "Game/System/AlreadyDoneFlagInGalaxy.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include <cstdio>

GameDataTemporaryInGalaxy::GameDataTemporaryInGalaxy() :
    mPlayerRestartIdInfo(nullptr),
    mAlreadyDoneFlag(nullptr)
{
    mPlayerRestartIdInfo = new JMapIdInfo(MR::getInitializeStartIdInfo());
    mAlreadyDoneFlag = new AlreadyDoneFlagInGalaxy(64);

    resetStageResultParam();

    _4 = false;
}

void GameDataTemporaryInGalaxy::resetPlayerRestartIdInfo() {
    *mPlayerRestartIdInfo = MR::getInitializeStartIdInfo();
}

void GameDataTemporaryInGalaxy::setPlayerRestartIdInfo(const JMapIdInfo& rPlayerRestartIdInfo) {
    *mPlayerRestartIdInfo = rPlayerRestartIdInfo;
}

void GameDataTemporaryInGalaxy::resetRaceBestTime() {
    _4C = 0;
    _50 = 0;
}

bool GameDataTemporaryInGalaxy::isStageCleared() const {
    return _8 == 1;
}

void GameDataTemporaryInGalaxy::resetStageResultPowerStarParam() {
    _8 = 0;
    mPowerStarId = 1;
    mStageName[0] = '\0';
}

void GameDataTemporaryInGalaxy::resetStageResultStarPieceParam() {
    mStarPieceNum = 0;
    mLast1upStarPieceNum = 0;
}

void GameDataTemporaryInGalaxy::resetStageResultCoinParam() {
    mCoinNum = 0;
}

void GameDataTemporaryInGalaxy::addStarPiece(int num) {
    s32 starPiece = mStarPieceNum + num;

    mStarPieceNum = MR::clamp(starPiece, 0, 999);
}

void GameDataTemporaryInGalaxy::setLast1upStarPieceNum(int num) {
    mLast1upStarPieceNum = num;
}

s32 GameDataTemporaryInGalaxy::getStarPieceNum() const {
    return MR::clamp(static_cast<s32>(mStarPieceNum), 0, 999);
}

void GameDataTemporaryInGalaxy::resetStageResultParam() {
    resetStageResultPowerStarParam();
    resetStageResultStarPieceParam();
    resetStageResultCoinParam();
}

void GameDataTemporaryInGalaxy::clearAlreadyDoneFlag() {
    mAlreadyDoneFlag->clear();
}

s32 GameDataTemporaryInGalaxy::setupAlreadyDoneFlag(const char* pZoneName, const JMapInfoIter& rIter, u32* pParam3) {
    return mAlreadyDoneFlag->setupFlag(pZoneName, rIter, pParam3);
}

void GameDataTemporaryInGalaxy::updateAlreadyDoneFlag(int index, u32 param2) {
    mAlreadyDoneFlag->updateValue(index, param2);
}

void GameDataTemporaryInGalaxy::receiveStageResultParam(const char* pStageName, s32 powerStarId, int starPieceNum, int coinNum) {
    resetStageResultParam();

    mPowerStarId = powerStarId;
    _8 = 1;
    mStarPieceNum = starPieceNum;
    mCoinNum = coinNum;

    snprintf(mStageName, sizeof(mStageName), "%s", pStageName);
}
