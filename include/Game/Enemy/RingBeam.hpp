#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RingBeam : public LiveActor {
public:
	RingBeam(const char *, LiveActor *, bool , bool);

	virtual ~RingBeam();
	virtual void init(const JMapInfoIter &rIter);
	virtual void appear();
	virtual void kill();
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void setSpeed(f32 speed);
	void setLife(s32 life);
	void setColor(f32 color);
};