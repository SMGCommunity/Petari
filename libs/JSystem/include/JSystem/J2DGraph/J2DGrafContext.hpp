#pragma once

#include "JSystem/JGeometry.hpp"
#include "JSystem/JGeometry/TBox.hpp"
#include "JSystem/JUtility/TColor.hpp"
#include <revolution.h>

enum J2DGrafType {
    J2DGraf_Base = 0,
    J2DGraf_Ortho = 1,
    J2DGraf_Persp = 2,
};

struct J2DGrafBlend {
    u8 mType;        // 0x00
    u8 mSrcFactor;   // 0x01
    u8 mDestFactor;  // 0x02
};

class J2DGrafContext {
public:
    J2DGrafContext(f32, f32, f32, f32);

    virtual ~J2DGrafContext() {}

    virtual void place(const TBox2f&);
    virtual void place(f32, f32, f32, f32);
    virtual void setPort();
    virtual void setup2D();
    virtual void setScissor();
    virtual J2DGrafType getGrafType() const;
    virtual void setLookat();

    void setColor(JUtility::TColor c) { this->setColor(c, c, c, c); }
    void setColor(JUtility::TColor, JUtility::TColor, JUtility::TColor, JUtility::TColor);
    void fillBox(const TBox2f&);

    TBox2f mBounds;             // 0x04
    TBox2f mScissorBounds;      // 0x14
    JUtility::TColor mColorTL;  // 0x24
    JUtility::TColor mColorTR;  // 0x28
    JUtility::TColor mColorBR;  // 0x2C
    JUtility::TColor mColorBL;  // 0x30
    u8 mLineWidth;              // 0x34
    TVec2f mPrevPos;            // 0x38
    Mtx44 mMtx44;               // 0x40
    Mtx mPosMtx;                // 0x80
    J2DGrafBlend _B0;
    J2DGrafBlend mLinePart;
    J2DGrafBlend mBoxPart;
};
