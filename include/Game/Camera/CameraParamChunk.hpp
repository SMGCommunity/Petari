#pragma once

#include "Game/Camera/CameraParamString.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraHolder;
class CameraParamChunkID;
class DotCamReader;

class CameraGeneralParam {
public:
    CameraGeneralParam();

    CameraGeneralParam& operator=(const CameraGeneralParam&);

    f32 mDist;                  // 0x0
    TVec3f mAxis;               // 0x4
    TVec3f mWPoint;             // 0x10
    TVec3f mUp;                 // 0x1C
    f32 mAngleA;                // 0x28
    f32 mAngleB;                // 0x2C
    s32 mNum1;                  // 0x30
    s32 mNum2;                  // 0x34
    CameraParamString mString;  // 0x38
};

class CameraParamChunk {
public:
    class ExParam {
    public:
        ExParam();

        void init();

        TVec3f mWOffset;    // 0x0
        f32 mLOffset;       // 0xC
        f32 mLOffsetV;      // 0x10
        f32 mRoll;          // 0x14
        f32 mFovy;          // 0x18
        s32 mCamInt;        // 0x1C
        u16 mFlag;          // 0x20
        u8 _22[2];          // 0x22
        f32 mUpper;         // 0x24
        f32 mLower;         // 0x28
        s32 mGndInt;        // 0x2C
        f32 mUPlay;         // 0x30
        f32 mLPlay;         // 0x34
        s32 mPushDelay;     // 0x38
        s32 mPushDelayLow;  // 0x3C
        s32 mUDown;         // 0x40
        s32 mVPanUse;       // 0x44
        TVec3f mVPanAxis;   // 0x48
    };

    CameraParamChunk(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const;

    s32 getZoneID() const;
    bool isOnNoReset() const;
    bool isOnUseFovy() const;
    bool isLOfsErpOff() const;
    bool isAntiBlurOff() const;
    bool isCollisionOff() const;
    bool isSubjectiveCameraOff() const;
    void getVPanAxis(TVec3f*) const;

    void setCameraType(const char*, const CameraHolder*);
    void setUseFovy(bool);
    void setLOfsErpOff(bool);
    void setCollisionOff(bool);

    void arrangeCamTypeName(u32, const char**);

    CameraParamChunkID* mParamChunkID;  // 0x4
    u8 mCameraTypeIndex;                // 0x8
    u8 _9[3];
    ExParam mExParam;                   // 0xC
    CameraGeneralParam* mGeneralParam;  // 0x60
    bool _64;
};

class CameraParamChunkGame : public CameraParamChunk {
public:
    CameraParamChunkGame(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const;

    u8 mThru;               // 0x65
    u8 mEnableEndErpFrame;  // 0x66
    u8 _67;
    u32 mCamEndInt;  // 0x68
};

class CameraParamChunkEvent : public CameraParamChunk {
public:
    CameraParamChunkEvent(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const;

    u8 mEnableErpFrame;  // 0x65
    u8 _66[2];
    u32 mEvFrame;           // 0x68
    u8 mEnableEndErpFrame;  // 0x6C
    u8 _6D[3];
    u32 mCamEndInt;   // 0x70
    u32 mEvPriority;  // 0x74
};