#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class StarPointerController;
class StarPointerLayout;

class StarPointerPeekZ {
public:
    StarPointerPeekZ();

    virtual void drawSyncCallback(u16);

    void setDrawSyncToken();

    /* 0x04 */ u16 mToken;
    /* 0x08 */ u32 _8;
    /* 0x0C */ f32 mProjection;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u32 _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ u32 _20;
    /* 0x24 */ u32 _24;
    /* 0x28 */ f32* mViewport;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ u32 _34;
    /* 0x38 */ u32 _38;
    /* 0x3C */ u32 _3C;
};

class StarPointerTransformHolder {
public:
    StarPointerTransformHolder();

    void movement();

    /* 0x00 */ TPos3f _0;
    /* 0x30 */ Mtx44 _30;
    /* 0x70 */ f32 mFovy;
    /* 0x74 */ f32 _74;
};

class StarPointerDirector {
public:
    StarPointerDirector();

    void update();
    void draw();

    void startHandPointer();
    void startHandPointerReactionWithCrossCursor();
    void startFingerPointer();
    void startStarPointer();
    void startStarPointerNozzle();
    void setGameSceneCameraMtx();
    StarPointerController* getStarPointerController(s32) const;

    /* 0x00 */ bool _0;
    /* 0x01 */ bool _1;
    /* 0x02 */ bool _2;
    /* 0x03 */ bool _3;
    /* 0x04 */ StarPointerController* mControllers;
    /* 0x08 */ StarPointerLayout* mStarPointerLayouts;
    /* 0x0C */ StarPointerTransformHolder* mTransHolder;
    /* 0x10 */ StarPointerPeekZ* mPeekZ;
    /* 0x14 */ u32* _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ u32 _20;
    /* 0x24 */ u32 _24;
};

namespace StarPointerFunction {
    bool isOnScreenEdge(const TVec2f&, f32, f32);
    bool isOnScreenEdge(s32);
    bool forceInsideScreenEdge(TVec2f*);
    const StarPointerDirector* getStarPointerDirector();
    s32 getNumStarPointer();
    s32 getPastPointNum(s32);
    const TVec2f& getPastPosition(s32, s32);
    s32 getNextPastPointNum(s32);
    bool canShoot(s32);
}  // namespace StarPointerFunction
