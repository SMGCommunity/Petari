#include "Game/System/ConfigDataHolder.h"
#include "JSystem/JSupport/JSUMemoryInputStream.h"
#include "JSystem/JSupport/JSUMemoryOutputStream.h"
#include <cstdio>

ConfigDataCreateChunk::ConfigDataCreateChunk() : mData(0) {
  initializeData();
}

u32 ConfigDataCreateChunk::makeHeaderHashCode() const {
    return 0x2432DA;
}

u32 ConfigDataCreateChunk::getSignature() const {
    return 0x434F4E46;
}

s32 ConfigDataCreateChunk::serialize(u8 *pData, u32 len) const {
    JSUMemoryOutputStream stream;
    stream.setBuffer(pData, len);
    u8 data = -(mData != 0); 
    stream.write(&data, 1);
    return stream.mPosition;
}

s32 ConfigDataCreateChunk::deserialize(const u8 *pData, u32 len) {
    initializeData();
    JSUMemoryInputStream stream;
    stream.setBuffer(pData, len);
    u8 data;
    stream.read(&data, 1);
    mData = data != 0;
    return 0;
}

void ConfigDataCreateChunk::initializeData() {
    mData = 0;
}

void ConfigDataHolder::setIsCreated(bool isCreated) {
    mCreateChunk->mData = isCreated;
}

bool ConfigDataHolder::isCreated() const {
    return mCreateChunk->mData;
} 

void ConfigDataHolder::setLastLoadedMario(bool isLoaded) {
    mMiscCreateChunk->setLastLoadedMario(isLoaded);
}

bool ConfigDataHolder::isLastLoadedMario() const {
    return mMiscCreateChunk->isLastLoadedMario();
}

void ConfigDataHolder::onCompleteEndingMario() {
    mMiscCreateChunk->onCompleteEndingMario();
}

void ConfigDataHolder::onCompleteEndingLuigi() {
    mMiscCreateChunk->onCompleteEndingLuigi();
}

bool ConfigDataHolder::isOnCompleteEndingMario() {
    return mMiscCreateChunk->isOnCompleteEndingMario();
}

bool ConfigDataHolder::isOnCompleteEndingLuigi() {
    return mMiscCreateChunk->isOnCompleteEndingLuigi();
}

void ConfigDataHolder::updateLastModified() {
    mMiscCreateChunk->updateLastModified();
}

OSTime ConfigDataHolder::getLastModified() const {
    return mMiscCreateChunk->getLastModified();
}

void ConfigDataHolder::setMiiOrIconId(const void *pMiiId, const u32 *pIconId) {
    mMiiCreateChunk->setMiiOrIconId(pMiiId, pIconId);
}

bool ConfigDataHolder::getMiiId(void *pId) const {
    return mMiiCreateChunk->getMiiId(pId);
}

bool ConfigDataHolder::getIconId(u32 *pId) const {
    return mMiiCreateChunk->getIconId(pId);
}

void ConfigDataHolder::resetAllData() {
    mCreateChunk->initializeData();
    mMiiCreateChunk->initializeData();
    mMiscCreateChunk->initializeData();
}

void ConfigDataHolder::makeFileBinary(u8 *pData, u32 len) {
    mChunkHolder->makeFileBinary(pData, len);
}

ConfigDataHolder::ConfigDataHolder() : mChunkHolder(0), mCreateChunk(0), mMiiCreateChunk(0), mMiscCreateChunk(0) {
    mChunkHolder = new BinaryDataChunkHolder(0x40, 3);
    mCreateChunk = new ConfigDataCreateChunk();
    mMiiCreateChunk = new ConfigDataMii();
    mMiscCreateChunk = new ConfigDataMisc();
    mChunkHolder->addChunk(mCreateChunk);
    mChunkHolder->addChunk(mMiiCreateChunk);
    mChunkHolder->addChunk(mMiscCreateChunk);
    resetAllData();
    snprintf(mName, 0x10, "config1");
}

void ConfigDataHolder::loadFromFileBinary(const char *pName, const u8 *pData, u32 len) {
    snprintf(mName, 0x10, "%s", pName);
    mChunkHolder->loadFromFileBinary(pData, len);
}