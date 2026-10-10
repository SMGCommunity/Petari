#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class DotCamReader {
public:
    DotCamReader() {
    }

    virtual ~DotCamReader();

    virtual u32 getVersion() const = 0;
    virtual bool hasMoreChunk() const = 0;
    virtual void nextToChunk() = 0;
    virtual bool getValueInt(const char*, s32*) = 0;
    virtual bool getValueFloat(const char*, f32*) = 0;
    virtual bool getValueVec(const char*, TVec3f*) = 0;
    virtual bool getValueString(const char*, const char**) = 0;
};

class DotCamReaderInBin : public DotCamReader {
public:
    DotCamReaderInBin(const void*);
    virtual ~DotCamReaderInBin();

    virtual u32 getVersion() const;
    virtual bool hasMoreChunk() const;
    virtual void nextToChunk();
    virtual bool getValueInt(const char*, s32*);
    virtual bool getValueFloat(const char*, f32*);
    virtual bool getValueVec(const char*, TVec3f*);
    virtual bool getValueString(const char*, const char**);

    void init(const void*);

    /* 0x04 */ u32 mVersion;
    /* 0x08 */ u8* _8;
    /* 0x0C */ JMapInfo mMapInfo;
    /* 0x14 */ JMapInfoIter mMapIter;
    /* 0x1C */ u8 _1C[0x10];
};
