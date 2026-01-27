#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PoltaSensorCtrl;
class PoltaArm;

class Polta : public LiveActor {
public:
	Polta(const char*);

	virtual ~Polta();
	virtual void init(const JMapInfoIter& rIter);
	virtual void calcAnim();
	virtual void makeActorAppeared();
	virtual void kill();
	virtual void makeActorDead();
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

	void initArm();
	void initPartsModel();
	void initSensor();
	void initBombTeresaHolder();
	void initCamera();
	void appearBreakModel(const TVec3f&, const char*);
	void appearBreakModelFirst(const TVec3f&);
	void appearBreakModelMiddle(const TVec3f&);
	void appearBreakModelLast(const TVec3f&);
	void appearStarPiece(s32);
	bool isEndBreakModel() const;
	void killBreakModel();
	void rotateToPlayer();
	void calcToPlayerRotateSpeed() const;
	void updateAction();
	void updatePose(f32, f32);
	void start();
	void setStartPose();
	PoltaSensorCtrl* mSensorCtrl;             // 0x8C
	PoltaArm* mLeftArm;                       // 0x90
	PoltaArm* mRightArm;                      // 0x94
};
