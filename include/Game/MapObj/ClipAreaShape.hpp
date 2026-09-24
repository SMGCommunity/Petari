#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class J3DModelData;

class ClipAreaShape {
public:
    ClipAreaShape(const char*);

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    bool isInArea(const TVec3f&, f32, const TPos3f&, const TVec3f&) const;
    void drawVolumeShape(const TPos3f&, const TVec3f&) const;

    /* 0x04 */ J3DModelData* mModelData;
};

class ClipAreaShapeSphere : public ClipAreaShape {
public:
    ClipAreaShapeSphere();

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    /* 0x08 */ f32 mRadius;
};

class ClipAreaShapeBox : public ClipAreaShape {
public:
    ClipAreaShapeBox(s32);

    virtual bool isInArea(const TVec3f&) const;
    virtual void calcVolumeMatrix(TPos3f*, const TPos3f&, const TVec3f&) const;

    /* 0x08 */ f32 mRadius;
    /* 0x0C */ s32 _C;
};

class ClipAreaShapeCone : public ClipAreaShape {
public:
    ClipAreaShapeCone(s32);

    virtual bool isInArea(const TVec3f&) const;

    /* 0x08 */ f32 _8;
    /* 0x0C */ f32 _C;
    /* 0x10 */ s32 _10;
};
