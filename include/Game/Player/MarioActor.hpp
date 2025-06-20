#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Player/Mario.hpp"

class FootPrint;
class JAIAudible;
class MarioNullBck;
class XjointTransform;
class MarioParts;
class RushEndInfo;
class MarioConst;
class MarioEffect;
class MarioAnimator;
class MarioMessenger;
class CollisionShadow;
class DLchanger;
class DLholder;
class J3DModelX;
class TornadoMario;
class ModelHolder;
class SPECIAL_STATUS_FOR_CAMERA;

bool gIsLuigi;    // (cc68 - 10000)(r13)

class MarioActor : public LiveActor {
public:
    MarioActor(const char *);

    ~MarioActor();

    virtual const TVec3f &getLastMove() const;
    virtual void getLastMove(TVec3f *) const;
    virtual void getFrontVec(TVec3f *) const;

    void init(const JMapInfoIter &);
    void init2(const TVec3f &, const TVec3f &, s32);
    void initAfterPlacement();
    void initAfterOpeningDemo();
    void calcBaseFrontVec(const TVec3f &);
    void playSound(const char *, s32);
    void changeAnimation(const char *, const char *);
    void changeAnimationNonStop(const char *);
    void changeAnimationUpper(const char *);
    void stopAnimation(const char *);
    bool isAnimationRun(const char *) const;
    void changeNullAnimation(const char *, s8);
    void clearNullAnimation(s8);
    bool isStopNullAnimation() const;
    void changeGameOverAnimation();
    XjointTransform *getJointCtrl(const char *) const;
    void updateRotationInfo();
    void exeWait();
    void movement();
    void control();
    void control2();
    void controlMain();
    void updateBehavior();
    void updateBindRatio();
    bool isInPunchTimerRange() const;
    void updatePunching();
    bool doRush();
    void updateSwingTimer();
    void updateSwingAction();
    bool checkClapCatchStart();
    void updateRealMtx();
    void setUpperRotateY(f32);
    void decLife(u16);
    void decLifeLarge();
    void decLifeMiddle();
    void decLifeSmall();
    void resetWaterLife();
    void updateLife();
    void incLife(u32);
    void changeMaxLife(s32);
    bool doPressing();
    void careMovingObject();
    bool doStun();
    void scaleMtx(MtxPtr);
    void updateBaseScaleMtx();
    void getRealMtx(MtxPtr, const char *) const NO_INLINE;
    void getRealPos(const char *, TVec3f *) const;
    void getGlobalJointMtx(const char *);
    void calcAnimInMovement();
    void forceSetBaseMtx(MtxPtr);
    void calcAnim();
    void calcAndSetBaseMtx();
    void setBlendMtxTimer(u16);
    void getGroundPos(TVec3f *dst) const;
    TVec3f* getShadowPos() const;
    bool isTurning() const;
    bool isJumping() const;
    bool isJumpRising() const;
    bool isPunching() const;
    bool isItemSwinging() const;
    bool isDamaging() const;
    bool isNeedDamageFog() const;
    bool isStaggering() const;
    bool isSleeping() const;
    bool isRefuseTalk() const;
    bool isDebugMode() const;
    void jumpHop();
    void calcCenterPos();
    void calcHeadPos();
    void setPress(u8, s32);
    XanimeResourceTable* getResourceTable() const;
    bool isEnableMoveMario() const;
    bool isEnableNerveChange() const;
    void forceGameOver();
    void forceGameOverAbyss();
    void forceGameOverBlackHole();
    void forceGameOverNonStop();
    void forceGameOverSink();
    void updateCameraInfo();
    bool binderFilter(const Triangle *);
    void setPunchHitTimer(u8);

