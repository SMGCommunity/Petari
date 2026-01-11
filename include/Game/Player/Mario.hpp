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
class Triangle;
class FloorCode;
struct SoundList;
class MarioState;
class MarioModuleTask;

class Mario : public MarioModule {
public:
    typedef bool (Mario::*Task)(const void*, void*, u32);
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
    void beeMarioOnAir();

    void blown(const TVec3f&);

    bool taskOnEffectCheck(u32);

    void doFlow();

    void tryStartFoo();

    bool isRising() const;
    bool checkWallRiseAndSlipFront();
    bool tryJump();
    bool tryTurnJump();
    bool trySquatJump();
    bool tryBackJump();
    bool tryTornadoJump();
    void startTornadoCentering(HitSensor*);
    void taskOnTornadoCentering(u32);
    bool trySpinJump(u8);
    bool tryForceJumpDelay(const TVec3f&);
    bool tryFreeJumpDelay(const TVec3f&);
    bool tryForceJump(const TVec3f&, bool);
    bool tryForceFreeJump(const TVec3f&);
    bool tryForcePowerJump(const TVec3f&, bool);
    bool tryFreeJump(const TVec3f&, bool);
    bool tryWallJump(const TVec3f&, bool);
    bool tryStickJump(const TVec3f&);
    bool trySlipUpJump();
    bool tryHangSlipUp();
    bool tryDrop();
    bool isDigitalJump() const;
    void initActiveJumpVec();
    void initJumpParam();
    bool isEnableFutureJump() const;
    bool procJump(bool);
    void checkWallRising();
    void checkWallJumpHit();
    void decideSlipUp();
    void moveWallSlide(f32);
    void jumpToHipDrop();
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
    void taskOnSlide(u32);
    void taskOnWallRising(u32);
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

    bool checkBeeStick();
    bool tryBeeStick(const HitSensor*);

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
    void checkWallCode(const char*, bool) const;
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

    bool isSliderFloor() const;
    void checkSliderMode() const;
    void startSlider();

    void checkStep();
    void startStep(const TVec3f&);

    void checkBump();
    void startBump(const TVec3f&);

    void checkEnforceMove();
    void checkEnforceMoveInner();
    void recordRelativePosition();
    void invalidateRelativePosition();
    MtxPtr getMoveBaseMtx() const;
    void moveRelativePosition(u32);
    void moveRelativePositionWall();
    void recordLastGround();
    void getLastGroundPos(TVec3f*) const;
    void getLastGroundEdgeNrm(u32) const;
    void getLastGroundEdgeIndex(const TVec3f&, const TVec3f&) const;
    void pushedByReaction();
    void addReaction(TVec3f&);
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
    void pushTask(Task, u32);
    void popTask(Task);
    void callExtraTasks(u32);
    void startHandy();
    void taskOnHipDropBlurHopper(u32);
    void taskOnHipDropBlur(u32);
    void taskOnHipDropSlide(u32);
    void taskOnFreezeEnd(u32);
    void startFreezeEnd();
    void taskOnHandy(u32);
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

    struct MovementStates {
        unsigned jumping : 1;  // _0
        unsigned _1 : 1;
        unsigned _2 : 1;
        unsigned turning : 1;  // _3
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
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned debugMode : 1;  // _16
        unsigned _17 : 1;
        unsigned _18 : 1;
        unsigned _19 : 1;
        unsigned _1A : 1;
        unsigned _1B : 1;
        unsigned _1C : 1;
        unsigned _1D : 1;
        unsigned digitalJump : 1;  // _1E
        unsigned _1F : 1;
        unsigned _20 : 1;
        unsigned _21 : 1;
        unsigned _22 : 1;
        unsigned _23 : 1;
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
        unsigned _38 : 1;
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
        unsigned _12 : 1;
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

    inline const MovementStates& getMovementStates() const { return mMovementStates; }
    inline const DrawStates& getDrawStates() const { return mDrawStates; }
    inline const DrawStates& getPrevDrawStates() const { return mPrevDrawStates; }

    union {
        MovementStates mMovementStates;  // 0x8
        struct {
            u32 mMovementStates_LOW_WORD;   // 0x8
            u32 mMovementStates_HIGH_WORD;  // 0xC
        };
    };

    union {
        MovementStates _10;
        struct {
            u32 _10_LOW_WORD;   // 0x10
            u32 _10_HIGH_WORD;  // 0x14
        };
    };

    union {
        DrawStates mDrawStates;  // 0x18
        u32 mDrawStates_WORD;
    };
    union {
        DrawStates _1C;
        u32 _1C_WORD;
    };
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;

    DrawStates mPrevDrawStates;  // 0x30
    // FAKE
    u32 _34;
    // NOT FAKE

