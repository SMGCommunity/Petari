#pragma once

#include "Game/Camera/Camera.hpp"

struct CanmFileHeader {
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u8 mType[4];
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ u32 mNrFrames;
    /* 0x1C */ u32 mValueOffset;
};

struct CanmKeyFrameComponentInfo {
    /* 0x0 */ u32 mCount;
    /* 0x4 */ u32 mOffset;
    /* 0x8 */ u32 mType;
};

struct CanmKeyFrameInfo {
    /* 0x00 */ CanmKeyFrameComponentInfo mPosX;
    /* 0x0C */ CanmKeyFrameComponentInfo mPosY;
    /* 0x18 */ CanmKeyFrameComponentInfo mPosZ;
    /* 0x24 */ CanmKeyFrameComponentInfo mWatchPosX;
    /* 0x30 */ CanmKeyFrameComponentInfo mWatchPosY;
    /* 0x3C */ CanmKeyFrameComponentInfo mWatchPosZ;
    /* 0x48 */ CanmKeyFrameComponentInfo mTwist;
    /* 0x54 */ CanmKeyFrameComponentInfo mFovy;
};

struct CamnFrameComponentInfo {
    /* 0x0 */ u32 mCount;
    /* 0x4 */ u32 mOffset;
};

struct CanmFrameInfo {
    /* 0x00 */ CamnFrameComponentInfo mPosX;
    /* 0x08 */ CamnFrameComponentInfo mPosY;
    /* 0x10 */ CamnFrameComponentInfo mPosZ;
    /* 0x18 */ CamnFrameComponentInfo mWatchPosX;
    /* 0x20 */ CamnFrameComponentInfo mWatchPosY;
    /* 0x28 */ CamnFrameComponentInfo mWatchPosZ;
    /* 0x30 */ CamnFrameComponentInfo mTwist;
    /* 0x38 */ CamnFrameComponentInfo mFovy;
};

class BaseCamAnmDataAccessor;
class CamAnmDataAccessor;
class KeyCamAnmDataAccessor;

class CameraAnim : public Camera {
public:
    CameraAnim(const char* pName = "アニメーションカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isInterpolationOff() const {
        return true;
    }
    virtual bool isCollisionOff() const {
        return true;
    }
    virtual bool isZeroFrameMoveOff() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void setParam(u8*, f32);
    bool isAnimEnd() const;
    static u32 getAnimFrame(u8*);
    bool loadBin(u8*);

    /* 0x4C */ s32 _4C;
    /* 0x50 */ s32 _50;
    /* 0x54 */ u32 mNrFrames;
    /* 0x58 */ bool mIsKey;
    /* 0x5C */ f32 mSpeed;
    /* 0x60 */ f32 mCurrentFrame;
    /* 0x64 */ BaseCamAnmDataAccessor* mFileDataAccessor;
    /* 0x68 */ CamAnmDataAccessor* mDataAccessor;
    /* 0x6C */ KeyCamAnmDataAccessor* mKeyDataAccessor;
    /* 0x70 */ u32 mNrValues;
    /* 0x74 */ s32 _74;
    /* 0x78 */ u8* mFileData;
    /* 0x7C */ bool mIsPaused;
};

class BaseCamAnmDataAccessor {
public:
    inline BaseCamAnmDataAccessor() {
    }

    virtual inline ~BaseCamAnmDataAccessor() {
    }

    virtual void set(void*, void*) = 0;
    virtual void getPos(TVec3f*, f32) const = 0;
    virtual void getWatchPos(TVec3f*, f32) const = 0;
    virtual f32 getTwist(f32) const = 0;
    virtual f32 getFovy(f32) const = 0;
};

class CamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline CamAnmDataAccessor() {
        mInfo = nullptr;
        mValues = nullptr;
    }

    virtual void set(void*, void*);
    virtual void getPos(TVec3f*, f32) const;
    virtual void getWatchPos(TVec3f*, f32) const;
    virtual f32 getTwist(f32) const;
    virtual f32 getFovy(f32) const;

    f32 get(f32, u32, u32) const;

    f32 getValue(int index) const {
        return mValues[index];
    }

    /* 0x4 */ CanmFrameInfo* mInfo;
    /* 0x8 */ f32* mValues;
};

class KeyCamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline KeyCamAnmDataAccessor() {
        mInfo = nullptr;
        mValues = nullptr;
    }

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

    /* 0x4 */ CanmKeyFrameInfo* mInfo;
    /* 0x8 */ f32* mValues;
};
