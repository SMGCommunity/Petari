#pragma once

#include "Game/System/NerveExecutor.hpp"

class Polta;
class PoltaActionBase;
class PoltaDemo;
class PoltaWaitStart;
class PoltaBattleLv1;
class PoltaBattleLv2;
class JMapInfoIter;
class HitSensor;

class PoltaActionSequencer : public NerveExecutor {
public:
	PoltaActionSequencer(Polta*, const JMapInfoIter &);

	void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
	bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

	bool updateAction() NO_INLINE; 
	bool startAction();

	void exeWaitStart();
	void exeOpeningDemo();
	void exeBattleLv1();
	void exePowerUpDemo();
	void exeBattleLv2();
	void exeDownDemo();


	Polta* mPoltaPtr; // 0x08
	PoltaDemo* mActionDemo; // 0x0C
	PoltaActionBase* mCurrentAction; // 0x10
	PoltaWaitStart* mActionWaitStart; // 0x14
	PoltaBattleLv1* mActionPoltaBattleLv1; // 0x18
	PoltaBattleLv2* mActionPoltaBattleLv2; // 0x1C
};
