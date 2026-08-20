#pragma once

#include "Game/Camera/CameraParamString.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CameraHolder;
class CameraParamChunkID;
class DotCamReader;

class CameraGeneralParam {
public:
    CameraGeneralParam()
        : mString(), mDist(1200.0f), mAxis(0.0f, 1.0f, 0.0f), mWPoint(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mAngleA(), mAngleB(0.3f), mNum1(),
          mNum2() {
    }

    CameraGeneralParam& operator=(const CameraGeneralParam&);

    /* 0x00 */ f32 mDist;
    /* 0x04 */ TVec3f mAxis;
    /* 0x10 */ TVec3f mWPoint;
    /* 0x1C */ TVec3f mUp;
    /* 0x28 */ f32 mAngleA;
    /* 0x2C */ f32 mAngleB;
    /* 0x30 */ s32 mNum1;
    /* 0x34 */ s32 mNum2;
    /* 0x38 */ CameraParamString mString;
};

class CameraParamChunk {
public:
    class ExParam {
    public:
        ExParam() {
        }

        void init() {
            mWOffset.set< f32 >(0.0f, 100.0f, 0.0f);
            mLOffset = 0.0f;
            mLOffsetV = 0.0f;
            mRoll = 0.0f;
            mFovy = 45.0f;
            mCamInt = 120;
            mFlag = 0;
            mUpper = 0.3f;
            mLower = 0.1f;
            mGndInt = 160;
            mUPlay = 300.0f;
            mLPlay = 800.0f;
            mPushDelay = 120;
            mPushDelayLow = 120;
            mUDown = 120;
            mVPanUse = true;
            mVPanAxis.set(0.0f, 1.0f, 0.0f);
        }

        void setWOffset(const TVec3f& rWOffset) {
            mWOffset.set(rWOffset);
        }

        /* 0x00 */ TVec3f mWOffset;
        /* 0x0C */ f32 mLOffset;
        /* 0x10 */ f32 mLOffsetV;
        /* 0x14 */ f32 mRoll;
        /* 0x18 */ f32 mFovy;
        /* 0x1C */ s32 mCamInt;
        /* 0x20 */ u16 mFlag;
        /* 0x24 */ f32 mUpper;
        /* 0x28 */ f32 mLower;
        /* 0x2C */ s32 mGndInt;
        /* 0x30 */ f32 mUPlay;
        /* 0x34 */ f32 mLPlay;
        /* 0x38 */ s32 mPushDelay;
        /* 0x3C */ s32 mPushDelayLow;
        /* 0x40 */ s32 mUDown;
        /* 0x44 */ s32 mVPanUse;
        /* 0x48 */ TVec3f mVPanAxis;
    };

    CameraParamChunk(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const {
        return "Base";
    }

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

    CameraGeneralParam* getGeneralParam() {
        return mGeneralParam;
    }

    u8 getCameraTypeIndex() const {
        return mCameraTypeIndex;
    }

    /* 0x04 */ CameraParamChunkID* mParamChunkID;
    /* 0x08 */ u8 mCameraTypeIndex;
    /* 0x0C */ ExParam mExParam;
    /* 0x60 */ CameraGeneralParam* mGeneralParam;
    /* 0x64 */ bool _64;
};

class CameraParamChunkGame : public CameraParamChunk {
public:
    CameraParamChunkGame(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const {
        return "Game";
    }

    /* 0x65 */ u8 mThru;
    /* 0x66 */ u8 mEnableEndErpFrame;
    /* 0x68 */ u32 mCamEndInt;
};

class CameraParamChunkEvent : public CameraParamChunk {
public:
    CameraParamChunkEvent(CameraHolder*, const CameraParamChunkID&);

    virtual void copy(const CameraParamChunk*);
    virtual void load(DotCamReader*, CameraHolder*);
    virtual void initiate();
    virtual const char* getClassName() const {
        return "Event";
    }

    /* 0x65 */ u8 mEnableErpFrame;
    /* 0x68 */ u32 mEvFrame;
    /* 0x6C */ u8 mEnableEndErpFrame;
    /* 0x70 */ u32 mCamEndInt;
    /* 0x74 */ u32 mEvPriority;
};
