#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LightCylinder;
class MapPartsRailMover;
class MapPartsRotator;

class GravityLight : public LiveActor {
public:
    GravityLight(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&mBaseMtx;
    };

    void exeWait() {};

    /* 0x8C */ TMtx34f mBaseMtx;
    /* 0xBC */ LightCylinder* mLightCylinder;
    /* 0xC0 */ MapPartsRailMover* mMapPartsRailMover;
    /* 0xC4 */ MapPartsRotator* mMapPartsRotator;
    /* 0xC8 */ f32 mRadius;
    /* 0xCC */ TVec3f mCenter;
    /* 0xD8 */ bool _D8;
};
