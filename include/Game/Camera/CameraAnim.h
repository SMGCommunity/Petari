#include "Game/Camera/Camera.h"
#include "revolution.h"

struct CanmFileHeader {
    u8 mMagic[4];
    u8 mType[4];
    s32 _8;
    s32 _C;
    s32 _10;
    s32 _14;
    u32 mNrFrames;
    u32 mValueOffset;
};

struct CanmKeyFrameComponentInfo {
    u32 mCount;
    u32 mOffset;
    u32 mType;
};

struct CanmKeyFrameInfo {
    CanmKeyFrameComponentInfo mPosX;
    CanmKeyFrameComponentInfo mPosY;
    CanmKeyFrameComponentInfo mPosZ;
    CanmKeyFrameComponentInfo mWatchPosX;
    CanmKeyFrameComponentInfo mWatchPosY;
    CanmKeyFrameComponentInfo mWatchPosZ;
    CanmKeyFrameComponentInfo mTwist;
    CanmKeyFrameComponentInfo mFovy;
};

struct CamnFrameComponentInfo {
    u32 mCount;
    u32 mOffset;
};

struct CanmFrameInfo {
    CamnFrameComponentInfo mPosX;
    CamnFrameComponentInfo mPosY;
    CamnFrameComponentInfo mPosZ;
    CamnFrameComponentInfo mWatchPosX;
    CamnFrameComponentInfo mWatchPosY;
    CamnFrameComponentInfo mWatchPosZ;
    CamnFrameComponentInfo mTwist;
    CamnFrameComponentInfo mFovy;
};

class BaseCamAnmDataAccessor {
public:
    inline BaseCamAnmDataAccessor();
    virtual inline ~BaseCamAnmDataAccessor();

    virtual void setParam(u32 *, f32 *) = 0;
    virtual void getPos(TVec3f *, float) const = 0;
    virtual void getWatchPos(TVec3f *, float) const = 0;
    virtual float getTwist(float) const = 0;
    virtual float getFovy(float) const = 0;
};

class KeyCamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline KeyCamAnmDataAccessor();
    virtual ~KeyCamAnmDataAccessor();
    
    virtual void setParam(u32 *, f32 *);
    virtual void getPos(TVec3f *, float) const;
    virtual void getWatchPos(TVec3f *, float) const;
    virtual float getTwist(float) const;
    virtual float getFovy(float) const;

    float get(float, unsigned long, unsigned long, unsigned long) const;
    u32 searchKeyFrameIndex(float, unsigned long, unsigned long, unsigned long) const;
    float get3f(float, unsigned long, unsigned long) const;
    float get4f(float, unsigned long, unsigned long) const;
    float calcHermite(float, float, float, float, float, float, float) const;

    CanmKeyFrameInfo *mInfo;
    f32 *mValues;
};

class CamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline CamAnmDataAccessor();
    virtual ~CamAnmDataAccessor();
    
    virtual void setParam(u32 *, f32 *);
    virtual void getPos(TVec3f *, float) const;
    virtual void getWatchPos(TVec3f *, float) const;
    virtual float getTwist(float) const;
    virtual float getFovy(float) const;

    float get(float, unsigned long, unsigned long) const;

    CanmFrameInfo *mInfo;
    f32 *mValues;
};

class CameraAnim : public Camera {
public:
    CameraAnim(const char *);
    virtual ~CameraAnim();

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