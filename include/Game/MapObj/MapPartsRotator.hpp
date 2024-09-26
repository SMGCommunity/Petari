#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsRotatorBase : public MapPartsFunction {
public:
    virtual ~MapPartsRotatorBase();

    virtual TMtx34f& getRotateMtx() const = 0;

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
    virtual TMtx34f& getRotateMtx() const;
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
    f32 mRotateAngle;       // 0x1C
    s32 mRotateStopTime;    // 0x20
    f32 mTargetAngle;       // 0x24
    f32 mRotateSpeed;       // 0x28
    f32 mAngle;             // 0x2C
    s32 mRotateAxis;        // 0x30
    s32 mRotateAccelType;   // 0x34
    s32 mRotateType;        // 0x38
    s32 mSignMotionType;    // 0x3C
    TMtx34f _40;
    TMtx34f _70;
    f32 _A0;
    bool mIsOnReverse;  // 0xA4
};

namespace NrvMapPartsRotator {
    NERVE(HostTypeNeverMove);
    NERVE(HostTypeWait);
    NERVE(HostTypeRotateStart);
    NERVE(HostTypeRotate);
    NERVE(HostTypeStopAtEnd);
};