#pragma once

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioModule.hpp"

class MarioActor;
class XanimeResourceTable;
class XanimePlayer;
class HitSensor;

class MarioAnimator : public MarioModule {
public:
    MarioAnimator(MarioActor*);

    void init();
    void update();
    void calc();

    void setHoming();
    bool isAnimationStop() const;
    bool isDefaultAnimationRun(const char*) const;
    void setSpeed(f32);
    void forceSetBlendWeight(const f32*);
    void waterToGround();
    void initCallbackTable();
    void change(const char*);
    void changeDefault(const char*);
    void changeUpper(const char*);
    void changeDefaultUpper(const char*);
    void stopUpper(const char*);
    void setUpperRotateY(f32);
    void entryCallback(const char*);
    f32 getFrame() const;
    f32 getUpperFrame() const;
    s32 getUpperJointID() const;
    void setBlendWeight(const f32*, f32);
    void targetWeight(f32*, f32, f32);
    void setWalkWeight(const f32*);
    void initWalkWeight();
    bool isLandingAnimationRun() const;
    bool isCancelableAnimationRun() const;
    bool isWalkOrWaitingMotion() const;
    void updateWalkBas(const char*, f32);
    void setHand();
    void setTilt();
    void resetTilt();
    void setHipSlidingTilt(f32, f32);
    void setHipSliderTilt();
    void setHipSlipTilt();
    void setWalkMode();
    void updateJointRumble();
    void addRumblePower(f32, u32);
    void clearAllJointTransform();
    bool isMirrorAnimation();
    void switchMirrorMode();
    void changePickupAnimation(const HitSensor*);
    void updateTakingAnimation(const HitSensor*);
    void changeThrowAnimation(const HitSensor*);
    void runningCallback();
    void closeCallback();
    void spinEntry();
    void spinUpdate();
    void spinClose();
    void stageInCheck();
    void throwCheck();

    inline void f1(const char* name) {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

        mXanimePlayer->setDefaultAnimation(name);
    }

    inline bool isTeresaClear() const { return !isPlayerModeTeresa(); }

    XanimeResourceTable* mResourceTable;  // 0x8
    XanimePlayer* mXanimePlayer;          // 0xC
    XanimePlayer* mXanimePlayerUpper;     // 0x10
    u8 _14;
    u8 _15;
    u8 _16;
    f32 _18;
    f32 _1C;
    f32 _20;
    f32 _24;
    TMtx34f _28;
    f32 _58;
    f32 _5C;
    TVec3f _60;
    bool _6C;
    f32 _70;
    u32 _74;
    u16 _78;
    TMtx34f _7C;
    TMtx34f _AC;
    TMtx34f _DC;
    bool _10C;
    bool _10D;
    bool mUpperDefaultSet;  // 0x10E
    f32 _110;
    const char* mCurrBck;  // 0x114
    f32 _118;

    u8 _11C[8];
};
