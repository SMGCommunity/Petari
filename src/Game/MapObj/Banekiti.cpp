#include "Game/MapObj/Banekiti.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util.hpp"

namespace NrvBanekiti {
    NEW_NERVE(BanekitiNrvWait, Banekiti, Wait);
    NEW_NERVE(BanekitiNrvRepel, Banekiti, Repel);
    NEW_NERVE_ONEND(BanekitiNrvDPDSwoon, Banekiti, DPDSwoon, DPDSwoon);
};  // namespace NrvBanekiti

Banekiti::Banekiti(const char* pName) : LiveActor(pName), mAnimScaleCtrl(), mBindStarPointer(), mRailMover() {
}

inline void Banekiti::initStarPointer() {
    TVec3f offset;
    offset.set(0.0f);
    MR::initStarPointerTarget(this, 120.0f, offset);
}

void Banekiti::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Banekiti", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(3);
    MR::addHitSensorMapObjSimple(this, "body", 4, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObjSimple(this, "right", 4, 80.0f, TVec3f(-100.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObjSimple(this, "left", 4, 80.0f, TVec3f(100.0f, 0.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    initRailRider(rIter);
    mRailMover = new MapPartsRailMover(this);
    mRailMover->init(rIter);
    mRailMover->start();
    initStarPointer();
    mAnimScaleCtrl = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);
    initNerve(GET_NERVE(Banekiti, BanekitiNrvWait));
    makeActorAppeared();
}

void Banekiti::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    MR::startLevelSound(this, "SE_OJ_LV_BANEKITI_MOVE");
}

void Banekiti::exeRepel() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
        MR::startBck(this, "Repel");
        MR::startSound(this, "SE_OJ_BANEKITI_REPEL");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Banekiti, BanekitiNrvWait));
    }
}

void Banekiti::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        mRailMover->mIsActive = false;
    }

    MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(Banekiti, BanekitiNrvWait));
}

void Banekiti::endDPDSwoon() {
    mRailMover->mIsActive = true;
    mBindStarPointer->kill();
}

void Banekiti::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MR::setBaseScale(this, mAnimScaleCtrl->_C * mScale);
}

void Banekiti::control() {
    mAnimScaleCtrl->updateNerve();
    mRailMover->movement();

    if (mRailMover->isWorking()) {
        mPosition.set(mRailMover->_28);
    }

    if (!isNerve(GET_NERVE(Banekiti, BanekitiNrvDPDSwoon)) && mBindStarPointer->tryStartPointBind()) {
        setNerve(GET_NERVE(Banekiti, BanekitiNrvDPDSwoon));
    }
}

bool Banekiti::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        mAnimScaleCtrl->startHitReaction();
        return true;
    }

    return false;
}

bool Banekiti::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (isNerve(GET_NERVE(Banekiti, BanekitiNrvRepel))) {
        return false;
    }

    if (msg == ACTMES_TERESA_PLAYER_TOUCH) {
        setNerve(GET_NERVE(Banekiti, BanekitiNrvRepel));

        return true;
    }

    return false;
}
