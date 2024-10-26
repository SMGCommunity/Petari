#include "Game/System/SysConfigFile.hpp"

SysConfigChunk::SysConfigChunk() {
    initHeaderSerializer();
    initializeData();
}

u32 SysConfigChunk::makeHeaderHashCode() const {
    return 1;
}

u32 SysConfigChunk::getSignature() const {
    return 0x53595343;
}

// SysConfigChunk::deserialize

void SysConfigChunk::initializeData() {
    _C = 0;
    _8 = 0;
    _14 = 0;
    _10 = 0;
    _18 = 0;
}

void SysConfigChunk::initHeaderSerializer() {
    mSerializer = new BinaryDataContentHeaderSerializer(new u8[0x20], 0x20);

    mSerializer->addAttribute("mTimeAnnounced", 8);
    mSerializer->addAttribute("mTimeSent", 8);
    mSerializer->addAttribute("mSentBytes", 4);
    mSerializer->flush();
}