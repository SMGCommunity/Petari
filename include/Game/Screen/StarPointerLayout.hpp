#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class StarPointerCommandStream;
class StarPointerDirector;

enum StarPointerKind {
    StarPointerKind_StarPointer = 0,
    StarPointerKind_HandPointer = 1,
    StarPointerKind_2 = 2,
    StarPointerKind_HandPointerReactionWithCrossCursor = 3,
    StarPointerKind_FingerPointer = 4,
    StarPointerKind_5 = 5,
    StarPointerKind_StarPointerNozzle = 6,
};

class StarPointerLayout : public LayoutActor {
public:
    StarPointerLayout(const char* pName = "スターポインタレイアウト");

    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void initWithPort(s32);
    void startCommandStream(const LiveActor*, const TVec3f*, bool);
    void endCommandStream(const LiveActor*);
    void forceEndCommandStream();
    bool isCommandStream(const LiveActor*);

    void exeWait();
    void exeOnReaction();
    void exeGrip();
    void exeHold();
    void exeHoldTouch();
    void exeRelease();
    void exeOutScreen();
    void exeNotConnected();
    void exeHide();
    void exeHideImmediately();
    void exeInvalidate();
    void exeInvalidateWait();

    void tearDownHold();
    void updateTouch();
    bool isTouch();
    void hideAll();
    void tearDownHide();
    void drawDecoration() const;
    void setPosition();
    void rotateTest();
    void calcPoseNozzle(const TVec2f&, const TVec2f&);
    void changeLayout(StarPointerKind);
    bool isChangeToRumble() const;
    void hideBlur();
    void resetSameNerve();
    void changeToStarPointer();
    void changeToHandPointer();
    void startAnimHandPaa();
    void startAnimHandGuu();
    void startAnimHandFinger();
    void updateDecoration();

    void show();  // in OnOffController

    /* 0x20 */ TVec2f _20;
    /* 0x28 */ StarPointerKind mPointerKind;
    /* 0x2C */ s32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ s32 _34;
    /* 0x38 */ f32 _38;
    /* 0x3C */ f32 _3C;
    /* 0x40 */ f32 _40;
    /* 0x44 */ StarPointerDirector* mDirector;
    /* 0x48 */ u32 _48;
    /* 0x4C */ StarPointerCommandStream* mCommandStream;
    /* 0x50 */ u32 _50;
    /* 0x54 */ u32 _54;
    /* 0x58 */ u32 _58;
    /* 0x5C */ u32 _5C;
    /* 0x60 */ u32 _60;
    /* 0x64 */ u32 _64;
    /* 0x68 */ bool _68;
    /* 0x69 */ bool _69;
    /* 0x6A */ bool _6A;
    /* 0x6B */ bool _6B;
    /* 0x6C */ bool _6C;
    /* 0x6D */ bool mShootDisabled;
    /* 0x6E */ bool _6E;
    /* 0x6F */ bool _6F;
    /* 0x70 */ u32 _70;
    /* 0x74 */ u32 _74;
};

class StarPointerNumber : public LayoutActor {
public:
    StarPointerNumber(StarPointerLayout*, s32, const char*);

    virtual void control();

    bool isOffState() const;
    void startAnimPosition(u32);

    /* 0x20 */ s32 _20;
    /* 0x24 */ const char* _24;
    /* 0x28 */ s32 _28;
    /* 0x2C */ bool _2C;
    /* 0x30 */ u32 _30;
};
