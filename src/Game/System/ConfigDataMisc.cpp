#include "Game/System/ConfigDataMisc.hpp"
#include "JSystem/JSupport/JSUMemoryInputStream.hpp"
#include "JSystem/JSupport/JSUMemoryOutputStream.hpp"

ConfigDataMisc::ConfigDataMisc() {
    mData = 1;
    mLastModified = 0;

    initializeData();
}

bool ConfigDataMisc::isLastLoadedMario() const {
    return !((mData & 0x1) - 1);
}

void ConfigDataMisc::setLastLoadedMario(bool lastLoaded) {
    if (lastLoaded) {
        mData |= 0x1;
    }
    else {
        mData &= 0xFE;
    }
}

void ConfigDataMisc::onCompleteEndingMario() {
    mData |= 0x2;
}

void ConfigDataMisc::onCompleteEndingLuigi() {
    mData |= 0x4;
}

bool ConfigDataMisc::isOnCompleteEndingMario() {
    return (mData >> 1) & 0x1;
}

bool ConfigDataMisc::isOnCompleteEndingLuigi() {
    return (mData >> 2) & 0x1;
}

OSTime ConfigDataMisc::getLastModified() const {
    return mLastModified;
}

void ConfigDataMisc::updateLastModified() {
    mLastModified = OSGetTime();
}

u32 ConfigDataMisc::makeHeaderHashCode() const {
    return 1;
}

u32 ConfigDataMisc::getSignature() const {
    return 0x4D495343;
}

s32 ConfigDataMisc::serialize(u8 *pData, u32 len) const {
    JSUMemoryOutputStream stream;
    stream.setBuffer(pData, len);
    u8 stack_8 = mData;
    stream.write(&stack_8, 1);
    OSTime stack_10 = mLastModified;
    stream.write(&stack_10, 8);
    return stream.mPosition;
}

s32 ConfigDataMisc::deserialize(const u8 *pData, u32 len) {
    initializeData();
    JSUMemoryInputStream stream;
    stream.setBuffer(pData, len);
    u8 stack_8;
    stream.read(&stack_8, 1);
    mData = stack_8;

    if (!stream.getAvailable()) {
        mLastModified = 0;
    }
    else {
        OSTime time;
        stream.read(&time, 8);
        mLastModified = time;
    }

    return 0;
}

void ConfigDataMisc::initializeData() {
    mData = 1;
    mLastModified = 0;
}