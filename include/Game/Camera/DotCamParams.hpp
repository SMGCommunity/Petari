#pragma once

#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution.h"

class DotCamReader {
public:
    DotCamReader() {
    }

    /* 0x08 */ virtual ~DotCamReader();

    virtual u32 getVersion() const = 0;
    /* 0x0C */ virtual bool hasMoreChunk() const = 0;
    /* 0x10 */ virtual void nextToChunk() = 0;
    /* 0x14 */ virtual bool getValueInt(const char*, s32*) = 0;
    /* 0x18 */ virtual bool getValueFloat(const char*, f32*) = 0;
    /* 0x1C */ virtual bool getValueVec(const char*, TVec3f*) = 0;
    /* 0x20 */ virtual bool getValueString(const char*, const char**) = 0;
};

class DotCamReaderInBin : public DotCamReader {
public:
    DotCamReaderInBin(const void*);
    /* 0x08 */ virtual ~DotCamReaderInBin();

    virtual u32 getVersion() const;
    /* 0x0C */ virtual bool hasMoreChunk() const;
    /* 0x10 */ virtual void nextToChunk();
    /* 0x14 */ virtual bool getValueInt(const char*, s32*);
    /* 0x18 */ virtual bool getValueFloat(const char*, f32*);
    /* 0x1C */ virtual bool getValueVec(const char*, TVec3f*);
    /* 0x20 */ virtual bool getValueString(const char*, const char**);

    void init(const void*);

    u32 mVersion;  // 0x4
    u8* _8;
    JMapInfo mMapInfo;      // 0xC
    JMapInfoIter mMapIter;  // 0x14
};