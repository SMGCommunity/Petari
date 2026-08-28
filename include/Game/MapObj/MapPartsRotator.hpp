#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsRotatorBase : public MapPartsFunction {
public:
    inline MapPartsRotatorBase(LiveActor* pActor, const char* pName) : MapPartsFunction(pActor, pName) {
    }

    /* 0x08 */ virtual ~MapPartsRotatorBase();

    /* 0x38 */ virtual const TMtx34f& getRotateMtx() const = 0;

    /* 0x3C */ virtual bool isMoving() const {
        return false;
    }

    /* 0x40 */ virtual bool isOnReverse() const {
        return false;
    }

    virtual f32 getRotateSpeed() const {
        return 1.0f;
    }
};

class MapPartsRotator : public MapPartsRotatorBase {
public:
    MapPartsRotator(LiveActor*);

    enum AxisType {

    };

    /* 0x08 */ virtual ~MapPartsRotator();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual bool isWorking() const;
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end();
    /* 0x38 */ virtual const TMtx34f& getRotateMtx() const {
        return _70;
    }
    /* 0x3C */ virtual bool isMoving() const;
    /* 0x40 */ virtual bool isOnReverse() const;
    virtual f32 getRotateSpeed() const;

    void startWithSignalMotion();
    void cancelSignalMotion();
    void updateBaseHostMtx();
    void updateVelocity();
    void updateAngle();
    void updateTargetAngle();
    void restartAtEnd();
    void initRotateSpeed(const JMapInfoIter&);
    bool isReachedTargetAngle() const;
    void updateRotateMtx(AxisType, f32);
    void calcRotateAxisDir(AxisType, TVec3f*) const;

    void exeNeverMove();
    void exeWait();
    void exeRotateStart();
    void exeRotate();
    void exeStopAtEnd();

    f32 _18;
    f32 mRotateAngle;      // 0x1C
    s32 mRotateStopTime;   // 0x20
    f32 mTargetAngle;      // 0x24
    f32 mRotateSpeed;      // 0x28
    f32 mAngle;            // 0x2C
    s32 mRotateAxis;       // 0x30
    s32 mRotateAccelType;  // 0x34
    s32 mRotateType;       // 0x38
    s32 mSignMotionType;   // 0x3C
    TMtx34f _40;
    TMtx34f _70;
    f32 _A0;
    bool mIsOnReverse;  // 0xA4
};
