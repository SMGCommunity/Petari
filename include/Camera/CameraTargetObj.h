#pragma once

#include "AreaObj/AreaObj.h"
#include "NameObj/NameObj.h"
#include "Gravity/GravityInfo.h"
#include "JGeometry/TVec3.h"

#include <revolution/mtx.h>

class CameraTargetObj : public NameObj
{
public:
    CameraTargetObj(const char *);

    virtual ~CameraTargetObj();

    virtual void init(const JMapInfoIter &);
    virtual void movement() = 0;
    virtual JGeometry::TVec3<f32> getPosition() const = 0;
    virtual JGeometry::TVec3<f32> getUpVec() const = 0;
    virtual JGeometry::TVec3<f32> getFrontVec() const = 0;
    virtual JGeometry::TVec3<f32> getSideVec() const = 0;
    virtual JGeometry::TVec3<f32> getLastMove() const = 0;
    virtual JGeometry::TVec3<f32> getGroundPos() const = 0;
    virtual JGeometry::TVec3<f32> getGravityVector() const = 0;
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