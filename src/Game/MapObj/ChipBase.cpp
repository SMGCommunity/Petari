#include "Game/MapObj/ChipBase.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/ChipHolder.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

namespace NrvChipBase {
    NEW_NERVE(ChipBaseNrvDeactive, ChipBase, Deactive);
    NEW_NERVE(ChipBaseNrvWait, ChipBase, Wait);
    NEW_NERVE(ChipBaseNrvControled, ChipBase, Controled);
    NEW_NERVE(ChipBaseNrvFlashing, ChipBase, Flashing);
    NEW_NERVE(ChipBaseNrvHide, ChipBase, Hide);
    NEW_NERVE(ChipBaseNrvGot, ChipBase, Got);
};  // namespace NrvChipBase

ChipBase::ChipBase(const char* pName, s32 chipType, const char* pChipName)
    : LiveActor(pName), mFlashingCtrl(nullptr), mRailMover(nullptr), mAirBubble(nullptr), mChipName(pChipName), mHost(nullptr),
      mClippingRange(0.0f, 0.0f, 0.0f), mGroupID(-1), mChipType(chipType), _B5(false) {}

void ChipBase::init(const JMapInfoIter& rIter) {
    MR::createChipHolder(mChipType);
    initJMapParam(rIter);
    initModel(rIter);
    initSensor();
    initShadow(rIter);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    initNerve(&NrvChipBase::ChipBaseNrvWait::sInstance);

    if (MR::isValidInfo(rIter)) {
        MR::setGroupClipping(this, rIter, 0x20);
    }

    mFlashingCtrl = new FlashingCtrl(this, true);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

/*
void ChipBase::initModel(const JMapInfoIter &rIter) {
    mScale.scaleInline(1.0f);
    initModelManagerWithAnm(mChipName, 0, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    if (ChipBase::isNeedBubble(rIter)) {
        mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "アワ", "AirBubble", 0);
        mAirBubble->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), 0);
        MR::startBck(mAirBubble, "Move", 0);
    }
}
*/

void ChipBase::initSensor() {
    f32 radius;
    f32 xScale = mScale.x;

    initHitSensor(1);

    if (mAirBubble != nullptr) {
        radius = 150.0f;
    } else {
        radius = 80.0f;
    }

    MR::addHitSensorEnemy(this, "body", 8, radius * xScale, TVec3f(0.0f, 0.0f, 0.0f));
}

#ifdef NON_MATCHING
void ChipBase::initShadow(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg2WithInit(rIter, &_B5);
    } else {
        _B5 = false;
    }

    s32 shadowShape = -1;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg5NoInit(rIter, &shadowShape);
    }

    if (shadowShape == 0) {
        MR::initShadowVolumeCylinder(this, 50.0f * mScale.x);
        _B5 = false;
    } else {
        MR::initShadowVolumeSphere(this, 50.0f * mScale.x);
    }

    f32 dropLength = 2000.0f;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg4NoInit(rIter, &dropLength);
    }
    MR::setShadowDropLength(this, 0, dropLength);

    if (mRailMover || _B5) {
        MR::onCalcShadowDropPrivateGravity(this, 0);
    } else {
        MR::onCalcShadowOneTime(this, 0);
        MR::onCalcShadowDropPrivateGravityOneTime(this, 0);
    }
}
#endif

void ChipBase::initJMapParam(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::getJMapInfoArg0NoInit(rIter, &mGroupID);

        if (MR::isConnectedWithRail(rIter)) {
            initRailRider(rIter);
            mRailMover = new MapPartsRailMover(this);
            mRailMover->init(rIter);
        }
    }
}

void ChipBase::initAfterPlacement() {
    MR::registerChip(mChipType, this, mGroupID);
    MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
}

void ChipBase::deactive() {
    makeActorDead();
    setNerve(&NrvChipBase::ChipBaseNrvDeactive::sInstance);
}

void ChipBase::setGroupID(s32 id) {
    mGroupID = id;
}

void ChipBase::setHost(LiveActor* pActor) {
    mHost = pActor;
}

void ChipBase::makeActorAppeared() {
    if (isNerve(&NrvChipBase::ChipBaseNrvDeactive::sInstance)) {
        return;
    }

    LiveActor::makeActorAppeared();

    if (mRailMover != nullptr) {
        mRailMover->start();
    }
}

void ChipBase::makeActorDead() {
    if (mRailMover != nullptr) {
        mRailMover->end();
    }

    LiveActor::makeActorDead();
}

