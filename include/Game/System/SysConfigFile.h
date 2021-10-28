#pragma once

#include <revolution.h>
#include "Game/System/BinaryDataContentAccessor.h"

class SysConfigChunk {
public:
    SysConfigChunk();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual void serialize();
    virtual u32 deserialize(const u8 *, u32);
    virtual void initializeData();
    
    void initHeaderSerializer();

    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    BinaryDataContentHeaderSerializer* mSerializer; // _1C
};