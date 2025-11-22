#include "Game/System/GameDataHolder.hpp"
#include "Game/System/ConfigDataHolder.hpp"
#include "Game/System/UserFile.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <RVLFaceLib.h>

#define USER_NAME_SIZE (RFL_NAME_LEN + 1)

UserFile::UserFile()
    : mGameDataHolder(nullptr),
      mConfigDataHolder(nullptr),
      mIsPlayerMario(true),
      mIsGameDataCorrupted(false),
      mIsConfigDataCorrupted(false),
      mUserName(nullptr) {
    mGameDataHolder = new GameDataHolder(this);
    mConfigDataHolder = new ConfigDataHolder();
    mUserName = new wchar_t[USER_NAME_SIZE];

    MR::zeroMemory(mUserName, USER_NAME_SIZE * sizeof(wchar_t));
}

bool UserFile::isCreated() const {
    return mConfigDataHolder->isCreated();
}

s32 UserFile::getPowerStarNum() const {
    return mGameDataHolder->calcCurrentPowerStarNum();
}

s32 UserFile::getStarPieceNum() const {
    return mGameDataHolder->getStockedStarPieceNum();
}

s32 UserFile::getPlayerMissNum() const {
    return mGameDataHolder->getPlayerMissNum();
}

bool UserFile::getMiiId(void* pMiiId) const {
    return mConfigDataHolder->getMiiId(pMiiId);
}

bool UserFile::getIconId(u32* pIconId) const {
    return mConfigDataHolder->getIconId(pIconId);
}

bool UserFile::isLastLoadedMario() const {
    return mConfigDataHolder->isLastLoadedMario();
}

bool UserFile::isOnCompleteEndingMarioAndLuigi() const {
    return mConfigDataHolder->isOnCompleteEndingMario() && mConfigDataHolder->isOnCompleteEndingLuigi();
}

OSTime UserFile::getLastModified() const {
    return mConfigDataHolder->getLastModified();
}

void UserFile::setCreated() {
    mConfigDataHolder->setIsCreated(true);
}

void UserFile::setMiiOrIconId(const void* pMiiId, const u32* pIconId) {
    mConfigDataHolder->setMiiOrIconId(pMiiId, pIconId);
}

void UserFile::setLastLoadedMario(bool lastLoadedMario) {
    mConfigDataHolder->setLastLoadedMario(lastLoadedMario);
}

void UserFile::onCompleteEndingCurrentPlayer() {
    if (mIsPlayerMario) {
        mConfigDataHolder->onCompleteEndingMario();
    } else {
        mConfigDataHolder->onCompleteEndingLuigi();
    }
}

void UserFile::updateLastModified() {
    mConfigDataHolder->updateLastModified();
}

void UserFile::setUserName(const wchar_t* pUserName) {
    MR::copyMemory(mUserName, pUserName, USER_NAME_SIZE * sizeof(wchar_t));
}

const char* UserFile::getGameDataName() const {
    return mGameDataHolder->mName;
}

void UserFile::makeGameDataBinary(u8* pBuffer, u32 size) const {
    mGameDataHolder->makeFileBinary(pBuffer, size);
}

void UserFile::loadFromGameDataBinary(const char* pName, const u8* pBuffer, u32 size) {
    mIsGameDataCorrupted = !mGameDataHolder->loadFromFileBinary(pName, pBuffer, size);
}

const char* UserFile::getConfigDataName() const {
    return mConfigDataHolder->mName;
}

void UserFile::makeConfigDataBinary(u8* pBuffer, u32 size) const {
    mConfigDataHolder->makeFileBinary(pBuffer, size);
}

void UserFile::loadFromConfigDataBinary(const char* pName, const u8* pBuffer, u32 size) {
    mIsConfigDataCorrupted = !mConfigDataHolder->loadFromFileBinary(pName, pBuffer, size);
}

void UserFile::resetAllData() {
    mGameDataHolder->resetAllData();
    mConfigDataHolder->resetAllData();

    mIsGameDataCorrupted = false;
    mIsConfigDataCorrupted = false;
}

bool UserFile::isViewNormalEnding() const {
    return mGameDataHolder->isOnGameEventFlag("ViewNormalEnding");
}

bool UserFile::isViewCompleteEnding() const {
    return mGameDataHolder->isOnGameEventFlag("ViewCompleteEnding");
}

bool UserFile::isPowerStarGetFinalChallengeGalaxy() const {
    return mGameDataHolder->isOnGameEventFlag("SpecialStarFinalChallenge");
}
