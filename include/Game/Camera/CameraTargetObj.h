#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry.h"

class CubeCameraArea;
class GravityInfo;
class LiveActor;
class MarioActor;

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char *);

    virtual inline ~CameraTargetObj() {

    }

    virtual void init(const JMapInfoIter &);

    virtual const TVec3f *getPosition() const = 0;
    virtual const TVec3f *getUpVec() const = 0;
    virtual const TVec3f *getFrontVec() const = 0;
    virtual const TVec3f *getSideVec() const = 0;
    virtual const TVec3f *getLastMove() const = 0;
    virtual const TVec3f *getGroundPos() const = 0;
    virtual const TVec3f *getGravityVector() const = 0;

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
    virtual CubeCameraArea *getCubeCameraArea() const;
    virtual void *getGroundTriangle() const; // TODO: return type
    virtual GravityInfo *getGravityInfo() const;
    virtual void enableCameraWall();
    virtual void disableCameraWall();
    virtual void setCameraWall(bool);
    virtual bool isDebugMode() const;
    virtual TMtx34f *getMapBaseMtx() const;
    virtual void resetStatus();

    bool mCameraWall;   // _C
    u8 _D[3];
};

class CameraTargetActor : public CameraTargetObj {
public:
    CameraTargetActor(const char *);
    virtual ~CameraTargetActor();

    virtual void movement();

    virtual const TVec3f *getPosition() const;
    virtual const TVec3f *getUpVec() const;
    virtual const TVec3f *getFrontVec() const;
    virtual const TVec3f *getSideVec() const;
    virtual const TVec3f *getLastMove() const;
    virtual const TVec3f *getGroundPos() const;
    virtual const TVec3f *getGravityVector() const;

    virtual f32 getRadius() const;
    virtual CubeCameraArea* getCubeCameraArea() const;
    virtual void *getGroundTriangle() const;

    const LiveActor *mActor;        // _10
    TVec3f mUp;                     // _14
    TVec3f mFront;                  // _20
    TVec3f mSide;                   // _2C
    CubeCameraArea *mCameraArea;    // _38
};

class CameraTargetPlayer : public CameraTargetObj {
public:
    CameraTargetPlayer(const char *);
    virtual ~CameraTargetPlayer();

    virtual const TVec3f *getPosition() const;
    virtual const TVec3f *getUpVec() const;
    virtual const TVec3f *getFrontVec() const;
    virtual const TVec3f *getSideVec() const;
    virtual const TVec3f *getLastMove() const;
    virtual const TVec3f *getGroundPos() const;
    virtual const TVec3f *getGravityVector() const;

    virtual bool isTurning() const;
    virtual bool isJumping() const;
    virtual bool isLongDrop() const;
    virtual bool isFastDrop() const;
    virtual bool isFastRise() const;
    virtual bool isWaterMode() const;
    virtual bool isOnWaterSurface() const;
    virtual bool isFooFighterMode() const;
    virtual u32 getSpecialMode() const;
    virtual bool isCameraStateOn(unsigned long) const;
    virtual CubeCameraArea *getCubeCameraArea() const;
    virtual void *getGroundTriangle() const;
    virtual GravityInfo *getGravityInfo() const;
    virtual bool isDebugMode() const;
    virtual TMtx34f *getMapBaseMtx() const;

    const MarioActor *mActor;       // _10
    TVec3f mSide;                   // _14
    TVec3f mUp;                     // _20
    TVec3f mFront;                  // _2C
    TVec3f mGravityVector;          // _38
    TVec3f mGroundPos;              // _44
    CubeCameraArea *mCameraArea;    // _50
    void *mGroundTriangle;          // _54
    u16 _58;
    bool _5A;
    u8 _5B;
};