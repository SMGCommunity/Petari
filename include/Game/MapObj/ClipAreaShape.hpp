#pragma once

#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JGeometry.hpp>

class ClipAreaShape {
public:
    ClipAreaShape(const char*);

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    bool isInArea(const TVec3f&, f32, const TPos3f&, const TVec3f&) const;
    void drawVolumeShape(const TPos3f&, const TVec3f&) const;

    J3DModelData* mModelData;  // 0x4
};

class ClipAreaShapeCone : public ClipAreaShape {
public:
    ClipAreaShapeCone(s32);

    virtual bool isInArea(const TVec3f&) const;

    f32 _8;
    f32 _C;
    s32 _10;
};

class ClipAreaShapeSphere : public ClipAreaShape {
public:
    ClipAreaShapeSphere();

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    f32 mRadius;  // 0x8
};

class ClipAreaShapeBox : public ClipAreaShape {
public:
    ClipAreaShapeBox(s32);

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    f32 mRadius;  // 0x8
    s32 _C;
};