#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Util/Color.hpp"

class BlackHole;
class CollisionShadow;
class DLchanger;
class DrawAdaptor;
class FixedPosition;
class FootPrint;
class HashSortTable;
class IceStep;
class J3DAnmTexPattern;
class J3DModel;
class J3DModelData;
class J3DModelX;
class JAIAudible;
class JUTTexture;
class JetTurtleShadow;
class MarioAnimator;
class MarioConst;
class MarioEffect;
class MarioMessenger;
class MarioNullBck;
class MarioParts;
class ModelHolder;
class ModelObj;
class MultiEmitter;
class RushEndInfo;
class TornadoMario;
class Triangle;
class XanimeResourceTable;
class XjointTransform;
struct DLholder;
struct ResTIMG;
struct SmokeEffectEntry;

template < int SIZE, class T, class U >
class AudGenericAudible;
class JAUDopplerAudibleChannel;
class JAUDopplerAudibleAbsPos;
typedef AudGenericAudible< 4, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos > AudGeneric;

extern bool gIsLuigi;  // (cc68 - 10000)(r13)

class MarioActor : public LiveActor {
public:
    MarioActor(const char*);

    virtual ~MarioActor();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgTaken(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual const TVec3f& getLastMove() const;
    virtual void getLastMove(TVec3f*) const;
    virtual void getFrontVec(TVec3f*) const;

    void init2(const TVec3f&, const TVec3f&, s32);
    void initAfterOpeningDemo();
    void calcBaseFrontVec(const TVec3f&);
    void playSound(const char*, s32);
    void changeAnimation(const char*, const char*);
    void changeAnimationNonStop(const char*);
    void changeAnimationUpper(const char*);
    const char* changeMorphString(const char*) const;
    void stopAnimation(const char*);
    bool isAnimationRun(const char*) const;
    void changeNullAnimation(const char*, s8);
    void clearNullAnimation(s8);
    void changeSpecialModeAnimation(const char*);
    bool isStopNullAnimation() const;
    void changeGameOverAnimation();
    XjointTransform* getJointCtrl(const char*) const;
    void updateRotationInfo();
    void exeWait();
    void exeGameOver();
    void exeGameOverAbyss();
    void exeGameOverFire();
    // void exeGameOverBlackHole();
    void exeGameOverNonStop();
    void exeGameOverSink();
    void exeTimeWait();
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
    void getRealMtx(MtxPtr, const char*) const NO_INLINE;
    void getRealPos(const char*, TVec3f*) const;
    MtxPtr getGlobalJointMtx(const char*);
    void calcAnimInMovement();
    void forceSetBaseMtx(MtxPtr);
    void setBlendMtxTimer(u16);
    void getGroundPos(TVec3f* dst) const;
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
    bool isRequestSpin() const;

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
    bool binderFilter(const Triangle*);

    void setPunchHitTimer(u8);
    void initEffect();
    void initMaterialEffect();
    void initCommonEffect();
    void addSoundObjHolder();
    void initParts();
    void initMorphStringTable();
    void setupSensors();
    void updateTransForCamera();

    void initMember();

    void init2D();

    void initDrawAndModel();
    void initBeeMario();
    void initTeresaMario();
    void initHopperMario();
    void initIceMario();
    void initInvincibleMario();
    void initTeresaMarioAnimation();
    void initTornadoMario();
    void initBoneMario();
    void initFireBall();
    void initShadow();
    void initHand();
    void swapTextureInit();
    void initBlur();
    void initScreenBox();
    bool isUseScreenBox() const;
    void captureScreenBox() const;
    void writeBackScreenBox() const;
    void changeDisplayMode(u8);
    bool isAllHidden() const;
    void calcViewMainModel();
    void initFace();
    void updateHand();
    void updateFace();
    void drawIndirect() const;
    void drawIndirectModel() const;
    void drawReflectModel() const;
    void drawModelBlur() const;
    void drawColdWaterDamage() const;
    void drawRasterScroll(f32, s16, f32) const;
    void drawMosaic() const;
    void drawLifeUp() const;
    void calcSpinEffect();
    void drawSpinEffect() const;
    void drawWallShade(const TVec3f&, const TVec3f&, f32) const;
    void drawShadow() const;
    void drawSilhouette() const;
    void drawPreWipe() const;
    void drawScreenBlend() const;

    void drawMarioModel() const;

    // Called by drawMarioModel
    void drawSpinInhibit() const;
    void drawSphereMask() const;
    bool drawDarkMask() const;
    void drawHand() const;
    void decideShadowMode();
    void calcViewSilhouetteModel();
    void calcViewWallShadowModel();
    void calcViewBlurModel();
    void calcViewFootPrint();
    void calc1stPersonView();
    void calcFogLighting();
    void calcViewReflectionModel();
    void calcViewSearchLight();
    void updateDarkMask(u16);

    void resetPadSwing();
    void initActionMatrix();

    TVec3f& getGravityVec() const;
    TVec3f& getGravityVector() const;
    const TVec3f& getAirGravityVec() const;
    void updateGravityVec(bool, bool);
    void changeTeresaAnimation(const char*, s32);

    MultiEmitter* playEffect(const char*);
    void playEffectTrans(const char*, const TVec3f&);
    MultiEmitter* playMaterialEffect(const char*);
    MultiEmitter* playCommonEffect(const char*);
    void stopEffect(const char*);
    void stopMaterialEffect(const char*);
    void stopCommonEffect(const char*);
    bool isCommonEffect(const char*) const NO_INLINE;
    bool isMaterialEffect(const char*) const NO_INLINE;
    s32 getFloorMaterialIndex(u32) const;

    void updateActionTrigger();
    void updateControllerSwing();
    void updateControllerSwingLeft();
    void update2D();
    void updateTakingPosition();
    void updateSearchLight();
    void updateThrowing();
    void updateBeeWingAnimation();
    void updateFairyStar();
    void createRainbowDL();
    void updateRandomTexture(f32);
    void updatePlayerMode();
    void updateEffect();
    bool checkEffectWaterColumn();
    void updateThrowVector();
    void updateForCamera();
    void updateTornado();

    bool tryJumpRush();
    void initForJump();
    void lockOnDPD();
    bool tryCoinPull();
    bool tryThrow();
    bool tryStandardRush();
    void checkPriorRushTarget();
    u8 selectAction(const char*) const;
    bool selectInvalidMovingCollision(const char*) const;
    bool tryRushInRush();
    void bodyClap();
    bool selectWaterInOut(const char*) const;
    bool selectWaterInOutEffect(const char*) const;
    bool selectWaterInOutRush(const HitSensor*) const;
    void playEffectRT(const char*, const TVec3f&, const TVec3f&);
    void playEffectRTZ(const char*, const TVec3f&, const TVec3f&);
    void playEffectRTW(const char*, const TVec3f&, const TVec3f&);
    void playEffectSRT(const char*, f32, const TVec3f&, const TVec3f&);
    void emitEffectWaterColumn(const TVec3f&, const TVec3f&);
    bool selectRecoverFlyMeter(const HitSensor*) const;
    void endRush(const RushEndInfo*);
    void stopSpinTicoEffect(bool);
    void stopEffectForce(const char*);
    bool isRequestRush() const;
    bool isRequestJump2P() const;
    bool isKeepJump() const;
    bool isKeepJump2P() const;
    bool isRequestHipDrop() const;
    bool isRequestSpinJump2P() const;
    bool tryReleaseBombTeresa();
    void initBlackHoleOut();  // void ?
    void exeGameOverBlackHole2();
    bool isEnableSpinPunch();
    bool trySpinPunch();
    void shootFireBall();
    void doFreezeAttack();
    void initBlink();
    void updateBlink();
    bool finalizeFreezeModel();
    void setBlink(const char*);
    void resetSensorCount();
    void getStickValue(f32*, f32*);
    bool checkButtonType(u16, bool) const;
    bool sendPunch(HitSensor*, bool);
    void reactionPunch(HitSensor*);
    void tryCoinPullInRush();
    void releaseThrowMemoSensor();
    void createIceFloor(const TVec3f&);
    void syncJumpBeeStickMode();

    bool isRequestJump() const;

    void damageDropThrowMemoSensor();

    void setPlayerMode(u16, bool);

    bool isActionOk(const char*) const;

    bool isInZeroGravitySpot() const;
    f32 getGravityRatio() const;
    u32 getGravityLevel() const;

    void forceKill(u32);

    bool sendMsgToSensor(HitSensor*, u32);

    void entryWallWalkMode(const TVec3f&, const TVec3f&);

    const HitSensor* getCarrySensor() const;
    HitSensor* getLookTargetSensor() const;
    bool selectHomingInSuperHipDrop(const char*) const;
    f32 getFaceLookHeight(const char*) const;
    void updateSpecialModeAnimation();
    J3DModelX* getJ3DModel() const;
    J3DModelData* getModelData() const;
    J3DModelX* getSimpleModel() const;
    void createTextureDL(DLholder*, u16, u16);
    void swapTexture(const char*, u8) const;
    void copyMaterial(J3DModel*, u16, s32);
    void rushDropThrowMemoSensor();
    void offTakingFlag();

    void changeHandMaterial();
    void calcScreenBoxRange();
    void updateRasterScroll();

    void settingRush();

    void resetCondition();
    bool isFixJumpRushSensor(const HitSensor*) const;
    bool isLandEffectRushSensor(const HitSensor*) const;
    void beginRush();

    bool takeSensor(HitSensor*);

    bool selectJumpRushSensor(const char*) const;

    void memorizeSensorThrow(HitSensor*);

    // Defined in MarioActorOffensiveMsg
    void attackOrPushSensor(HitSensor*, f32);
    void attackOrPushSensorInDamage(HitSensor*, f32);
    void attackOrPushSensorInRush(HitSensor*, f32);
    void tryAddClapCoin(HitSensor*);
    bool tryTornadoAttack(HitSensor*);
    bool isUnderTarget(HitSensor*);
    bool tryHipDropAttack(HitSensor*);
    bool checkAndTryTrampleAttack(HitSensor*, f32, bool);
    bool tryTrampleAttack(HitSensor*);
    bool cylinderHorizontalCheck(HitSensor*);
    bool tryJetAttack(HitSensor*);
    void tryCounterJetAttack(HitSensor*);
    bool tryGetItem(HitSensor*);
    bool cylinderPushCheck(const TVec3f&, f32, f32, f32);
    void attackOrPushPolygons();
    void sendWallTouch(HitSensor*, HitSensor*);
    bool sendMsgUpperPunch(HitSensor*);

    // Defined in MarioActorDefensiveMsg
    bool tryAttackMsg(u32, const HitSensor*, bool*);
    bool tryVectorAttackMsg(u32, const TVec3f&);
    void cylinderHitCheck(const HitSensor*, TVec3f*);

    bool selectDamageFireColor(const char*) const;
    void trampleJump(f32, f32);

    const MarioConst& getConst() const {
        return *mConst;
    }

    inline u32 getHealth() const {
        return mHealth;
    }

    inline const Mario::MovementStates& getMovementStates() const {
        return mMario->mMovementStates;
    }

    inline const Mario::DrawStates& getDrawStates() const {
        return mMario->mDrawStates;
    }

    inline const Mario::DrawStates& getPrevDrawStates() const {
        return mMario->mPrevDrawStates;
    }

    // Only used in isSleeping() to make it match
    inline bool IsMarioAnimationRun(const char* pAnimName) const {
        return mMario->isAnimationRun(pAnimName);
    }

    struct FBO {
        u32 _0;
        u32 _4;
    };

    /* 0x08C */ u8 _8C;
    /* 0x090 */ DLchanger* mDLchanger;
    /* 0x094 */ DLchanger* _94[0x40];
    /* 0x194 */ u8* mDL[2];
    /* 0x19C */ u32 mDLSize;
    /* 0x1A0 */ u8 mCurrDL;
    /* 0x1A1 */ u8 _1A1;
    /* 0x1A4 */ f32 _1A4;
    /* 0x1A8 */ u16 _1A8;
    /* 0x1AA */ u16 _1AA;
    /* 0x1AC */ f32 _1AC;
    /* 0x1B0 */ Color8 _1B0;
    /* 0x1B4 */ u8 _1B4;
    /* 0x1B5 */ bool _1B5;
    /* 0x1B8 */ u32 _1B8;
    /* 0x1BC */ MarioMessenger* _1BC;
    /* 0x1C0 */ bool _1C0;
    /* 0x1C1 */ bool _1C1;
    /* 0x1C2 */ bool _1C2;
    /* 0x1C3 */ bool _1C3;
    /* 0x1C4 */ u16 _1C4;
    /* 0x1C6 */ u16 _1C6;
    /* 0x1C8 */ f32 _1C8;
    /* 0x1CC */ f32 _1CC;
    /* 0x1D0 */ u8 _1D0;
    /* 0x1D1 */ u8 _1D1;
    /* 0x1D4 */ f32 _1D4;
    /* 0x1D8 */ FBO* _1D8;
    /* 0x1DC */ FBO* _1DC;
    /* 0x1E0 */ bool _1E0;
    /* 0x1E1 */ u8 _1E1;
    /* 0x1E2 */ u8 _1E2;
    /* 0x1E4 */ f32 _1E4;
    /* 0x1E8 */ u16 _1E8;
    /* 0x1EC */ f32 _1EC;
    /* 0x1F0 */ TVec3f _1F0;
    /* 0x1FC */ TVec3f _1FC;
    /* 0x208 */ f32 _208;
    /* 0x20C */ u32 _20C;
    /* 0x210 */ u8 _210;
    /* 0x211 */ u8 _211;
    /* 0x214 */ CollisionShadow* _214;
    /* 0x218 */ DrawAdaptor* _218;
    /* 0x21C */ DrawAdaptor* _21C;
    /* 0x220 */ DrawAdaptor* _220;
    /* 0x224 */ u32 _224;
    /* 0x228 */ DrawAdaptor* _228;
    /* 0x22C */ DrawAdaptor* _22C;
    /* 0x230 */ Mario* mMario;
    /* 0x234 */ MarioAnimator* mMarioAnim;
    /* 0x238 */ MarioEffect* mMarioEffect;
    /* 0x23C */ MarioConst* mConst;
    /* 0x240 */ TVec3f _240;
    /* 0x24C */ TVec3f _24C;
    /* 0x258 */ TVec3f _258;
    /* 0x264 */ TVec3f _264;
    /* 0x270 */ TVec3f _270;
    /* 0x27C */ TVec3f _27C;
    /* 0x288 */ TVec3f _288;
    /* 0x294 */ TVec3f _294;
    /* 0x2A0 */ TVec3f _2A0;
    /* 0x2AC */ TVec3f _2AC;
    /* 0x2B8 */ TVec3f _2B8;
    /* 0x2C4 */ TVec3f _2C4;
    /* 0x2D0 */ TVec3f _2D0;
    /* 0x2DC */ TVec3f _2DC;
    /* 0x2E8 */ TVec3f _2E8;
    /* 0x2F4 */ TVec3f _2F4;
    /* 0x300 */ TVec3f _300;
    /* 0x30C */ TVec3f _30C;
    /* 0x318 */ TVec3f _318;
    /* 0x324 */ TVec3f _324;
    /* 0x330 */ u16 _330;
    /* 0x332 */ u16 _332;
    /* 0x334 */ u16 _334;
    /* 0x336 */ u16 _336;
    /* 0x338 */ u16 _338;
    /* 0x33C */ TVec3f _33C;
    /* 0x348 */ TVec3f _348;
    /* 0x354 */ TVec3f _354;
    /* 0x360 */ TVec3f _360;
    /* 0x36C */ GravityInfo* _36C;
    /* 0x370 */ u8 _370;
    /* 0x374 */ f32 _374;
    /* 0x378 */ u16 _378;
    /* 0x37C */ u32 _37C;
    /* 0x380 */ u32 mHealth;
    /* 0x394 */ u32 mWaterLife;
    /* 0x388 */ u32 _388;
    /* 0x38C */ u16 _38C;
    /* 0x390 */ u32 _390;
    /* 0x394 */ u32 _394;
    /* 0x398 */ u32 _398;
    /* 0x39C */ u8 _39C;
    /* 0x39D */ u8 _39D;
    /* 0x39E */ u8 _39E;
    /* 0x3A0 */ u32 _3A0;
    /* 0x3A4 */ u32 _3A4;
    /* 0x3A8 */ u16 _3A8;
    /* 0x3AA */ u16 _3AA;
    /* 0x3AC */ u16 _3AC;
    /* 0x3B0 */ f32 _3B0;
    /* 0x3B4 */ TVec3f _3B4;
    /* 0x3C0 */ bool _3C0;
    /* 0x3C1 */ bool _3C1;
    /* 0x3C4 */ TVec3f _3C4;
    /* 0x3D0 */ u16 _3D0;
    /* 0x3D2 */ u16 _3D2;
    /* 0x3D4 */ u16 mPlayerMode;
    /* 0x3D6 */ u16 _3D6;
    /* 0x3D8 */ u16 _3D8;
    /* 0x3DA */ u16 _3DA;
    /* 0x3DC */ u16 _3DC;
    /* 0x3De */ u8 _3DE;
    /* 0x3DF */ u8 _3DF;
    /* 0x3E0 */ u32 mMaxHealth;
    /* 0x3E4 */ bool _3E4;
    /* 0x3E5 */ bool _3E5;
    /* 0x3E6 */ bool _3E6;
    /* 0x3E7 */ bool _3E7;
    /* 0x3E8 */ bool _3E8;
    /* 0x3EC */ TMtx34f _3EC;
    /* 0x41C */ u32 _41C;
    /* 0x420 */ u32 _420;
    /* 0x424 */ HitSensor* _424;
    /* 0x428 */ HitSensor* _428[4];
    /* 0x438 */ u8 _438[0x30];

    union {
        struct {
            /* 0x468 */ u32 _468;
            /* 0x46C */ HitSensor* _46C;
            /* 0x470 */ u32 _470;
        };

        /* 0x468 */ TVec3f _468Vec;
    };

    /* 0x474 */ u32 _474;
    /* 0x478 */ f32 _478;
    /* 0x47C */ u32 _47C;
    /* 0x480 */ bool _480;
    /* 0x481 */ u8 _481;
    /* 0x482 */ bool _482;
    /* 0x483 */ u8 _483;
    /* 0x484 */ TVec3f _484;
    /* 0x490 */ f32 _490;
    /* 0x494 */ FixedPosition* _494;
    /* 0x498 */ FixedPosition* _498;
    /* 0x49C */ FixedPosition* _49C;
    /* 0x4A0 */ FixedPosition* _4A0;
    /* 0x4A4 */ void* _4A4;  // used in calcAnimInMovement()
    /* 0x4A8 */ u32 _4A8;
    /* 0x4AC */ f32 _4AC;
    /* 0x4B0 */ f32 _4B0;
    /* 0x4B4 */ f32 _4B4;
    /* 0x4B8 */ TVec3f _4B8;
    /* 0x4C4 */ TVec3f _4C4;
    /* 0x4D0 */ u32 _4D0[128];
    /* 0x6D0 */ u8 _6D0;
    /* 0x6D4 */ f32 _6D4;
    /* 0x6D8 */ f32 _6D8;
    /* 0x6DC */ HitSensor* _6DC[64];
    /* 0x7DC */ u16 _7DC; // _6DC count
    /* 0x7DE */ u16 _7DE;
    /* 0x7E0 */ u16 _7E0;
    /* 0x7E2 */ u8 _7E2;
    /* 0x7E4 */ u32 _7E4[64];
    /* 0x8E4 */ u8 _8E4[64];
    /* 0x924 */ HitSensor* _924;
    /* 0x928 */ u32 _928;
    /* 0x92C */ u32 _92C;
    /* 0x930 */ u32 _930;
    /* 0x934 */ bool _934;
    /* 0x935 */ bool _935;
    /* 0x938 */ TVec3f _938;
    /* 0x944 */ u8 _944;
    /* 0x945 */ u8 _945;
    /* 0x946 */ u16 _946;
    /* 0x948 */ u16 _948;
    /* 0x94A */ u16 _94A;
    /* 0x94C */ u16 _94C;
    /* 0x94E */ u16 _94E;
    /* 0x950 */ u8 _950;
    /* 0x951 */ u8 _951;
    /* 0x954 */ u32 _954[8];
    /* 0x974 */ u8 _974;
    /* 0x978 */ TVec3f _978;
    /* 0x984 */ f32 _984;
    /* 0x988 */ u8 _988;
    /* 0x989 */ u8 _989;
    /* 0x98C */ TornadoMario* mTornadoMario;
    /* 0x990 */ u8 _990;
    /* 0x994 */ u32 _994;
    /* 0x998 */ u32 _998;
    /* 0x99C */ u32 _99C;
    /* 0x9A0 */ JetTurtleShadow* _9A0;
    /* 0x9A4 */ MarioParts* _9A4;
    /* 0x9A8 */ f32 _9A8;
    /* 0x9AC */ f32 _9AC;
    /* 0x9B0 */ f32 _9B0;
    /* 0x9B4 */ u16 _9B4;
    /* 0x9B8 */ u32 _9B8;
    /* 0x9BC */ u32 _9BC;
    /* 0x9C0 */ ModelHolder* _9C0;
    /* 0x9C4 */ LiveActor* _9C4;
    /* 0x9C8 */ ModelHolder* _9C8;
    /* 0x9CC */ f32 _9CC;
    /* 0x9D0 */ f32 _9D0;
    /* 0x9D4 */ u32 _9D4;
    /* 0x9D8 */ TVec3f _9D8;
    /* 0x9E4 */ ModelHolder* _9E4;
    /* 0x9E8 */ MarioParts* _9E8;
    /* 0x9EC */ LiveActor* _9EC;
    /* 0x9F0 */ bool _9F0;
    /* 0x9F1 */ bool mAlphaEnable;
    /* 0x9F2 */ u16 _9F2;
    /* 0x9F4 */ TVec3f _9F4;
    /* 0xA00 */ ModelHolder* _A00;
    /* 0xA04 */ ModelHolder* _A04;
    /* 0xA08 */ u8 _A08;
    /* 0xA09 */ u8 _A09;
    /* 0xA0A */ u8 mCurrModel;
    /* 0xA0B */ u8 _A0B;
    /* 0xA0C */ u8 _A0C;
    /* 0xA10 */ u32 _A10;
    /* 0xA14 */ J3DModelX* _A14;
    /* 0xA18 */ TVec3f _A18;
    /* 0xA24 */ u8 _A24;
    /* 0xA25 */ u8 _A25;
    /* 0xA28 */ J3DModelX* mModels[6];
    /* 0xA40 */ ModelHolder* _A40;
    /* 0xA44 */ ModelHolder* _A44;
    /* 0xA48 */ ModelHolder* _A48;
    /* 0xA4C */ ModelHolder* _A4C;
    /* 0xA50 */ ModelHolder* _A50;
    /* 0xA54 */ ModelHolder* _A54;
    /* 0xA58 */ u8 _A58;
    /* 0xA59 */ u8 _A59;
    /* 0xA5A */ u8 _A5A;
    /* 0xA5B */ u8 _A5B;
    /* 0xA5C */ ModelHolder* _A5C;
    /* 0xA60 */ u8 _A60;
    /* 0xA61 */ bool _A61;
    /* 0xA62 */ bool _A62;
    /* 0xA64 */ u32 _A64;
    /* 0xA68 */ f32 _A68;
    /* 0xA6C */ u16 _A6C;
    /* 0xA6E */ bool _A6E;
    /* 0xA70 */ Mtx* _A70[8];
    /* 0xA90 */ Mtx* _A90[8];
    /* 0xAB0 */ TMtx34f _AB0;
    /* 0xAE0 */ TMtx34f _AE0;
    /* 0xB10 */ u16 _B10;
    /* 0xB12 */ u16 _B12;
    /* 0xB14 */ u16 _B14;
    /* 0xB18 */ TVec3f _B18;
    /* 0xB24 */ f32 _B24;
    /* 0xB28 */ f32 _B28;
    /* 0xB2C */ f32 _B2C;
    /* 0xB30 */ f32 _B30;
    /* 0xB34 */ f32 _B34;
    /* 0xB38 */ f32 _B38;
    /* 0xB3C */ f32 _B3C;
    /* 0xB40 */ f32 _B40;
    /* 0xB44 */ u32 _B44;
    /* 0xB48 */ FootPrint* _B48;
    /* 0xB4C */ IceStep** _B4C;
    /* 0xB50 */ u16 _B50;
    /* 0xB54 */ u32 _B54[3];
    /* 0xB60 */ u16 _B60;
    /* 0xB64 */ ResTIMG** _B64;
    /* 0xB68 */ u8 _B68;
    /* 0xB6A */ u16 _B6A;
    /* 0xB6C */ DLholder* _B6C;
    /* 0xB70 */ u16 _B70;
    /* 0xB72 */ u8 _B72;
    /* 0xB74 */ u16 _B74;
    /* 0xB78 */ J3DAnmTexPattern* mEyeRes;
    /* 0xB7C */ JUTTexture* _B7C;
    /* 0xB80 */ JUTTexture* _B80;
    /* 0xB84 */ JUTTexture* _B84;
    /* 0xB88 */ u16 _B88;
    /* 0xB8C */ MarioNullBck* mNullAnimation;
    /* 0xB90 */ bool _B90;
    /* 0xB91 */ bool _B91;
    /* 0xB92 */ s8 _B92;
    /* 0xB94 */ u16 _B94;
    /* 0xB96 */ u8 _B96;
    /* 0xB98 */ u32 _B98;
    /* 0xB9C */ u16 _B9C;
    /* 0xB9E */ u16 _B9E;
    /* 0xBA0 */ SmokeEffectEntry** _BA0;
    /* 0xBA4 */ HashSortTable* _BA4;
    /* 0xBA8 */ ModelObj* _BA8;
    /* 0xBAC */ TVec3f _BAC;
    /* 0xBB8 */ TVec3f _BB8;
    /* 0xBC4 */ u16 _BC4;
    /* 0xBC8 */ TMtx34f _BC8;
    /* 0xBF8 */ TMtx34f _BF8;
    /* 0xC28 */ Mtx* _C28;
    /* 0xC2C */ TMtx34f _C2C;
    /* 0xC5C */ TMtx34f _C5C;
    /* 0xC8C */ TMtx34f _C8C;
    /* 0xCBC */ TMtx34f _CBC;
    /* 0xCEC */ TMtx34f _CEC;
    /* 0xD1C */ TMtx34f _D1C;
    /* 0xD4C */ TMtx34f _D4C;
    /* 0xD7C */ TMtx34f _D7C;
    /* 0xDAC */ TMtx34f _DAC;
    /* 0xDDC */ TMtx34f _DDC;
    /* 0xE0C */ TMtx34f _E0C;
    /* 0xE3C */ TMtx34f _E3C;
    /* 0xE6C */ TMtx34f _E6C;
    /* 0xE9C */ u16 mBlendMtxTimer;
    /* 0xEA0 */ f32 _EA0;
    /* 0xEA4 */ bool _EA4;
    /* 0xEA5 */ bool _EA5;
    /* 0xEA6 */ bool _EA6;
    /* 0xEA8 */ TMtx34f _EA8;
    /* 0xED8 */ TVec3f _ED8;
    /* 0xEE4 */ u32 _EE4;
    /* 0xEE8 */ bool mSuperKinokoCollected;
    /* 0xEE9 */ bool mPowerupCollected;
    /* 0xEEA */ bool mTransforming;
    /* 0xEEB */ bool _EEB;
    /* 0xEEC */ bool _EEC;
    /* 0xEED */ bool _EED;
    /* 0xEEE */ bool _EEE;
    /* 0xEF0 */ u16 _EF0;
    /* 0xEF2 */ u16 _EF2;
    /* 0xEF4 */ u16 _EF4;
    /* 0xEF6 */ u16 _EF6;
    /* 0xEF8 */ u32 _EF8;
    /* 0xEFC */ u32 _EFC;
    /* 0xF00 */ u8 _F00;
    /* 0xF04 */ u32 _F04;
    /* 0xF08 */ u32 _F08;
    /* 0xF0C */ bool _F0C;
    /* 0xF0D */ u8 _F0D;
    /* 0xF0E */ u16 _F0E;
    /* 0xF10 */ u16 _F10;
    /* 0xF12 */ u16 _F12;
    /* 0xF14 */ u16 _F14;
    /* 0xF16 */ u16 _F16;
    /* 0xF18 */ u16 _F18;
    /* 0xF1A */ u16 _F1A;
    /* 0xF1C */ u16 _F1C;
    /* 0xF1E */ u16 _F1E;
    /* 0xF20 */ u8 _F20;
    /* 0xF21 */ u8 _F21;
    /* 0xF24 */ u32 _F24;
    /* 0xF28 */ u16 _F28;
    /* 0xF2C */

    union {
        /* 0xF2C */ u32 _F2C;
        /* 0xF2C */ TVec3f _F2CVec;
    };

    /* 0xF38 */ u8 _F38;

    union {
        /* 0xF3C */ AudGeneric* _F3C;
        /* 0xF3C */ TVec3f* _F3CVec;
    };

    /* 0xF40 */ u16 _F40;
    /* 0xF42 */ u16 _F42;
    /* 0xF44 */ bool _F44;
    /* 0xF48 */ HitSensor* _F48;
    /* 0xF4C */ BlackHole* mBlackHole;
    /* 0xF50 */ TVec3f mBlackHolePosition;
    /* 0xF5C */ TVec3f mBlackHoleRotateAxis;
    /* 0xF68 */ TVec3f mPosRelativeToBlackHole;
    /* 0xF74 */ bool _F74;
    /* 0xF78 */ TVec3f mCamPos;
    /* 0xF84 */ TVec3f mCamDirX;
    /* 0xF90 */ TVec3f mCamDirY;
    /* 0xF9C */ TVec3f mCamDirZ;
    /* 0xFA8 */ TVec3f _FA8;
    /* 0xFB4 */ const Nerve* _FB4;
    /* 0xFB8 */ u16 _FB8;  // a timer
    /* 0xFBC */ u32 _FBC;
    /* 0xFC0 */ u32 _FC0;
    /* 0xFC4 */ u32 _FC4;
    /* 0xFC8 */ u32 _FC8;
    /* 0xFCC */ bool _FCC;
    /* 0xFCD */ bool _FCD;
};

// header defined since these actually get defined in CamHeliEffector
// and in sinit of NameObjFactory
namespace NrvMarioActor {
    NERVE_DECL_EXE(MarioActorNrvWait, MarioActor, Wait);
    NERVE_DECL_EXE(MarioActorNrvGameOver, MarioActor, GameOver);
    NERVE_DECL_EXE(MarioActorNrvGameOverAbyss, MarioActor, GameOverAbyss);
    NERVE_DECL_EXE(MarioActorNrvGameOverAbyss2, MarioActor, GameOverAbyss);
    NERVE_DECL_EXE(MarioActorNrvGameOverFire, MarioActor, GameOverFire);
    NERVE_DECL_EXE(MarioActorNrvGameOverBlackHole, MarioActor, GameOverBlackHole2);
    NERVE_DECL_EXE(MarioActorNrvGameOverNonStop, MarioActor, GameOverNonStop);
    NERVE_DECL_EXE(MarioActorNrvGameOverSink, MarioActor, GameOverSink);
    NERVE_DECL_EXE(MarioActorNrvTimeWait, MarioActor, TimeWait);
    NERVE_DECL_EXE(MarioActorNrvNoRush, MarioActor, Wait);
};  // namespace NrvMarioActor
