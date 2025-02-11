#include "Game/MapObj/SwitchBox.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TVec.hpp"

SwitchBox::SwitchBox(const char *pName) : LiveActor(pName) {

}

void SwitchBox::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("SwitchBox", nullptr, false);
	MR::connectToSceneMapObjNoCalcAnim(this);
	initSound(2, false);
	initNerve(&NrvSwitchBox::SwitchBoxNrvWait::sInstance);
	initEffectKeeper(3, nullptr, false);
	f32 newScale = mScale.x * 90.0f;
	initHitSensor(2);
	TVec3f sensorOffset;
	sensorOffset.set(0.0f, newScale, 0.0f);
	MR::addHitSensorMapObj(this, "body", 8, newScale, sensorOffset);

	s32 local_38 = 1;

	if (MR::checkJMapDataEntries(rIter)) {
		MR::getJMapInfoArg1NoInit(rIter, &local_38);
	}

	setNerve(&NrvSwitchBox::SwitchBoxNrvWait::sInstance);
	MR::initCollisionParts(this, "SwitchBox", getSensor("body"), nullptr);
	MR::validateClipping(this);

	_92 = 0;

	if (local_38) {
		_92 = 1;
	}

	if (_92) {
		MR::initShadowVolumeBox(this, TVec3f(150.0f, 150.0f, 150.0f));
		MR::setShadowVolumeStartDropOffset(this, nullptr, 77.0f);
		MR::setShadowVolumeEndDropOffset(this, nullptr, 150.0f);
		MR::excludeCalcShadowToMyCollision(this, nullptr);
		MR::onShadowVolumeCutDropLength(this, nullptr);
		MR::onCalcGravity(this);
		MR::onCalcShadowOneTime(this, nullptr);
	}

	MR::addToAttributeGroupSearchTurtle(this);

	if (MR::useStageSwitchReadAppear(this, rIter)) {
		MR::syncStageSwitchAppear(this);
	}

	MR::needStageSwitchWriteDead(this, rIter);
	appear();
}

void SwitchBox::initAfterPlacement() {

}

void SwitchBox::exeWait() {
	if (_8c != 0) {
		--_8c;
	}

	if (_8e != 0) {
		--_8e;
		if (_8e == 0) {
			_90 = 1;
		}
	}

	if (_90 == 0) {
		setNerve(&NrvSwitchBox::SwitchBoxNrvHit::sInstance);
	}
}

void SwitchBox::exeHit() {
	if (getNerveStep() == 0) {
		getSensor("body")->invalidate();
	}

	if (getNerveStep() == 5) {
		MR::hideModel(this);
	}

	if (getNerveStep() == 15) {
		kill();
	}
}

void SwitchBox::appear() {
	MR::offSwitchDead(this);

	_90 = 1;
	_8e = 0;
	_8c = 0;

	LiveActor::appear();
	MR::showModel(this);
	setNerve(&NrvSwitchBox::SwitchBoxNrvWait::sInstance);
}

void SwitchBox::kill() {
	MR::onSwitchDead(this);
	LiveActor::kill();
}

void SwitchBox::doHit(HitSensor *pSender, HitSensor *pReceiver) {
	if (_90 != 0) {
		--_90;
	}

	if (_90 == 0) {
		MR::invalidateCollisionParts(this);
	}

	_8e = 30;

	if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
		MR::startSound(this, "SE_OJ_S_BLOCK_BREAK_W", -1, -1);
		MR::emitEffect(this, "BreakWater");
	} else {
		MR::startSound(this, "SE_OJ_S_BLOCK_BREAK", -1, -1);
		MR::emitEffect(this, "Break");
	}
}

bool SwitchBox::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgPlayerUpperPunch(msg) && _90 != 0) {
		doHit(pSender, pReceiver);
		pSender->receiveMessage(0x40, pReceiver);
		return true;
	}

	if (MR::isMsgPlayerHitAll(msg) && _90 != 0) {
		doHit(pSender, pReceiver);
		return true;
	}

	if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerHipDropFloor(msg)) {
		if (_8c != 0) {
			doHit(pSender, pReceiver);
			return true;
		}

		return true;
	}

	return false;
}

bool SwitchBox::receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (_90 != 0) {
		doHit(pSender, pReceiver);
		return true;
	}

	return false;
}

bool SwitchBox::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (msg == 0x1b) {
		return _90;
	}

	if (msg == 0x41) {
		return !_90;
	}

	if (MR::isMsgFloorTouch(msg)) {
		_8c = 2;
	}

	return false;
}

namespace NrvSwitchBox {
	INIT_NERVE(SwitchBoxNrvWait);
	INIT_NERVE(SwitchBoxNrvHit);

	void SwitchBoxNrvHit::execute(Spine *pSpine) const {
		SwitchBox *pActor = reinterpret_cast<SwitchBox *>(pSpine->mExecutor);
		pActor->exeHit();
	}

	void SwitchBoxNrvWait::execute(Spine *pSpine) const {
		SwitchBox *pActor = reinterpret_cast<SwitchBox *>(pSpine->mExecutor);
		pActor->exeWait();
	}
};
