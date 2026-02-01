#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"

class PoltaSensorCtrl;
class PoltaArm;
class PoltaActionSequencer;
class PoltaRockHolder;
class PoltaGroundRockHolder;
class BombTeresa;

class BombTeresaHolder : public DeriveActorGroup<BombTeresa> {
public:
	BombTeresaHolder();

	virtual ~BombTeresaHolder();

	void disperseAll();
};

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
	PoltaActionSequencer* mSequencer;         // 0x98
	BombTeresaHolder* mBombTeresaHolder;      // 0x9C
	PoltaRockHolder* mRockHolder;             // 0xA0
	PoltaGroundRockHolder* mGroundRockHolder; // 0xA4
	ActorCameraInfo* mCameraInfo;             // 0xA8
	ModelObj* mBreakModel;                    // 0xAC
	ModelObj* mFormationModel;                // 0xB0
	TQuat4f _B4;                              // 0xB4
	TVec3f _C4;                               // 0xC4
	TQuat4f _D0;                              // 0xD0
	TVec3f _E0;                               // 0xE0
	f32 _E4;                                  // 0xE4
};
