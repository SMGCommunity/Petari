#pragma once

#include "Camera/CameraHolder.h"
#include "Camera/CameraParamChunkID.h"
#include "Camera/CameraParamString.h"
#include "Camera/DotCamParams.h"
#include "JGeometry/TVec3.h"

class CameraGeneralParam
{
public:
    CameraGeneralParam();

    CameraGeneralParam* operator=(const CameraGeneralParam &);

    f32 _0;
    JGeometry::TVec3<f32> _4;
    JGeometry::TVec3<f32> _10;
    JGeometry::TVec3<f32> _1C;
    f32 _28;
    f32 _2C;
    u32 _30;
    u32 _34;
    CameraParamString mStringParam; // _38
};

class CameraParamChunk
{
public:
    class ExParam
    {
    public:
        ExParam();

        void init();

        JGeometry::TVec3<f32> mWOffset; // _0
        f32 mLOffset; // _C
        f32 mLOffsetV; // _10
        f32 mRoll; // _14
        f32 mFovy; // _18
        s32 mCamInt; // _1C
        u16 mFlags; // _20
        f32 mUpper; // _24
        f32 mLower; // _28
        u32 mGNDInt; // _2C
        f32 mUPlay; // _30
        f32 mLPlay; // _34
        u32 mPushDelay; // _38
        u32 mPushDelayLow; // _3C
        u32 mUDown; // _40
        u32 mVPanUse; // _44
        JGeometry::TVec3<f32> mPanAxis; // _48
    };

    CameraParamChunk(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char* getClassName() const;

    s32 getZoneID() const;
    bool isOnNoReset() const;
    bool isOnUseFovy() const;
    bool isLOfsErpOff() const;
    bool isAntiBlurOff() const;
    bool isCollisionOff() const;
    bool isSubjectiveCameraOff() const;
    void getVPanAxis(JGeometry::TVec3<f32> *) const;

    void setUseFovy(bool);
    void setLOFsErpOff(bool);
    void setCollisionOff(bool);

    CameraParamChunkID* mChunk; // _4
    s8 mDefaultCamera; // _8
    CameraParamChunk::ExParam mParams; // _C
    CameraGeneralParam* mGeneralParams; // _60
    u8 _64;
};

namespace
{
    static CameraParamChunk::ExParam sUndoExParam;
    static CameraGeneralParam sUndoParam;

    const char* sFlagName[0x6] =
    {
        "flag.noreset",
        "flag.nofovy",
        "flag.lofserpoff",
        "flag.antibluroff",
        "flag.collisionoff",
        "flag.subjectiveoff"
    };
};