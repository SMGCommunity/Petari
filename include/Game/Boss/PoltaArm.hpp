#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class PoltaArm : public ModelObj {
public:
	PoltaArm(const char* pName, const char* pModelName, MtxPtr pMtx);

	virtual void init(const JMapInfoIter& rIter);
	virtual void appear();
	virtual void makeActorDead();
	virtual void control();

	void initPartsModel();
	void exeDamage();
	void exeBreak();
	void exeBreakBody();
	void exeRepair();
    void endRepair();
	void exeBroken();
    void exeControlled();
    void exeWaitDamageEnd();
    void exeWaitRepairEnd();
	bool isEnableHitSensor() const;
	bool isEnableAttack() const;
	bool isBroken() const;
	void start();
	bool requestBreakBody();
	bool requestDamage();
	bool requestStartControll();
	bool requestControlled(const char*);
	void appearBreakModel();

	ModelObj* mFormationModel; // 0x90
	ModelObj* mBreakModel;     // 0x94
	TPos3f _98;
	s32 _C8;
	s32 _CC;
	s32 mBrokenCounter;        // 0xD0
	s32 _D4;                   // 0xD4
	bool mIsLeftArm;           // 0xD8
};
