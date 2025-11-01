#pragma once

#include "Game/System/BinaryDataContentAccessor.hpp"
#include "Game/System/BinaryDataChunkHolder.hpp"
#include <revolution/os.h>

class SysConfigChunk : public BinaryDataChunkBase {
public:
    SysConfigChunk();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual void* serialize();
    virtual s32 deserialize(const u8 *, u32);
    virtual void initializeData();
    
    void initHeaderSerializer();

    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    BinaryDataContentHeaderSerializer* mSerializer; // 0x1C
};

class SysConfigFile {
public:
    /// @brief Creates a new `SysConfigFile`.
    SysConfigFile();

    OSTime getTimeAnnounced();
    void updateTimeAnnounced();
    OSTime getTimeSent();
    void setTimeSent(OSTime);
    u32 getSentBytes();
    void setSentBytes(u32);
    void makeDataBinary(u8*, u32) const;
    void loadFromDataBinary(const u8*, u32);

private:
    /* 0x00 */ SysConfigChunk* mChunk;
    /* 0x04 */ BinaryDataChunkHolder* mChunkHolder;
};
