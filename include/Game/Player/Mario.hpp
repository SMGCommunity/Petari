#pragma once

#include "Game/Player/MarioModule.hpp"
#include "Game/Util/TriangleFilter.hpp"

class MarioActor;
class MarioFlow;
class MarioWall;
class MarioDamage;
class MarioFaint;
class MarioBlown;
class MarioHang;
class MarioSwim;
class MarioSlider;
class MarioFireDamage;
class MarioFireRun;
class MarioFireDance;
class MarioAbyssDamage;
class MarioDarkDamage;
class MarioStep;
class MarioBump;
class MarioParalyze;
class MarioStun;
class MarioCrush;
class MarioFreeze;
class MarioMagic;
class MarioFpView;
class MarioRecovery;
class MarioFlip;
class MarioSideStep;
class MarioFrontStep;
class MarioStick;
class MarioRabbit;
class MarioSukekiyo;
class MarioBury;
class MarioWait;
class MarioClimb;
class MarioSkate;
class MarioFoo;
class MarioWarp;
class MarioTeresa;
class MarioTalk;
class MarioMove;
class HitInfo;
class HitSensor;
class Triangle;
class FloorCode;
struct SoundList;
class MarioState;
class MarioModuleTask;

class Mario : public MarioModule {
public:
    typedef bool (Mario::*Task)(u32);
    Mario(MarioActor*);

    virtual bool postureCtrl(MtxPtr);

    void initMember();

    void updateAndClearStrideParameter();
    void checkKeyLock();
    void updateTimers();
    void updateMorphResetTimer();
    void doExtraServices();
    bool isEnableCheckGround() NO_INLINE;
    void setGroundNorm(const TVec3f&);
    bool checkForceGrounding();
    void updateGroundInfo();
    void fixHeadFrontVecByGravity();
    bool isNonFixHeadVec() const;
    void createMtxDir(MtxPtr, const TVec3f&, const TVec3f&, const TVec3f&);
    void createDirectionMtx(MtxPtr);
    void createCorrectionMtx(MtxPtr, TVec3f*);
    void createAngleMtx(MtxPtr, bool);
    void slopeTiltHead(TVec3f*);
    void fixFrontVecByGravity();
    void fixFrontVecFromUpSide();
    void fixSideVecFromFrontUp();
    void setSideVec(const TVec3f&);
    void setHeadVec(const TVec3f&);
    void setFrontVec(const TVec3f&);
    void setFrontVecKeepUp(const TVec3f&, f32);
    void setFrontVecKeepUp(const TVec3f&, u32);
    void setFrontVecKeepUp(const TVec3f&);
    void setFrontVecKeepUpAngle(const TVec3f&, f32);
    void setFrontVecKeepSide(const TVec3f&);
    void setHeadAndFrontVecFromRotate(const TVec3f&);
    void forceSetHeadVecKeepSide(const TVec3f&);
    void setGravityVec(const TVec3f&);
    void draw() const;
    void addTrans(const TVec3f&, const char*);
    void setTrans(const TVec3f&, const char*);
    bool isEnableRush() const;
    bool isForceStopRush() const;
    bool isInvincible() const;
    bool isCeiling() const;
    void inputStick();
    void updateSoundCode();
    const TVec3f& getShadowNorm() const;
    const TVec3f& getAirGravityVec() const;
    const TVec3f& getAirFrontVec() const;
    const TVec3f* getGravityVec() const;
    void initAfterConst();
    void writeBackPhysicalVector();
    void update();
    void updateLookOfs();
    void actionMain();
    void touchWater();

    bool isBlendWaitGround() const;
    bool checkSpecialWaitAnimation();
    void resetSleepTimer();

    void connectToClimb();

    bool isIgnoreTriangle(const Triangle*);
    void checkBaseTransBall();
    void createAtField(bool, f32);
    void doSwimmingHitCheck(const HitInfo*, u32);
    void doSpinPunchAroundPolygons();
    void checkMap();
    f32 calcDistToCeil(bool);
    f32 calcDistToCeilOnPress();
    f32 calcDistToCeilHead();
    void fixTransBetweenWall(const TVec3f&, const TVec3f&);
    f32 calcDistWidth();
    void updateCameraPolygon();
    void setCameraPolygon(const Triangle*);
    void checkAllWall(const TVec3f&, f32);
    void calcFrontFloor();
    const TVec3f& getWallNorm() const;
    const TVec3f& getSideWallNorm() const;
    const TVec3f& getFrontWallNorm() const;
    const TVec3f& getBackWallNorm() const;
    const TVec3f& getWallPos() const;
    const Triangle* getWallPolygon() const;
    const Triangle* getGroundPolygon() const;
    void updateFloorCode();
    void updateWallFloorCode();
    void saveLastSafetyTrans();
    void setNotSafetyTimer();
    TVec3f* getLastSafetyTrans(TVec3f*) const;
    bool checkCurrentFloorCodeSevere(u32) const;
    bool isCurrentFloorSink() const;
    bool isCurrentFloorSand() const;
    bool isCurrentShadowFloorDangerAction() const;
    bool checkBaseTransPoint();
    bool checkHeadPoint();
    const TVec3f* calcShadowPos();
    void updateBinderInfo();
    bool isThroughWall(const Triangle*) const;
    void checkGround();
    void getCameraCubeCode() const;
    void updateCubeCode();

