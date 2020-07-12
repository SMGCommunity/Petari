#pragma once

#include "Actor/NameObj/NameObj.h"
#include "Camera/CameraDirector.h"
#include "JGeometry/TMatrix34.h"

class CameraPoseParam;

class CameraMan : public NameObj
{
public:
    CameraMan(const char *);

    virtual ~CameraMan();
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void calc();
    virtual void notifyActivate();
    virtual void notifyDeactivate();
    virtual bool isInterpolationOff() const;
    virtual bool isCollisionOff() const;
    virtual bool isZeroFrameMoveOff() const;
    virtual bool isSubjectiveCameraOff() const;
    virtual bool isCorrectingErpPositionOff() const;
    virtual bool isEnableToReset() const;
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual void roundLeft();
    virtual void roundRight();

    void owned(CameraDirector *);
    void released(CameraDirector *);
    void activate(CameraDirector *);
    void deactivate(CameraDirector *);

    CameraDirector* mDirector; // _C
    CameraPoseParam* mParams; // _10
    bool mIsActive; // _14
    u8 _15;
    u16 _16;
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx; // _18
};