#include "Game/System/ConfigDataMii.hpp"
#include "Game/Util.hpp"
#include "JSystem/JSupport/JSUMemoryInputStream.hpp"
#include "JSystem/JSupport/JSUMemoryOutputStream.hpp"

ConfigDataMii::ConfigDataMii() {
    _4 = 0;
    mIconID = 1;
    mBuffer = 0;
    mBuffer = new u8[0x8];
    initializeData();
}

void ConfigDataMii::setMiiOrIconId(const void *pData, const u32 *a2) {
    if (pData) {
        MR::copyMemory(mBuffer, pData, 8);
        _4 |= 0x2;
        mIconID = 0;
    }
    else {
        MR::fillMemory(mBuffer, 0, 8);
        mIconID = *a2;
    }
}

bool ConfigDataMii::getIconId(u32 *pIconId) const {
    *pIconId = mIconID;
    return mIconID != 0;
}

bool ConfigDataMii::getMiiId(void *pData) const {
    MR::copyMemory(pData, mBuffer, 8);
    return !mIconID;
}

u32 ConfigDataMii::makeHeaderHashCode() const {
    return 0x2836E9;
}

u32 ConfigDataMii::getSignature() const {
    return 0x4D494920;
}

s32 ConfigDataMii::serialize(u8 *pData, u32 len) const {
    JSUMemoryOutputStream stream;
    stream.setBuffer(pData, len);
    u8 stack_9 = _4;
    stream.write(&stack_9, 1);
    stream.write(mBuffer, 8);
    u8 stack_8 = mIconID;
    stream.write(&stack_8, 1);
    return stream.mPosition;
}

s32 ConfigDataMii::deserialize(const u8 *pData, u32 len) {
    initializeData();
    JSUMemoryInputStream stream;
    stream.setBuffer(pData, len);
    u8 stack_8;
    stream.read(&stack_8, 1);
    _4 = stack_8;
    stream.read(mBuffer, 8);

    if (!stream.getAvailable()) {
        if (_4 & 0x1) {
            mIconID = 0;
        }
    }
    else {
        stream.read(&mIconID, 1);
    }

    return 0;
}

void ConfigDataMii::initializeData() {
    _4 = 0;
    mIconID = 1;
    MR::zeroMemory(mBuffer, 8);
}