    bool isDamaging() const;
    void damageLarge(const TVec3f&);
    void decDamageAfterTimer();
    bool checkDamage();
    u16 getDamageAfterTimer() const;
    void damageFloorCheck();
    void damageWallCheck();
    void damagePolygonCheck(const Triangle*);
    void flipLarge(const TVec3f&);
    bool isEnableAddDamage() const;
    void damage(const TVec3f&);
    void doAbyssDamage();
    void connectToFireRun();
    void doFireDanceWithInitialDamage(u8);
    void doFireObjHitWithInitialDamage();
    void doNeedleWithInitialDamage(u8);
    void doNeedleWithInitialDamage(const Triangle*);
    void doNeedle(const Triangle*);
    void doFireDance();
    void checkKarikariDamage();
    void doDarkDamage();
    void doParalyze();
    void doFreeze();
    void requestCrush();
    void tryCrush();

    void doFlipWeak(const TVec3f&);
    void faint(const TVec3f&);

    void doFlipJump(const TVec3f&);
    void doFlipBackRoll(const TVec3f&);

    void doFrontStep();

    void beeMarioOnGround();
    bool beeMarioOnAir();

    void blown(const TVec3f&);

    void taskOnEffectCheck(u32);

    void doFlow();

    void tryStartFoo();

    bool isRising() const;
    void checkWallRiseAndSlipFront();
    void tryJump();
    void tryTurnJump();
    void trySquatJump();
    void tryBackJump();
    void tryTornadoJump();
    void startTornadoCentering(HitSensor*);
    bool taskOnTornadoCentering(u32);
    void trySpinJump(u8);
    void tryForceJumpDelay(const TVec3f&);
    void tryFreeJumpDelay(const TVec3f&);
    void tryForceJump(const TVec3f&, bool);
    void tryForceFreeJump(const TVec3f&);
    void tryForcePowerJump(const TVec3f&, bool);
    void tryFreeJump(const TVec3f&, bool);
    void tryWallJump(const TVec3f&, bool);
    void tryStickJump(const TVec3f&);
    void trySlipUpJump();
    void tryHangSlipUp();
    void tryDrop();
    bool isDigitalJump() const NO_INLINE;
    void initActiveJumpVec();
    void initJumpParam();
    bool isEnableFutureJump() const;
    void procJump(bool);
    void checkWallRising() NO_INLINE;
    void checkWallJumpHit();
    void decideSlipUp();
    void moveWallSlide(f32);
    bool jumpToHipDrop();
    void procHipDrop();
    void doAirWalk();
    void stopJump();
    void cancelTornadoJump();
    void setRocketBooster(const TVec3f&, f32, u16);
    void procRocketBooster();
    bool isSoftLandingFloor() const;
    void checkAndTryForceJump();
    void doLanding();
    void startSlidingTask(u32, f32, u16);
    bool taskOnSlide(u32);
    bool taskOnWallRising(u32);
    void incAirWalkTimer();

    void mainMove();
    bool isEnableTurn();
    void recordTurnSlipAngle();
    void decideInertia(f32);
    void decideInertiaOnIce(f32);
    void decideInertiaOnSlip(f32);
    void calcShadowDir(const TVec3f&, TVec3f*);
    void retainMoveDir(f32, f32, TVec3f*);
    void calcMoveDir(f32, f32, TVec3f*, bool);
    void checkLockOnHoming();
    void doLockOnHoming();
    void fixPositionInTower();

    void check2DMode();
    void calcMoveDir2D(f32, f32, TVec3f*);
    void calcShadowDir2D(const TVec3f&, TVec3f*);
    void stick2Dadjust(f32&, f32&);
    void set2DMode(bool);
    void beforeJumping2D();
    void afterLanding2D();
    void clear2DStick();

    void set25DMode(const AreaObj*);
    void update25DMode();
    void updateAxisFromMode(u8);
    void calcMoveDir25D(f32, f32, TVec3f*);

