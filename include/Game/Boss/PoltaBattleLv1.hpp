#pragma once

#include "Game/Boss/PoltaActionBase.hpp"


class PoltaStateStagger;
class PoltaStateGroundRockAttack;
class PoltaStateGenerateRock;

class PoltaBattleLv1 : public PoltaActionBase {
public:
	PoltaBattleLv1(Polta*);

	virtual void appear();
	virtual void control();
	virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
	virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

	bool isEnableSensor(const HitSensor*) const;
	bool isEnableDamage() const;
	bool isEnableScream() const NO_INLINE;

	void exeWait();
	void exeBreakBody();
	void exeStagger();
	void exeDamageCore();
	void addDamageBody();
	inline void exeGenerateRock();
	inline void exeGenerateGroundRock();
	inline void exeDamageBody();
	inline void endStagger();

	PoltaStateStagger* mStateStagger; // 0x14
	PoltaStateGenerateRock* mStateGenerateRock; // 0x18
	PoltaStateGroundRockAttack* mStateGroundRockAttack; // 0x1C
	s32 mPoltaHealth; // 0x20
	s32 _24; // 0x24
};
