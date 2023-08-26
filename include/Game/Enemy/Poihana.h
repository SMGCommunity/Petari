#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/Enemy/AnimScaleController.h"
#include "Game/Enemy/WalkerStateBindStarPointer.h"

#define POIHANA_BEHAVIOR_NORMAL 0
#define POIHANA_BEHAVIOR_SLEEP 1
#define POIHANA_BEHAVIOR_NEW_HOME 2

class ActorCameraInfo;

class Poihana : public LiveActor {
public:
	Poihana(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void initAfterPlacement();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPush(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void exeNonActive();
	void endNonActive();
	void exeWait();
	void exeWalkAround();
	void exeSleepStart();
	void exeSleep();
	void exeGetUp();
	void exeSearch();
	void exeChasePlayer();
	void exeShootUpCharge();
	void exeShootUp();
	void endShootUp() NO_INLINE;
	void exeGoBack();
	void exeShock();
	void exeSwoonLand();
	void exeSwoon();
	void exeRecover();
	void exeShake();
	void exeDrown();
	void exeHide();
	void exeAppear();
	void exeDPDSwoon();
	void endDPDSwoon() NO_INLINE;

	bool tryToStartBind(HitSensor *pSender);
	void updateBindActorMtx();
	void endBind();
	void startBound() NO_INLINE;
	void calcBound();
	void contactMario(HitSensor *pSender, HitSensor *pReceiver);
	void controlVelocity();
	void calcMyGravity();
	bool tryNonActive();
	bool tryDrown();
	bool tryDPDSwoon();
	bool tryShock();
	bool tryHipDropShock();
	bool isNerveTypeWalkOrWait() const;
	bool isNeedForBackHome() const;
	bool isNeedForGetUp() const;
	bool isBackAttack(HitSensor *pMySensor) const NO_INLINE;

	AnimScaleController* mAnimScaleCtrl; // 0x8C
	WalkerStateBindStarPointer* mBindStarPointer; // 0x90
	ActorCameraInfo* mCamInfo; // 0x94
	LiveActor* mBindedActor; // 0x98
	f32 mLaunchIntensity; // 0x9C
	TVec3f mRespawnPos; // 0xA0
	f32 _AC; // 0xAC
	f32 _B0; // 0xB0
	TVec3f mHomePos; // 0xB4
	f32 _C0; // 0xC0
	s32 mBoundTimer; // 0xC4
	s32 mRandDir; // 0xC8
	s32 mBehavior; // 0xCC
	bool mCanDrown; // 0xD0
	ModelObj* mWaterColumn; // 0xD4
	TVec3f mFrontVec; // 0xD8
	bool mIsActive; // 0xE4
	u8 _E5; // 0xE5
};

namespace NrvPoihana {
	NERVE_EXECEND(PoihanaNrvNonActive);
	NERVE(PoihanaNrvWait);
	NERVE(PoihanaNrvWalkAround);
	NERVE(PoihanaNrvSleepStart);
	NERVE(PoihanaNrvSleep);
	NERVE(PoihanaNrvGetUp);
	NERVE(PoihanaNrvSearch);
	NERVE(PoihanaNrvChasePlayer);
	NERVE(PoihanaNrvShootUpCharge);
	NERVE_EXECEND(PoihanaNrvShootUp);
	NERVE(PoihanaNrvGoBack);
	NERVE(PoihanaNrvShock);
	NERVE(PoihanaNrvSwoon);
	NERVE(PoihanaNrvSwoonLand);
	NERVE(PoihanaNrvRecover);
	NERVE(PoihanaNrvShake);
	NERVE(PoihanaNrvDrown);
	NERVE(PoihanaNrvHide);
	NERVE(PoihanaNrvAppear);
	NERVE_EXECEND(PoihanaNrvDPDSwoon);
};