void ChipBase::control() {
    if (mRailMover != nullptr) {
        mRailMover->movement();
        mPosition.x = mRailMover->_28.x;
        mPosition.y = mRailMover->_28.y;
        mPosition.z = mRailMover->_28.z;
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
    }
}

void ChipBase::appearWait() {
    if (isNerve(&NrvChipBase::ChipBaseNrvDeactive::sInstance)) {
        return;
    }

    makeActorAppeared();
    MR::validateClipping(this);
    setNerve(&NrvChipBase::ChipBaseNrvWait::sInstance);
}

void ChipBase::appearFlashing(s32 a1) {
    if (isNerve(&NrvChipBase::ChipBaseNrvDeactive::sInstance)) {
        return;
    }

    appear();
    MR::invalidateClipping(this);
    mFlashingCtrl->start(a1);
    setNerve(&NrvChipBase::ChipBaseNrvFlashing::sInstance);
}

bool ChipBase::requestGet(HitSensor* pSender, HitSensor* pReceiver) {
    if (isGettable()) {
        MR::noticeGetChip(mChipType, this, mGroupID);
        setNerve(&NrvChipBase::ChipBaseNrvGot::sInstance);

        if (mHost != nullptr) {
            mHost->receiveMessage(0x87, pSender, pReceiver);
        }

        return true;
    }

    return false;
}

bool ChipBase::requestShow() {
    if (isNerve(&NrvChipBase::ChipBaseNrvHide::sInstance)) {
        MR::startBck(this, "Wait", 0);
        MR::showModel(this);
        setNerve(&NrvChipBase::ChipBaseNrvWait::sInstance);

        return true;
    }

    return false;
}

bool ChipBase::requestHide() {
    if (isGettable()) {
        MR::invalidateHitSensors(this);
        MR::hideModel(this);
        MR::stopBck(this);
        MR::forceDeleteEffectAll(this);
        setNerve(&NrvChipBase::ChipBaseNrvHide::sInstance);

        return true;
    }

    return false;
}

bool ChipBase::requestStartControl() {
    if (isNerve(&NrvChipBase::ChipBaseNrvWait::sInstance)) {
        setNerve(&NrvChipBase::ChipBaseNrvControled::sInstance);

        return true;
    }

    return false;
}

bool ChipBase::requestEndControl() {
    if (isNerve(&NrvChipBase::ChipBaseNrvControled::sInstance)) {
        setNerve(&NrvChipBase::ChipBaseNrvWait::sInstance);

        return true;
    }

    return false;
}

void ChipBase::exeDeactive() {}

void ChipBase::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        MR::validateHitSensors(this);
    }
}

void ChipBase::exeControled() {
    MR::zeroVelocity(this);
}

void ChipBase::exeFlashing() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        MR::validateHitSensors(this);
    }

    if (mFlashingCtrl->mIsEnded) {
        kill();
    }
}

void ChipBase::exeHide() {}

#ifdef NON_MATCHING
// mAirBubble load isn't loading twice
void ChipBase::exeGot() {
    if (MR::isFirstStep(this)) {
        PartsModel* mdl = mAirBubble;
        if (mdl != 0) {
            MR::emitEffect(mdl, "RecoveryBubbleBreak");
            MR::incPlayerOxygen(8);
            mAirBubble->kill();
        }

        MR::emitEffect(this, "Get");
        MR::tryRumblePadMiddle(this, 0);

        if (!mChipType) {
            MR::startSystemSE("SE_SY_BLUECHIP_GET", MR::getGotChipCount(mChipType, mGroupID), -1);
        } else if (mChipType == 1) {
            MR::startSystemSE("SE_SY_YELLOWCHIP_GET", MR::getGotChipCount(mChipType, mGroupID), -1);
        }
    }

    kill();
}
#endif

bool ChipBase::isGettable() const {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::isLessStep(this, 40)) {
        return false;
    }

    return isNerve(&NrvChipBase::ChipBaseNrvWait::sInstance) || isNerve(&NrvChipBase::ChipBaseNrvFlashing::sInstance);
}

bool ChipBase::isNeedBubble(const JMapInfoIter& rIter) {
    if (!MR::isValidInfo(rIter)) {
        return false;
    }

    bool arg3 = false;
    MR::getJMapInfoArg3WithInit(rIter, &arg3);

    return arg3;
}

void ChipBase::makeArchiveList(NameObjArchiveListCollector* pList, const JMapInfoIter& rIter) {
    if (ChipBase::isNeedBubble(rIter)) {
        pList->addArchive("AirBubble");
    }
}