    void initDrawAndModel();
    void initBeeMario();
    void initHopperMario();
    void initIceMario();
    void initInvincibleMario();
    void initTeresaMario();
    void initTornadoMario();
    void initBoneMario();
    void changeDisplayMode(u8);
    void calcViewAndEntry();
    void drawMarioModel() const;
    bool isAllHidden() const;
    void swapTextureInit();
    void initFace();
    void updateFace();
    void initBlur();
    void getJ3DModel() const;
    void getModelData() const;
    void getSimpleModel() const;
    void calcViewMainModel();
    void draw() const;
    void drawIndirect();
    void drawIndirectModel();
    void drawReflectModel();
    void drawModelBlur();
    void createTextureDL(DLholder *, u16, u16);
    void swapTexture(const char *, u8) const;
    void copyMaterial(J3DModel *, u16, s32);

    bool isEnableSpinPunch();
    void reactionPunch(HitSensor *);
    bool sendPunch(HitSensor *, bool);
    void doFreezeAttack(HitSensor *);
    void doFreezeAttack();
    bool trySpinPunch();
    void printHitMark(HitSensor *);
    bool tryPunchAirAfter(HitSensor *);
    void sendBodyAttack(HitSensor *);

    void getMapBaseMtx() const;
    bool isFastRise() const;
    bool isLongDrop() const;
    bool isFastDrop() const;
    void getSpecialMode() const;
    bool isCameraStateOn(SPECIAL_STATUS_FOR_CAMERA) const;
    TVec3f& getSideVec(TVec3f *) const;
    TVec3f& getUpVec(TVec3f *) const;
    void getTransForCamera() const;
    void updateTransForCamera();
    void updateForCamera();
    void bodyClap();
    void forceKill(u32);
    void exeGameOver();
    void exeGameOverAbyss();
    void exeGameOverFire();
    void exeGameOverSink();
    void exeGameOverNonStop();
    void exeTimeWait();

    TVec3f &getGravityVec() const;
    TVec3f &getGravityVector() const;
    TVec3f &getAirGravityVec() const;
    TVec3f &getGravityVector(TVec3f *) const;
    void getGravityRatio() const;
    void getGravityInfo() const;
    void getGravityLevel() const;
    bool checkBeeWallStick(TVec3f &);
    bool checkBeeFloorStick(TVec3f &);
    void syncJumpBeeStickMode();
    void updateBeeModeGravity(TVec3f &);
    bool isInZeroGravitySpot() const;
    void updateGravityVec(bool, bool);
    bool checkBeeCeilStick(TVec3f &);
    void updateBeeModeStick(TVec3f &);

    void initHand();
    void updateHand();
    void changeHandMaterial();
    void updateHandAtMorph();
    void drawHand() const;
    void drawIndirectHand() const;

    void initMember();
    
    void resetPadSwing();

    bool isRequestHipDrop() const;
    bool isRequestRush() const;
    bool isRequestThrow() const;
    bool isRequestSpin() const;
    bool isRequestSpinJump2P() const;
    bool isRequestJump() const;
    bool isRequestJump2P() const;
    bool isKeepJump() const;
    bool isKeepJump2P() const;
    bool checkButtonType(u16, bool) const;
    void getStickValue(f32 *, f32 *);
    void updateActionTrigger();
    void updateControllerSwing();
    void updateControllerSwingLeft();
    void lockOnDPD();

    void init2D();
    void initParts();
    void updateBeeWingAnimation();
    void updateTornado();
    void updateTakingPosition();
    const HitSensor &getCarrySensor() const;
    void changeSpecialModeAnimation(const char *);
    void updateSpecialModeAnimation();
    void initFireBall();
    void shootFireBall();
    void showFreezeModel();
    void hideFreezeModel();
    void updateFairyStar();
    void update2D();
    void updateThrowVector();
    void createIceFloor(const TVec3f &);
    void createIceFloor(const TVec3f &, const TVec3f &);
    void createIceWall(const TVec3f &, const TVec3f &);
    void updateBaseMtxTeresa(MtxPtr);
    void finalizeFreezeModel();
    void offTakingFlag();