    bool checkPressDamage();
    bool checkVerticalPress(bool);
    bool checkSidePressPre();
    bool checkSidePress();

    void startRabbitMode();
    void endRabbitMode();

    bool checkWallJumpCode();
    void doSideStep();

    bool isSkatableFloor() const;
    void doSkate();

    u32 initSoundTable(SoundList* list, u32);
    void initSound();
    bool playSoundJ(const char*, s32);
    void stopSoundJ(const char*, u32);
    void startBas(const char*, bool, f32, f32);
    bool isRunningBas(const char*) const;
    void skipBas(f32);
    void playSoundTeresaFlying();
    void playSoundTrampleCombo(u8);
    void setSeVersion(u32);

    bool checkOnimasu(const HitSensor*);
    bool isDossun(const Triangle*) const;
    bool isStageCameraRotate2D() const;
    bool isNoWalkFallOnDossun() const;
    bool isNotReflectGlassGround() const;
    bool isUseAnotherMovingPolygon() const;
    bool isUseFoolSpecialGravity(const TVec3f&, TVec3f*) const;
    void updateOnimasu();
    bool isHeadPushEnableArea() const;
    bool isOnimasuBinderPressSkip() const;

    void checkTornado();
    void resetTornado();
    void calcTornadoTilt();
    void reflectWallOnSpinning(const TVec3f&, u16);
    void forceStopTornado();
    void startRotationTask(u32);
    void doSpinWallEffect();
    void taskOnRotation(u32);

    void sendStateMsg(u32);
    bool updatePosture(MtxPtr);
    void changeStatus(MarioState*);
    void closeStatus(MarioState*);
    u32 getCurrentStatus() const;
    bool isStatusActive(u32) const;

    void checkBeeStick();
    void tryBeeStick(const HitSensor*);

    void startTalk(const LiveActor*);
    void endTalk();
    void setLastNonFixMoveVec(const TVec3f&);

    bool isWalling() const;
    void checkWallStick();
    void checkStickWallSide();
    s32 checkStickFrontBack();
    void fixWallingPosition(bool);
    void fixWallingDir();
    void fixWallingTop();
    void checkWallFloorCode(u16) const;
    bool checkWallCode(const char*, bool) const;
    void checkWallCodeNorm(u16, TVec3f*, bool) const;
    void setWallCancel();
    void keepDistFrontWall();
    bool isEnableStickWall();
    void fixWallingDist();
    bool isInhibitWall() const;
    bool tryWallPunch();

    bool isHanging() const;
    void fixHangDir(const TVec3f&, TVec3f*);
    bool isEnableBackHang();
    bool isEnableSideHang();
    void checkHang();
    bool isEnableHang();

    bool isSwimming() const;
    bool forceStartSwimAndShoot(const TVec3f&);
    bool forceExitSwim();
    bool checkStartSwim();
    void startSwim();

    bool checkWaterDamage();

    void clearSlope();
    bool isEnableSlopeMove() const;
    void moveSlopeSlide();
    void slopeMove();
    void taskOnSlipTurn(u32);

    void lockGroundCheck(void*, bool);
    void unlockGroundCheck(void*);
    bool isUseSimpleGroundCheck() const;
    bool checkGroundOnSlope();

    void stopWalk();
    void cancelSquatMode();
    f32 getTargetWalkSpeed() const;
    void decideSquatWalkAnimation();
    void decideWalkSpeed();
    void decideWalkAnimation();
    void doBrakingAnimation();
    bool checkWallPush();
    void updateBrakeAnimation();
    void updateWalkSpeed();
    void decideOnIceAnimation();
    void updateOnSand();
    void updateOnPoison();
    void updateOnWater();

    void doRecovery();

    bool isSliderFloor() const NO_INLINE;
    bool checkSliderMode() const;
    void startSlider();

    void checkStep();
    void startStep(const TVec3f&);

    void checkBump();
    void startBump(const TVec3f&);

    void checkEnforceMove();
    void checkEnforceMoveInner();
    void recordRelativePosition();
    void invalidateRelativePosition();
    TPos3f* getMoveBaseMtx() const;
    u32 moveRelativePosition(u32);
    u32 moveRelativePositionWall();
    void recordLastGround();
    void getLastGroundPos(TVec3f*) const;
    const TVec3f* getLastGroundEdgeNrm(u32) const;
    void getLastGroundEdgeIndex(const TVec3f&, const TVec3f&) const;
    void pushedByReaction();
    void addReaction(const TVec3f&);
    bool tryPushToVelocity();
    void push(const TVec3f&);
    void push2(const TVec3f&);
    void powerAreaMove();
    void powerRailMove();
    void recordJumpEnforceMove();
    void doEnforceJump(f32);
    void pushedByWind();

