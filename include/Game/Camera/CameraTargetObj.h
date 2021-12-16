#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char *);
    virtual ~CameraTargetObj();

    virtual void init(const JMapInfoIter &);

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

    u8 mCameraWall;
};