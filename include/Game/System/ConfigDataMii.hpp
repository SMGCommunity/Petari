#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

class ConfigDataMii : public BinaryDataChunkBase {
public:
    ConfigDataMii();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    void setMiiOrIconId(const void*, const u32*);
    bool getIconId(u32*) const;
    bool getMiiId(void*) const;

public:
    /* 0x04 */ u8 mFlag;
    /* 0x05 */ u8 mIconId;
    /* 0x08 */ void* mMiiId;
};
