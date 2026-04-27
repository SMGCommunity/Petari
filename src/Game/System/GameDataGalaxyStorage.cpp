#include "Game/System/GameDataGalaxyStorage.hpp"
#include "Game/System/BinaryDataContentAccessor.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/StringUtil.hpp"

bool GameDataSomeScenarioAccessor::hasPowerStar() const {
    return mSomeGalaxyStorage->hasPowerStar(mScenarioNum - 1);
}

bool GameDataSomeScenarioAccessor::isAlreadyVisited() const {
    return mSomeGalaxyStorage->isAlreadyVisited(mScenarioNum - 1);
}

s32 GameDataSomeScenarioAccessor::getMaxCoinNum() const {
    return MR::clamp(mSomeGalaxyStorage->getMaxCoinNum(mScenarioNum - 1), 0, 999L);
}

void GameDataSomeScenarioAccessor::setPowerStarFlag(bool val) {
    setBitFlagAccordingToBool(&mSomeGalaxyStorage->mPowerStarOwnedFlags, val);
}

void GameDataSomeScenarioAccessor::setFlagAlreadyVisited(bool val) {
    setBitFlagAccordingToBool(&mSomeGalaxyStorage->mAlreadyVisitedFlags, val);
}

void GameDataSomeScenarioAccessor::updateMaxCoinNum(int coinNum) {
    if (getMaxCoinNum() < coinNum) {
        s32 scenarioNo = mScenarioNum - 1;
        u16 maxCoins = MR::clamp(coinNum, 0, 999L);
        mSomeGalaxyStorage->setMaxCoinNum(scenarioNo, maxCoins);
    }
}

void GameDataSomeScenarioAccessor::setBitFlagAccordingToBool(u8* pFlags, bool val) {
    if (val) {
        *pFlags |= (1 << (mScenarioNum - 1));
    } else {
        *pFlags &= ~(1 << (mScenarioNum - 1));
    }
}

GameDataSomeGalaxyStorage::GameDataSomeGalaxyStorage(const GalaxyStatusAccessor& rAccessor) {
    mGalaxyName = rAccessor.getName();
    mPowerStarNum = rAccessor.getPowerStarNum();
    resetAllData();
}

void GameDataSomeGalaxyStorage::resetAllData() {
    mPowerStarOwnedFlags = 0b00000000;
    mAlreadyVisitedFlags = 0b00000000;
    _A = 0;
    _B = 0;

    for (s32 idx = 0; idx < 8; idx++) {
        mMaxCoinNum[idx] = 0;
    }
}

s32 GameDataSomeGalaxyStorage::getPowerStarNumOwned() const {
    s32 numOwned = 0;
    for (s32 scenarioNo = 1; scenarioNo <= mPowerStarNum; scenarioNo++) {
        if (hasPowerStar(scenarioNo - 1)) {
            numOwned++;
        }
    }
    return numOwned;
}

s32 GameDataSomeGalaxyStorage::deserialize(const BinaryDataContentAccessor& rAccessor, const u8* pData) {
    u8* powerStarFlags = (u8*)rAccessor.getPointer("mPowerStarFlag", (u8*)pData);
    if (powerStarFlags != nullptr) {
        mPowerStarOwnedFlags = *powerStarFlags;
    } else {
        mPowerStarOwnedFlags = 0b00000000;
    }

    u8* firstPlayFlags = (u8*)rAccessor.getPointer("mFirstPlayFlag", (u8*)pData);
    if (firstPlayFlags != nullptr) {
        mAlreadyVisitedFlags = *firstPlayFlags;
    } else {
        mAlreadyVisitedFlags = 0b00000000;
    }

    u16* maxCoinNums = (u16*)rAccessor.getPointer("mMaxCoinNum", (u8*)pData);
    for (s32 idx = 0; idx < 8; idx++) {
        if (maxCoinNums != nullptr) {
            mMaxCoinNum[idx] = maxCoinNums[idx];
        } else {
            mMaxCoinNum[idx] = 0;
        }
    }

    if (powerStarFlags == nullptr || firstPlayFlags == nullptr || maxCoinNums == nullptr) {
        return 1;
    }

    return 0;
}

GameDataAllGalaxyStorage::GameDataAllGalaxyStorage() : mHeaderSerializer(nullptr) {
    mHeaderSerializer = new BinaryDataContentHeaderSerializer(new u8[32], 32);
    mSomeGalaxyStorages.init(64);

    ScenarioDataIter iter = MR::makeBeginScenarioDataIter();
    while (!iter.isEnd()) {
        GalaxyStatusAccessor accessor = iter.makeAccessor();
        if (accessor.getPowerStarNum() != 0) {
            mSomeGalaxyStorages.push_back(new GameDataSomeGalaxyStorage(accessor));
        }
        iter.goNext();
    }
    initHeaderSerializer();
}

bool GameDataAllGalaxyStorage::isExistAccessor(const char* pGalaxyName, int scenarioNum) const {
    if (findIndex(pGalaxyName) < 0) {
        return false;
    }

    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
    if (scenarioNum >= 1 && scenarioNum <= accessor.getScenarioNum()) {
        return true;
    }

    return false;
}

GameDataSomeScenarioAccessor GameDataAllGalaxyStorage::makeAccessor(const char* pGalaxyName, int scenarioNum) const {
    return GameDataSomeScenarioAccessor(mSomeGalaxyStorages[findIndex(pGalaxyName)], scenarioNum);
}

