#pragma once

#include "Game/Map/WaterInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"

class MarioActor;
class MarineSnow;

class MarioSwim : public MarioState {
public:
    enum WaterExitAction {
        EXIT_ACTION_NONE = 0,
        EXIT_ACTION_JUMP = 1,        // Shallow water jump
        EXIT_ACTION_POWER_JUMP = 2,  // Spin jump or paddle jump
        EXIT_ACTION_FALL = 3,

        EXIT_ACTION_SLIDE = 5,
        EXIT_ACTION_SURFACE = 6
    };
    enum SwimState { SWIM_STATE_NONE = 0, SWIM_STATE_ENTERING = 1, SWIM_STATE_UNDERWATER = 2, SWIM_STATE_SURFACE = 3 };

    MarioSwim(MarioActor*);

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f&, HitSensor*);
    virtual void hitPoly(u8, const TVec3f&, HitSensor*);
    virtual bool passRing(const HitSensor*);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;
    virtual f32 getStickY() const;
    virtual const TVec3f& getGravityVec() const;
    virtual void addVelocity(const TVec3f& rVelocity);
    virtual void addVelocity(const TVec3f& rVelocity, f32);


    bool checkWaterCube(bool);
    void onSurface() NO_INLINE;
    void doJetJump(u8);
    void updateLifeByTime();
    bool surfacePaddle();
    void flowOnWave(f32);
    bool checkWaterBottom();
    void spin();
    void decideVelocity();
    void procBuoyancy();
    void decideAnimation();
    void decideEffect(bool);
    void updateTilt();
    void jet();
    void pushedByWaterWall();
    void setDamage(const TVec3f&, u16);
    void updateUnderWater();
    bool startJet(u32);
    void addDamage(const TVec3f&);
    void addFaint(const TVec3f&);
    AreaInfo* getWaterAreaInfo(WaterInfo*, const TVec3f&, TVec2f*);
    void decOxygen(u16 amount);
    void incOxygen();
    void incLife();
    void decLife();
    f32 calcRingAcc();
    void hitPunch(const TVec3f& rPunchDir);
    f32 getSurface() const;
    bool tryJetAttack(HitSensor*);
    void dropJet(bool);
    void updateOxygenWatch();
    void doDecLifeByCold();
    void startSpinDash();
    void ringDash();
    void resetJet();
    void forceStopSpin();
    void resetAndFixPose();
    f32 checkUnderWaterFull(const TVec3f&);
    void hitHead(const HitInfo*);

    static inline f32 getWorthlessNumber() { return 0.523598790169f; }

    /* The worthlesser variable, as its name suggests, has no purpose.
       It achieves nothing in life. Its existence is completely devoid of value.
       Nevertheless, the compiler left evidence of this forgettable variable behind.
       As a result, we know that it existed, but we do not know how precisely it achieved nothing.

       Summary:
       worthlesser does not impact the behavior of this function.

     */

    inline void funReferenceTime(bool& worthlesser) {
        if (mVerticalAngle > 1.57079637051f) {
            worthlesser = true;
        } else {
            if (mIdleWaitTimer > 0x1E) {
                _1E = 1;
            }
            worthlesser = false;
            if (getStickY() > 0.0f) {
                f32 stickY = getStickY();
                // Note: The binary does not tell us whether this comparison is > or <.
                // It is not == because that generates an fcmpu instruction, not fcmpo.
                // It is not <= or >= because those generate cror instructions.
                if (getWorthlessNumber() + getWorthlessNumber() * stickY < mVerticalAngle

                    // This comparison needs to be present for the compiler to optimize the condition
                    // accurately.
                    && worthlesser) {
                    return;
                }
            }
        }
        worthlesser = true;
    }

    inline TVec3f getPlayer380() const { return getPlayer()->_380; }

    inline bool check7Aand7C() const { return mSpinTimer || mSpinDashTimer; }
    inline f32 getSwimFrontJetSpeed();
    inline f32 getSwimFrontJetSpeedSlow();
    inline f32 getSwimFrontMaxSpeed();
    inline void updateSwimWeight(int animIndex, const MarioConstTable* table);
    inline void setupSwimSpeeds(f32* speeds);
    
    /* 0x014 */ MarineSnow* mMarineSnow;
    /* 0x018 */ u8 _18;
    /* 0x019 */ bool mIsOnSurface;
    /* 0x01A */ bool mEnteredWater;
    /* 0x01B */ bool _1B;
    /* 0x01C */ bool _1C;
    /* 0x01D */ bool _1D;
    /* 0x01E */ bool _1E;
    /* 0x01F */ bool _1F;
    /* 0x020 */ bool mIsSwimmingAtSurface;
    /* 0x021 */ bool mIsSinking;
    /* 0x022 */ bool _22;
    /* 0x024 */ u32 mStateTimer;
    /* 0x028 */ u32 mSurfacePaddleHoldTimer;
    /* 0x02C */ u16 mActionLockTimer;
    /* 0x02E */ u16 mSinkTimer;
    /* 0x030 */ u16 mSwimYAccTimer;
    /* 0x032 */ u16 mZSinkTimer;
    /* 0x034 */ u16 mJumpDiveTimer;
    /* 0x036 */ u16 mDashTimer;
    /* 0x038 */ u16 _38;
    /* 0x03A */ u16 mBuoyancyDelayTimer;
    /* 0x03C */ u16 mIdleWaitTimer;
    /* 0x03E */ u16 mWaveFlowTimer;
    /* 0x040 */ u16 mJetCooldownTimer;
    /* 0x042 */ u16 mFloorContactTimer;
    /* 0x044 */ u16 _44;
    /* 0x048 */ f32 mSurfaceOffset;
    /* 0x04C */ f32 mStickInputYInertia;
    /* 0x050 */ f32 mStickInputXInertia;
    /* 0x054 */ f32 mForwardSpeed;
    /* 0x058 */ f32 mBuoyancySpeed;
    /* 0x05C */ f32 mVerticalAngle;
    /* 0x060 */ TVec3f mFrontVec;
    /* 0x06C */ TVec3f mUpVec;
    /* 0x078 */ bool _78;
    /* 0x07A */ u16 mSpinTimer;
    /* 0x07C */ u16 mSpinDashTimer;
    /* 0x080 */ f32 _80;
    /* 0x084 */ f32 _84;
    /* 0x088 */ u8 _88;
    /* 0x08A */ u16 mJetTimer;
    /* 0x08C */ u16 mRingDashTimer;
    /* 0x08E */ u16 mRingDashChargeTimer;
    /* 0x090 */ u16 mRingDashMaxDuration;
    /* 0x094 */ f32 mRingDashSpeedScale;
    /* 0x098 */ f32 mBlurOffset;
    /* 0x09C */ bool _9C;
    /* 0x09D */ u8 _9D;
    /* 0x09E */ u8 mNextAction;
    /* 0x09F */ u8 mEffectState;
    /* 0x0A0 */ TVec3f mKnockbackVel;
    /* 0x0AC */ u8 mDamageType;
    /* 0x0AD */ u8 _AD;
    /* 0x0AE */ u16 mKnockbackTimer;
    /* 0x0B0 */ TMtx34f mUpperBodyJointMtx;
    /* 0x0E0 */ f32 mCurrentTiltX;
    /* 0x0E4 */ f32 mCurrentTiltY;
    /* 0x0E8 */ u16 mDrownTimer;
    /* 0x0EA */ u16 mOxygen;
    /* 0x0EC */ u16 mOxygenWarningTimer;
    /* 0x0EE */ u16 mOxygenDecreaseDelay;
    /* 0x0F0 */ u16 mColdWaterDamageInterval;
    /* 0x0F4 */ WaterInfo mWaterInfo;
    /* 0x144 */ s32 mSwimState;
    /* 0x148 */ TVec3f _148;
    /* 0x154 */ TVec3f mAreaFollowMtxPos;
    /* 0x160 */ TVec3f mSurfacePos;
    /* 0x16C */ TVec3f mSurfaceNorm;
    /* 0x178 */ TVec3f _178;
    /* 0x184 */ TVec3f mWaterStreamVel;
    /* 0x190 */ TVec3f mWaterStreamDir;
    /* 0x19C */ f32 mWaterDepth;
    /* 0x1A0 */ f32 _1A0;
    /* 0x1A4 */ f32 mDistToFloor;
    /* 0x1A8 */ f32 _1A8;
    /* 0x1AC */ f32 mDistanceToWaterSurface;
    /* 0x1B0 */ u16 mWallStickTimer;
    /* 0x1B2 */ u8 _1B2;
    /* 0x1B4 */ f32 mWaterDistanceTarget;
    /* 0x1B8 */ f32 _1B8;
};
