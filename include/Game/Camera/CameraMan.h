#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraDirector;
class CameraPoseParam;

class CameraMan : public NameObj {
public:
    CameraMan(const char *);
    
    virtual inline ~CameraMan() {

    }

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
    
    CameraDirector *mDirector;      // _C
    CameraPoseParam *mPoseParam;    // _10
    bool mIsActivated;              // _14
    u8 _15;
    u8 _16[2];
    TMtx34f mMatrix;                // _18
};