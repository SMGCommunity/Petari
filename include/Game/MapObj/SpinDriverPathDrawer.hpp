#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

class SpinDriverPathDrawInit : public NameObj {
public:
    SpinDriverPathDrawInit();

    /* 0x08 */ virtual ~SpinDriverPathDrawInit();

    void initDraw();

    JUTTexture* mOrangeTexture;  // 0xC
    JUTTexture* mGreenTexture;   // 0x10
    JUTTexture* mPinkTexture;    // 0x14
    JUTTexture* mMaskTexture;    // 0x18
    bool mIsPathAtOpa;           // 0x1C
};

class SpinDriverPathDrawer : public LiveActor {
public:
    SpinDriverPathDrawer(SpinDriverShootPath*);

    /* 0x08 */ virtual ~SpinDriverPathDrawer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;

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
    void sendPoint(const TVec3f&, f32, f32);

    SpinDriverShootPath* mShootPath;  // 0x8C
    TVec3f* _90;
    TVec3f* _94;
    TVec3f* _98;
    void* _9C;
    s32 mPositionCount;  // 0xA0
    u32 _A4;
    u32 _A8;
    f32 _AC;
    f32 _B0;
    s32 mColor;  // 0xB4
    f32 _B8;
    f32 mFadeScale;   // 0xBC
    f32 mMaskLength;  // 0xC0
};

namespace MR {
    void setSpinDriverPathColorNormal();
    void setSpinDriverPathColorGreen();
    void setSpinDriverPathColorPink();
    bool isDrawSpinDriverPathAtOpa();
    void onDrawSpinDriverPathAtOpa();
    void offDrawSpinDriverPathAtOpa();
};  // namespace MR