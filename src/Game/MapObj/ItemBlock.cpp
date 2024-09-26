#include "Game/MapObj/ItemBlock.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvItemBlock {
	INIT_NERVE(ItemBlockNrvStandby);
	INIT_NERVE(ItemBlockNrvWait);
	INIT_NERVE(ItemBlockNrvAppearItem);
	INIT_NERVE(ItemBlockNrvAppearItemSplash);

	void ItemBlockNrvAppearItemSplash::execute(Spine *pSpine) const {
		ItemBlock *block = reinterpret_cast<ItemBlock *>(pSpine->mExecutor);
		block->exeAppearItemSplash();
	}

	void ItemBlockNrvAppearItem::execute(Spine *pSpine) const {
		ItemBlock *block = reinterpret_cast<ItemBlock *>(pSpine->mExecutor);
		block->exeAppearItem();
	}

	void ItemBlockNrvWait::execute(Spine *pSpine) const {
		ItemBlock *block = reinterpret_cast<ItemBlock *>(pSpine->mExecutor);
		block->exeWait();
	}

	void ItemBlockNrvStandby::execute(Spine *pSpine) const {
		ItemBlock *block = reinterpret_cast<ItemBlock *>(pSpine->mExecutor);
		if (MR::isFirstStep(block)) {
			MR::startBck(block, "Wait", nullptr);
		}
	}
};

namespace {
	static Color8 hPointLight(0xFF, 0xE6, 0, 0xFF);
};

ItemBlock::ItemBlock(const char *pName) : LiveActor(pName) {
	mKind = 0;
	mTimer = 0;
	mItemCount = 8;
	_98 = 0;
	mCalcShadowOneTime = false;
}

void ItemBlock::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	checkKind(rIter);
	initBlock();
	MR::connectToSceneMapObjStrongLight(this);
	MR::useStageSwitchSleep(this, rIter);
	initEffectKeeper(1, "CoinBlock", false);
	initSound(4, false);
	initNerve(&NrvItemBlock::ItemBlockNrvStandby::sInstance);
	s32 arg = -1;
	MR::getJMapInfoArg4NoInit(rIter, &arg);
	MR::initShadowVolumeBox(this, TVec3f(150.0f, 150.0f, 150.0f));
	MR::setShadowVolumeStartDropOffset(this, 0, 77.0f);

	if (arg != -1) {
		MR::setShadowDropLength(this, nullptr, arg);
	}

	MR::onShadowVolumeCutDropLength(this, nullptr);
	MR::excludeCalcShadowToMyCollision(this, nullptr);

	if (mCalcShadowOneTime) {
		MR::onCalcShadowOneTime(this, nullptr);
	} else {
		MR::onCalcShadow(this, nullptr);
	}

	if (MR::isZeroGravity(this)) {
		TVec3f upVec;
		MR::calcUpVec(&upVec, this);
		MR::setShadowDropDirection(this, 0, -upVec);
	} else {
		MR::calcGravity(this);
	}

	MR::offCalcGravity(this);
	MR::setGroupClipping(this, rIter, 16);
	MR::validateClipping(this);

	appear();
}

void ItemBlock::startClipped() {
	LiveActor::startClipped();
	MR::forceDeleteEffect(this, "Glow");
}

void ItemBlock::endClipped() {
	LiveActor::endClipped();
	MR::emitEffect(this, "Glow");
}

void ItemBlock::checkKind(const JMapInfoIter &rIter) {
	const char *name = nullptr;
	MR::getObjectName(&name, rIter);

	if (MR::isEqualString(name, "ItemBlockSwitch")) {
		MR::needStageSwitchWriteDead(this, rIter);
		mKind = 5;
	} else if (MR::isEqualString(name, "TimerCoinBlock")) {
		mKind = 1;
		MR::declareCoin(this, 10);
	} else if (MR::isEqualString(name, "SplashCoinBlock")) {
		mKind = 2;
		MR::declareCoin(this, mItemCount);
	} else if (MR::isEqualString(name, "TimerPieceBlock")) {
		mKind = 3;
		MR::declareStarPiece(this, 20);
	} else if (MR::isEqualString(name, "SplashPieceBlock")) {
		mKind = 4;
		MR::declareStarPiece(this, mItemCount);
	}

	s32 arg = -1;

	if (mKind == 2 || mKind == 4) {
		arg = -1;
		MR::getJMapInfoArg0NoInit(rIter, &arg);

		if (arg >= 0) {
			mItemCount = arg;
		} else {
			mItemCount = 8;
		}
	}

	if (mKind == 1 || mKind == 3) {
		MR::getJMapInfoArg1NoInit(rIter, &arg);

		if (arg == -1) {
			mTimer = 240;
		} else {
			mTimer = arg;
		}
	}

	arg = -1;
	MR::getJMapInfoArg2NoInit(rIter, &arg);
	mCalcShadowOneTime = !(arg - 1);
}

