#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class BezierSurface {
public:
    BezierSurface(s32, u32);

    void setST(const TVec2f&, const TVec2f&, const TVec2f&);
    void setDivideLevel(s32);
    void calcBuffer();
    void draw() const;
    void calcTrianglePatchVertix(TVec3f*) const;
    void calcTrianglePatchNormal(TVec3f*) const;
    void drawTrianglePatchPos() const;
    void drawTrianglePatchPosNorm() const;
    void drawTrianglePatchPosST() const;
    void drawTrianglePatchPosNormST() const;

    /* 0x00 */ TVec3f mCtrlPts[13];
    /* 0x9C */ TVec3f* mTrianglePatchVertices;  // array
    /* 0xA0 */ TVec3f* mTrianglePatchNormals;   // array
    /* 0xA4 */ TVec2f mSTPoints[3];
    /* 0xBC */ s32 mDivideLevel;
    /* 0xC0 */ s32 _C0;
    /* 0xC4 */ u32 _C4;
};

typedef f32 TrianglePatchCoefs[13];

struct TrianglePatch {
    /* 0x00 */ TrianglePatchCoefs* mCoefTable;
    /* 0x04 */ TrianglePatchCoefs* mCoefTangentSTable;
    /* 0x08 */ TrianglePatchCoefs* mCoefTangentTTable;
    /* 0x0C */ s32 mNumPoints;
    /* 0x10 */ const u16* mTriangleStripOrder;
    /* 0x14 */ u16 mNumTriangleStripPoints;
    /* 0x18 */ const u16* mLineStripOrder;
    /* 0x1C */ u16 mNumLineStripPoints;
    /* 0x20 */ const Vec* mTriangleST;
};