    void settingRush();
    bool isFixJumpRushSensor(const HitSensor *) const;
    bool isLandEffectRushSensor(const HitSensor *) const;
    void resetCondition();
    void beginRush();
    void endRush(const RushEndInfo *);
    void takeSensor(HitSensor *);

    void setupSensors();
    void updateHitSensor(HitSensor *);
    void doTrampleJump(HitSensor *);
    void trampleJump(f32, f32);
    void attackSensor(HitSensor *, HitSensor *);
    void sendMsgToSensor(HitSensor *, u32);
    void resetSensorCount();
    void recordScoutingObject(HitSensor *);
    void updateScouter();
    void initScouter();
    void initForJump();

    void initShadow();
    void calcViewReflectionModel();
    void calcViewShadowModel();
    void drawShadow() const;
    void decideShadowMode();
    void calcViewSilhouetteModel();
    void calcCylinderToCenter(const AreaObj *, TVec3f *);
    void calcViewBlurModel();
    void calcViewFootPrint();
    void drawSilhouette() const;

    void initScreenBox();
    bool isUseScreenBox() const;
    void calcScreenBoxRange();
    void captureScreenBox() const;
    void writeBackScreenBox() const;
    void calc1stPersonView();
    void showBeeFur();
    void hideBeeFur();
    void calcFogLighting();
    void resetFog();
    void updateAlphaDL(u8);
    void updateSimpleAlphaDL(u8);
    void updateReflectAlphaDL(u8);
    void updateLightDL(const Color8 &, const Color8 &, const Color8 &, f32);
    void createRainbowDL();
    void drawScreenBlend() const;
    void updateRandomTexture(f32);
    void drawWallShade(const TVec3f &, const TVec3f &, f32) const;
    void drawSpinInhibit() const;
    void drawColdWaterDamage() const;
    void setRasterScroll(s32, s32, s32);
    void drawMosaic() const;
    void drawLifeUp() const;
    void calcSpinEffect();
    void drawSpinEffect() const;
    void drawSphereMask() const;
    void initDarkMask();
    void updateDarkMask(u16);
    bool drawDarkMask() const;
    void getBlurOffset() const;

    void initActionMatrix();
    bool isActionOk(const char *) const;
    u8 selectAction(const char *) const;
    void selectAutoBind(const char *, u8 *) const;
    void selectCustomEffectSpinHitSound(const char *) const;
    void selectSpinCatchInRush(const char *) const;
    void selectHomingInSuperHipDrop(const char *) const;
    void selectNotHomingSensor(const char *) const;
    void selectInvalidMovingCollision(const char *) const;
    void selectQuickResetBeeWallGravity(const char *) const;
    void selectJumpRushSensor(const char *) const;
    void selectDamageFireColor(const char *) const;
    void selectWaterInOutEffect(const char *) const;
    void selectOnDamageRush(const char *) const;
    void selectTiltPress(const HitSensor *) const;
    void selectHandyRush(const HitSensor *) const;
    bool selectRecoverFlyMeter(const HitSensor *) const;
    void selectHideFlyMeter(const HitSensor *) const;
    void selectTeresaThru(const HitSensor *) const;
    void selectRebindTimer(const HitSensor *) const;
    void selectHipDropRush(const HitSensor *) const;
    void selectPushOff(const HitSensor *) const;
    bool selectWaterInOutRush(const HitSensor *) const;
    void selectLandEffect(const HitSensor *) const;
    void selectNoShadow(const HitSensor *) const;
    void selectDamagePop(const HitSensor *) const;
    void selectGetFaceLookHeight(const HitSensor *) const;

    void setPlayerMode(u16, bool);
    void resetPlayerModeOnDamage();
    void resetPlayerModeOnNoDamage();
    void updatePlayerMode();
    void touchSensor(HitSensor *);
    void initMorphStringTable();
    void changeMorphString(const char *) const;

