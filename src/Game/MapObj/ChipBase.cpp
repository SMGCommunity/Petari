#include "Game/MapObj/ChipBase.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/ChipHolder.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void ChipBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const f32 sBubbleHitRadius = 150.0f;
    static const f32 sBodyHitRadius = 80.0f;
    static const f32 sShadowRadius = 50.0f;
    static const f32 sBaseScale = 1.0f;
    // static const f32 sLife = 0.0f;
    static const s32 sCannotGetTime = 40;
    static const f32 sBodyHitYOffset = 0.0f;
};  // namespace

namespace NrvChipBase {
    NEW_NERVE(ChipBaseNrvDeactive, ChipBase, Deactive);
    NEW_NERVE(ChipBaseNrvWait, ChipBase, Wait);
    NEW_NERVE(ChipBaseNrvControled, ChipBase, Controled);
    NEW_NERVE(ChipBaseNrvFlashing, ChipBase, Flashing);
    NEW_NERVE(ChipBaseNrvHide, ChipBase, Hide);
    NEW_NERVE(ChipBaseNrvGot, ChipBase, Got);
};  // namespace NrvChipBase

ChipBase::ChipBase(const char* pName, s32 chipType, const char* pChipName)
    : LiveActor(pName), mFlashingCtrl(), mRailMover(), mAirBubble(), mChipName(pChipName), mHost(), mClippingRange(0.0f, 0.0f, 0.0f), mGroupID(-1),
      mChipType(chipType), mIsCalcShadow() {
}

void ChipBase::init(const JMapInfoIter& rIter) {
    MR::createChipHolder(mChipType);

    initJMapParam(rIter);
    initModel(rIter);
    initSensor();
    initShadow(rIter);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(GET_NERVE(ChipBase, ChipBaseNrvWait));

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

void ChipBase::initModel(const JMapInfoIter& rIter) {
    mScale *= ::sBaseScale;

    initModelManagerWithAnm(mChipName, 0, false);

    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    if (isNeedBubble(rIter)) {
        mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "アワ", "AirBubble", 0);
        mAirBubble->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), 0);
        MR::startBck(mAirBubble, "Move");
    }
}

void ChipBase::initSensor() {
    f32 radius;
    f32 scale = mScale.x;

    initHitSensor(1);

    if (mAirBubble != nullptr) {
        radius = ::sBubbleHitRadius;
    } else {
        radius = ::sBodyHitRadius;
    }

    MR::addHitSensorEnemy(this, "body", 8, radius * scale, TVec3f(0.0f, ::sBodyHitYOffset, 0.0f));
}

void ChipBase::initShadow(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg2WithInit(rIter, &mIsCalcShadow);
    } else {
        mIsCalcShadow = false;
    }

    s32 shadowShape = -1;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg5NoInit(rIter, &shadowShape);
    }

    switch (shadowShape) {
    case 0:
        MR::initShadowVolumeCylinder(this, ::sShadowRadius * mScale.x);
        mIsCalcShadow = false;

        break;
    default:
        MR::initShadowVolumeSphere(this, ::sShadowRadius * mScale.x);

        break;
    }

    f32 dropLength = 2000.0f;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg4NoInit(rIter, &dropLength);
    }

    MR::setShadowDropLength(this, 0, dropLength);

    if (mRailMover || mIsCalcShadow) {
        MR::onCalcShadowDropPrivateGravity(this, 0);
    } else {
        MR::onCalcShadowOneTime(this, 0);
        MR::onCalcShadowDropPrivateGravityOneTime(this, 0);
    }
}

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
    setNerve(GET_NERVE(ChipBase, ChipBaseNrvDeactive));
}

void ChipBase::setGroupID(s32 id) {
    mGroupID = id;
}

void ChipBase::setHost(LiveActor* pActor) {
    mHost = pActor;
}

void ChipBase::makeActorAppeared() {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvDeactive))) {
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
        mPosition.set(mRailMover->_28);
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
    }
}

