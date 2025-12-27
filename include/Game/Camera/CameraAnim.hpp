#pragma once

#include "Game/Camera/Camera.hpp"
#include "revolution.h"

struct CanmFileHeader {
    u8 mMagic[4];  // 0x0
    u8 mType[4];   // 0x4
    s32 _8;
    s32 _C;
    s32 _10;
    s32 _14;
    u32 mNrFrames;     // 0x18
    u32 mValueOffset;  // 0x1C
};

struct CanmKeyFrameComponentInfo {
    u32 mCount;   // 0x0
    u32 mOffset;  // 0x4
    u32 mType;    // 0x8
};

struct CanmKeyFrameInfo {
    CanmKeyFrameComponentInfo mPosX;       // 0x0
    CanmKeyFrameComponentInfo mPosY;       // 0xC
    CanmKeyFrameComponentInfo mPosZ;       // 0x18
    CanmKeyFrameComponentInfo mWatchPosX;  // 0x24
    CanmKeyFrameComponentInfo mWatchPosY;  // 0x30
    CanmKeyFrameComponentInfo mWatchPosZ;  // 0x3C
    CanmKeyFrameComponentInfo mTwist;      // 0x48
    CanmKeyFrameComponentInfo mFovy;       // 0x54
};

struct CamnFrameComponentInfo {
    u32 mCount;   // 0x0
    u32 mOffset;  // 0x4
};

struct CanmFrameInfo {
    CamnFrameComponentInfo mPosX;       // 0x0
    CamnFrameComponentInfo mPosY;       // 0x8
    CamnFrameComponentInfo mPosZ;       // 0x10
    CamnFrameComponentInfo mWatchPosX;  // 0x18
    CamnFrameComponentInfo mWatchPosY;  // 0x20
    CamnFrameComponentInfo mWatchPosZ;  // 0x28
    CamnFrameComponentInfo mTwist;      // 0x30
    CamnFrameComponentInfo mFovy;       // 0x38
};

class BaseCamAnmDataAccessor {
public:
    inline BaseCamAnmDataAccessor() {}

    virtual inline ~BaseCamAnmDataAccessor() {}

    virtual void set(void*, void*) = 0;
    virtual void getPos(TVec3f*, f32) const = 0;
    virtual void getWatchPos(TVec3f*, f32) const = 0;
    virtual f32 getTwist(f32) const = 0;
    virtual f32 getFovy(f32) const = 0;
};

class KeyCamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline KeyCamAnmDataAccessor() {
        mInfo = nullptr;
        mValues = nullptr;
    }

    virtual ~KeyCamAnmDataAccessor();

    virtual void set(void*, void*);
    virtual void getPos(TVec3f*, f32) const;
    virtual void getWatchPos(TVec3f*, f32) const;
    virtual f32 getTwist(f32) const;
    virtual f32 getFovy(f32) const;

    f32 get(f32, u32, u32, u32) const;
    u32 searchKeyFrameIndex(f32, u32, u32, u32) const;
    f32 get3f(f32, u32, u32) const;
    f32 get4f(f32, u32, u32) const;
    f32 calcHermite(f32, f32, f32, f32, f32, f32, f32) const;

    CanmKeyFrameInfo* mInfo;  // 0x4
    f32* mValues;             // 0x8
};

class CamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline CamAnmDataAccessor() {
        mInfo = nullptr;
        mValues = nullptr;
    }

    virtual ~CamAnmDataAccessor();

    virtual void set(void*, void*);
    virtual void getPos(TVec3f*, f32) const;
    virtual void getWatchPos(TVec3f*, f32) const;
    virtual f32 getTwist(f32) const;
    virtual f32 getFovy(f32) const;

    f32 get(f32, u32, u32) const;

    CanmFrameInfo* mInfo;  // 0x4
    f32* mValues;          // 0x8
};

class CameraAnim : public Camera {
public:
    CameraAnim(const char* pName = "アニメーションカメラ");
    virtual ~CameraAnim();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isZeroFrameMoveOff() const;
    virtual bool isCollisionOff() const;
    virtual bool isInterpolationOff() const;
    virtual CamTranslatorBase* createTranslator();

    void setParam(u8*, f32);
    bool isAnimEnd() const;
    static u32 getAnimFrame(u8*);
    bool loadBin(u8*);

    s32 _4C;
    s32 _50;
    u32 mNrFrames;  // 0x54
    u8 mIsKey;      // 0x58
    u8 _59[3];
    f32 mSpeed;                                 // 0x5C
    f32 mCurrentFrame;                          // 0x60
    BaseCamAnmDataAccessor* mFileDataAccessor;  // 0x64
    CamAnmDataAccessor* mDataAccessor;          // 0x68
    KeyCamAnmDataAccessor* mKeyDataAccessor;    // 0x6C
    u32 mNrValues;
    s32 _74;
    u8* mFileData;  // 0x78
    u8 _7C;
    u8 _7D[3];
};