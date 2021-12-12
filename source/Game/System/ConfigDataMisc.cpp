#include "Game/System/ConfigDataMisc.h"

#ifdef NON_MATCHING
// weird issues with the call at the bottom
ConfigDataMisc::ConfigDataMisc() {
    mData = 1;
    mLastModified = 0;

    initializeData();
}
#endif

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
