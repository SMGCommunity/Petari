#pragma once

#include <revolution.h>

class StarPointerController;
class StarPointerLayout;

class StarPointerPeekZ {
public:
    StarPointerPeekZ();

    virtual void drawSyncCallback(u16);

    u16 mToken;  // 0x4
    u32 _8;
    f32 mProjection;  // 0xC
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    f32* mViewport;  // 0x28
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
    u32 _3C;
};

class StarPointerTransformHolder {
public:
    StarPointerTransformHolder();

    void movement();

    Mtx _0;
    Mtx44 _30;
    f32 mFovy;  // 0x70
    f32 _74;
};

class StarPointerDirector {
public:
    StarPointerDirector();

    void draw();
    void update();

    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    StarPointerController* mControllers;       // 0x4
    StarPointerLayout* mStarPointerLayouts;    // 0x8
    StarPointerTransformHolder* mTransHolder;  // 0xC
    StarPointerPeekZ* mPeekZ;                  // 0x10
    u32* _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
};

namespace StarPointerFunction {

    bool isOnScreenEdge(s32);
    bool canShoot(s32);

}
