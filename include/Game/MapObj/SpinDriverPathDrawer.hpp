#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

class SpinDriverPathDrawInit : public NameObj {
public:
    SpinDriverPathDrawInit();

    virtual ~SpinDriverPathDrawInit();

    void initDraw();

    JUTTexture* mOrangeTexture; // _C
    JUTTexture* mGreenTexture;  // _10
    JUTTexture* mPinkTexture;   // _14
    JUTTexture* mMaskTexture;   // _18
    bool mIsPathAtOpa;          // _1C
};

class SpinDriverPathDrawer : public LiveActor {
public:
    SpinDriverPathDrawer(SpinDriverShootPath *);

    virtual ~SpinDriverPathDrawer();
    virtual void init(const JMapInfoIter &);
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
    void sendPoint(const TVec3f &, f32, f32);

    SpinDriverShootPath* mShootPath;    // _8C
    TVec3f* _90;
    TVec3f* _94;
    TVec3f* _98;
    void* _9C;
    s32 mPositionCount;                 // _A0
    u32 _A4;
    u32 _A8;
    f32 _AC;
    f32 _B0;
    s32 mColor;                         // _B4
    f32 _B8;
    f32 mFadeScale;                     // _BC
    f32 mMaskLength;                    // _C0
};

namespace MR {
    void setSpinDriverPathColorNormal();
    void setSpinDriverPathColorGreen();
    void setSpinDriverPathColorPink();
    bool isDrawSpinDriverPathAtOpa();
    void onDrawSpinDriverPathAtOpa();
    void offDrawSpinDriverPathAtOpa();
};