    f32 _38;
    f32 _3C;
    TVec3f _40;
    TVec3f _4C;
    TVec3f _58;
    TMtx34f _64;
    TMtx34f _94;
    TMtx34f _C4;
    TMtx34f _F4;
    f32 _124;
    f32 _128;
    u32 _12C;
    TVec3f mPosition;  // 0x130
    TVec3f _13C;
    TVec3f _148;
    TVec3f _154;
    TVec3f mVelocity;  // 0x160
    TVec3f _16C;
    TVec3f mVelocityAfter;  // 0x178
    TVec3f _184;
    TVec3f _190;
    TVec3f _19C;
    TVec3f _1A8;
    TVec3f _1B4;
    TVec3f mStickPos;       // 0x1C0
    TVec3f mWorldPadDir;    // 0x1CC
    TVec3f mAirGravityVec;  // 0x1D8
    TVec3f _1E4;
    TVec3f mHeadVec;  // 0x1F0
    TVec3f _1FC;
    TVec3f mFrontVec;  // 0x208
    TVec3f _214;
    TVec3f _220;
    TVec3f _22C;
    TVec3f _238;
    TVec3f _244;
    TVec3f _250;
    TVec3f _25C;
    TVec3f _268;
    u8 _274;
    f32 _278;
    f32 _27C;
    f32 _280;
    TVec3f _284;
    TVec3f _290;
    TVec3f _29C;
    TVec3f _2A8;
    f32 _2B4;
    TVec3f _2B8;
    TVec3f _2C4;
    f32 _2D0;
    TVec3f mJumpVec;  // 0x2D4
    TVec3f _2E0;
    TVec3f _2EC;
    TVec3f _2F8;
    TVec3f _304;
    TVec3f mSideVec;    // 0x310
    TVec3f mShadowPos;  // 0x31C
    TVec3f _328;
    TVec3f _334;
    f32 _340;
    TVec3f _344;  // left/right?
    TVec3f _350;
    TVec3f _35C;
    TVec3f _368;
    TVec3f _374;
    TVec3f _380;
    TVec3f _38C;
    TVec3f _398;
    TVec3f _3A4;
    TVec3f _3B0;
    u16 _3BC;
    u16 _3BE;
    u16 _3C0;
    u16 _3C2;
    u16 _3C4;
    u16 _3C6;
    u16 _3C8;
    u16 _3CA;
    u16 _3CC;
    u16 _3CE;  // a timer
    u16 _3D0;
    u16 _3D2;
    u16 _3D4;
    TVec3f _3D8;
    TVec3f _3E4;
    f32 _3F0;
    f32 _3F4;
    u16 _3F8;
    u16 _3FA;
    u16 _3FC;
    u16 _3FE;
    u16 _400;
    u16 _402;
    u16 _404;
    u16 _406;
    u16 _408;
    u16 _40A;
    u16 _40C;
    u16 _40E;
    u16 _410;
    u16 _412;
    u16 _414;              // a timer
    u16 mMorphResetTimer;  // 0x416
    u16 _418;
    u16 _41A;
    u16 _41C;
    u16 _41E;
    u16 _420;
    u16 _422;
    u16 _424;
    u16 _426;
    u16 _428;
    u16 _42A;
    u16 _42C;
    u32 _430;
    u16 _434;  // a timer
    u16 _436;
    u16 _438;
    u16 _43A;
    TVec3f _43C;  // front?
    TVec3f _448;
    f32 _454;
    TriangleFilterDelegator< Mario >* _458;
    Triangle* _45C;
    Triangle* _460;
    Triangle* mGroundPolygon;  // 0x464
    Triangle* _468;
    Triangle* _46C;
    Triangle* _470;
    Triangle* _474;
    Triangle* mTmpPolygon;  // 0x478
    Triangle* _47C;
    Triangle* _480;
    Triangle* _484;
    f32 _488;
    TVec3f mGroundPos;  // 0x48C
    TVec3f _498;
    TVec3f _4A4;
    TVec3f _4B0;
    TVec3f _4BC;
    Triangle* _4C8;
    Triangle* mFrontWallTriangle;  // 0x4CC
    Triangle* mBackWallTriangle;   // 0x4D0
    Triangle* mSideWallTriangle;   // 0x4D4
    Triangle* _4D8;
    Triangle* _4DC;
    f32 _4E0;
    f32 _4E4;
    TVec3f _4E8;
    TVec3f _4F4;

    // FAKE
    TVec3f _500;
    // NOT FAKE

    TVec3f _50C;
    TVec3f _518;
    f32 _524;
    f32 _528;
    f32 _52C;
    f32 _530;
    u16 _534;
    f32 _538;
    f32 _53C;
    f32 mYAngleOffset;  // 0x540
    u16 _544;
    f32 _548;
    TVec3f _54C;
    u32 _558;

    // FAKE
    u32 _55C;
    u32 _560;
    // NOT FAKE

    s32 _564;
    u32 _568;
    u32 _56C;
    u8 _570;
    u32 _574;
    u32 _578;
    Triangle* _57C[0x20];
    u32 _5FC;
    TVec3f _600;
    u8 _60C;
    u8 _60D;  // bool?
    u8 _60E;
    u8 _60F;
    u8 _610;
    u8 _611;
    f32 _614;
    f32 _618;
    f32 _61C;
    f32 _620;
    u8 _624;
    TVec3f _628;
    f32 _634;

