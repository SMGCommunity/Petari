#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;
class SpinDriverShootPath;

class SpinDriverPathDrawer : public LiveActor {
public:
    SpinDriverPathDrawer(SpinDriverShootPath*);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    s32 calcPositionCount(f32, f32) const;
    void initPositionList(f32, f32);
    void initClipping();
    void initPathEnd();
    f32 calcDrawCode() const;
    f32 calcDrawBaseCode() const;
    void setCoord(f32);
    void setColorNormal();
    void setColorGreen();
    void setColorPink();
    void setMaskLength(f32);
    void setFadeScale(f32);
    void sendPoint(const TVec3f&, f32, f32) const;

    /* 0x8C */ SpinDriverShootPath* mShootPath;
    /* 0x90 */ TVec3f* _90;
    /* 0x94 */ TVec3f* _94;
    /* 0x98 */ TVec3f* _98;
    /* 0x9C */ f32* _9C;
    /* 0xA0 */ s32 mPositionCount;
    /* 0xA4 */ u32 _A4;
    /* 0xA8 */ u32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ f32 _B0;
    /* 0xB4 */ s32 mColor;
    /* 0xB8 */ f32 _B8;
    /* 0xBC */ f32 mFadeScale;
    /* 0xC0 */ f32 mMaskLength;
};

class SpinDriverPathDrawInit : public NameObj {
public:
    SpinDriverPathDrawInit();

    void initDraw();

    /* 0x0C */ JUTTexture* mOrangeTexture;
    /* 0x10 */ JUTTexture* mGreenTexture;
    /* 0x14 */ JUTTexture* mPinkTexture;
    /* 0x18 */ JUTTexture* mMaskTexture;
    /* 0x1C */ bool mIsPathAtOpa;
};

namespace MR {
    void setSpinDriverPathColorNormal() NO_INLINE;
    void setSpinDriverPathColorGreen() NO_INLINE;
    void setSpinDriverPathColorPink() NO_INLINE;
    bool isDrawSpinDriverPathAtOpa();
    void onDrawSpinDriverPathAtOpa();
    void offDrawSpinDriverPathAtOpa();
};  // namespace MR
