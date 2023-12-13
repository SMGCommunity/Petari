#pragma once

#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class JMapInfo;

struct KC_PrismData {
    f32 mHeight;                // _0
    u16 mPositionIndex;         // _4
    u16 mNormalIndex;           // _6
    u16 mEdgeIndices[3];        // _8
    u16 mAttribute;             // _E
};

class Fxyz {
public:
    inline Fxyz() {

    }

    Fxyz &operator=(const Fxyz &);

    f32 x, y, z;
};

struct KCLFile {
    union {
        TVec3f *mPos;           // _0
        u32 mPosOffset;         // _0
    };
    union {
        TVec3f *mNorms;         // _4
        u32 mNormOffset;        // _4
    };
    union {
        KC_PrismData *mPrisms;  // _8
        u32 mPrismOffset;       // _8
    };
    union {
        void *mOctree;          // _C
        u32 mOctreeOffset;      // _C
    };
    f32 mThickness;             // _10
    TVec3f mMin;                // _14
    s32 mXMask;                 // _20
    s32 mYMask;                 // _24
    s32 mZMask;                 // _28
    s32 mBlockWidthShift;       // _2C
    s32 mBlockXShift;           // _30
    s32 mBlockXYShift;          // _34
};

class KCollisionServer {
public:
    class V3u {
    public:
        inline V3u() {

        }

        void setUsingCast(const TVec3f &);

        s32 x;
        s32 y;
        s32 z;
    };

    KCollisionServer();

    void init(void *, const void *);
    void setData(void *);
    void calcFarthestVertexDistance();
    static bool isBinaryInitialized(const void *);
    void checkPoint(Fxyz *, float, float *);
    void checkArea3D(Fxyz *, Fxyz *, KC_PrismData **, unsigned long);
    void checkSphere(Fxyz *, float, float, unsigned long, KC_PrismData **, float *, unsigned char *);
    void checkSphereWithThickness(Fxyz *, float, float, unsigned long, KC_PrismData **, float *, unsigned char *, float);
    void checkArrow(const TVec3f, const TVec3f, float *, unsigned char *, unsigned long *, KC_PrismData **, unsigned long) const;
    void KCHitSphere(KC_PrismData *, Fxyz *, float, float, float *, unsigned char *);
    void KCHitSphereWithThickness(KC_PrismData *, Fxyz *, float, float, float *, unsigned char *, float);
    bool isNearParallelNormal(const KC_PrismData *) const;
    void KCHitArrow(KC_PrismData *, const TVec3f, const TVec3f, float *, unsigned char *) const;
    s32 toIndex(const KC_PrismData *) const;
    TVec3f *getFaceNormal(const KC_PrismData *) const;
    TVec3f *getEdgeNormal1(const KC_PrismData *) const;
    TVec3f *getEdgeNormal2(const KC_PrismData *) const;
    TVec3f *getEdgeNormal3(const KC_PrismData *) const;
    TVec3f *getNormal(unsigned long) const;
    void calXvec(const Fxyz *, const Fxyz *, Fxyz *) const;
    TVec3f getPos(const KC_PrismData *, int) const;
    KC_PrismData *getPrismData(unsigned long) const;
    s32 getTriangleNum() const;
    JMapInfoIter getAttributes(unsigned long) const;
    s32 *searchBlock(long *, const unsigned long &, const unsigned long &, const unsigned long &) const;
    bool isInsideMinMaxInLocalSpace(const V3u &) const;
    bool outCheck(const TVec3f *, const TVec3f *, V3u *, V3u *) const;
    void objectSpaceToLocalSpace(V3u *, const TVec3f &) const;

    KCLFile* mFile;         // _0
    JMapInfo *mapInfo;      // _4
    f32 mMaxVertexDistance; // _8
};