s32 GameDataAllGalaxyStorage::calcCurrentPowerStarNum() const {
    s32 powerStarNum = 0;
    for (s32 idx = 0; idx < getGalaxyNum(); idx++) {
        powerStarNum += mSomeGalaxyStorages[idx]->getPowerStarNumOwned();
    }
    return powerStarNum;
}

s32 GameDataAllGalaxyStorage::getPowerStarNumOwned(const char* pGalaxyName) const {
    return mSomeGalaxyStorages[findIndex(pGalaxyName)]->getPowerStarNumOwned();
}

s32 GameDataAllGalaxyStorage::getGalaxyNum() const {
    return mSomeGalaxyStorages.size();
}

GameDataSomeGalaxyStorage* GameDataAllGalaxyStorage::getGalaxyStorage(int index) {
    return mSomeGalaxyStorages[index];
}

u32 GameDataAllGalaxyStorage::makeHeaderHashCode() const {
    return 0xBF0640EE;
}

u32 GameDataAllGalaxyStorage::getSignature() const {
    return 'GALA';
}

s32 GameDataAllGalaxyStorage::deserialize(const u8* pData, u32 dataSize) {
    // FIXME: regswaps
    // https://decomp.me/scratch/1nmoc

    s32 attributeNum = *(u16*)(pData + 0);
    BinaryDataContentAccessor accessor((u8*)(pData + 2));
    u32 readOffset = accessor.getHeaderSize() + 2;

    bool readError = false;

    for (s32 idx = 0; idx < attributeNum; idx++) {
        const char* name = "mGalaxyName";
        s32 galaxyIndex = findIndex(*(u16*)accessor.getPointer(name, (u8*)(pData + readOffset)));
        if (galaxyIndex >= 0) {
            switch (mSomeGalaxyStorages[galaxyIndex]->deserialize(accessor, (u8*)(pData + readOffset))) {
            case 0:
                break;
            case 1:
                readError = true;
                break;
            default:
                break;
            }
        } else {
            readError = true;
        }
        readOffset += accessor.getDataSize();
    }

    if (readError) {
        return 1;
    }

    return 0;
}

void GameDataAllGalaxyStorage::initializeData() {
    for (s32 idx = 0; idx < getGalaxyNum(); idx++) {
        getGalaxyStorage(idx)->resetAllData();
    }
}

void GameDataAllGalaxyStorage::initHeaderSerializer() {
    mHeaderSerializer->addAttribute("mGalaxyName", sizeof(u16));
    mHeaderSerializer->addAttribute("mPowerStarFlag", sizeof(u8));
    mHeaderSerializer->addAttribute("mFirstPlayFlag", sizeof(u8));
    mHeaderSerializer->addAttribute("mMaxCoinNum", sizeof(u16) * 8);
    mHeaderSerializer->flush();
}

s32 GameDataAllGalaxyStorage::findIndex(const char* pGalaxyName) const {
    for (s32 idx = 0; idx < getGalaxyNum(); idx++) {
        if (MR::isEqualString(pGalaxyName, mSomeGalaxyStorages[idx]->mGalaxyName)) {
            return idx;
        }
    }
    return -1;
}

s32 GameDataAllGalaxyStorage::findIndex(u16 galaxyNameHash) const {
    for (s32 idx = 0; idx < getGalaxyNum(); idx++) {
        if ((u16)MR::getHashCode(mSomeGalaxyStorages[idx]->mGalaxyName) == galaxyNameHash) {
            return idx;
        }
    }
    return -1;
}

void GameDataSomeGalaxyStorage::serialize(const BinaryDataContentAccessor& rAccessor, u8* pData) const {
    u8* powerStarFlags = (u8*)rAccessor.getPointer("mPowerStarFlag", pData);
    *powerStarFlags = mPowerStarOwnedFlags;

    u8* firstPlayFlags = (u8*)rAccessor.getPointer("mFirstPlayFlag", pData);
    *firstPlayFlags = mAlreadyVisitedFlags;

    u16* maxCoinNums = (u16*)rAccessor.getPointer("mMaxCoinNum", pData);
    for (s32 idx = 0; idx < 8; idx++) {
        maxCoinNums[idx] = mMaxCoinNum[idx];
    }
}

s32 GameDataAllGalaxyStorage::serialize(u8* pData, u32 dataSize) const {
    // FIXME: regswap and missing load
    // https://decomp.me/scratch/NCESx

    *(u16*)(pData + 0) = getGalaxyNum();
    MR::copyMemory(pData + 2, getSerializer()->getBuffer(), getSerializer()->getHeaderSize());

    s32 writeOffset = getSerializer()->getHeaderSize() + 2;

    BinaryDataContentAccessor accessor((u8*)getSerializer()->getBuffer());
    u8* offs;
    for (s32 idx = 0; idx < getGalaxyNum(); idx++) {
        GameDataSomeGalaxyStorage* storage = mSomeGalaxyStorages[idx];
        const char* name = "mGalaxyName";
        u16* writePtr = (u16*)accessor.getPointer(name, pData + writeOffset);
        *writePtr = MR::getHashCode(storage->mGalaxyName);
        storage->serialize(accessor, pData + writeOffset);
        writeOffset += getSerializer()->getDataSize();
    }

    return writeOffset;
}
