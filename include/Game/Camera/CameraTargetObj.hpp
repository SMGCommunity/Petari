#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry.hpp"

class CubeCameraArea;
class GravityInfo;
class LiveActor;
class MarioActor;
class Triangle;

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char*);

    virtual inline ~CameraTargetObj() {}

    virtual void init(const JMapInfoIter&);

    virtual const TVec3f* getPosition() const = 0;
    virtual const TVec3f* getUpVec() const = 0;
    virtual const TVec3f* getFrontVec() const = 0;
    virtual const TVec3f* getSideVec() const = 0;
    virtual const TVec3f* getLastMove() const = 0;
    virtual const TVec3f* getGroundPos() const = 0;
    virtual const TVec3f* getGravityVector() const = 0;

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
    virtual bool isCameraStateOn(u32) const { return false; };
    virtual CubeCameraArea* getCubeCameraArea() const;
    virtual Triangle* getGroundTriangle() const;
    virtual GravityInfo* getGravityInfo() const;
    virtual void enableCameraWall();
    virtual void disableCameraWall();
    virtual void setCameraWall(bool);
    virtual bool isDebugMode() const;
    virtual TMtx34f* getMapBaseMtx() const;
    virtual void resetStatus();

    bool mCameraWall;  // 0xC
    u8 _D[3];
};

class CameraTargetActor : public CameraTargetObj {
public:
    CameraTargetActor(const char*);
    virtual ~CameraTargetActor();

    virtual void movement();

    virtual const TVec3f* getPosition() const;
    virtual const TVec3f* getUpVec() const;
    virtual const TVec3f* getFrontVec() const;
    virtual const TVec3f* getSideVec() const;
    virtual const TVec3f* getLastMove() const;
    virtual const TVec3f* getGroundPos() const;
    virtual const TVec3f* getGravityVector() const;

    virtual f32 getRadius() const;
    virtual CubeCameraArea* getCubeCameraArea() const;
    virtual Triangle* getGroundTriangle() const;

    const LiveActor* mActor;      // 0x10
    TVec3f mUp;                   // 0x14
    TVec3f mFront;                // 0x20
    TVec3f mSide;                 // 0x2C
    CubeCameraArea* mCameraArea;  // 0x38
};

class CameraTargetPlayer : public CameraTargetObj {
public:
    CameraTargetPlayer(const char*);
    virtual ~CameraTargetPlayer();

    virtual const TVec3f* getPosition() const;
    virtual const TVec3f* getUpVec() const;
    virtual const TVec3f* getFrontVec() const;
    virtual const TVec3f* getSideVec() const;
    virtual const TVec3f* getLastMove() const;
    virtual const TVec3f* getGroundPos() const;
    virtual const TVec3f* getGravityVector() const;

    virtual bool isTurning() const;
    virtual bool isJumping() const;
    virtual bool isLongDrop() const;
    virtual bool isFastDrop() const;
    virtual bool isFastRise() const;
    virtual bool isWaterMode() const;
    virtual bool isOnWaterSurface() const;
    virtual bool isFooFighterMode() const;
    virtual u32 getSpecialMode() const;
    virtual bool isCameraStateOn(u32) const;
    virtual CubeCameraArea* getCubeCameraArea() const;
    virtual Triangle* getGroundTriangle() const;
    virtual GravityInfo* getGravityInfo() const;
    virtual bool isDebugMode() const;
    virtual TMtx34f* getMapBaseMtx() const;

    const MarioActor* mActor;     // 0x10
    TVec3f mSide;                 // 0x14
    TVec3f mUp;                   // 0x20
    TVec3f mFront;                // 0x2C
    TVec3f mGravity;              // 0x38
    TVec3f mGroundPos;            // 0x44
    CubeCameraArea* mCameraArea;  // 0x50
    Triangle* mGroundTriangle;    // 0x54
    u16 _58;
    bool _5A;
    u8 _5B;
};
