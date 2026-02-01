#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;

class PoltaActionBase : public ActorStateBase<Polta> {
public:
	PoltaActionBase(const char*, Polta*);

	virtual ~PoltaActionBase();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool updateWait();
    bool updateDamageBody(bool);
    bool updateBreakBody();
    bool updateDamageCore();
    void updateScreamSensor();

    Polta* mPoltaPtr;
    s32 _10;
};
