#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Player/Mario.h"
#include "Game/Player/MarioConst.h"
#include "Game/Player/MarioEffect.h"
#include "Game/Player/MarioAnimator.h"
#include "Game/Player/MarioShadow.h"
#include "Game/Player/MarioMessenger.h"

class FootPrint;
class JAIAudible;

namespace MR {
	unsigned int getFrameBufferWidth();
}

class MarioActor : LiveActor {
public:
	MarioActor(const char*);
	~MarioActor();
	void init(const JMapInfoIter&);
	void init2(const TVec3f&, const TVec3f&, long);
	void initAfterPlacement();
	void initAfterOpeningDemo();
	void calcBaseFrontVec(const TVec3f&);
	void playSound(const char*, int);
	void changeAnimation(const char*, const char*);
	void changeAnimationNonStop(const char*);
	void changeAnimationUpper(const char*);
	void stopAnimation(const char*);
	bool isAnimationRun(const char*) const;
	void changeNullAnimation(const char*, signed char);
	void clearNullAnimation(signed char);
	bool isStopNullAnimation() const;
	void changeGameOverAnimation();
	void getJointCtrl(const char*) const;
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
	void doRush();
	void updateSwingTimer();
	void updateSwingAction();
	void checkClapCatchStart();
	void updateRealMtx();
	void setUpperRotateY(f32);
	void decLife(unsigned short);
	void decLifeLarge();
	void decLifeMiddle();
	void decLifeSmall();
	void resetWaterLife();
	void updateLife();
	void incLife(unsigned int);
	void changeMaxLife(int);
	void doPressing();
	void careMovingObject();
	void doStun();
	void scaleMtx(f32* [4]);
	void updateBaseScaleMtx();
	void getRealMtx(f32* [4], const char*);
	void getGlobalJointMtx(const char*);
	void calcAnimInMovement();
	void forceSetBaseMtx(f32* [4]);
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