/*void ItemBlock::initBlock() {
	initHitSensor(1);

	if (mKind >= 6 || mKind < 1) {
		return;
	}

	initModelManagerWithAnm("CoinBlock", nullptr, false);
	TVec3f *mtx = new TVec3f(0.0f, 100.0f, 0.0f);
	MR::addHitSensorMtxMapObj(this, "body", 8, 100.0f, MR::getJointMtx(this, "CoinBlock"), *mtx);
	MR::initCollisionPartsAutoEqualScale(this, "CoinBlock", getSensor("body"), (MtxPtr)mtx);
}*/

void ItemBlock::appear() {
	LiveActor::appear();
	initNerve(&NrvItemBlock::ItemBlockNrvStandby::sInstance);

	if (mKind > 6 || mKind < 1) {
		return;
	}

	MR::startBrk(this, "CoinBlock");
}

void ItemBlock::kill() {
	MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
	MR::emitEffect(this, "Break");
	LiveActor::kill();
}

/*void ItemBlock::control() {
	TVec3f upVec;
	MR::calcUpVec(&upVec, this);
	MR::requestPointLight(this, TVec3f(upVec * 100.0f), hPointLight, 0.998646f, -1);
}*/

bool ItemBlock::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
	if (MR::isMsgPlayerUpperPunch(msg) && MR::isPlayerExistDown(this, 0.0f, 0.25f) && tryStartJumpPunch()) {
		MR::sendArbitraryMsg(64, a1, a2);
		return true;
	}

	return false;
}

bool ItemBlock::tryStartJumpPunch() {
	if (isNerve(&NrvItemBlock::ItemBlockNrvStandby::sInstance)) {
		if (mKind == 1 || mKind == 3) {
			MR::startBck(this, "Appear", nullptr);
			calcAnim();
			mNoCalcAnim = true;
			setNerve(&NrvItemBlock::ItemBlockNrvAppearItem::sInstance);
		} else if (mKind == 2 || (u32)(mKind - 4) <= 1) {
			setNerve(&NrvItemBlock::ItemBlockNrvAppearItemSplash::sInstance);
		}

		return true;
	}

	if (isNerve(&NrvItemBlock::ItemBlockNrvWait::sInstance)) {
		if (mKind == 1 || mKind == 3) {
			if (mKind == 1) {
				if (MR::getDeclareRemnantCoinCount(this) == 1) {
					mTimer = 0;
				}
			} else {
				if (!(MR::getDeclareRemnantStarPieceCountNotExist(this) - 11)) {
					mTimer = 0;
				}
			}

			MR::startBck(this, "Appear", nullptr);
			calcAnim();
			mNoCalcAnim = true;
			setNerve(&NrvItemBlock::ItemBlockNrvAppearItem::sInstance);

			return true;
		}
	}

	return false;
}

void ItemBlock::exeWait() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Wait", nullptr);
		MR::invalidateClipping(this);
	}

	decTimer();
}

/*void ItemBlock::exeAppearItem() {
	if (MR::isFirstStep(this)) {
		if (!mTimer) {
			MR::forceDeleteEffect(this, "Glow");
			MR::startBck(this, "Bomb", nullptr);
		}

		MR::invalidateClipping(this);
	}

	if (MR::isStep(this, 1)) {
		TVec3f gravVec;
		TVec3f position(mPosition);
		MR::calcGravityVector(this, &gravVec, nullptr, 0);

		position.subInline(mPosition, gravVec * 100.0f);

		if (mKind == 2) {
			MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
		}

		if (mKind >= 1) {
			MR::hopCoin(this, position, -gravVec);
		}

		if (mKind >= 4) {
		} else {
			if (mTimer || MR::getDeclareRemnantStarPieceCountNotExist(this) != 11) {
				MR::appearStarPieceToDirection(this, position, -gravVec, 11, 10.0f, 40.0f, false);
			} else {
				MR::hopStarPiece(this, position, -gravVec);
			}

			MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
		}
	}

	decTimer();

	if (!MR::isBckStopped(this)) {
		return;
	}

	if (mTimer) {
		setNerve(&NrvItemBlock::ItemBlockNrvWait::sInstance);
	} else {
		kill();
	}
}*/

void ItemBlock::exeAppearItemSplash() {
	if (MR::isFirstStep(this)) {
		MR::forceDeleteEffect(this, "Glow");
		MR::startBck(this, "Bomb", nullptr);
	}

	if (!MR::isBckStopped(this)) {
		return;
	}

	if (mKind == 2) {
		if (mItemCount == 1) {
			MR::appearCoinPop(this, mPosition, 1);
		} else {
			MR::appearCoinCircle(this, mPosition, mItemCount);
		}
	} else if (mKind == 4) {
		MR::appearStarPiece(this, mPosition, mItemCount, 10.0f, 40.0f, false);
		MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
	} else if (mKind == 5) {
		MR::onSwitchDead(this);
	}

	mTimer = 0;
	kill();
}

void ItemBlock::decTimer() {
	--mTimer;
	if (mTimer > 0) {
		return;
	}
	mTimer = 0;
}

void ItemBlock::calcAnim() {
	if (!mNoCalcAnim) {
		LiveActor::calcAnim();
	}
	mNoCalcAnim = false;
}

ItemBlock::~ItemBlock() {

}
