#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraDirector;
class CameraPoseParam;

class CameraMan : public NameObj {
public:
    CameraMan(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&) {
    }
    /* 0x14 */ virtual void movement();
    /* 0x24 */ virtual void calc() {
    }
    /* 0x28 */ virtual void notifyActivate() {
    }
    /* 0x2C */ virtual void notifyDeactivate() {
    }
    /* 0x30 */ virtual bool isInterpolationOff() const {
        return false;
    }
    /* 0x34 */ virtual bool isCollisionOff() const {
        return false;
    }
    /* 0x38 */ virtual bool isZeroFrameMoveOff() const {
        return false;
    }
    /* 0x3C */ virtual bool isSubjectiveCameraOff() const {
        return false;
    }
    /* 0x40 */ virtual bool isCorrectingErpPositionOff() const {
        return false;
    }
    /* 0x44 */ virtual bool isEnableToReset() const {
        return false;
    }
    /* 0x48 */ virtual bool isEnableToRoundLeft() const {
        return false;
    }
    /* 0x4C */ virtual bool isEnableToRoundRight() const {
        return false;
    }
    /* 0x50 */ virtual void roundLeft() {
    }
    /* 0x54 */ virtual void roundRight() {
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
