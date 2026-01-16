#pragma once

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioModule.hpp"

class MarioActor;
class HitSensor;
class HashSortTable;
class XanimeResourceTable;
class XanimePlayer;

class MarioAnimator : public MarioModule {
public:
    MarioAnimator(MarioActor*);

    void init();
    void update();

    void setHoming();
    bool isAnimationStop() const;
    bool isLandingAnimationRun() const;
    bool isCancelableAnimationRun() const;
    bool isWalkOrWaitingMotion() const;
    bool isDefaultAnimationRun(const char*) const;
    void setSpeed(f32);
    void setWalkWeight(const f32*);
    void setBlendWeight(const f32*, f32);
    void forceSetBlendWeight(const f32*);
    void targetWeight(f32*, f32, f32);
    void waterToGround();
    void initCallbackTable();
    void change(const char*);
    void changeDefault(const char*);
    void changeUpper(const char*);
    void changeDefaultUpper(const char*);
    void stopUpper(const char*);
    void setUpperRotateY(f32);
    void updateJointRumble();
    void clearAllJointTransform();
    bool isMirrorAnimation();
    void switchMirrorMode();
    void changePickupAnimation(const HitSensor*);
    void updateTakingAnimation(const HitSensor*);
    void changeThrowAnimation(const HitSensor*);
    void updateWalkBas(const char*, f32);
    void setHand();
    void setTilt();
    void resetTilt();
    void setHipSlidingTilt(f32, f32);
    void setHipSliderTilt();
    void setHipSlipTilt();
    void setWalkMode();
    void calc();
    void entryCallback(const char*);
    void runningCallback();
    void closeCallback();
    void spinEntry();
    void spinUpdate();
    void spinClose();
    void stageInCheck();
    void throwCheck();
    void throwEntry();
    void throwClose();
    void squatSpinCheck();
    void walkinClose();
    f32 getFrame() const;
    f32 getUpperFrame() const;
    s32 getUpperJointID() const;
    void initWalkWeight();
    void addRumblePower(f32, u32);

    inline void f1(const char* name) {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

        mXanimePlayer->setDefaultAnimation(name);
    }

    inline bool isTeresaClear() const { return !isPlayerModeTeresa(); }

    /* 0x08 */ XanimeResourceTable* mResourceTable;
    /* 0x0C */ XanimePlayer* mXanimePlayer;
    /* 0x10 */ XanimePlayer* mXanimePlayerUpper;
    /* 0x14 */ u8 _14;
    /* 0x15 */ u8 _15;
    /* 0x16 */ u8 _16;
    /* 0x17 */ u8 _17;
    /* 0x18 */ f32 _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 _24;
    /* 0x28 */ TMtx34f _28;
    /* 0x58 */ f32 _58;
    /* 0x5C */ f32 _5C;
    /* 0x60 */ TVec3f _60;
    /* 0x6C */ bool _6C;
    /* 0x6D */ u8 _6D[3];
    /* 0x70 */ f32 _70;
    /* 0x74 */ u32 _74;
    /* 0x78 */ u16 _78;
    /* 0x7A */ u8 _7A[2];
    /* 0x7C */ TMtx34f _7C;
    /* 0xAC */ TMtx34f _AC;
    /* 0xDC */ TMtx34f _DC;
    /* 0x10C */ u8 _10C;
    /* 0x10D */ u8 _10D;
    /* 0x10E */ bool mUpperDefaultSet;
    /* 0x10F */ u8 _10F;
    /* 0x110 */ f32 _110;
    /* 0x114 */ const char* mCurrBck;
    /* 0x118 */ f32 _118;
    /* 0x11C */ s32 _11C;
    /* 0x120 */ HashSortTable* _120;
};
