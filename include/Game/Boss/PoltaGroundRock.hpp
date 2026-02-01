#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

class Polta;

class PoltaGroundRock : public LiveActor {
public:
	PoltaGroundRock(const char*);

	virtual void init(const JMapInfoIter& rIter);
	virtual void control();
	virtual void calcAndSetBaseMtx();
	virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

	void initBreakModel();
	void start(LiveActor*, const TVec3f&);
	void requestBreak();
	bool isEnableAttack() const NO_INLINE;
	bool isEnableBreak() const NO_INLINE;

	void exeSign();
	void exeJut();
	void exeWait();
	void exeBreak();

	LiveActor* mOwner;		// 0x8C
	ModelObj* mBreakModel;	// 0x90
	TQuat4f _94;			// 0x94
	TVec3f _A4;				// 0xA4
};
