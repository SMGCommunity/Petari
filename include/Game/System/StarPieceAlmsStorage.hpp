#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

// NOTE/TODO: This dummy is here to ensure vtable alignment in GameDataAllGalaxyStorage.
// This double-inheritance with BinaryDataChunkBase behavior is repeated for a few
// different classes. What actually is the class that takes this role?
class Dummy2 {
public:
    Dummy2() {};
};

class StarPieceAlmsStorage : public Dummy2, public BinaryDataChunkBase {
public:
    StarPieceAlmsStorage();

    virtual ~StarPieceAlmsStorage();
    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual void initializeData();
    virtual s32 deserialize(const u8*, u32);

    s32 getValue(int) const;
    s32 getMaxValue(int) const;
    void addValue(int, int);

private:
    /* 0x8 */ u16* mValueArray;
};