    void delTask(MarioModuleTask*);
    void execTask();
    void drawTask() const;
    void initTask();
    bool isActiveTask(Task);
    bool isActiveTaskID(u32);
    bool pushTask(Task, u32);
    void popTask(Task);
    void callExtraTasks(u32);
    void startHandy();
    bool taskOnHipDropBlurHopper(u32);
    bool taskOnHipDropBlur(u32);
    bool taskOnHipDropSlide(u32);
    bool taskOnFreezeEnd(u32);
    void startFreezeEnd();
    bool taskOnHandy(u32);
    void startHipDropBlur();
    void startHipDropSlide(const HitSensor*);
    void startJumpDropSlide(const HitSensor*);

    void startTeresaMode();
    void getHitWallNorm(TVec3f*);
    void resetTeresaMode();
    void doTeresaReflection(const TVec3f&, bool);
    void startTeresaDisappear();

    void stopPunch();
    void startMagic();

    void doObjWarp(LiveActor*);
    void doPointWarp(const TVec3f&, const TVec3f&, s32);
    bool isVisibleRecoveryWarpBubble() const;
    void doCubeWarp();
    void doPointWarpRecovery(const TVec3f&, const TVec3f&);

    bool isDisableStayHere() const;
    bool isDisableFpViewMode() const;
    void tryFpViewMode();

    void stick2DadjustGround(f32&, f32&);
    void calcDir2D(f32, f32, TVec3f*);

    // instruction comments to make it easier to identify each bit
    struct MovementStates {
        /* 0x00 */
        unsigned jumping : 1;  // _0 (srwi rX, rX, 31)
        unsigned _1 : 1;       // extrwi rX, rX, 1, 1
        unsigned _2 : 1;
        unsigned turning : 1;  // _3
        unsigned _4 : 1;
        unsigned _5 : 1;
        unsigned _6 : 1;
        unsigned _7 : 1;
        unsigned _8 : 1;
        unsigned _9 : 1;
        unsigned _A : 1;  // extrwi rX, rX, 1, 10
        unsigned _B : 1;
        unsigned _C : 1;
        unsigned _D : 1;
        unsigned _E : 1;
        unsigned _F : 1;
        unsigned _10 : 1;  // rlwinm rX, rX, 0, 17, 15
        unsigned _11 : 1;
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned debugMode : 1;  // _16
        unsigned _17 : 1;
        unsigned _18 : 1;  // rlwinm rX, rX, 0, 25, 23
        unsigned _19 : 1;  // extrwi rX, rX, 1, 25
        unsigned _1A : 1;  // extrwi rX, rX, 1, 26
        unsigned _1B : 1;
        unsigned _1C : 1;
        unsigned _1D : 1;
        unsigned digitalJump : 1;  // _1E
        unsigned _1F : 1;
        /* 0x04 */
        unsigned _20 : 1;
        unsigned _21 : 1;
        unsigned _22 : 1;
        unsigned _23 : 1;  // extrwi rX, rX, 1, 3
        unsigned _24 : 1;
        unsigned _25 : 1;
        unsigned _26 : 1;
        unsigned _27 : 1;
        unsigned _28 : 1;
        unsigned _29 : 1;
        unsigned _2A : 1;
        unsigned _2B : 1;
        unsigned _2C : 1;
        unsigned _2D : 1;
        unsigned _2E : 1;
        unsigned _2F : 1;
        unsigned _30 : 1;
        unsigned _31 : 1;
        unsigned _32 : 1;
        unsigned _33 : 1;
        unsigned _34 : 1;
        unsigned _35 : 1;
        unsigned _36 : 1;
        unsigned _37 : 1;
        unsigned _38 : 1;  // rlwimi rX, rX, 7, 24, 24
        unsigned _39 : 1;
        unsigned _3A : 1;
        unsigned _3B : 1;
        unsigned _3C : 1;
        unsigned _3D : 1;
        unsigned _3E : 2;
    };
    struct DrawStates {
        unsigned _0 : 1;
        unsigned _1 : 1;
        unsigned _2 : 1;
        unsigned _3 : 1;
        unsigned _4 : 1;
        unsigned _5 : 1;
        unsigned _6 : 1;
        unsigned _7 : 1;
        unsigned _8 : 1;
        unsigned _9 : 1;
        unsigned _A : 1;
        unsigned _B : 1;
        unsigned _C : 1;
        unsigned _D : 1;
        unsigned _E : 1;
        unsigned _F : 1;
        unsigned _10 : 1;
        unsigned _11 : 1;
        unsigned mIsUnderwater : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned _16 : 1;
        unsigned _17 : 1;
        unsigned _18 : 1;
        unsigned _19 : 1;
        unsigned _1A : 1;
        unsigned _1B : 1;
        unsigned _1C : 1;
        unsigned _1D : 1;
        unsigned _1E : 1;
        unsigned _1F : 1;
    };

