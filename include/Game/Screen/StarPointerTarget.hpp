#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LayoutActor;

class StarPointerTarget {
public:
    StarPointerTarget(f32, const TVec3f*, MtxPtr, TVec3f);

    void calcScreenPosition(TVec2f*) const;
    bool isPointing(const TVec2f&, f32, f32) const;
    void calcPosition(TVec3f*) const;
    f32 calcRadius2d(f32, f32, f32) const;
    bool calcScreenPositionFromView(TVec2f*, const TVec3f&) const;

    /* 0x00 */ f32 mRadius3d;
    /* 0x04 */ TVec3f mOffset;
    /* 0x10 */ u32 _10;
    /* 0x14 */ const TVec3f* mPosition;
    /* 0x18 */ MtxPtr mMtx;
};

class StarPointerLayoutTarget {
public:
    enum TargetType { TargetType_Circle = 0, TargetType_Rectangle = 1 };
    StarPointerLayoutTarget(LayoutActor*, const char*, const char*);

    bool isPointing(const TVec2f&) const;
    bool isPointingCircle(const TVec2f&) const;
    bool isPointingRectangle(const TVec2f&) const;
    void calcBasePos(TVec2f*) const;

    /* 0x00 */ LayoutActor* mLayout;
    /* 0x04 */ const char* mLayoutName;
    /* 0x08 */ const char* mPaneName;
    /* 0x0C */ s32 mType;
    /* 0x10 */ TVec2f mPosition;
    /* 0x18 */ f32 mRadius;
    /* 0x1C */ f32 mWidth;
    /* 0x20 */ f32 mHeight;
};

class StarPointerLayoutTargetKeeper {
public:
    StarPointerLayoutTargetKeeper(int);

    void addTargetCircle(LayoutActor*, const char*, f32, const TVec2f&, const char*);
    StarPointerLayoutTarget* getTarget(const char*) const;
    void registerTarget(StarPointerLayoutTarget*);

    /* 0x00 */ s32 mNumTargets;
    /* 0x04 */ s32 mMaxNumTargets;
    /* 0x08 */ StarPointerLayoutTarget** mTargets;
};
