#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/BlackHole.hpp"
#include "Game/Player/Mario.hpp"

class FootPrint;
class J3DModelData;
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
class J3DModelX;
class TornadoMario;
class ModelHolder;

bool gIsLuigi;  // (cc68 - 10000)(r13)

class MarioActor : public LiveActor {
public:
    MarioActor(const char*);

    ~MarioActor();

    virtual const TVec3f& getLastMove() const;
    virtual void getLastMove(TVec3f*) const;
    virtual void getFrontVec(TVec3f*) const;

    void init(const JMapInfoIter&);
    void init2(const TVec3f&, const TVec3f&, s32);
    void initAfterPlacement();
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
    void getRealMtx(MtxPtr, const char*) const NO_INLINE;
    void getRealPos(const char*, TVec3f*) const;
    void getGlobalJointMtx(const char*);
    void calcAnimInMovement();
    void forceSetBaseMtx(MtxPtr);
    void calcAnim();
    void calcAndSetBaseMtx();
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
    void addSoundObjHolder();
    void initParts();
    void initMorphStringTable();
    void setupSensors();
    void updateTransForCamera();

    void initMember();

    void init2D();

    void initDrawAndModel();
    bool isAllHidden() const;

    void drawMarioModel() const;

    // Called by drawMarioModel
    void drawSpinInhibit() const;
    void drawSphereMask() const;
    bool drawDarkMask() const;
    void drawHand() const;

    void resetPadSwing();
    void initActionMatrix();

    TVec3f& getGravityVec() const;
    TVec3f& getGravityVector() const;
    void updateGravityVec(bool, bool);
    void changeTeresaAnimation(const char*, s32);

    void playEffect(const char*);
    void playEffectTrans(const char*, const TVec3f&);
    void stopEffect(const char*);

    void updateActionTrigger();
    void updateControllerSwing();
    void updateControllerSwingLeft();
    void update2D();
    void updateTakingPosition();
    void updateSearchLight();
    void updateThrowing();
    void updateBeeWingAnimation();
    void updateFairyStar();
    void updatePlayerMode();
    void updateEffect();
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
    bool tryRushInRush();
    void bodyClap();
    bool selectWaterInOut(const char*) const;
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
    bool isRequestSpinJump2P() const;
    bool tryReleaseBombTeresa();
    void initBlackHoleOut();  // void ?
    void exeGameOverBlackHole2();
    bool isEnableSpinPunch();
    bool trySpinPunch();
    void shootFireBall();
    void doFreezeAttack();
    void initBlink();
    void setBlink(const char*);
    void resetSensorCount();
    void getStickValue(f32*, f32*);
    bool checkButtonType(u16, bool) const;
    bool sendPunch(HitSensor*, bool);
    void reactionPunch(HitSensor*);
    void tryCoinPullInRush();
    bool tryJetAttack(HitSensor*);
    void releaseThrowMemoSensor();
    void createIceFloor(const TVec3f&);

    bool isRequestJump() const;

    void damageDropThrowMemoSensor();

    void setPlayerMode(u16, bool);

    bool isActionOk(const char*) const;

    bool isInZeroGravitySpot() const;

    void forceKill(u32);

    void sendMsgUpperPunch(HitSensor*);

    void entryWallWalkMode(const TVec3f&, const TVec3f&);

    const HitSensor* getCarrySensor() const;
    HitSensor* getLookTargetSensor() const;
    f32 getFaceLookHeight(const char*) const;
    void updateSpecialModeAnimation();
    J3DModelData* getModelData() const;
    void rushDropThrowMemoSensor();
    void offTakingFlag();

    const MarioConst& getConst() const { return *mConst; }

    inline u32 getHealth() const { return mHealth; }

    inline const Mario::MovementStates& getMovementStates() const { return mMario->mMovementStates; }

    inline const Mario::DrawStates& getDrawStates() const { return mMario->mDrawStates; }

    inline const Mario::DrawStates& getPrevDrawStates() const { return mMario->mPrevDrawStates; }

    struct FBO {
        u32 _0;
        u32 _4;
    };

