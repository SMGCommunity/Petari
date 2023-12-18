#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Player/Mario.h"

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

namespace MR {
    unsigned int getFrameBufferWidth();
}

class MarioActor : public LiveActor {
public:
    MarioActor(const char*);

    ~MarioActor();

    virtual const TVec3f& getLastMove() const;
    virtual void getLastMove(TVec3f *) const;
    virtual void getFrontVec(TVec3f *) const;
    
    void init(const JMapInfoIter&);
    void init2(const TVec3f&, const TVec3f&, long);
    void initAfterPlacement();
    void initAfterOpeningDemo();
    void calcBaseFrontVec(const TVec3f&);
    void playSound(const char*, long);
    void changeAnimation(const char*, const char*);
    void changeAnimationNonStop(const char*);
    void changeAnimationUpper(const char*);
    void stopAnimation(const char*);
    bool isAnimationRun(const char*) const;
    void changeNullAnimation(const char*, signed char);
    void clearNullAnimation(signed char);
    bool isStopNullAnimation() const;
    void changeGameOverAnimation();
    XjointTransform* getJointCtrl(const char*) const;
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
    void decLife(unsigned short);
    void decLifeLarge();
    void decLifeMiddle();
    void decLifeSmall();
    void resetWaterLife();
    void updateLife();
    void incLife(unsigned long);
    void changeMaxLife(long);
    bool doPressing();
    void careMovingObject();
    bool doStun();
    void scaleMtx(MtxPtr);
    void updateBaseScaleMtx();
    void getRealMtx(f32 (*) [4], const char*);
    void getGlobalJointMtx(const char*);
    void calcAnimInMovement();
    void forceSetBaseMtx(f32 (*) [4]);
    void calcAnim();
    void calcAndSetBaseMtx();
    void setBlendMtxTimer(unsigned short);
    void getGroundPos(TVec3f* dst) const;
    void getShadowPos() const;
    
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
    void setPress(unsigned char, int);
    void getResourceTable() const;

    bool isEnableMoveMario() const;
    bool isEnableNerveChange() const;

    void forcceGameOver();
    void forceGameOverAbyss();
    void forceGameOverBlackHole();
    void forceGameOverNonStop();
    void forceGameOverSink();

    void updateCameraInfo();
    bool binderFilter(const Triangle*);
    
    void setPunchHitTimer(unsigned char);
    void initEffect();
    void addSoundObjHolder();
    void initParts();
    void initMorphStringTable();
    void setupSensors();
    void updateTransForCamera();

    void initMember();

    void init2D();

    void initDrawAndModel();

    void resetPadSwing();
    void initActionMatrix();

    TVec3f& getGravityVec();
    TVec3f& getGravityVector();
    void updateGravityVec(bool, bool);
    void changeTeresaAnimation(const char *, long);

    void playEffect(const char *);
    void stopEffect(const char *);

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
    unsigned char selectAction(const char *) const;
    bool tryRushInRush();
    void bodyClap();
    bool selectWaterInOut(const char *) const;
    bool selectWaterInOutRush(const HitSensor *) const;
    void playEffectRT(const char *, const TVec3f &, const TVec3f &);
    void emitEffectWaterColumn(const TVec3f &, const TVec3f &);
    bool selectRecoverFlyMeter(const HitSensor *) const;
    void endRush(const RushEndInfo *);
    void stopSpinTicoEffect(bool);
    void stopEffectForce(const char *);
    bool isRequestRush() const;
    bool isRequestSpinJump2P() const;
    bool tryReleaseBombTeresa();
    bool isEnableSpinPunch();
    bool trySpinPunch();
    void shootFireBall();
    void doFreezeAttack();
    void setBlink(const char *);
    void resetSensorCount();
    void getStickValue(f32 *, f32 *);
    const HitSensor& getCarrySensor() const;

    inline u32 getHealth() const {
        return mHealth;
    }

    inline const Mario::MovementStates& getMovementStates() const {
        return mMario -> mMovementStates;
    }

    inline const Mario::DrawStates& getDrawStates() const {
        return mMario -> mDrawStates;
    }

    inline const Mario::DrawStates& getPrevDrawStates() const {
        return mMario -> mPrevDrawStates;
    }

    struct FBO {
        u32 _0;
        u32 _4;
    };

