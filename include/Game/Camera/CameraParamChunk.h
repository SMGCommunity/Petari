#pragma once

#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraParamChunkID.h"
#include "Game/Camera/CameraParamString.h"
#include "JSystem/JGeometry/TVec.h"

class CameraGeneralParam {
public:
    CameraGeneralParam();

    CameraGeneralParam &operator=(const CameraGeneralParam &);

    f32 _0;
    TVec3f _4;
    TVec3f _10;
    TVec3f _1C;
    f32 _28;
    f32 _2C;
    u32 _30;
    u32 _34;
    CameraParamString mParamString; // _38
};

class CameraParamChunk {
public:
    class ExParam {
    public:
        ExParam();

        void init();

        TVec3f _0;
        f32 _C;
        f32 _10;
        f32 _14;
        f32 _18;
        u32 _1C;
        u16 mFlag;
        u8 _22[2];
        f32 _24;
        f32 _28;
        u32 _2C;
        f32 _30;
        f32 _34;
        u32 _38;
        u32 _3C;
        u32 _40;
        u32 _44;
        TVec3f mVPanAxis;
    };

    CameraParamChunk(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    //virtual void load(DotCamReader *, CameraHolder *); // TODO
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
    //virtual void load(DotCamReader *, CameraHolder *); // TODO
    virtual void initiate();
    virtual const char *getClassName() const;

    u8 _65;
    u8 _66;
    u8 _67;
    u32 _68;
};

class CameraParamChunkEvent : public CameraParamChunk {
public:
    CameraParamChunkEvent(CameraHolder *, const CameraParamChunkID &);

    virtual void copy(const CameraParamChunk *);
    //virtual void load(DotCamReader *, CameraHolder *); // TODO
    virtual void initiate();
    virtual const char *getClassName() const;

    u8 _65;
    u8 _66[2];
    u32 _68;
    u8 _6C;
    u8 _6D[3];
    u32 _70;
    u32 _74;
};