    inline const MovementStates& getMovementStates() const {
        return mMovementStates;
    }
    inline const DrawStates& getDrawStates() const {
        return mDrawStates;
    }
    inline const DrawStates& getPrevDrawStates() const {
        return mPrevDrawStates;
    }

    union {
        /* 0x008 */ MovementStates mMovementStates;
        struct {
            /* 0x008 */ u32 mMovementStates_LOW_WORD;
            /* 0x00C */ u32 mMovementStates_HIGH_WORD;
        };
    };

    union {
        /* 0x010 */ MovementStates _10;
        struct {
            /* 0x010 */ u32 _10_LOW_WORD;
            /* 0x014 */ u32 _10_HIGH_WORD;
        };
    };

    union {
        /* 0x018 */ DrawStates mDrawStates;
        /* 0x018 */ u32 mDrawStates_WORD;
    };
    union {
        /* 0x01C */ DrawStates _1C;
        /* 0x01C */ u32 _1C_WORD;
    };
    /* 0x020 */ u32 _20;
    /* 0x024 */ u32 _24;
    /* 0x028 */ u32 _28;
    /* 0x02C */ u32 _2C;

    /* 0x030 */ DrawStates mPrevDrawStates;
    // FAKE
    /* 0x034 */ u32 _34;
    // NOT FAKE

