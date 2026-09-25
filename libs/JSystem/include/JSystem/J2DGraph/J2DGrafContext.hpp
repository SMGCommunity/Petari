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
    /* 0x00 */ u8 mType;
    /* 0x01 */ u8 mSrcFactor;
    /* 0x02 */ u8 mDestFactor;
};

class J2DGrafContext {
public:
    J2DGrafContext(f32, f32, f32, f32);

    virtual ~J2DGrafContext() {
    }

    virtual void place(const TBox2f&);
    virtual void place(f32 x, f32 y, f32 width, f32 height) {
        place(TBox2f(x, y, x + width, y + height));
    }

    virtual void setPort();
    virtual void setup2D();
    virtual void setScissor();
    virtual J2DGrafType getGrafType() const;
    virtual void setLookat();

    void setColor(JUtility::TColor c) {
        setColor(c, c, c, c);
    }

    void setColor(JUtility::TColor, JUtility::TColor, JUtility::TColor, JUtility::TColor);
    void fillBox(const TBox2f&);
    void fillBox(f32 x, f32 y, f32 width, f32 height) {
        fillBox(TBox2f(x, y, x + width, y + height));
    }

    /* 0x04 */ TBox2f mBounds;
    /* 0x14 */ TBox2f mScissorBounds;
    /* 0x24 */ JUtility::TColor mColorTL;
    /* 0x28 */ JUtility::TColor mColorTR;
    /* 0x2C */ JUtility::TColor mColorBR;
    /* 0x30 */ JUtility::TColor mColorBL;
    /* 0x34 */ u8 mLineWidth;
    /* 0x38 */ TVec2f mPrevPos;
    /* 0x40 */ Mtx44 mMtx44;
    /* 0x80 */ Mtx mPosMtx;
    J2DGrafBlend _B0;
    J2DGrafBlend mLinePart;
    J2DGrafBlend mBoxPart;
};
