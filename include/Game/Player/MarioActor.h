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

	virtual void getFrontVec(TVec3f *) const;
	virtual void getLastMove(TVec3f *) const;
	virtual TVec3f getLastMove() const;
	
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

	u8 _8c;
	//padding
	u8 _90[4];
	//u32 _90;
	u32 _94[0x40];
	u8 _98[0xc];
	//padding
	u8 _1a0;
	u8 _1a1;
	//padding
	f32 _1a4;
	u16 _1a8;
	u16 _1aa;
	f32 _1ac;
	Color8 _1b0;
	u8 _1b4;
	bool _1b5;
	//padding
	u8 _1b8[4];
	MarioMessenger* _1bc;
	bool _1c0;
	bool _1c1;
	bool _1c2;
	bool _1c3;
	u16 _1c4;
	u16 _1c6;
	f32 _1c8;
	f32 _1cc;
	u8 _1d0;
	u8 _1d1;
	f32 _1d4;
	FBO* _1d8;
	FBO* _1dc;
	bool _1e0;
	u8 _1e1;
	u8 _1e2;
	//padding
	f32 _1e4;
	u16 _1e8;
	//padding
	f32 _1ec;
	TVec3f _1f0;
	TVec3f _1fc;
	f32 _208;
	u8 _20c[4];
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
	TVec3f _24c;
	TVec3f _258;
	TVec3f _264;
	TVec3f _270;
	TVec3f _27c;
	TVec3f _288;
	TVec3f _294;
	TVec3f _2a0;
	TVec3f _2ac;
	TVec3f _2b8;
	TVec3f _2c4;
	TVec3f _2d0;
	TVec3f _2dc;
	TVec3f _2e8;
	TVec3f _2f4;
	TVec3f _300;
	TVec3f _30c;
	TVec3f _318;
	TVec3f _324;
	u16 _330;
	u16 _332;
	u16 _334;
	u16 _336;
	u16 _338;
	//padding
	TVec3f _33c;
	TVec3f _348;
	TVec3f _354;
	TVec3f _360;
	GravityInfo* _36c;
	u8 _370;
	//padding
	f32 _374;
	u16 _378;
	//padding
	u32 _37c;
	u32 mHealth;
	u32 _384;
	u32 _388;
	u16 _38c;
	u32 _390;
	u32 _394;
	u32 _398;;
	u8 _39c;
	u8 _39d;
	u8 _39e[0xa];
	u16 _3a8;
	u16 _3aa;
	u16 _3ac;
	//padding
	float _3b0;
	TVec3f _3b4;
	bool _3c0;
	bool _3c1;
	//padding
	TVec3f _3c4;
	u16 _3d0;
	u16 _3d2;
	u16 _3d4;
	u16 _3d6;
	u16 _3d8;
	u16 _3da;
	u16 _3dc;
	u8 _3de;
	u8 _3df;
	u32 mMaxHealth;
	bool _3e4;
	bool _3e5;
	bool _3e6;
	bool _3e7;
	bool _3e8;
	//padding
	TMtx34f _3ec;
	u32 _41c;
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
	u32 _47c;
	u8 _480;
	u8 _481;
	bool _482;
	u8 _483;
	TVec3f _484;
	f32 _490;
	u8 _494[4];
	FixedPosition* _498;
	FixedPosition* _49c;
	u32 _4a0;
	u32 _4a4;
	u32 _4a8;
	f32 _4ac;
	f32 _4b0;
	f32 _4b4;
	TVec3f _4b8;
	TVec3f _4c4;
	u32 _4c8[0x80];
	//padding
	u8 _6d0;
	f32 _6d4;
	f32 _6d8;
	u32 _6dc[0x40];
	u16 _7dc;
	u16 _7de;
	u16 _7e0;
	u8 _7e2;
	//padding
	u32 _7e4[0x40];
	u8 _8e4[0x40];
	HitSensor* _924;
	u32 _928;
	u32 _92c;
	u32 _930;
	bool _934;
	bool _935;
	//padding
	TVec3f _938;
	u8 _944;
	u8 _945;
	u16 _946;
	u16 _948;
	u16 _94a;
	u16 _94c;
	u16 _94e;
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
	u32 _98c;
	u8 _990;
	//padding
	u8 _991[0x13];
	MarioParts* _9a4;
	f32 _9a8;
	f32 _9ac;
	f32 _9b0;
	u16 _9b4;
	//padding
	u8 _9b6[0x16];
	f32 _9cc;
	f32 _9d0;
	u8 _9d4[4];
	TVec3f _9d8;
	u8 _9e4[0xc];
	bool _9f0;
	bool _9f1;
	u16 _9f2;
	TVec3f _9f4;
	u8 _a00[8];
	u8 _a08;
	u8 _a09;
	u8 _a0a;
	u8 _a0b;
	u8 _a0c;
	u8 _a10[8];
	TVec3f _a18;
	u8 _a24;
	u8 _a25;
	//padding
	u32 _a28[6];
	u8 _a40[0x18];
	u8 _a58;
	u8 _a59;
	u8 _a5a;
	u8 _a5b;
	u8 _a5c[4];
	bool _a60;
	bool _a61;
	bool _a62;
	//padding
	u32 _a64;
	f32 _a68;
	u16 _a6c;
	bool _a6e;
	//padding
	u32 _a70[8];
	u32 _a90[8];
	TMtx34f _ab0;
	TMtx34f _ae0;
	u16 _b10;
	u16 _b12;
	u16 _b14;
	//padding
	TVec3f _b18;
	f32 _b24;
	f32 _b28;
	f32 _b2c;
	f32 _b30;
	f32 _b34;
	f32 _b38;
	f32 _b3c;
	f32 _b40;
	u32 _b44;
	FootPrint* _b48;
	u32 _b4c;
	u16 _b50;
	//padding
	u32 _b54[3];
	u16 _b60;
	//padding
	u32 _b64;
	u8 _b68;
	//padding
	u16 _b6a;
	u32 _b6c;
	u16 _b70;
	u8 _b72;
	//padding
	u16 _b74;
	//padding
	u32 _b78;
	u32 _b7c;
	u32 _b80;
	u32 _b84;
	u16 _b88;
	//padding
	MarioNullBck* mNullAnimation;
	bool _b90;
	bool _b91;
	s8 _b92;
	//padding
	u16 _b94;
	u8 _b96;

	u32 _b98;
	
	u16 _b9c;
	u16 _b9e;
	u8 _ba0[0xc];
	TVec3f _bac;
	TVec3f _bb8;
	u16 _bc4;
	TMtx34f _bc8;
	TMtx34f _bf8;
	void* _c28;
	TMtx34f _c2c;
	TMtx34f _c5c;
	TMtx34f _c8c;
	TMtx34f _cbc;
	TMtx34f _cec;
	TMtx34f _d1c;
	TMtx34f _d4c;
	TMtx34f _d7c;
	TMtx34f _dac;
	TMtx34f _ddc;
	TMtx34f _e0c;
	TMtx34f _e3c;
	TMtx34f _e6c;
	u16 _e9c;
	f32 _ea0;
	bool _ea4;
	bool _ea5;
	bool _ea6;
	TMtx34f _ea8;
	TVec3f _ed8;
	u32 _ee4;
	bool mSuperKinokoCollected; // _ee8
	bool mPowerupCollected; // _ee9
	bool mTransforming; // _eea
	bool _eeb;
	bool _eec;
	bool _eed;
	bool _eee;
	//padding
	u16 _ef0;
	u16 _ef2;
	u16 _ef4;
	u16 _ef6;
	u8 _ef8[8];
	u8 _f00;
	//padding
	u32 _f04;
	u32 _f08;
	bool _f0c;
	u8 _f0d;
	u16 _f0e;
	u16 _f10;
	u16 _f12;
	u16 _f14;
	u16 _f16;
	u16 _f18;
	u16 _f1a;
	u16 _f1c;
	u16 _f1e;
	u8 _f20;
	u8 _f21;
	//padding
	u32 _f24;
	u16 _f28;
	//padding
	TVec3f _f2c;
	u8 _f38;
	//padding
	union {
		JAIAudible* _f3c;
		TVec3f* _f3c_vec;
	};
	u16 _f40;
	u16 _f42; 
	bool _f44;
	//padding
	u32 _f48;
	u32 _f4c;
	TVec3f _f50;
	TVec3f _f5c;
	TVec3f _f68;
	u8 _f74;
	//padding
	TVec3f _f78;
	TVec3f _f84;
	TVec3f _f90;
	TVec3f _f9c;
	TVec3f _fa8;
	const Nerve* _fb4;
	u16 _fb8;
	u8 _fbc[0x10];
	bool _fcc;
	bool _fcd;
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
