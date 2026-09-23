#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SpringValue;

class SpringStep {
public:
    SpringStep(f32, f32, f32);

    void addSpringVelocity(f32);
    void setSpringBaseValue(f32);

    /* 0x00 */ f32 mCondition;
    /* 0x04 */ f32 mSpeed;
    /* 0x08 */ f32 mAngle;
    /* 0x0C */ SpringValue* mSpringValue;
};

class MapPartsFloatingForce : public MapPartsFunction {
public:
    MapPartsFloatingForce(LiveActor* pActor);

    virtual ~MapPartsFloatingForce();
    virtual void init(const JMapInfoIter& rIter);
    virtual void start();

    void setObjectName(const char* pName);
    void initForNormalMotion(const JMapInfoIter& rIter);
    void initForSpringMotion(const JMapInfoIter& rIter);
    void updateVelocity();
    void updateVelocitySpring();
    bool tryOn();
    bool tryReturn();
    void exeWait();
    void exeMove();
    void exeMoveSpring();
    void exeMoveReturn();

    /* 0x18 */ SpringStep* mSpringStep;
    /* 0x1C */ const char* mObjectName;
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 mRotateAngle;
    /* 0x28 */ f32 mRotateSpeed;
    /* 0x2C */ s32 mRotateAccelType;
    /* 0x30 */ TVec3f _30;
    /* 0x3C */ TVec3f mUpDirection;
    /* 0x48 */ f32 _48;
    /* 0x4C */ f32 _4C;
    /* 0x50 */ TVec3f _50;
    /* 0x5C */ f32 _5C;
};