    u8 _8C;
    //padding
    u8 _90[4];
    //u32 _90;
    u32 _94[0x40];
    u8 _98[0xc];
    //padding
    u8 _1A0;
    u8 _1A1;
    //padding
    f32 _1A4;
    u16 _1A8;
    u16 _1AA;
    f32 _1AC;
    Color8 _1B0;
    u8 _1B4;
    bool _1B5;
    //padding
    u8 _1B8[4];
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
    //padding
    f32 _1E4;
    u16 _1E8;
    //padding
    f32 _1EC;
    TVec3f _1F0;
    TVec3f _1FC;
    f32 _208;
    u8 _20C[4];
    u8 _210;
    u8 _211;
    //padding
    CollisionShadow* _214;
    u8 _218[0x18];
    Mario* mMario; // _230
    MarioAnimator* mMarioAnim; // _234
    MarioEffect* mMarioEffect; // _238
    MarioConst* mConst; // _23C
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
    //padding
    TVec3f _33C;
    TVec3f _348;
    TVec3f _354;
    TVec3f _360;
    GravityInfo* _36C;
    u8 _370;
    //padding
    f32 _374;
    u16 _378;
    //padding
    u32 _37C;
    u32 mHealth;
    u32 _384;
    u32 _388;
    u16 _38C;
    u32 _390;
    u32 _394;
    u32 _398;;
    u8 _39C;
    u8 _39D;
    u8 _39E[0xa];
    u16 _3A8;
    u16 _3AA;
    u16 _3AC;
    //padding
    float _3B0;
    TVec3f _3B4;
    bool _3C0;
    bool _3C1;
    //padding
    TVec3f _3C4;
    u16 _3D0;
    u16 _3D2;
    u16 _3D4;
    u16 _3D6;
    u16 _3D8;
    u16 _3DA;
    u16 _3DC;
    u8 _3DE;
    u8 _3DF;
    u32 mMaxHealth;
    bool _3E4;
    bool _3E5;
    bool _3E6;
    bool _3E7;
    bool _3E8;
    //padding
    TMtx34f _3EC;
    u32 _41C;
    u32 _420;
    u32 _424;
    u32 _428[4];
    u8 _438[0x30];
    union {
        TVec3f _468;
        JGeometry::TVec3<long> _468l;
    };
    u32 _474;
    f32 _478;
    u32 _47C;
    u8 _480;
    u8 _481;
    bool _482;
    u8 _483;
    TVec3f _484;
    f32 _490;
    u8 _494[4];
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
    u32 _4C8[0x80];
    //padding
    u8 _6D0;
    f32 _6D4;
    f32 _6D8;
    u32 _6DC[0x40];
    u16 _7DC;
    u16 _7DE;
    u16 _7E0;
    u8 _7E2;
    //padding
    u32 _7E4[0x40];
    u8 _8E4[0x40];
    HitSensor* _924;
    u32 _928;
    u32 _92C;
    u32 _930;
    bool _934;
    bool _935;
    //padding
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
    //padding
    u32 _954[8];
    u8 _974;
    //padding
    TVec3f _978;
    f32 _984;
    u8 _988;
    u8 _989;
    //padding
    u32 _98C;
    u8 _990;
    //padding
    u8 _991[0x13];
    MarioParts* _9A4;
    f32 _9A8;
    f32 _9AC;
    f32 _9B0;
    u16 _9B4;
    //padding
    u8 _9B6[0x16];
    f32 _9CC;
    f32 _9D0;
    u8 _9D4[4];
    TVec3f _9D8;
    u8 _9E4[0xc];
    bool _9F0;
    bool _9F1;
    u16 _9F2;
    TVec3f _9F4;
    u8 _A00[8];
    u8 _A08;
    u8 _A09;
    u8 _A0A;
    u8 _A0B;
    u8 _A0C;
    u8 _A10[8];
    TVec3f _A18;
    u8 _A24;
    u8 _A25;
    //padding
    u32 _A28[6];
    u8 _A40[0x18];
    u8 _A58;
    u8 _A59;
    u8 _A5A;
    u8 _A5B;
    u8 _A5C[4];
    bool _A60;
    bool _A61;
    bool _A62;
    //padding
    u32 _A64;
    f32 _A68;
    u16 _A6C;
    bool _A6E;
    //padding
    u32 _A70[8];
    u32 _A90[8];
    TMtx34f _AB0;
    TMtx34f _AE0;
    u16 _B10;
    u16 _B12;
    u16 _B14;
    //padding
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
    //padding
    u32 _B54[3];
    u16 _B60;
    //padding
    u32 _B64;
    u8 _B68;
    //padding
    u16 _B6A;
    u32 _B6C;
    u16 _B70;
    u8 _B72;
    //padding
    u16 _B74;
    //padding
    u32 _B78;
    u32 _B7C;
    u32 _B80;
    u32 _B84;
    u16 _B88;
    //padding
    MarioNullBck* mNullAnimation;
    bool _B90;
    bool _B91;
    s8 _B92;
    //padding
    u16 _B94;
    u8 _B96;

    u32 _B98;
    
    u16 _B9C;
    u16 _B9E;
    u8 _BA0[0xc];
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
    u16 _E9C;
    f32 _EA0;
    bool _EA4;
    bool _EA5;
    bool _EA6;
    TMtx34f _EA8;
    TVec3f _ED8;
    u32 _EE4;
    bool mSuperKinokoCollected; // _EE8
    bool mPowerupCollected; // _EE9
    bool mTransforming; // _EEA
    bool _EEB;
    bool _EEC;
    bool _EED;
    bool _EEE;
    //padding
    u16 _EF0;
    u16 _EF2;
    u16 _EF4;
    u16 _EF6;
    u8 _EF8[8];
    u8 _F00;
    //padding
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
    //padding
    u32 _F24;
    u16 _F28;
    //padding
    TVec3f _F2C;
    u8 _F38;
    //padding
    union {
        JAIAudible* _F3C;
        TVec3f* _F3C_vec;
    };
    u16 _F40;
    u16 _F42; 
    bool _F44;
    //padding
    u32 _F48;
    u32 _F4C;
    TVec3f _F50;
    TVec3f _F5C;
    TVec3f _F68;
    u8 _F74;
    //padding
    TVec3f _F78;
    TVec3f _F84;
    TVec3f _F90;
    TVec3f _F9C;
    TVec3f _FA8;
    const Nerve* _FB4;
    u16 _FB8;
    u8 _FBC[0x10];
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
}