    void initBlink();
    void setBlink(const char *);
    void updateBlink();

    void attackOrPushSensor(HitSensor *, f32);
    void attackOrPushSensorInDamage(HitSensor *, f32);
    void attackOrPushSensorInRush(HitSensor *, f32);
    bool tryAddClapCoin(HitSensor *);
    bool tryTornadoAttack(HitSensor *);
    bool isUnderTarget(HitSensor *);
    bool tryHipDropAttack(HitSensor *);
    bool checkAndTryTrampleAttack(HitSensor *, f32, bool);
    bool tryTrampleAttack(HitSensor *);
    bool cylinderHorizontalCheck(HitSensor *);
    bool tryJetAttack(HitSensor *);
    bool tryCounterJetAttack(HitSensor *);
    bool tryGetItem(HitSensor *);
    bool cylinderPushCheck(const TVec3f &, f32, f32, f32);
    void attackOrPushPolygons();
    void sendWallTouch(HitSensor *, HitSensor *);
    void sendMsgUpperPunch(HitSensor *);

    bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    bool tryAttackMsg(u32, const HitSensor *, bool *);
    bool receiveMsgPush(HitSensor *, HitSensor *);
    bool receiveOtherMsg(u32, HitSensor *, HitSensor *);
    bool tryVectorAttackMsg(u32, const TVec3f &);
    u32 receiveMsgTaken(HitSensor *, HitSensor *);
    bool cylinderHitCheck(const HitSensor *, TVec3f *);
    void hitWall(const TVec3f &, HitSensor *);
    void passRing(const HitSensor *);

    void addRushSensor(HitSensor *, bool);
    bool tryStandardRush();
    void checkPriorRushTarget();
    void getNearestRushTarget(bool) const;
    void getNearestJumpTarget() const;
    void getPriorityHighTarget() const;
    void getRescueTarget() const;
    bool tryStartRush(bool);
    bool tryJumpRush();
    bool tryRushInRush();

    void memorizeSensorThrow(HitSensor *);
    bool tryThrow();
    bool tryReleaseDirect(const HitSensor *);
    void damageDropThrowMemoSensor();
    void rushDropThrowMemoSensor();
    bool trySetLockOnTarget(HitSensor *);
    bool tryCoinPull();
    void tryCoinPullInRush();
    void flushCoinPull();
    void tryCoinPullOne(HitSensor *);
    void tryPullTrans(TVec3f *, const TVec3f &);
    void releaseThrowMemoSensor();
    bool tryReleaseWithMsg(u32);
    bool tryTornadoPull(HitSensor *);
    bool tryReleaseBombTeresa();

    void initBlackHoleOut();
    void exeGameOverBlackHole2();

    void entryWallWalkMode(const TVec3f &, const TVec3f &);

    void initMaterialEffect();
    void getFloorMaterialIndex(u32) const;
    void playMaterialEffect(const char *);
    void stopMaterialEffect(const char *);
    void initCommonEffect();
    void playCommonEffect(const char *);
    void initEffect();
    void emitEffectWaterColumn(const TVec3f &, const TVec3f &);
    bool isCommonEffect(const char *) const;
    bool isMaterialEffect(const char *) const;
    void playEffect(const char *);
    void playEffectTrans(const char *, const TVec3f &);
    void playEffectRT(const char *, const TVec3f &, const TVec3f &);
    void playEffectRTZ(const char *, const TVec3f &, const TVec3f &);
    void playEffectRTW(const char *, const TVec3f &, const TVec3f &);
    void playEffectSRT(const char *, f32, const TVec3f &, const TVec3f &);
    void stopCommonEffect(const char *);
    void stopEffect(const char *);
    void stopEffectForce(const char *);
    void updateEffect();
    bool checkEffectWaterColumn();
    void stopSpinTicoEffect(bool);

