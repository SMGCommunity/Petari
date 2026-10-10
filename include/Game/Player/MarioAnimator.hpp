#pragma once

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioModule.hpp"

class HashSortTable;
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
    u16 getUpperJointID() const;
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
    void stopWaitAnimation();
    void controlWaitAnimation();
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

    inline void f1(const char* name) {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

        mXanimePlayer->setDefaultAnimation(name);
    }

    inline bool isTeresaClear() const {
        return !isPlayerModeTeresa();
    }

    inline XanimePlayer* getXanimePlayer() {
        return mXanimePlayer;
    }

    /* 0x008 */ XanimeResourceTable* mResourceTable;
    /* 0x00C */ XanimePlayer* mXanimePlayer;
    /* 0x010 */ XanimePlayer* mXanimePlayerUpper;
    /* 0x014 */ u8 _14;
    /* 0x015 */ u8 _15;
    /* 0x016 */ u8 _16;
    /* 0x018 */ f32 _18;
    /* 0x01C */ f32 _1C;
    /* 0x020 */ f32 _20;
    /* 0x024 */ f32 _24;
    /* 0x028 */ TMtx34f _28;
    /* 0x058 */ f32 _58;
    /* 0x05C */ f32 _5C;
    /* 0x060 */ TVec3f _60;
    /* 0x06C */ bool _6C;
    /* 0x070 */ f32 _70;
    /* 0x074 */ u32 _74;
    /* 0x078 */ u16 _78;
    /* 0x07C */ TMtx34f _7C;
    /* 0x0AC */ TMtx34f _AC;
    /* 0x0DC */ TMtx34f _DC;
    /* 0x10C */ bool _10C;
    /* 0x10D */ bool _10D;
    /* 0x10E */ bool mUpperDefaultSet;
    /* 0x10F */ bool mCallbackEnded;
    /* 0x110 */ f32 _110;
    /* 0x114 */ const char* mCurrBck;
    /* 0x118 */ f32 _118;

    /* 0x11C */ s32 mCallbackId;
    /* 0x120 */ HashSortTable* mCallbackTable;
};