    u8 _8C;
    DLchanger* mDLchanger;  // 0x90
    u32 _94[0x40];
    u8* mDL[2];   // 0x194
    u32 mDLSize;  // 0x19C
    u8 mCurrDL;   // 0x1A0
    u8 _1A1;
    f32 _1A4;
    u16 _1A8;
    u16 _1AA;
    f32 _1AC;
    Color8 _1B0;
    u8 _1B4;
    bool _1B5;
    u32 _1B8;
    MarioMessenger* _1BC;
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
    FBO* _1D8;
    FBO* _1DC;
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
    CollisionShadow* _214;
    u32 _218;
    u32 _21C;
    u32 _220;
    u32 _224;
    u32 _228;
    u32 _22C;
    Mario* mMario;              // 0x230
    MarioAnimator* mMarioAnim;  // 0x234
    MarioEffect* mMarioEffect;  // 0x238
    MarioConst* mConst;         // 0x23C
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
    GravityInfo* _36C;
    u8 _370;
    // padding
    f32 _374;
    u16 _378;
    // padding
    u32 _37C;
    u32 mHealth;     // 0x380
    u32 mWaterLife;  // 0x384
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
    u16 mPlayerMode;  // 0x3D4
    u16 _3D6;
    u16 _3D8;
    u16 _3DA;
    u16 _3DC;
    u8 _3DE;
    u8 _3DF;
    u32 mMaxHealth;  // 0x3E0
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
    u32 _428[4]; // 0x428 - 0x434
    u8 _438[0x30];
    union {
        struct {
            u32 _468;
            HitSensor* _46C;
            u32 _470;
        };
        TVec3f _468Vec;
    };

    // TVec3f _468;
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
    FixedPosition* _498;
    FixedPosition* _49C;
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
    HitSensor* _924;
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
    TornadoMario* mTornadoMario;  // 0x98C
    u8 _990;
    u32 _994;
    u32 _998;
    u32 _99C;
    u32 _9A0;
    MarioParts* _9A4;
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
    bool mAlphaEnable;  // 0x9F1
    u16 _9F2;
    TVec3f _9F4;
    u32 _A00;
    u32 _A04;
    u8 _A08;
    u8 _A09;
    u8 mCurrModel;  // 0xA0A
    u8 _A0B;
    u8 _A0C;
    u32 _A10;
    u32 _A14;
    TVec3f _A18;
    u8 _A24;
    u8 _A25;
    // padding
    J3DModelX* mModels[6];  // 0xA28
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
    ModelHolder* _A5C;
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
    FootPrint* _B48;
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
    void* mEyeRes;  // 0xB78
    u32 _B7C;
    u32 _B80;
    u32 _B84;
    u16 _B88;
    MarioNullBck* mNullAnimation;  // 0xB8C
    bool _B90;                     // animations
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
    void* _C28;
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
    u16 mBlendMtxTimer;  // 0xE9C
    f32 _EA0;
    bool _EA4;
    bool _EA5;
    bool _EA6;
    TMtx34f _EA8;
    TVec3f _ED8;
    u32 _EE4;
    bool mSuperKinokoCollected;  // 0xEE8
    bool mPowerupCollected;      // 0xEE9
    bool mTransforming;          // 0xEEA
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
    union {
        u32 _F2C;
        TVec3f _F2CVec;
    };
    u8 _F38;
    // padding
    union {
        JAIAudible* _F3C;
        TVec3f* _F3C_vec;
    };
    u16 _F40;
    u16 _F42;
    bool _F44;
    // padding
    u32 _F48;
    BlackHole* mBlackHole;
    TVec3f mBlackHolePosition;
    TVec3f mBlackHoleRotateAxis;
    TVec3f mPosRelativeToBlackHole;
    u8 _F74;
    // padding
    TVec3f mCamPos;   // 0xF78
    TVec3f mCamDirX;  // 0xF84
    TVec3f mCamDirY;  // 0xF90
    TVec3f mCamDirZ;  // 0xF9C
    TVec3f _FA8;
    const Nerve* _FB4;
    u16 _FB8;  // a timer
    u32 _FBC;
    u32 _FC0;
    u32 _FC4;
    u32 _FC8;
    bool _FCC;
    bool _FCD;
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
}  // namespace NrvMarioActor
