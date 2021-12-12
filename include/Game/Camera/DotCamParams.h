#pragma once

#include "JSystem/JGeometry/TVec.h"
#include "revolution.h"
#include "Game/Util/JMapInfo.h"

class DotCamReader {
public:
    inline DotCamReader();
    virtual ~DotCamReader();

    virtual u32 getVersion() const = 0;
    virtual bool hasMoreChunk() const = 0;
    virtual void nextToChunk() = 0;
    virtual bool getValueInt(const char *, long *) = 0;
    virtual bool getValueFloat(const char *, float *) = 0;
    virtual bool getValueVec(const char *, TVec3f *) = 0;
    virtual bool getValueString(const char *, const char **) = 0;
};

class DotCamReaderInBin : public DotCamReader {
public:
    DotCamReaderInBin(const void *);
    virtual ~DotCamReaderInBin();

    virtual u32 getVersion() const;
    virtual bool hasMoreChunk() const;
    virtual void nextToChunk();
    virtual bool getValueInt(const char *, long *);
    virtual bool getValueFloat(const char *, float *);
    virtual bool getValueVec(const char *, TVec3f *);
    virtual bool getValueString(const char *, const char **);

    void init(const void *);

    u32 mVersion;           // _4
    u8 *_8;
    JMapInfo mMapInfo;      // _C
    JMapInfoIter mMapIter;  // _14
};