#pragma once

#include "Game/Camera/CameraParamString.h"
#include "JSystem/JGeometry/TVec.h"

class CameraHolder;
class CameraParamChunkID;
class DotCamReader;

class CameraGeneralParam {
public:
    CameraGeneralParam();

    CameraGeneralParam &operator=(const CameraGeneralParam &);

    f32 mDist;                  // _0
    TVec3f mAxis;               // _4
    TVec3f mWPoint;             // _10
    TVec3f mUp;                 // _1C
    f32 mAngleA;                // _28
    f32 mAngleB;                // _2C
    s32 mNum1;                  // _30
    s32 mNum2;                  // _34
    CameraParamString mString;  // _38
};

class CameraParamChunk {
public:
    class ExParam {
    public:
        ExParam();

        void init();

        TVec3f mWOffset;    // _0
        f32 mLOffset;       // _C
        f32 mLOffsetV;      // _10
        f32 mRoll;          // _14
        f32 mFovy;          // _18
        s32 mCamInt;        // _1C
        u16 mFlag;          // _20
        u8 _22[2];          // _22
        f32 mUpper;         // _24
        f32 mLower;         // _28
        s32 mGndInt;        // _2C
        f32 mUPlay;         // _30
        f32 mLPlay;         // _34
        s32 mPushDelay;     // _38
        s32 mPushDelayLow;  // _3C
        s32 mUDown;         // _40
        s32 mVPanUse;       // _44
        TVec3f mVPanAxis;   // _48
    };

    CameraParamChunk(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char *getClassName() const;

    s32 getZoneID() const;
    bool isOnNoReset() const;
    bool isOnUseFovy() const;
    bool isLOfsErpOff() const;
    bool isAntiBlurOff() const;
    bool isCollisionOff() const;
    bool isSubjectiveCameraOff() const;
    void getVPanAxis(TVec3f *) const;

    void setCameraType(const char *, const CameraHolder *);
    void setUseFovy(bool);
    void setLOfsErpOff(bool);
    void setCollisionOff(bool);

    void arrangeCamTypeName(unsigned long, const char **);

    CameraParamChunkID *mParamChunkID;  // _4
    u8 mCameraTypeIndex;                // _8
    u8 _9[3];
    ExParam mExParam;                   // _C
    CameraGeneralParam *mGeneralParam;  // _60
    u8 _64;
};

class CameraParamChunkGame : public CameraParamChunk {
public:
    CameraParamChunkGame(CameraHolder *, const CameraParamChunkID &);
    
    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char *getClassName() const;

    u8 mThru;               // _65
    u8 mEnableEndErpFrame;  // _66
    u8 _67;
    u32 mCamEndInt;         // _68
};

class CameraParamChunkEvent : public CameraParamChunk {
public:
    CameraParamChunkEvent(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    virtual void load(DotCamReader *, CameraHolder *);
    virtual void initiate();
    virtual const char *getClassName() const;

    u8 mEnableErpFrame;     // _65
    u8 _66[2];
    u32 mEvFrame;           // _68
    u8 mEnableEndErpFrame;  // _6C
    u8 _6D[3];
    u32 mCamEndInt;         // _70
    u32 mEvPriority;        // _74
};