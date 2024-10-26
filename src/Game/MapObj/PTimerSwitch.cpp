#include "Game/MapObj/PTimerSwitch.hpp"
#include "Game/Map/CollisionParts.hpp"

PTimerSwitch::PTimerSwitch(const char *pName) : LiveActor(pName) {
	mSwitchCollision = nullptr;
	mConnector = nullptr;
	_94 = 0;
	_98 = false;
}

void PTimerSwitch::reset() {
	LiveActor::appear();
	setNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance);
	MR::validateCollisionParts(mSwitchCollision);
	MR::startBck(this, "Wait", 0);
	_94 = 0;
	_98 = false;
}

void PTimerSwitch::init(const JMapInfoIter& rIter) {
	MR::initDefaultPos(this, rIter);
	initModelManagerWithAnm("PTimerSwitch", nullptr, false);
	MR::connectToSceneMapObj(this);
	initHitSensor(2);
	MR::addHitSensorMapObj(this, "body", 0x10, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
	MR::addHitSensor(this, "hit", 0x4f, 0x10, 120.0f, TVec3f(0.0f, 50.0f, 0.0f));
	MR::initCollisionParts(this, "PTimerSwitch", getSensor("body"), 0);
	mSwitchCollision = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), (MR::CollisionScaleType)2);
	initSound(4, false);
	MR::needStageSwitchWriteA(this, rIter);
	initNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance);
	initEffectKeeper(0, nullptr, false);
	MR::startBck(this, "Move", 0);
	mConnector = new MapObjConnector(this);
	appear();
}

void PTimerSwitch::initAfterPlacement() {
	mConnector->attachToUnder();
}

void PTimerSwitch::appear() {
	reset();
}

void PTimerSwitch::kill() {
	LiveActor::kill();
	MR::emitEffect(this, "Delete");
	MR::invalidateCollisionParts(mSwitchCollision);
}

void PTimerSwitch::calcAnim() {
	LiveActor::calcAnim();
	TPos3f mtx;
	mtx.setInline(MR::getJointMtx(this, "Move"));
	mSwitchCollision->setMtx(mtx);
}

void PTimerSwitch::control() {
	if (_98) {
		++_94;
	} else if (_94 > 0) {
		--_94;
	}

	_98 = false;
}

void PTimerSwitch::calcAndSetBaseMtx() {
	LiveActor::calcAndSetBaseMtx();
	mConnector->connect();
}

bool PTimerSwitch::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	return false;
}

bool PTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (!MR::isMsgPlayerTrample(msg)) {
		return false;
	}

	if (pReceiver->mSensorType != 0x4f) {
		return false;
	}

	if (!isNerve(&NrvPTimerSwitch::PTimerSwitchNrvOff::sInstance)) {
		return false;
	}

	_98 = true;
	return true;
}

bool PTimerSwitch::trySwitchDown() {
	if (_94 > 0) {
		setNerve(&NrvPTimerSwitch::PTimerSwitchNrvSwitchDown::sInstance);
		return true;
	}

	return false;
}

bool PTimerSwitch::tryOn() {
	if (MR::isBckStopped(this)) {
		setNerve(&NrvPTimerSwitch::PTimerSwitchNrvOn::sInstance);
		return true;
	}

	return false;
}

void PTimerSwitch::exeOff() {
	if (MR::isFirstStep(this)) {
		MR::validateClipping(this);
		MR::offSwitchA(this);
	}

	if (!trySwitchDown()) {
		return;
	}
}

void PTimerSwitch::exeSwitchDown() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "On", nullptr);
		MR::invalidateClipping(this);
	}

	if (!tryOn()) {
		return;
	}
}

void PTimerSwitch::exeOn() {
	if (MR::isFirstStep(this)) {
		MR::onSwitchA(this);
		MR::shakeCameraNormal();
		MR::startSound(this, "SE_OJ_SWITCH_1", -1, -1);
	}

	if (MR::isGreaterStep(this, 25)) {
		kill();
	}
}

namespace NrvPTimerSwitch {
	INIT_NERVE(PTimerSwitchNrvOn);
	INIT_NERVE(PTimerSwitchNrvSwitchDown);
	INIT_NERVE(PTimerSwitchNrvOff);

	void PTimerSwitchNrvOn::execute(Spine *pSpine) const {
		PTimerSwitch *actor = reinterpret_cast<PTimerSwitch *>(pSpine->mExecutor);
		actor->exeOn();
	}

	void PTimerSwitchNrvSwitchDown::execute(Spine *pSpine) const {
		PTimerSwitch *actor = reinterpret_cast<PTimerSwitch *>(pSpine->mExecutor);
		actor->exeSwitchDown();
	}

	void PTimerSwitchNrvOff::execute(Spine *pSpine) const {
		PTimerSwitch *actor = reinterpret_cast<PTimerSwitch *>(pSpine->mExecutor);
		actor->exeOff();
	}
}
