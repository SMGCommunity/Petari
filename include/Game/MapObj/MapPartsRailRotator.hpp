#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailRotator : public MapPartsFunction {
public:
    MapPartsRailRotator(LiveActor*);

    virtual ~MapPartsRailRotator();
    virtual void init(const JMapInfoIter&);
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();
    virtual f32 getJMapArgAngleFactor() const;

    bool hasRotation(s32) const;
    void rotateAtPoint(s32);
    bool hasRotationBetweenPoints(s32) const;
    void rotateBetweenPoints(s32, f32);
    void updateHostRotateMtx();
    void updateInfo(s32);
    bool isReachedTargetAngle() const;

    void initWithRotateMtx(const JMapInfoIter&, MtxPtr);

    void exeRotate();

    u32 _18;
    u32 _1C;
    f32 _20;
    f32 _24;
    f32 _28;
    TMtx34f _2C;
    TMtx34f _5C;
    u32 _8C;
};