    /* 0x038 */ f32 _38;
    /* 0x03C */ f32 _3C;
    /* 0x040 */ TVec3f _40;
    /* 0x04C */ TVec3f _4C;
    /* 0x058 */ TVec3f _58;
    /* 0x064 */ TMtx34f _64;
    /* 0x094 */ TMtx34f _94;
    /* 0x0C4 */ TMtx34f _C4;
    /* 0x0F4 */ TMtx34f _F4;
    /* 0x124 */ f32 _124;
    /* 0x128 */ f32 _128;
    /* 0x12C */ u32 _12C;
    /* 0x130 */ TVec3f mPosition;
    /* 0x13C */ TVec3f _13C;
    /* 0x148 */ TVec3f _148;
    /* 0x154 */ TVec3f _154;
    /* 0x160 */ TVec3f mVelocity;
    /* 0x16C */ TVec3f _16C;
    /* 0x178 */ TVec3f mVelocityAfter;
    /* 0x184 */ TVec3f _184;
    /* 0x190 */ TVec3f _190;
    /* 0x19C */ TVec3f _19C;
    /* 0x1A8 */ TVec3f _1A8;
    /* 0x1B4 */ TVec3f _1B4;
    /* 0x1C0 */ TVec3f mStickPos;
    /* 0x1CC */ TVec3f mWorldPadDir;
    /* 0x1D8 */ TVec3f mAirGravityVec;
    /* 0x1E4 */ TVec3f _1E4;
    /* 0x1F0 */ TVec3f mHeadVec;
    /* 0x1FC */ TVec3f _1FC;
    /* 0x208 */ TVec3f mFrontVec;
    /* 0x214 */ TVec3f _214;
    /* 0x220 */ TVec3f _220;
    /* 0x22C */ TVec3f _22C;
    /* 0x238 */ TVec3f _238;
    /* 0x244 */ TVec3f _244;
    /* 0x250 */ TVec3f _250;
    /* 0x25C */ TVec3f _25C;
    /* 0x268 */ TVec3f _268;
    /* 0x274 */ u8 _274;
    /* 0x278 */ f32 _278;
    /* 0x27C */ f32 _27C;
    /* 0x280 */ f32 _280;
    /* 0x284 */ TVec3f _284;
    /* 0x290 */ TVec3f _290;
    /* 0x29C */ TVec3f _29C;
    /* 0x2A8 */ TVec3f _2A8;
    /* 0x2B4 */ f32 _2B4;
    /* 0x2B8 */ TVec3f _2B8;
    /* 0x2C4 */ TVec3f _2C4;
    /* 0x2D0 */ f32 _2D0;
    /* 0x2D4 */ TVec3f mJumpVec;
    /* 0x2E0 */ TVec3f _2E0;
    /* 0x2EC */ TVec3f _2EC;
    /* 0x2F8 */ TVec3f _2F8;
    /* 0x304 */ TVec3f _304;
    /* 0x310 */ TVec3f mSideVec;
    /* 0x31C */ TVec3f mShadowPos;
    /* 0x328 */ TVec3f _328;
    /* 0x334 */ TVec3f _334;
    /* 0x340 */ f32 _340;
    /* 0x344 */ TVec3f _344;  // left/right?
    /* 0x350 */ TVec3f _350;
    /* 0x35C */ TVec3f _35C;
    /* 0x368 */ TVec3f _368;
    /* 0x374 */ TVec3f _374;
    /* 0x380 */ TVec3f _380;
    /* 0x38C */ TVec3f _38C;
    /* 0x398 */ TVec3f _398;
    /* 0x3A4 */ TVec3f _3A4;
    /* 0x3B0 */ TVec3f _3B0;
    /* 0x3BC */ u16 _3BC;
    /* 0x3BE */ u16 _3BE;
    /* 0x3C0 */ u16 _3C0;
    /* 0x3C2 */ u16 _3C2;
    /* 0x3C4 */ u16 _3C4;
    /* 0x3C6 */ u16 _3C6;
    /* 0x3C8 */ u16 _3C8;
    /* 0x3CA */ u16 _3CA;
    /* 0x3CC */ u16 _3CC;
    /* 0x3CE */ u16 _3CE;  // a timer
    /* 0x3D0 */ u16 _3D0;
    /* 0x3D2 */ u16 _3D2;
    /* 0x3D4 */ u16 _3D4;
    /* 0x3D8 */ TVec3f _3D8;
    /* 0x3E4 */ TVec3f _3E4;
    /* 0x3F0 */ f32 _3F0;
    /* 0x3F4 */ f32 _3F4;
    /* 0x3F8 */ u16 _3F8;
    /* 0x3FA */ u16 _3FA;
    /* 0x3FC */ u16 _3FC;
    /* 0x3FE */ u16 _3FE;
    /* 0x400 */ u16 _400;
    /* 0x402 */ u16 _402;
    /* 0x404 */ u16 _404;
    /* 0x406 */ u16 _406;
    /* 0x408 */ u16 _408;
    /* 0x40A */ u16 _40A;
    /* 0x40C */ u16 _40C;
    /* 0x40E */ u16 _40E;
    /* 0x410 */ u16 _410;
    /* 0x412 */ u16 _412;
    /* 0x414 */ u16 _414;  // a timer
    /* 0x416 */ u16 mMorphResetTimer;
    /* 0x418 */ u16 _418;
    /* 0x41A */ u16 _41A;
    /* 0x41C */ u16 _41C;
    /* 0x41E */ u16 _41E;
    /* 0x420 */ u16 _420;
    /* 0x422 */ u16 _422;
    /* 0x424 */ u16 _424;
    /* 0x426 */ u16 _426;
    /* 0x428 */ u16 _428;
    /* 0x42A */ u16 _42A;
    /* 0x42C */ u16 _42C;
    /* 0x430 */ u32 _430;
    /* 0x434 */ u16 _434;  // a timer
    /* 0x436 */ u16 _436;
    /* 0x438 */ u16 _438;
    /* 0x43A */ u16 _43A;
    /* 0x43C */ TVec3f _43C;  // front?
    /* 0x448 */ TVec3f _448;
    /* 0x454 */ f32 _454;
    /* 0x458 */ TriangleFilterDelegator<Mario>* _458;
    /* 0x45C */ Triangle* _45C;
    /* 0x460 */ Triangle* _460;
    /* 0x464 */ Triangle* mGroundPolygon;
    /* 0x468 */ Triangle* _468;
    /* 0x46C */ Triangle* _46C;
    /* 0x470 */ Triangle* _470;
    /* 0x474 */ Triangle* _474;
    /* 0x478 */ Triangle* mTmpPolygon;
    /* 0x47C */ Triangle* _47C;
    /* 0x480 */ Triangle* _480;
    /* 0x484 */ Triangle* _484;
    /* 0x488 */ f32 mVerticalSpeed;
    /* 0x48C */ TVec3f mGroundPos;
    /* 0x498 */ TVec3f _498;
    /* 0x4A4 */ TVec3f _4A4;
    /* 0x4B0 */ TVec3f _4B0;
    /* 0x4BC */ TVec3f _4BC;
    /* 0x4C8 */ Triangle* _4C8;
    /* 0x4CC */ Triangle* mFrontWallTriangle;
    /* 0x4D0 */ Triangle* mBackWallTriangle;
    /* 0x4D4 */ Triangle* mSideWallTriangle;
    /* 0x4D8 */ Triangle* _4D8;
    /* 0x4DC */ Triangle* _4DC;
    /* 0x4E0 */ f32 _4E0;
    /* 0x4E4 */ f32 _4E4;
    /* 0x4E8 */ TVec3f _4E8;
    /* 0x4F4 */ TVec3f _4F4;

