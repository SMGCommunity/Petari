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

    f32 mDist; // _0
    JGeometry::TVec3<f32> mAxis; // _4
    JGeometry::TVec3<f32> mWPoint; // _10
    JGeometry::TVec3<f32> mUp; // _1C
    f32 mAngleA; // _28
    f32 mAngleB; // _2C
    s32 mNum1; // _30
    s32 mNum2; // _34
    CameraParamString mString; // _38
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
        s32 mGNDInt; // _2C
        f32 mUPlay; // _30
        f32 mLPlay; // _34
        s32 mPushDelay; // _38
        s32 mPushDelayLow; // _3C
        s32 mUDown; // _40
        s32 mVPanUse; // _44
        JGeometry::TVec3<f32> mPanAxis; // _48
    };

    CameraParamChunk(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char* getClassName() const;

    void arrangeCamTypeName(u32, const char **);
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