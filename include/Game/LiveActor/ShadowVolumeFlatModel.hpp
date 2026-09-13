#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class J3DModelData;

class ShadowVolumeFlatModel : public ShadowVolumeModel {
public:
    ShadowVolumeFlatModel(const char*);
    ShadowVolumeFlatModel();

    inline virtual ~ShadowVolumeFlatModel();
    virtual void draw() const;
    virtual void loadModelDrawMtx() const;

    void initModel(const char*);
    void setBaseMatrixPtr(MtxPtr);
    void initJointDrawMtxResister();
    void calcRootJoint(TPos3f*, const TVec3f&) const;
    void calcDropJoint(TPos3f*, const TVec3f&) const;

    /* 0x20 */ MtxPtr mBaseMtx;
    /* 0x24 */ s32 mRootDrawMtxIndex;
    /* 0x28 */ s32 mDropDrawMtxIndex;
};
