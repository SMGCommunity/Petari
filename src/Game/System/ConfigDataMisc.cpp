#include "Game/System/ConfigDataMisc.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

#define FLAG_LAST_LOADED_MARIO 0x1
#define FLAG_COMPLETE_ENDING_MARIO 0x2
#define FLAG_COMPLETE_ENDING_LUIGI 0x4

ConfigDataMisc::ConfigDataMisc()
    : mFlag(FLAG_LAST_LOADED_MARIO),
      mLastModified(0) {
    initializeData();
}

u32 ConfigDataMisc::makeHeaderHashCode() const {
    return 0x1;
}

u32 ConfigDataMisc::getSignature() const {
    return 'MISC';
}

s32 ConfigDataMisc::serialize(u8* pBuffer, u32 size) const {
    JSUMemoryOutputStream stream = JSUMemoryOutputStream(pBuffer, size);

    u8 flag = mFlag;
    stream.write(&flag, sizeof(flag));

    OSTime lastModified = mLastModified;
    stream.write(&lastModified, sizeof(lastModified));

    return stream.mPosition;
}

s32 ConfigDataMisc::deserialize(const u8* pBuffer, u32 size) {
    initializeData();

    JSUMemoryInputStream stream = JSUMemoryInputStream(pBuffer, size);

    u8 flag;
    stream.read(&flag, sizeof(flag));
    mFlag = flag;

    if (stream.getAvailable() == 0) {
        mLastModified = 0;
    } else {
        OSTime lastModified;
        stream.read(&lastModified, sizeof(lastModified));
        mLastModified = lastModified;
    }

    return 0;
}

void ConfigDataMisc::initializeData() {
    mFlag = FLAG_LAST_LOADED_MARIO;
    mLastModified = 0;
}

bool ConfigDataMisc::isLastLoadedMario() const {
    return !((mFlag & FLAG_LAST_LOADED_MARIO) - 1);
}

void ConfigDataMisc::setLastLoadedMario(bool lastLoadedMario) {
    if (lastLoadedMario) {
        mFlag |= FLAG_LAST_LOADED_MARIO;
    } else {
        mFlag &= ~FLAG_LAST_LOADED_MARIO;
    }
}

void ConfigDataMisc::onCompleteEndingMario() {
    mFlag |= FLAG_COMPLETE_ENDING_MARIO;
}

void ConfigDataMisc::onCompleteEndingLuigi() {
    mFlag |= FLAG_COMPLETE_ENDING_LUIGI;
}

bool ConfigDataMisc::isOnCompleteEndingMario() {
    return (mFlag & FLAG_COMPLETE_ENDING_MARIO) != 0;
}

bool ConfigDataMisc::isOnCompleteEndingLuigi() {
    return (mFlag & FLAG_COMPLETE_ENDING_LUIGI) != 0;
}

OSTime ConfigDataMisc::getLastModified() const {
    return mLastModified;
}

void ConfigDataMisc::updateLastModified() {
    mLastModified = OSGetTime();
}