    u8 _638;
    TVec3f _63C;
    TVec3f _648;
    TVec3f _654;
    TVec3f _660;

    u8 _66C;
    TVec3f _670;
    TVec3f _67C;

    TVec3f _688;
    TVec3f _694;
    TVec3f _6A0;
    u8 _6AC;
    u8 _6AD;
    TVec3f _6B0;
    TVec3f _6BC;
    f32 _6C8;
    f32 _6CC;
    TVec3f _6D0;
    TVec3f _6DC;
    TVec3f _6E8;
    TVec3f _6F4;
    TVec3f _700;
    TVec3f _70C;
    f32 _718;
    u8 _71C;
    u8 _71D;
    u8 _71E;
    u8 _71F;
    const char* _720;
    const char* _724;
    const char* _728;
    f32 _72C;
    u32 _730;
    u8 _734;
    u8 _735;
    f32 _738;
    f32 _73C;
    f32 _740;
    f32 _744;
    u8 _748;
    f32 _74C;
    u32 _750;
    u32 _754;
    MarioWall* mWall;  // 0x758
    TVec3f _75C;
    MarioStick* mStick;  // 0x768
    u16 _76C;
    f32 _770;
    u16 _774;
    MarioRabbit* mRabbit;      // 0x778
    MarioFoo* mFoo;            // 0x77C
    MarioSukekiyo* mSukekiyo;  // 0x780
    MarioBury* mBury;          // 0x784
    MarioWait* mWait;          // 0x788
    MarioClimb* mClimb;        // 0x78C
    TVec3f _790;
    MarioHang* mHang;            // 0x79C
    MarioRecovery* mRecovery;    // 0x7A0
    MarioWarp* mWarp;            // 0x7A4
    MarioFlip* mFlip;            // 0x7A8
    MarioSideStep* mSideStep;    // 0x7AC
    MarioFrontStep* mFrontStep;  // 0x7B0
    MarioSkate* mSkate;          // 0x7B4
    MarioTalk* mTalk;            // 0x7B8
    MarioTeresa* mTeresa;        // 0x7BC
    MarioDamage* mDamage;        // 0x7C0
    TVec3f _7C4;
    u16 _7D0;
    TVec3f _7D4;
    Triangle* _7E0;
    TMtx34f _7E4;
    TVec3f _814;
    Triangle* _820;
    TMtx34f _824;
    MarioFlow* mFlow;                // 0x854
    MarioFireDamage* mFireDamage;    // 0x858
    MarioFireDance* mFireDance;      // 0x85C
    MarioFireRun* mFireRun;          // 0x860
    MarioParalyze* mParalyze;        // 0x864
    MarioStun* mStun;                // 0x868
    MarioCrush* mCrush;              // 0x86C
    MarioFreeze* mFreeze;            // 0x870
    MarioAbyssDamage* mAbyssDamage;  // 0x874
    MarioDarkDamage* mDarkDamage;    // 0x878
    MarioFaint* mFaint;              // 0x87C
    MarioBlown* mBlown;              // 0x880
    MarioSwim* mSwim;                // 0x884
    MarioSlider* mSlider;            // 0x888
    MarioStep* mStep;                // 0x88c
    MarioBump* mBump;                // 0x890
    MarioMagic* mMagic;              // 0x894
    u8 _898;
    MarioFpView* mFpView;  // 0x89C
    MarioMove* mMove;      // 0x8A0
    TVec3f _8A4;
    TVec3f _8B0;
    TVec3f _8BC;
    Triangle* _8C8;
    Triangle* _8CC;
    Triangle* _8D0;
    u32 _8D4;
    u32 _8D8;
    TVec3f _8DC;
    Triangle* _8E8;
    u8 _8EC;

    // FAKE?
    f32 _8F0;
    // NOT FAKE

    f32 _8F4;

    // FAKE
    TVec3f _8F8;
    TVec3f _904;
    // NOT FAKE

    TVec3f _910;
    TVec3f _91C;
    TVec3f _928;
    f32 _934;
    TVec3f _938;
    TVec3f _944;
    TVec3f _950;
    FloorCode* _95C;
    u16 _960;
    u16 _962;
    u16 _964[3];
    u16 _96A;

    HashSortTable* _96C;  // Sounds
    const char* _970;     // Sounds or somthing
    u32 _974;

    // Fake
    u32 _978;
    // Not fake

    MarioState* _97C;
    MarioState* _980;

    Task _984[0xb];
    u32 _A08[11];
    u16 _A34;
    u32 _A38;
    u16 _A3C;
    TVec3f _A40;
    TVec3f _A4C;
    TVec3f _A58;
    f32 _A64;
    u32 _A68;
    u8 _A6C[0x20];  // animations
    TVec3f _A8C[9];
};
