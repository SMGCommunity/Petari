#include "Game/System/BinaryDataContentAccessor.hpp"
#include "Game/Util/HashUtil.hpp"
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

BinaryDataContentHeaderSerializer::BinaryDataContentHeaderSerializer(u8* pData, u32 dataSize)
    : mStream(pData, dataSize), mAttributeNum(0), mDataSize(0) {
    u16 numAttributes = 0;
    mStream.write(&numAttributes, sizeof(u16));
    u16 streamDataSize = 0;
    mStream.write(&streamDataSize, sizeof(u16));
}

void BinaryDataContentHeaderSerializer::addAttribute(const char* pName, u32 attributeSize) {
    u16 hash = MR::getHashCode(pName);
    mStream.write(&hash, sizeof(u16));
    u16 offset = mDataSize;
    mStream.write(&offset, sizeof(u16));
    mDataSize += attributeSize;
    mAttributeNum++;
}

void BinaryDataContentHeaderSerializer::flush() {
    s32 restorePos = mStream.getPosition();
    mStream.seek(0, SEEK_FROM_START);
    u16 attributeNum = mAttributeNum;
    mStream.write(&attributeNum, sizeof(u16));
    u16 dataSize = mDataSize;
    mStream.write(&dataSize, sizeof(u16));
    mStream.seek(restorePos, SEEK_FROM_START);
}

u32 BinaryDataContentHeaderSerializer::getHeaderSize() const {
    return mStream.getPosition();
}

u32 BinaryDataContentHeaderSerializer::getDataSize() const {
    return mDataSize;
}

BinaryDataContentAccessor::BinaryDataContentAccessor(u8* pData) : mData(pData) {
}

s32 BinaryDataContentAccessor::getHeaderSize() const {
    return *reinterpret_cast< u16* >(mData + 0) * 4 + 4;
}

s32 BinaryDataContentAccessor::getDataSize() const {
    return *reinterpret_cast< u16* >(mData + 2);
}

// stripped
s32 BinaryDataContentAccessor::getAttributeNum() const {
    return *reinterpret_cast< u16* >(mData + 0);
}

void* BinaryDataContentAccessor::getPointer(const char* pAttributeName, u8* pData) const {
    JSUMemoryInputStream inStream = JSUMemoryInputStream(mData, getHeaderSize());
    inStream.skip(4);

    for (s32 idx = 0; idx < getAttributeNum(); idx++) {
        u16 hash = inStream.readU16();
        u16 position = inStream.readU16();
        if (hash == static_cast< u16 >(MR::getHashCode(pAttributeName))) {
            return pData + position;
        }
    }

    return nullptr;
}