    // FAKE
    /* 0x500 */ TVec3f _500;
    // NOT FAKE

    /* 0x50C */ TVec3f _50C;
    /* 0x518 */ TVec3f _518;
    /* 0x524 */ f32 _524;
    /* 0x528 */ f32 _528;
    /* 0x52C */ f32 _52C;
    /* 0x530 */ f32 _530;
    /* 0x534 */ u16 _534;
    /* 0x538 */ f32 _538;
    /* 0x53C */ f32 _53C;
    /* 0x540 */ f32 mYAngleOffset;
    /* 0x544 */ u16 _544;
    /* 0x548 */ f32 _548;
    /* 0x54C */ TVec3f _54C;
    /* 0x558 */ u32 _558;

    /// FAKE
    /* 0x55C */ u32 _55C;
    /* 0x560 */ u32 _560;
    // NOT FAKE

    /* 0x564 */ s32 _564;
    /* 0x568 */ u32 _568;
    /* 0x56C */ u32 _56C;
    /* 0x570 */ u8 _570;
    /* 0x574 */ u32 _574;
    /* 0x578 */ u32 _578;
    /* 0x57C */ Triangle* _57C[0x20];
    /* 0x5FC */ u32 _5FC;
    /* 0x600 */ TVec3f _600;
    /* 0x60C */ u8 _60C;
    /* 0x60D */ u8 _60D;  // bool?
    /* 0x60E */ u8 _60E;
    /* 0x60F */ u8 _60F;
    /* 0x610 */ u8 _610;
    /* 0x611 */ u8 _611;
    /* 0x614 */ f32 _614;
    /* 0x618 */ f32 _618;
    /* 0x61C */ f32 _61C;
    /* 0x620 */ f32 _620;
    /* 0x624 */ u8 _624;
    /* 0x628 */ TVec3f _628;
    /* 0x634 */ f32 _634;

    /* 0x638 */ u8 _638;
    /* 0x63C */ TVec3f _63C;
    /* 0x648 */ TVec3f _648;
    /* 0x654 */ TVec3f _654;
    /* 0x660 */ TVec3f _660;

    /* 0x66C */ u8 _66C;
    /* 0x670 */ TVec3f _670;
    /* 0x67C */ TVec3f _67C;

