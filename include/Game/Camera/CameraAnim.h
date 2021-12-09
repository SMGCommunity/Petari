#include "Game/Camera/Camera.h"
#include "revolution.h"

class BaseCamAnmDataAccessor {
public:
    inline BaseCamAnmDataAccessor();
    virtual inline ~BaseCamAnmDataAccessor();

    virtual void setParam(u8 *, u8 *) = 0;
    virtual void getPos(const TVec3f *, const float) = 0;
    virtual void getWatchPos(const TVec3f *, const float) = 0;
    virtual float getTwist(const float) = 0;
    virtual float getFovy(const float) = 0;
};

class KeyCamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline KeyCamAnmDataAccessor();
    virtual ~KeyCamAnmDataAccessor();
    
    virtual void setParam(u8 *, u8 *);
    virtual void getPos(const TVec3f *, const float);
    virtual void getWatchPos(const TVec3f *, const float);
    virtual float getTwist(const float);
    virtual float getFovy(const float);

    u8 *_04;
    u8 *_08;
};

class CamAnmDataAccessor : public BaseCamAnmDataAccessor {
public:
    inline CamAnmDataAccessor();
    virtual ~CamAnmDataAccessor();
    
    virtual void setParam(u8 *, u8 *);
    virtual void getPos(const TVec3f *, const float);
    virtual void getWatchPos(const TVec3f *, const float);
    virtual float getTwist(const float);
    virtual float getFovy(const float);

    u8 *_04;
    u8 *_08;
};

struct CanmFileHeader {
    u8 mMagic[4]; // _00
    u8 mType[4];  // _04
    s32 _08;
    s32 _0C;
    s32 _10;
    s32 _14;
    s32 _18;
    s32 _1C;
};

class CameraAnim : public Camera {
public:
    CameraAnim(const char *);
    virtual ~CameraAnim();

    bool loadBin(unsigned char *);
    
    s32 _4C;
    s32 _50;
    s32 _54;
    u8 mIsKey;                                  // _58
    u8 _59[3];
    f32 _5C;
    f32 _60;
    BaseCamAnmDataAccessor *mFileDataAccessor;  // _64
    CamAnmDataAccessor *mDataAccessor;          // _68
    KeyCamAnmDataAccessor *mKeyDataAccessor;    // _6C
    u32 _70;
    s32 _74;
    u8* mFileData;                              // _78
    u8 _7C;
    u8 _7D[3];
};