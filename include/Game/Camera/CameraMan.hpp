#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraDirector;
class CameraPoseParam;

class CameraMan : public NameObj {
public:
    CameraMan(const char*);

    virtual void init(const JMapInfoIter&) {
    }
    virtual void movement();
    virtual void calc() {
    }
    virtual void notifyActivate() {
    }
    virtual void notifyDeactivate() {
    }
    virtual bool isInterpolationOff() const {
        return false;
    }
    virtual bool isCollisionOff() const {
        return false;
    }
    virtual bool isZeroFrameMoveOff() const {
        return false;
    }
    virtual bool isSubjectiveCameraOff() const {
        return false;
    }
    virtual bool isCorrectingErpPositionOff() const {
        return false;
    }
    virtual bool isEnableToReset() const {
        return false;
    }
    virtual bool isEnableToRoundLeft() const {
        return false;
    }
    virtual bool isEnableToRoundRight() const {
        return false;
    }
    virtual void roundLeft() {
    }
    virtual void roundRight() {
    }

    void owned(CameraDirector*);
    void released(CameraDirector*);
    void activate(CameraDirector*);
    void deactivate(CameraDirector*);

    /* 0x0C */ CameraDirector* mDirector;
    /* 0x10 */ CameraPoseParam* mPoseParam;
    /* 0x14 */ bool mIsActivated;
    /* 0x15 */ bool mRequestLOfsReset;
    /* 0x18 */ TPos3f mMatrix;
};
