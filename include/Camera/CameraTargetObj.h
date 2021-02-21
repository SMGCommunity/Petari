#pragma once

#include "AreaObj/AreaObj.h"
#include "LiveActor/LiveActor.h"
#include "NameObj/NameObj.h"
#include "Gravity/GravityInfo.h"
#include "JGeometry/TMatrix34.h"
#include "JGeometry/TVec3.h"

#include <revolution/mtx.h>

class CameraTargetObj : public NameObj
{
public:
    CameraTargetObj(const char *);

    virtual ~CameraTargetObj();

    virtual void init(const JMapInfoIter &);
    virtual void movement() = 0;
    virtual JGeometry::TVec3<f32>& getPosition() const = 0;
    virtual JGeometry::TVec3<f32>& getUpVec() const = 0;
    virtual JGeometry::TVec3<f32>& getFrontVec() const = 0;
    virtual JGeometry::TVec3<f32>& getSideVec() const = 0;
    virtual JGeometry::TVec3<f32>& getLastMove() const = 0;
    virtual JGeometry::TVec3<f32>& getGroundPos() const = 0;
    virtual JGeometry::TVec3<f32>& getGravityVector() const = 0;
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
    virtual bool isCameraStateOn(u32) const;
    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;
    virtual GravityInfo* getGravityInfo() const;
    virtual void enableCameraWall();
    virtual void disableCameraWall();
    virtual void setCameraWall(bool);
    virtual bool isDebugMode() const;
    virtual Mtx* getMapBaseMtx() const;
    virtual void resetStatus();

    bool mIsCameraWall; // _C
};

class CameraTargetMtx : public CameraTargetObj
{
public:
    CameraTargetMtx(const char *);

    virtual ~CameraTargetMtx();

    virtual void movement();

    virtual JGeometry::TVec3<f32>& getPosition() const;
    virtual JGeometry::TVec3<f32>& getUpVec() const;
    virtual JGeometry::TVec3<f32>& getFrontVec() const;
    virtual JGeometry::TVec3<f32>& getSideVec() const;
    virtual JGeometry::TVec3<f32>& getLastMove() const;
    virtual JGeometry::TVec3<f32>& getGroundPos() const;
    virtual JGeometry::TVec3<f32>& getGravityVector() const;

    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;

    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx; // _10
    JGeometry::TVec3<f32> mPosition; // _40
    JGeometry::TVec3<f32> mLastMove; // _4C
    JGeometry::TVec3<f32> mGravity; // _58
    JGeometry::TVec3<f32> mUpVec; // _64
    JGeometry::TVec3<f32> mFrontVec; // _70
    JGeometry::TVec3<f32> mSideVec; // _7C
    bool mIsLastMoveValid; // _88
    u8 _89;
    u16 _8A;
    AreaObj* mCubeCameraArea; // _8C
};

class CameraTargetPlayer : public CameraTargetObj
{
public:
    CameraTargetPlayer(const char *);

    virtual ~CameraTargetPlayer();

    virtual void movement();

    virtual JGeometry::TVec3<f32>& getPosition() const;
    virtual JGeometry::TVec3<f32>& getUpVec() const;
    virtual JGeometry::TVec3<f32>& getFrontVec() const;
    virtual JGeometry::TVec3<f32>& getSideVec() const;
    virtual JGeometry::TVec3<f32>& getLastMove() const;
    virtual JGeometry::TVec3<f32>& getGroundPos() const;
    virtual JGeometry::TVec3<f32>& getGravityVector() const;

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
    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;
    virtual GravityInfo* getGravityInfo() const;
    
    virtual bool isDebugMode() const;
    virtual Mtx* getMapBaseMtx() const;

    LiveActor* mPlayerActor; // _10
    JGeometry::TVec3<f32> mSideVec; // _14
    JGeometry::TVec3<f32> mUpVec; // _20
    JGeometry::TVec3<f32> mFrontVec; // _2C
    JGeometry::TVec3<f32> mGravityVec; // _38
    JGeometry::TVec3<f32> mGroundPos; // _44
    u32 _50;
    u32 _54;
    u16 _58;
    u8 _5A;
};

class CameraTargetActor : public CameraTargetObj
{
public:
    CameraTargetActor(const char *);

    virtual ~CameraTargetActor();

    virtual void movement();

    virtual JGeometry::TVec3<f32>& getPosition() const;
    virtual JGeometry::TVec3<f32>& getUpVec() const;
    virtual JGeometry::TVec3<f32>& getFrontVec() const;
    virtual JGeometry::TVec3<f32>& getSideVec() const;
    virtual JGeometry::TVec3<f32>& getLastMove() const;
    virtual JGeometry::TVec3<f32>& getGroundPos() const;
    virtual JGeometry::TVec3<f32>& getGravityVector() const;
    virtual f32 getRadius() const;

    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;

    const LiveActor* mActor; // _10
    JGeometry::TVec3<f32> mUp; // _14
    JGeometry::TVec3<f32> mFront; // _20
    JGeometry::TVec3<f32> mSide; // _2C
    AreaObj* mCubeCameraArea; // _38
};