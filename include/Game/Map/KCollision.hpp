#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class JMapInfo;

struct KC_PrismData {
    /* 0x00 */ f32 mHeight;
    /* 0x04 */ u16 mPositionIndex;
    /* 0x06 */ u16 mNormalIndex;
    /* 0x08 */ u16 mEdgeIndices[3];
    /* 0x0E */ u16 mAttribute;
};

class Fxyz {
public:
    inline Fxyz() {
    }

    Fxyz& operator=(const Fxyz&);

    f32 x, y, z;
};

struct KCLFile {
    union {
        /* 0x00 */ TVec3f* mPos;
        /* 0x00 */ u32 mPosOffset;
    };
    union {
        /* 0x04 */ TVec3f* mNorms;
        /* 0x04 */ u32 mNormOffset;
    };
    union {
        /* 0x08 */ KC_PrismData* mPrisms;
        /* 0x08 */ u32 mPrismOffset;
    };
    union {
        /* 0x0C */ void* mOctree;
        /* 0x0C */ u32 mOctreeOffset;
    };
    /* 0x10 */ f32 mThickness;
    /* 0x14 */ TVec3f mMin;
    /* 0x20 */ s32 mXMask;
    /* 0x24 */ s32 mYMask;
    /* 0x28 */ s32 mZMask;
    /* 0x2C */ s32 mBlockWidthShift;
    /* 0x30 */ s32 mBlockXShift;
    /* 0x34 */ s32 mBlockXYShift;
};

class KCollisionServer {
public:
    class V3u {
    public:
        inline V3u() {
        }

        void setUsingCast(const TVec3f&);

        s32 x;
        s32 y;
        s32 z;
    };

    KCollisionServer();

    void init(void*, const void*);
    void setData(void*);
    bool calcFarthestVertexDistance();
    static bool isBinaryInitialized(const void*) NO_INLINE;
    KC_PrismData* checkPoint(Fxyz*, f32, f32*);
    u32 checkArea3D(Fxyz*, Fxyz*, KC_PrismData**, u32);
    u32 checkSphere(Fxyz*, f32, f32, u32, KC_PrismData**, f32*, u8*);
    u32 checkSphereWithThickness(Fxyz*, f32, f32, u32, KC_PrismData**, f32*, u8*, f32);
    KC_PrismData* checkArrow(const TVec3f&, const TVec3f&, f32*, u8*, u32*, KC_PrismData**, u32) const;
    bool KCHitSphere(KC_PrismData*, Fxyz*, f32, f32, f32*, u8*);
    bool KCHitSphereWithThickness(KC_PrismData*, Fxyz*, f32, f32, f32*, u8*, f32);
    bool isNearParallelNormal(const KC_PrismData*) const;
    bool KCHitArrow(KC_PrismData*, const TVec3f&, const TVec3f&, f32*, u8*) const;
    s32 toIndex(const KC_PrismData*) const;
    TVec3f* getFaceNormal(const KC_PrismData*) const;
    TVec3f* getEdgeNormal1(const KC_PrismData*) const;
    TVec3f* getEdgeNormal2(const KC_PrismData*) const;
    TVec3f* getEdgeNormal3(const KC_PrismData*) const;
    TVec3f* getNormal(u32) const;
    void calXvec(const Fxyz*, const Fxyz*, Fxyz*) const;
    TVec3f getPos(const KC_PrismData*, int) const;
    KC_PrismData* getPrismData(u32) const;
    s32 getTriangleNum() const;
    JMapInfoIter getAttributes(u32) const;
    s32* searchBlock(s32*, const u32&, const u32&, const u32&) const;
    bool isInsideMinMaxInLocalSpace(const V3u&) const;
    bool outCheck(const TVec3f*, const TVec3f*, V3u*, V3u*) const;
    void objectSpaceToLocalSpace(V3u*, const TVec3f&) const;

    /* 0x00 */ KCLFile* mFile;
    /* 0x04 */ JMapInfo* mapInfo;
    /* 0x08 */ f32 mMaxVertexDistance;
};
