#pragma once

#include "Game/Camera/Camera.hpp"
#include "revolution.h"

struct CanmFileHeader {
    u8 mMagic[4];       // _0
    u8 mType[4];        // _4
    s32 _8;
    s32 _C;
    s32 _10;
    s32 _14;
    u32 mNrFrames;      // _18
    u32 mValueOffset;   // _1C
};

struct CanmKeyFrameComponentInfo {
    u32 mCount;     // _0
    u32 mOffset;    // _4
    u32 mType;      // _8
};

struct CanmKeyFrameInfo {
    CanmKeyFrameComponentInfo mPosX;        // _0
    CanmKeyFrameComponentInfo mPosY;        // _C
    CanmKeyFrameComponentInfo mPosZ;        // _18
    CanmKeyFrameComponentInfo mWatchPosX;   // _24
    CanmKeyFrameComponentInfo mWatchPosY;   // _30
    CanmKeyFrameComponentInfo mWatchPosZ;   // _3C
    CanmKeyFrameComponentInfo mTwist;       // _48
    CanmKeyFrameComponentInfo mFovy;        // _54
};

struct CamnFrameComponentInfo {
    u32 mCount;     // _0
    u32 mOffset;    // _4
};

struct CanmFrameInfo {
    CamnFrameComponentInfo mPosX;       // _0
    CamnFrameComponentInfo mPosY;       // _8
    CamnFrameComponentInfo mPosZ;       // _10
    CamnFrameComponentInfo mWatchPosX;  // _18
    CamnFrameComponentInfo mWatchPosY;  // _20
    CamnFrameComponentInfo mWatchPosZ;  // _28
    CamnFrameComponentInfo mTwist;      // _30
    CamnFrameComponentInfo mFovy;       // _38
};

class BaseCamAnmDataAccessor {
public:
    inline BaseCamAnmDataAccessor() {
    
    }

    virtual inline ~BaseCamAnmDataAccessor() {
    
    }

    virtual void set(void *, void *) = 0;
    virtual void getPos(TVec3f *, float) const = 0;
    virtual void getWatchPos(TVec3f *, float) const = 0;
    virtual float getTwist(float) const = 0;
    virtual float getFovy(float) const = 0;
};

class KeyCamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline KeyCamAnmDataAccessor() {
        mInfo = NULL;
        mValues = NULL;
    }

    virtual ~KeyCamAnmDataAccessor();
    
    virtual void set(void *, void *);
    virtual void getPos(TVec3f *, float) const;
    virtual void getWatchPos(TVec3f *, float) const;
    virtual float getTwist(float) const;
    virtual float getFovy(float) const;

    float get(float, unsigned long, unsigned long, unsigned long) const;
    u32 searchKeyFrameIndex(float, unsigned long, unsigned long, unsigned long) const;
    float get3f(float, unsigned long, unsigned long) const;
    float get4f(float, unsigned long, unsigned long) const;
    float calcHermite(float, float, float, float, float, float, float) const;

    CanmKeyFrameInfo *mInfo;    // _4
    f32 *mValues;               // _8
};

class CamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline CamAnmDataAccessor() {
        mInfo = NULL;
        mValues = NULL;
    }

    virtual ~CamAnmDataAccessor();
    
    virtual void set(void *, void *);
    virtual void getPos(TVec3f *, float) const;
    virtual void getWatchPos(TVec3f *, float) const;
    virtual float getTwist(float) const;
    virtual float getFovy(float) const;

    float get(float, unsigned long, unsigned long) const;

    CanmFrameInfo *mInfo;   // _4
    f32 *mValues;           // _8
};

class CameraAnim : public Camera {
public:
    CameraAnim(const char *);
    virtual ~CameraAnim();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isZeroFrameMoveOff() const;
    virtual bool isCollisionOff() const;
    virtual bool isInterpolationOff() const;
    virtual CamTranslatorBase *createTranslator();

    void setParam(unsigned char *, float);
    bool isAnimEnd() const;
    static u32 getAnimFrame(unsigned char *);
    bool loadBin(unsigned char *);
    
    s32 _4C;
    s32 _50;
    u32 mNrFrames;                              // _54
    u8 mIsKey;                                  // _58
    u8 _59[3];
    f32 mSpeed;                                 // _5C
    f32 mCurrentFrame;                          // _60
    BaseCamAnmDataAccessor *mFileDataAccessor;  // _64
    CamAnmDataAccessor *mDataAccessor;          // _68
    KeyCamAnmDataAccessor *mKeyDataAccessor;    // _6C
    u32 mNrValues;
    s32 _74;
    u8 *mFileData;                              // _78
    u8 _7C;
    u8 _7D[3];
};