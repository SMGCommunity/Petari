#pragma once

#include "Game/MapObj/MapPartsFunction.h"
#include "Game/Util/SpringValue.h"

class SpringStep {
public:
    SpringStep(f32, f32, f32);

    void addSpringVelocity(f32);
    void setSpringBaseValue(f32);

    f32 mCondition;             // _0
    f32 mSpeed;                 // _4
    f32 mAngle;                 // _8
    SpringValue* mSpringValue;  // _C
};

class MapPartsFloatingForce : public MapPartsFunction {
public:
    MapPartsFloatingForce(LiveActor *);

    virtual ~MapPartsFloatingForce();
    virtual void init(const JMapInfoIter &);
    virtual void start();

    void setObjectName(const char *);
    void initForNormalMotion(const JMapInfoIter &);
    void initForSpringMotion(const JMapInfoIter &);
    void updateVelocity();
    void updateVelocitySpring();
    bool tryOn();
    bool tryReturn();
    void exeWait();
    void exeMove();
    void exeMoveSpring();
    void exeMoveReturn();

    SpringStep* mSpringStep;    // _18
    const char* mObjectName;    // _1C
    f32 _20;
    f32 mRotateAngle;           // _24
    f32 mRotateSpeed;           // _28
    s32 mRotateAccelType;       // _2C
    TVec3f _30;
    f32 _3C;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4C;
    TVec3f _50;
    f32 _5C;
};

namespace NrvMapPartsFloatingForce {
    NERVE(HostTypeWait);
    NERVE(HostTypeMove);
    NERVE(HostTypeMoveSpring);
    NERVE(HostTypeMoveReturn);
};