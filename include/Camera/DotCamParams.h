#pragma once

#include "JGeometry/TVec3.h"
#include "JMap/JMapInfoIter.h"

class DotCamReader
{
public:
    virtual ~DotCamReader();
    virtual s32 getVersion() const = 0;
    virtual bool hasMoreChunk() const = 0;
    virtual void nextToChunk() = 0;
    virtual bool getValueInt(const char *, s32 *) = 0;
    virtual bool getValueFloat(const char *, f32 *) = 0;
    virtual void getValueVec(const char *, JGeometry::TVec3<f32> *) = 0;
    virtual s32 getValueString(const char *, const char **) = 0;
};

class DotCamReaderInBin : public DotCamReader
{
public:
    DotCamReaderInBin(const void *);

    virtual s32 getVersion() const;
    virtual bool hasMoreChunk() const;
    virtual void nextToChunk();
    virtual bool getValueInt(const char *, s32 *);
    virtual bool getValueFloat(const char *, f32 *);
    virtual void getValueVec(const char *, JGeometry::TVec3<f32> *);
    virtual s32 getValueString(const char *, const char **);

    void init(const void *);

    s32 mVersion; // _4
    u32 _8;
    JMapInfo mInfo; // _C
    JMapInfoIter mIter; // _14
};