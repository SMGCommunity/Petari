#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class StarPointerCommandStream;
class StarPointerDirector;
class StarPointerBlur;
class StarPointerNumber;

// TODO: what are the missing values?
enum StarPointerKind {
    StarPointerKind_NULL = -1,
    StarPointerKind_StarPointer = 0,
    StarPointerKind_HandPointer = 1,
    StarPointerKind_2 = 2,
    StarPointerKind_HandPointerReactionWithCrossCursor = 3,
    StarPointerKind_FingerPointer = 4,
    StarPointerKind_5 = 5,
    StarPointerKind_StarPointerNozzle = 6,
};

enum AnimType {
    AnimType_NULL = -1,
    AnimType_Finger = 0,
    AnimType_HandGuu = 1,
    AnimType_HandPaa = 2,
    AnimType_StarPointer = 3,
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
    bool startCommandStream(const LiveActor*, const TVec3f*, bool);
    void endCommandStream(const LiveActor*);
    void forceEndCommandStream();
    bool isCommandStream(const LiveActor*);

    void exeWait();
    void exeOnReaction();
    void endOnReaction();
    void exeGrip();
    void exeHold();
    void endHold();
    void exeRelease();
    void exeOutScreen();
    void exeNotConnected();
    void endNotConnected();
    void exeHide();
    void endHide();

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
    bool isChanceToRumble() const;
    void hideBlur();
    void resetSameNerve();
    void changeToStarPointer();
    void changeToHandPointer();
    void startAnimStarPointer(const char*);
    void startAnimHandPaa();
    void startAnimHandGuu();
    void startAnimHandFinger();
    void updateDecoration();

    void show() {
        if (!mIsAppear) {
            mAppearTime = -3;
        }
        mIsAppear = true;
    }

    bool isAppear() const {
        return mIsAppear && mAppearTime >= 0;
    }

    AnimType getAnimType() const {
        return mAnimType;
    }

    /* 0x20 */ TVec2f mPosition;
    /* 0x28 */ StarPointerKind mPointerKind;
    /* 0x2C */ AnimType mAnimType;
    /* 0x30 */ s32 mTouchTimer;
    /* 0x34 */ s32 mPadChannel;
    /* 0x38 */ f32 mRadius;
    /* 0x3C */ f32 _3C;
    /* 0x40 */ f32 mRotateAngle;
    /* 0x44 */ StarPointerDirector* mDirector;
    /* 0x48 */ StarPointerBlur* mBlur;
    /* 0x4C */ StarPointerCommandStream* mCommandStream;
    /* 0x50 */ StarPointerNumber* mNumber;
    /* 0x54 */ const LiveActor* mActor;
    /* 0x58 */ u64 _58;  // some way of tracking last touched
    /* 0x60 */ u64 _60;  // past of _58
    /* 0x68 */ bool mIsNewTouch;
    /* 0x69 */ bool mStartTouch;
    /* 0x6A */ bool _6A;
    /* 0x6B */ bool _6B;
    /* 0x6C */ bool _6C;
    /* 0x6D */ bool mShootDisabled;
    /* 0x6E */ bool isPointerValid;
    /* 0x6F */ bool mIsAppear;
    /* 0x70 */ s32 mAppearTime;
    /* 0x74 */ u32 _74;
};

class StarPointerNumber : public LayoutActor {
public:
    StarPointerNumber(StarPointerLayout*, s32, const char*);

    virtual void control();

    bool isOffState() const;
    void startAnimPosition(u32) NO_INLINE;

    /* 0x20 */ StarPointerLayout* mParent;
    /* 0x24 */ s32 mPadChannel;
    /* 0x28 */ AnimType mAnimType;
    /* 0x2C */ bool mIsAppear;
    /* 0x30 */ s32 mAppearTime;
};
