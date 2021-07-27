#pragma once

#include "Camera/CameraHolder.h"
#include "Camera/CameraParamChunkID.h"
#include "Camera/CameraParamString.h"
#include "Camera/DotCamParams.h"
#include "JSystem/JGeometry/TVec3.h"

#include "smg.h"

class CameraGeneralParam
{
public:
    CameraGeneralParam();

    CameraGeneralParam& operator=(const CameraGeneralParam &);

    f32 mDist; // _0
    JGeometry::TVec3f mAxis; // _4
    JGeometry::TVec3f mWPoint; // _10
    JGeometry::TVec3f mUp; // _1C
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

        JGeometry::TVec3f mWOffset; // _0
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
        JGeometry::TVec3f mPanAxis; // _48
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
    void getVPanAxis(JGeometry::TVec3f *) const;
    void setCameraType(const char *, const CameraHolder *);
    void setUseFovy(bool);
    void setLOFsErpOff(bool);
    void setCollisionOff(bool);
    void arrangeCamTypeName(u32, const char **);

    CameraParamChunkID* mChunk; // _4
    s8 mDefaultCamera; // _8
    CameraParamChunk::ExParam mParams; // _C
    CameraGeneralParam* mGeneralParams; // _60
    u8 _64;
};

class CameraParamChunkGame : public CameraParamChunk
{
public:
    CameraParamChunkGame(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char* getClassName() const;

    s8 mGameFlagThru; // _65
    s8 mEnabledEndErpFrame; // _66
    s8 _67;
    s32 mCamEndInt; // _68
};

class CameraParamChunkEvent : public CameraParamChunk
{
public:
    CameraParamChunkEvent(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char* getClassName() const;

    s8 mEnableErpFrame; // _65
    u8 _66;
    u8 _67;
    s32 mEVFrm; // _68
    s8 mEnableEndErpFrame; // _6C
    u8 _6D;
    u8 _6E;
    u8 _6F;
    s32 mCamEndInt; // _70
    s32 mEventPriority; // _74
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