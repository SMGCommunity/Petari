#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

class GameEventFlag;

namespace MR {
    class BitArray;
};  // namespace MR

class GameEventFlagStorage : public BinaryDataChunkBase {
public:
    GameEventFlagStorage();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    void set(const GameEventFlag*, bool);
    bool isOn(const GameEventFlag*) const;

    /* 0x04 */ MR::BitArray* mFlagBitArray;
};
