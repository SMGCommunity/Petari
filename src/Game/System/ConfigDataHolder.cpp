#include "Game/System/ConfigDataHolder.hpp"
#include "Game/System/ConfigDataMii.hpp"
#include "Game/System/ConfigDataMisc.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <cstdio>

ConfigDataHolder::ConfigDataHolder() :
    mChunkHolder(nullptr),
    mCreateChunk(nullptr),
    mMii(nullptr),
    mMisc(nullptr)
{
    mChunkHolder = new BinaryDataChunkHolder(64, 3);
    mCreateChunk = new ConfigDataCreateChunk();
    mMii = new ConfigDataMii();
    mMisc = new ConfigDataMisc();

    mChunkHolder->addChunk(mCreateChunk);
    mChunkHolder->addChunk(mMii);
    mChunkHolder->addChunk(mMisc);
    resetAllData();
    snprintf(mName, sizeof(mName), "config1");
}

void ConfigDataHolder::setIsCreated(bool isCreated) {
    mCreateChunk->mIsCreated = isCreated;
}

bool ConfigDataHolder::isCreated() const {
    return mCreateChunk->mIsCreated;
}

void ConfigDataHolder::setLastLoadedMario(bool lastLoadedMario) {
    mMisc->setLastLoadedMario(lastLoadedMario);
}

bool ConfigDataHolder::isLastLoadedMario() const {
    return mMisc->isLastLoadedMario();
}

void ConfigDataHolder::onCompleteEndingMario() {
    mMisc->onCompleteEndingMario();
}

void ConfigDataHolder::onCompleteEndingLuigi() {
    mMisc->onCompleteEndingLuigi();
}

bool ConfigDataHolder::isOnCompleteEndingMario() {
    return mMisc->isOnCompleteEndingMario();
}

bool ConfigDataHolder::isOnCompleteEndingLuigi() {
    return mMisc->isOnCompleteEndingLuigi();
}

void ConfigDataHolder::updateLastModified() {
    mMisc->updateLastModified();
}

OSTime ConfigDataHolder::getLastModified() const {
    return mMisc->getLastModified();
}

void ConfigDataHolder::setMiiOrIconId(const void* pMiiId, const u32* pIconId) {
    mMii->setMiiOrIconId(pMiiId, pIconId);
}

bool ConfigDataHolder::getMiiId(void* pMiiId) const {
    return mMii->getMiiId(pMiiId);
}

bool ConfigDataHolder::getIconId(u32* pIconId) const {
    return mMii->getIconId(pIconId);
}

void ConfigDataHolder::resetAllData() {
    mCreateChunk->initializeData();
    mMii->initializeData();
    mMisc->initializeData();
}

s32 ConfigDataHolder::makeFileBinary(u8* pBuffer, u32 size) {
    return mChunkHolder->makeFileBinary(pBuffer, size);
}

bool ConfigDataHolder::loadFromFileBinary(const char* pName, const u8* pBuffer, u32 size) {
    snprintf(mName, sizeof(mName), "%s", pName);

    return mChunkHolder->loadFromFileBinary(pBuffer, size);
}

ConfigDataCreateChunk::ConfigDataCreateChunk() :
    mIsCreated(false)
{
    initializeData();
}

u32 ConfigDataCreateChunk::makeHeaderHashCode() const {
    return 0x2432DA;
}

u32 ConfigDataCreateChunk::getSignature() const {
    return 'CONF';
}

s32 ConfigDataCreateChunk::serialize(u8* pBuffer, u32 size) const {
    JSUMemoryOutputStream stream = JSUMemoryOutputStream(pBuffer, size);

    u8 isCreated = -(mIsCreated != false);
    stream.write(&isCreated, sizeof(mIsCreated));

    return stream.mPosition;
}

s32 ConfigDataCreateChunk::deserialize(const u8* pBuffer, u32 size) {
    initializeData();

    JSUMemoryInputStream stream = JSUMemoryInputStream(pBuffer, size);

    u8 isCreated;
    stream.read(&isCreated, sizeof(mIsCreated));
    mIsCreated = isCreated != 0;

    return 0;
}

void ConfigDataCreateChunk::initializeData() {
    mIsCreated = false;
}
