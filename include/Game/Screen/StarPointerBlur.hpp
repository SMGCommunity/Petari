#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class JUTTexture;

class StarPointerBlur : public NameObj {
public:
    class BlurPoint {
    public:
        BlurPoint();

        /* 0x00 */ TVec2f mPos;
        /* 0x08 */ TVec2f mDir;
        /* 0x10 */ bool mIsHidden;
    };

    StarPointerBlur(JUTTexture* pTexture);

    virtual ~StarPointerBlur();

    virtual void movement();
    virtual void draw() const;

    void show();
    void hide();
    void hideAll();
    void updatePoint(const TVec2f&, bool, TVec2f*);
    u16 findDrawPointRange(u16*, u16*) const;
    void draw0(GXColor) const;
    void draw1() const;
    void draw2() const;
    void initDraw() const;

    void setPortAndColor(s32 padChannel, const GXColor* pMainColor, const GXColor* pSubColor) NO_INLINE {
        mPadChannel = padChannel;
        mMainColor = pMainColor;
        mSubColor = pSubColor;
    }

    /* 0x0C */ JUTTexture* mTexture;
    /* 0x10 */ s32 mPadChannel;
    /* 0x14 */ const GXColor* mMainColor;
    /* 0x18 */ const GXColor* mSubColor;
    /* 0x1C */ BlurPoint* mBlurPoints;
    /* 0x20 */ f32* mBlurThicks;
    /* 0x24 */ f32* mBlurTexCoords;
    /* 0x28 */ s32 mNumHiddenPoints;
    /* 0x2C */ bool _2C;
    /* 0x2C */ bool _2D;
    /* 0x30 */ f32 _30;
    /* 0x34 */ f32 _34;
    /* 0x38 */ f32 _38;
    /* 0x3C */ f32 _3C;
};
