#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/Util/SpringValue.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SpringStep {
public:
    SpringStep(f32, f32, f32);

    void addSpringVelocity(f32);
    void setSpringBaseValue(f32);

    f32 mCondition;             // 0x0
    f32 mSpeed;                 // 0x4
    f32 mAngle;                 // 0x8
    SpringValue* mSpringValue;  // 0xC
};

class MapPartsFloatingForce : public MapPartsFunction {
public:
    MapPartsFloatingForce(LiveActor*);

    virtual ~MapPartsFloatingForce();
    virtual void init(const JMapInfoIter&);
    virtual void start();

    void setObjectName(const char*);
    void initForNormalMotion(const JMapInfoIter&);
    void initForSpringMotion(const JMapInfoIter&);
    void updateVelocity();
    void updateVelocitySpring();
    bool tryOn();
    bool tryReturn();
    void exeWait();
    void exeMove();
    void exeMoveSpring();
    void exeMoveReturn();

    SpringStep* mSpringStep;  // 0x18
    const char* mObjectName;  // 0x1C
    f32 _20;
    f32 mRotateAngle;      // 0x24
    f32 mRotateSpeed;      // 0x28
    s32 mRotateAccelType;  // 0x2C
    TVec3f _30;
    f32 _3C;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4C;
    TVec3f _50;
    f32 _5C;
};