    void initThrowing();
    void initSearchLight();
    void updateThrowing();
    void updateSearchLight();
    void calcViewSearchLight();
    void drawSearchLight() const;
    void getThrowVec(TVec3f *) const;

    void initTeresaMarioAnimation();
    void runTeresaBaseAnimation();
    void changeTeresaAnimation(const char *, s32);
    void updateTeresaAnimation();

    const MarioConst &getConst() const { return *mConst; }

    inline u32 getHealth() const { return mHealth; }

    inline const Mario::MovementStates &getMovementStates() const { return mMario->mMovementStates; }

    inline const Mario::DrawStates &getDrawStates() const { return mMario->mDrawStates; }

    inline const Mario::DrawStates &getPrevDrawStates() const { return mMario->mPrevDrawStates; }

    struct FBO {
        u32 _0;
        u32 _4;
    };

    u8 _8C;
    DLchanger *mDLchanger;    // 0x90
    u32 _94[0x40];
    u8 *mDL[2];     // 0x194
    u32 mDLSize;    // 0x19C
    u8 mCurrDL;     // 0x1A0
    u8 _1A1;
    f32 _1A4;
    u16 _1A8;
    u16 _1AA;
    f32 _1AC;
    Color8 _1B0;
    u8 _1B4;
    bool _1B5;
    u32 _1B8;
    MarioMessenger *_1BC;
    bool _1C0;
    bool _1C1;
    bool _1C2;
    bool _1C3;
    u16 _1C4;
    u16 _1C6;
    f32 _1C8;
    f32 _1CC;
    u8 _1D0;
    u8 _1D1;
    f32 _1D4;
    FBO *_1D8;
    FBO *_1DC;
    bool _1E0;
    u8 _1E1;
    u8 _1E2;
    // padding
    f32 _1E4;
    u16 _1E8;
    // padding
    f32 _1EC;
    TVec3f _1F0;
    TVec3f _1FC;
    f32 _208;
    u32 _20C;
    u8 _210;
    u8 _211;
    // padding
    CollisionShadow *_214;
    u32 _218;
    u32 _21C;
    u32 _220;
    u32 _224;
    u32 _228;
    u32 _22C;
    Mario *mMario;                // 0x230
    MarioAnimator *mMarioAnim;    // 0x234
    MarioEffect *mMarioEffect;    // 0x238
    MarioConst *mConst;           // 0x23C
    TVec3f _240;
    TVec3f _24C;
    TVec3f _258;
    TVec3f _264;
    TVec3f _270;
    TVec3f _27C;
    TVec3f _288;
    TVec3f _294;
    TVec3f _2A0;
    TVec3f _2AC;
    TVec3f _2B8;
    TVec3f _2C4;
    TVec3f _2D0;
    TVec3f _2DC;
    TVec3f _2E8;
    TVec3f _2F4;
    TVec3f _300;
    TVec3f _30C;
    TVec3f _318;
    TVec3f _324;
    u16 _330;
    u16 _332;
    u16 _334;
    u16 _336;
    u16 _338;
    // padding
    TVec3f _33C;
    TVec3f _348;
    TVec3f _354;
    TVec3f _360;
    GravityInfo *_36C;
    u8 _370;
    // padding
    f32 _374;
    u16 _378;
    // padding
    u32 _37C;
    u32 mHealth;    // 0x380
    u32 mWaterLife; // 0x384
    u32 _388;
    u16 _38C;
    u32 _390;
    u32 _394;
    u32 _398;
    u8 _39C;
    u8 _39D;
    u8 _39E;
    u32 _3A0;
    u32 _3A4;
    u16 _3A8;
    u16 _3AA;
    u16 _3AC;
    // padding
    f32 _3B0;
    TVec3f _3B4;
    bool _3C0;
    bool _3C1;
    // padding
    TVec3f _3C4;
    u16 _3D0;
    u16 _3D2;
    u16 mPlayerMode;          // 0x3D4
    u16 _3D6;
    u16 _3D8;
    u16 _3DA;
    u16 _3DC;
    u8 _3DE;
    u8 _3DF;
    u32 mMaxHealth;    // 0x3E0
    bool _3E4;
    bool _3E5;
    bool _3E6;
    bool _3E7;
    bool _3E8;
    // padding
    TMtx34f _3EC;
    u32 _41C;
    u32 _420;
    u32 _424;
    u32 _428[4];
    u8 _438[0x30];
    TVec3f _468;
    u32 _474;
    f32 _478;
    u32 _47C;
    u8 _480;
    u8 _481;
    bool _482;
    u8 _483;
    TVec3f _484;
    f32 _490;
    u32 _494;
    FixedPosition *_498;
    FixedPosition *_49C;
    u32 _4A0;
    u32 _4A4;
    u32 _4A8;
    f32 _4AC;
    f32 _4B0;
    f32 _4B4;
    TVec3f _4B8;
    TVec3f _4C4;
    u32 _4D0[0x80];
    // padding
    u8 _6D0;
    f32 _6D4;
    f32 _6D8;
    HitSensor* _6DC[0x40];
    u16 _7DC;
    u16 _7DE;
    u16 _7E0;
    u8 _7E2;
    u32 _7E4[0x40];
    u8 _8E4[0x40];
    HitSensor *_924;
    u32 _928;
    u32 _92C;
    u32 _930;
    bool _934;
    bool _935;
    TVec3f _938;
    u8 _944;
    u8 _945;
    u16 _946;
    u16 _948;
    u16 _94A;
    u16 _94C;
    u16 _94E;
    u8 _950;
    u8 _951;
    u32 _954[8];
    u8 _974;
    TVec3f _978;
    f32 _984;
    u8 _988;
    u8 _989;
    TornadoMario *mTornadoMario;    // 0x98C
    u8 _990;
    u32 _994;
    u32 _998;
    u32 _99C;
    u32 _9A0;
    MarioParts *_9A4;
    f32 _9A8;
    f32 _9AC;
    f32 _9B0;
    u16 _9B4;
    u32 _9B8;
    u32 _9BC;
    u32 _9C0;
    u32 _9C4;
    u32 _9C8;
    f32 _9CC;
    f32 _9D0;
    u32 _9D4;
    TVec3f _9D8;
    u32 _9E4;
    u32 _9E8;
    u32 _9EC;
    bool _9F0;
    bool mAlphaEnable;    // 0x9F1
    u16 _9F2;
    TVec3f _9F4;
    u32 _A00;
    u32 _A04;
    u8 _A08;
    u8 _A09;
    u8 mCurrModel;    // 0xA0A
    u8 _A0B;
    u8 _A0C;
    u32 _A10;
    u32 _A14;
    TVec3f _A18;
    u8 _A24;
    u8 _A25;
    // padding
    J3DModelX *mModels[6];    // 0xA28
    u32 _A40;
    u32 _A44;
    u32 _A48;
    u32 _A4C;
    u32 _A50;
    u32 _A54;
    u8 _A58;
    u8 _A59;
    u8 _A5A;
    u8 _A5B;
    ModelHolder *_A5C;
    bool _A60;
    bool _A61;
    bool _A62;
    // padding
    u32 _A64;
    f32 _A68;
    u16 _A6C;
    bool _A6E;
    // padding
    u32 _A70[8];
    u32 _A90[8];
    TMtx34f _AB0;
    TMtx34f _AE0;
    u16 _B10;
    u16 _B12;
    u16 _B14;
    // padding
    TVec3f _B18;
    f32 _B24;
    f32 _B28;
    f32 _B2C;
    f32 _B30;
    f32 _B34;
    f32 _B38;
    f32 _B3C;
    f32 _B40;
    u32 _B44;
    FootPrint *_B48;
    u32 _B4C;
    u16 _B50;
    // padding
    u32 _B54[3];
    u16 _B60;
    // padding
    u32 _B64;
    u8 _B68;
    // padding
    u16 _B6A;
    u32 _B6C;
    u16 _B70;
    u8 _B72;
    // padding
    u16 _B74;
    // padding
    u32 _B78;
    u32 _B7C;
    u32 _B80;
    u32 _B84;
    u16 _B88;
    MarioNullBck *mNullAnimation;    // 0xB8C
    bool _B90;  // animations
    bool _B91;
    s8 _B92;
    // padding
    u16 _B94;
    u8 _B96;