    /* 0x688 */ TVec3f _688;
    /* 0x694 */ TVec3f _694;
    /* 0x6A0 */ TVec3f _6A0;
    /* 0x6AC */ u8 _6AC;
    /* 0x6AD */ u8 _6AD;
    /* 0x6B0 */ TVec3f _6B0;
    /* 0x6BC */ TVec3f _6BC;
    /* 0x6C8 */ f32 _6C8;
    /* 0x6CC */ f32 _6CC;
    /* 0x6D0 */ TVec3f _6D0;
    /* 0x6DC */ TVec3f _6DC;
    /* 0x6E8 */ TVec3f _6E8;
    /* 0x6F4 */ TVec3f _6F4;
    /* 0x700 */ TVec3f _700;
    /* 0x70C */ TVec3f _70C;
    /* 0x718 */ f32 _718;
    /* 0x71C */ u8 _71C;
    /* 0x71D */ u8 _71D;
    /* 0x71E */ u8 _71E;
    /* 0x71F */ u8 _71F;
    /* 0x720 */ const char* _720;
    /* 0x724 */ const char* _724;
    /* 0x728 */ const char* _728;
    /* 0x72C */ f32 _72C;
    /* 0x730 */ u32 _730;
    /* 0x734 */ u8 _734;
    /* 0x735 */ u8 _735;
    /* 0x738 */ f32 _738;
    /* 0x73C */ TVec3f _73C;
    /* 0x748 */ u8 _748;
    /* 0x74C */ f32 _74C;
    /* 0x750 */ u32 _750;
    /* 0x754 */ u32 _754;
    /* 0x758 */ MarioWall* mWall;
    /* 0x75C */ TVec3f _75C;
    /* 0x768 */ MarioStick* mStick;
    /* 0x76C */ u16 _76C;
    /* 0x770 */ f32 _770;
    /* 0x774 */ u16 _774;
    /* 0x778 */ MarioRabbit* mRabbit;
    /* 0x77C */ MarioFoo* mFoo;
    /* 0x780 */ MarioSukekiyo* mSukekiyo;
    /* 0x784 */ MarioBury* mBury;
    /* 0x788 */ MarioWait* mWait;
    /* 0x78C */ MarioClimb* mClimb;
    /* 0x790 */ TVec3f _790;
    /* 0x79C */ MarioHang* mHang;
    /* 0x7A0 */ MarioRecovery* mRecovery;
    /* 0x7A4 */ MarioWarp* mWarp;
    /* 0x7A8 */ MarioFlip* mFlip;
    /* 0x7AC */ MarioSideStep* mSideStep;
    /* 0x7B0 */ MarioFrontStep* mFrontStep;
    /* 0x7B4 */ MarioSkate* mSkate;
    /* 0x7B8 */ MarioTalk* mTalk;
    /* 0x7BC */ MarioTeresa* mTeresa;
    /* 0x7C0 */ MarioDamage* mDamage;
    /* 0x7C4 */ TVec3f _7C4;
    /* 0x7D0 */ u16 _7D0;
    /* 0x7D4 */ TVec3f _7D4;
    /* 0x7E0 */ Triangle* _7E0;
    /* 0x7E4 */ TMtx34f _7E4;
    /* 0x814 */ TVec3f _814;
    /* 0x820 */ Triangle* _820;
    /* 0x824 */ TMtx34f _824;
    /* 0x854 */ MarioFlow* mFlow;
    /* 0x858 */ MarioFireDamage* mFireDamage;
    /* 0x85C */ MarioFireDance* mFireDance;
    /* 0x860 */ MarioFireRun* mFireRun;
    /* 0x864 */ MarioParalyze* mParalyze;
    /* 0x868 */ MarioStun* mStun;
    /* 0x86C */ MarioCrush* mCrush;
    /* 0x870 */ MarioFreeze* mFreeze;
    /* 0x874 */ MarioAbyssDamage* mAbyssDamage;
    /* 0x878 */ MarioDarkDamage* mDarkDamage;
    /* 0x87C */ MarioFaint* mFaint;
    /* 0x880 */ MarioBlown* mBlown;
    /* 0x884 */ MarioSwim* mSwim;
    /* 0x888 */ MarioSlider* mSlider;
    /* 0x88C */ MarioStep* mStep;
    /* 0x890 */ MarioBump* mBump;
    /* 0x894 */ MarioMagic* mMagic;
    /* 0x898 */ u8 _898;
    /* 0x89C */ MarioFpView* mFpView;
    /* 0x8A0 */ MarioMove* mMove;
    /* 0x8A4 */ TVec3f _8A4;
    /* 0x8B0 */ TVec3f _8B0;
    /* 0x8BC */ TVec3f _8BC;
    /* 0x8C8 */ Triangle* _8C8;
    /* 0x8CC */ Triangle* _8CC[2];
    /* 0x8D4 */ u32 _8D4;
    /* 0x8D8 */ u32 _8D8;
    /* 0x8DC */ TVec3f _8DC;
    /* 0x8E8 */ Triangle* _8E8;
    /* 0x8EC */ u8 _8EC;

    // FAKE?
    /* 0x8F0 */ f32 _8F0;
    // NOT FAKE

    /* 0x8F4 */ f32 _8F4;

    // FAKE
    /* 0x8F8 */ TVec3f _8F8;
    /* 0x904 */ TVec3f _904;
    // NOT FAKE

    /* 0x910 */ TVec3f _910;
    /* 0x91C */ TVec3f _91C;
    /* 0x928 */ TVec3f _928;
    /* 0x934 */ f32 _934;
    /* 0x938 */ TVec3f _938;
    /* 0x944 */ TVec3f _944;
    /* 0x950 */ TVec3f _950;
    /* 0x95C */ FloorCode* _95C;
    /* 0x960 */ u16 _960;
    /* 0x962 */ u16 _962;
    /* 0x964 */ u16 _964[3];
    /* 0x96A */ u16 _96A;

    /* 0x96C */ HashSortTable* _96C;  // Sounds
    /* 0x970 */ const char* _970;     // Sounds or somthing
    /* 0x974 */ MarioModuleTask* _974;

    // Fake
    /* 0x978 */ u32 _978;
    // Not fake

    /* 0x97C */
    MarioState* _97C;
    /* 0x980 */ MarioState* _980;

    /* 0x984 */ Task _984[0xb];
    /* 0xA08 */ u32 _A08[11];
    /* 0xA34 */ u16 _A34;
    /* 0xA38 */ u32 _A38;
    /* 0xA3C */ u16 _A3C;
    /* 0xA40 */ TVec3f _A40;
    /* 0xA4C */ TVec3f _A4C;
    /* 0xA58 */ TVec3f _A58;
    /* 0xA64 */ f32 _A64;
    /* 0xA68 */ HitSensor* _A68;
    /* 0xA6C */ u8 _A6C[0x20];  // animations
    /* 0xA8C */ TVec3f _A8C[9];
};