void ChipBase::appearWait() {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvDeactive))) {
        return;
    }

    makeActorAppeared();

    MR::validateClipping(this);

    setNerve(GET_NERVE(ChipBase, ChipBaseNrvWait));
}

void ChipBase::appearFlashing(s32 a1) {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvDeactive))) {
        return;
    }

    appear();

    MR::invalidateClipping(this);

    mFlashingCtrl->start(a1);

    setNerve(GET_NERVE(ChipBase, ChipBaseNrvFlashing));
}

bool ChipBase::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgItemGet(msg)) {
        return requestGet(pReceiver, pSender);
    }

    if (MR::isMsgItemShow(msg)) {
        return requestShow();
    }

    if (MR::isMsgItemHide(msg)) {
        return requestHide();
    }

    if (MR::isMsgItemStartMove(msg)) {
        return requestStartControl();
    }

    if (MR::isMsgItemEndMove(msg)) {
        return requestEndControl();
    }

    return false;
}

bool ChipBase::requestGet(HitSensor* pSender, HitSensor* pReceiver) {
    if (isGettable()) {
        MR::noticeGetChip(mChipType, this, mGroupID);
        setNerve(GET_NERVE(ChipBase, ChipBaseNrvGot));

        if (mHost != nullptr) {
            mHost->receiveMessage(ACTMES_ITEM_GET, pSender, pReceiver);
        }

        return true;
    }

    return false;
}

bool ChipBase::requestShow() {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvHide))) {
        MR::startBck(this, "Wait");
        MR::showModel(this);

        setNerve(GET_NERVE(ChipBase, ChipBaseNrvWait));

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

        setNerve(GET_NERVE(ChipBase, ChipBaseNrvHide));

        return true;
    }

    return false;
}

bool ChipBase::requestStartControl() {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvWait))) {
        setNerve(GET_NERVE(ChipBase, ChipBaseNrvControled));
        return true;
    }

    return false;
}

bool ChipBase::requestEndControl() {
    if (isNerve(GET_NERVE(ChipBase, ChipBaseNrvControled))) {
        setNerve(GET_NERVE(ChipBase, ChipBaseNrvWait));
        return true;
    }

    return false;
}

void ChipBase::exeDeactive() {
}

void ChipBase::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::validateHitSensors(this);
    }
}

void ChipBase::exeControled() {
    MR::zeroVelocity(this);
}

void ChipBase::exeFlashing() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::validateHitSensors(this);
    }

    if (mFlashingCtrl->mIsEnded) {
        kill();
    }
}

void ChipBase::exeHide() {
}

void ChipBase::exeGot() {
    if (MR::isFirstStep(this)) {
        if (hasAirBubble()) {
            MR::emitEffect(mAirBubble, "RecoveryBubbleBreak");

            MR::incPlayerOxygen(8);

            mAirBubble->kill();
        }

        MR::emitEffect(this, "Get");

        MR::tryRumblePadMiddle(this, WPAD_CHAN0);

        if (mChipType == Type_Blue) {
            MR::startSystemSE("SE_SY_BLUECHIP_GET", MR::getGotChipCount(mChipType, mGroupID));
        } else if (mChipType == Type_Yellow) {
            MR::startSystemSE("SE_SY_YELLOWCHIP_GET", MR::getGotChipCount(mChipType, mGroupID));
        }
    }

    kill();
}

bool ChipBase::isGettable() const {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::isLessStep(this, ::sCannotGetTime)) {
        return false;
    }

    return isNerve(GET_NERVE(ChipBase, ChipBaseNrvWait)) || isNerve(GET_NERVE(ChipBase, ChipBaseNrvFlashing));
}

bool ChipBase::isNeedBubble(const JMapInfoIter& rIter) {
    if (!MR::isValidInfo(rIter)) {
        return false;
    }

    bool arg3 = false;
    MR::getJMapInfoArg3WithInit(rIter, &arg3);

    return arg3;
}

void ChipBase::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (ChipBase::isNeedBubble(rIter)) {
        pCollector->addArchive("AirBubble");
    }
}