    u32 _B98;

    u16 _B9C;
    u16 _B9E;
    u32 _BA0;
    u32 _BA4;
    u32 _BA8;
    TVec3f _BAC;
    TVec3f _BB8;
    u16 _BC4;
    TMtx34f _BC8;
    TMtx34f _BF8;
    void *_C28;
    TMtx34f _C2C;
    TMtx34f _C5C;
    TMtx34f _C8C;
    TMtx34f _CBC;
    TMtx34f _CEC;
    TMtx34f _D1C;
    TMtx34f _D4C;
    TMtx34f _D7C;
    TMtx34f _DAC;
    TMtx34f _DDC;
    TMtx34f _E0C;
    TMtx34f _E3C;
    TMtx34f _E6C;
    u16 mBlendMtxTimer;            // 0xE9C
    f32 _EA0;
    bool _EA4;
    bool _EA5;
    bool _EA6;
    TMtx34f _EA8;
    TVec3f _ED8;
    u32 _EE4;
    bool mSuperKinokoCollected;    // 0xEE8
    bool mPowerupCollected;        // 0xEE9
    bool mTransforming;            // 0xEEA
    bool _EEB;
    bool _EEC;
    bool _EED;
    bool _EEE;
    // padding
    u16 _EF0;
    u16 _EF2;
    u16 _EF4;
    u16 _EF6;
    u32 _EF8;
    u32 _EFC;
    u8 _F00;
    // padding
    u32 _F04;
    u32 _F08;
    bool _F0C;
    u8 _F0D;
    u16 _F0E;
    u16 _F10;
    u16 _F12;
    u16 _F14;
    u16 _F16;
    u16 _F18;
    u16 _F1A;
    u16 _F1C;
    u16 _F1E;
    u8 _F20;
    u8 _F21;
    // padding
    u32 _F24;
    u16 _F28;
    // padding
    TVec3f _F2C;
    u8 _F38;
    // padding
    union {
        JAIAudible *_F3C;
        TVec3f *_F3C_vec;
    };
    u16 _F40;
    u16 _F42;
    bool _F44;
    // padding
    u32 _F48;
    u32 _F4C;
    TVec3f _F50;
    TVec3f _F5C;
    TVec3f _F68;
    u8 _F74;
    // padding
    TVec3f mCamPos;              // 0xF78
    TVec3f mCamDirX;             // 0xF84
    TVec3f mCamDirY;             // 0xF90
    TVec3f mCamDirZ;             // 0xF9C
    TVec3f _FA8;
    const Nerve *_FB4;
    u16 _FB8; // a timer
    u32 _FBC;
    u32 _FC0;
    u32 _FC4;
    u32 _FC8;
    bool _FCC;
    bool _FCD;
};

namespace NrvMarioActor {
    NERVE(MarioActorNrvWait);
    NERVE(MarioActorNrvGameOver);
    NERVE(MarioActorNrvGameOverAbyss);
    NERVE(MarioActorNrvGameOverAbyss2);
    NERVE(MarioActorNrvGameOverFire);
    NERVE(MarioActorNrvGameOverBlackHole);
    NERVE(MarioActorNrvGameOverSink);
    NERVE(MarioActorNrvTimeWait);
    NERVE(MarioActorNrvNoRush);
}    // namespace NrvMarioActor
