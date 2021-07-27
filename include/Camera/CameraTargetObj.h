#pragma once

#include "AreaObj/AreaObj.h"
#include "LiveActor/LiveActor.h"
#include "NameObj/NameObj.h"
#include "Gravity/GravityInfo.h"
#include "JSystem/JGeometry/TMatrix34.h"
#include "JSystem/JGeometry/TVec3.h"

#include <revolution/mtx.h>

class CameraTargetObj : public NameObj
{
public:
    CameraTargetObj(const char *);

    virtual ~CameraTargetObj();

    virtual void init(const JMapInfoIter &);
    virtual void movement() = 0;
    virtual JGeometry::TVec3f& getPosition() const = 0;
    virtual JGeometry::TVec3f& getUpVec() const = 0;
    virtual JGeometry::TVec3f& getFrontVec() const = 0;
    virtual JGeometry::TVec3f& getSideVec() const = 0;
    virtual JGeometry::TVec3f& getLastMove() const = 0;
    virtual JGeometry::TVec3f& getGroundPos() const = 0;
    virtual JGeometry::TVec3f& getGravityVector() const = 0;
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

    virtual JGeometry::TVec3f& getPosition() const;
    virtual JGeometry::TVec3f& getUpVec() const;
    virtual JGeometry::TVec3f& getFrontVec() const;
    virtual JGeometry::TVec3f& getSideVec() const;
    virtual JGeometry::TVec3f& getLastMove() const;
    virtual JGeometry::TVec3f& getGroundPos() const;
    virtual JGeometry::TVec3f& getGravityVector() const;

    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;

    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx; // _10
    JGeometry::TVec3f mPosition; // _40
    JGeometry::TVec3f mLastMove; // _4C
    JGeometry::TVec3f mGravity; // _58
    JGeometry::TVec3f mUpVec; // _64
    JGeometry::TVec3f mFrontVec; // _70
    JGeometry::TVec3f mSideVec; // _7C
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

    virtual JGeometry::TVec3f& getPosition() const;
    virtual JGeometry::TVec3f& getUpVec() const;
    virtual JGeometry::TVec3f& getFrontVec() const;
    virtual JGeometry::TVec3f& getSideVec() const;
    virtual JGeometry::TVec3f& getLastMove() const;
    virtual JGeometry::TVec3f& getGroundPos() const;
    virtual JGeometry::TVec3f& getGravityVector() const;

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
    JGeometry::TVec3f mSideVec; // _14
    JGeometry::TVec3f mUpVec; // _20
    JGeometry::TVec3f mFrontVec; // _2C
    JGeometry::TVec3f mGravityVec; // _38
    JGeometry::TVec3f mGroundPos; // _44
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

    virtual JGeometry::TVec3f& getPosition() const;
    virtual JGeometry::TVec3f& getUpVec() const;
    virtual JGeometry::TVec3f& getFrontVec() const;
    virtual JGeometry::TVec3f& getSideVec() const;
    virtual JGeometry::TVec3f& getLastMove() const;
    virtual JGeometry::TVec3f& getGroundPos() const;
    virtual JGeometry::TVec3f& getGravityVector() const;
    virtual f32 getRadius() const;

    virtual AreaObj* getCubeCameraArea() const;
    virtual u32* getGroundTriangle() const;

    const LiveActor* mActor; // _10
    JGeometry::TVec3f mUp; // _14
    JGeometry::TVec3f mFront; // _20
    JGeometry::TVec3f mSide; // _2C
    AreaObj* mCubeCameraArea; // _38
};