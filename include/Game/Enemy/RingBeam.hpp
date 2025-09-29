#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class RingBeamShadowDrawer : public ShadowVolumeDrawer {
public:
	RingBeamShadowDrawer(const LiveActor *);

	virtual ~RingBeamShadowDrawer();
    virtual void loadModelDrawMtx() const;
	virtual void drawShape() const;
    virtual bool isDraw() const;

	const LiveActor * _1c;
	f32 _20;
};

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
	void setRadius(f32 radius);
	void initPos(const LiveActor * actor);
	void startBrk(const char *);
	void exeSpread();

	LiveActor* _8c;
	ModelObj* mBloomModel;
	f32 mSpeed;
	s32 mLife;
	bool _9c;
	bool _9d;
	RingBeamShadowDrawer* _a0;
	ModelObj* _a4;
	TVec3f _a8;
	TVec3f _b4;
	TVec3f _c0;
	TVec3f _cc;
};
