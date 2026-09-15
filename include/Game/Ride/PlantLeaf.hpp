#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class J3DMaterial;
class J3DShape;
class J3DShapeDraw;

class PlantLeaf : public LiveActor {
public:
    PlantLeaf(f32, const TVec3f&, const TVec3f&, f32);

    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&mBaseMtx;
    }

    void updateGrowUp(const TVec3f&, const TVec3f&, f32, f32);
    bool updateSpring(const TVec3f&, f32, f32);
    bool updateSpring(f32);

    MtxPtr getPosMtx() const {
        return (MtxPtr)&mPosMtx;
    }

    /* 0x8C */ f32 mSpringCoord;
    /* 0x90 */ f32 mSpringSpeed;
    /* 0x94 */ f32 mLeafCoord;
    /* 0x98 */ f32 mLeafSize;
    /* 0x9C */ TVec3f mSide;
    /* 0xA8 */ TVec3f mUp;
    /* 0xB4 */ TVec3f mFront;
    /* 0xC0 */ TPos3f mBaseMtx;
    /* 0xF0 */ TPos3f mPosMtx;
};

class PlantLeafDrawInit : public LiveActor {
public:
    PlantLeafDrawInit(const char*);

    virtual void init(const JMapInfoIter&);

    void initDraw() const;

    /* 0x8C */ J3DMaterial* mMaterial;
    /* 0x90 */ J3DShape* mShape;
    /* 0x94 */ J3DShapeDraw* mShapeDraw;
};
