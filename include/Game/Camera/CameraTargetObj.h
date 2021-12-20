#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TVec.h"

class LiveActor;

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char *);
    virtual ~CameraTargetObj();

    virtual void init(const JMapInfoIter &);

    virtual TVec3f *getPosition() const = 0;
    virtual TVec3f *getUpVec() const = 0;
    virtual TVec3f *getFrontVec() const = 0;
    virtual TVec3f *getSideVec() const = 0;
    virtual TVec3f *getLastMove() const = 0;
    virtual TVec3f *getGroundPos() const = 0;
    virtual TVec3f *getGravityVector() const = 0;

    virtual f32 getRadius() const;
    virtual bool isTurning() const;
    virtual bool isJumping() const;
    virtual bool isLongDrop() const;
    virtual bool isFastDrop() const;
    virtual bool isFastRise() const;
    virtual bool isWaterMode() const;
    virtual bool isOnWaterSurface() const;
    virtual bool isBeeMode() const;
    virtual bool isFooFighterMode() const;
    virtual u32 getSpecialMode() const;
    virtual bool isCameraStateOn(unsigned long) const;
    //virtual X getCubeCameraArea() const;
    //virtual X getGroundTriangle() const;
    //virtual X getGravityInfo() const;
    virtual void enableCameraWall();
    virtual void disableCameraWall();
    virtual void setCameraWall(bool);
    virtual bool isDebugMode() const;
    virtual TMtx34f *getMapBaseMtx() const;
    virtual void resetStatus();

    u8 mCameraWall; // _C
    u8 _D[3];
};

class CameraTargetActor : public CameraTargetObj {
public:
    CameraTargetActor(const char *);
    virtual ~CameraTargetActor();

    virtual TVec3f *getPosition() const;
    virtual TVec3f *getUpVec() const;
    virtual TVec3f *getFrontVec() const;
    virtual TVec3f *getSideVec() const;
    virtual TVec3f *getLastMove() const;
    virtual TVec3f *getGroundPos() const;
    virtual TVec3f *getGravityVector() const;

    const LiveActor *_10;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    f32 _24;
    f32 _28;
    f32 _2C;
    f32 _30;
    f32 _34;
    u32 _38;
};

class CameraTargetPlayer : public CameraTargetObj {
public:
    CameraTargetPlayer(const char *);

    virtual TVec3f *getPosition() const;
    virtual TVec3f *getUpVec() const;
    virtual TVec3f *getFrontVec() const;
    virtual TVec3f *getSideVec() const;
    virtual TVec3f *getLastMove() const;
    virtual TVec3f *getGroundPos() const;
    virtual TVec3f *getGravityVector() const;

    void *_10; // const MarioActor *
    u8 _14[0x24];
    f32 _38;
    f32 _3C;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4C;
    u32 _50;
    u32 _54;
    u16 _58;
    u8 _5A;
    u8 _5B;
};