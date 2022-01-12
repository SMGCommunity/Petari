#pragma once

#include "Game/MapObj/MapPartsFunction.h"
#include "Game/LiveActor/Nerve.h"

class MapPartsRotatorBase : public MapPartsFunction {
public:
    virtual ~MapPartsRotatorBase();

    virtual bool isMoving() const {
        return false;
    }

    virtual bool isOnReverse() const {
        return false;
    }

    virtual f32 getRotateSpeed() const {
        return 1.0f;
    }
};

class MapPartsRotator : public MapPartsRotatorBase {
public:
    MapPartsRotator(LiveActor *);

    enum AxisType {
        
    };

    virtual ~MapPartsRotator();
    virtual void init(const JMapInfoIter &);
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();
    virtual MtxPtr getRotateMtx() const;
    virtual bool isMoving() const;
    virtual bool isOnReverse() const;
    virtual f32 getRotateSpeed() const;

    void startWithSignalMotion();
    void cancelSignalMotion();
    void updateBaseHostMtx();
    void updateVelocity();
    void updateAngle();
    void updateTargetAngle();
    void restartAtEnd();
    void initRotateSpeed(const JMapInfoIter &);
    bool isReachedTargetAngle() const;
    void updateRotateMtx(AxisType, f32);
    void calcRotateAxisDir(AxisType, TVec3f *) const;
    
    void exeRotate();
    void exeRotateStart();

    f32 _18;
    f32 mRotateAngle;       // _1C
    s32 mRotateStopTime;    // _20
    f32 mTargetAngle;       // _24
    f32 mRotateSpeed;       // _28
    f32 mAngle;             // _2C
    s32 mRotateAxis;        // _30
    s32 mRotateAccelType;   // _34
    s32 mRotateType;        // _38
    s32 mSignMotionType;    // _3C
    TMtx34f _40;
    TMtx34f _70;
    f32 _A0;
    bool mIsOnReverse;  // _A4
};

namespace NrvMapPartsRotator {
    NERVE(HostTypeNeverMove);
    NERVE(HostTypeWait);
    NERVE(HostTypeRotateStart);
    NERVE(HostTypeRotate);
    NERVE(HostTypeStopAtEnd);
};