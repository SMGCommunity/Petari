#include "Game/MapObj/SpinLeverSwitch.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sStepToOnSwitch = 15;
    static const s32 sStepOnSE = 8;
    static const s32 sStepForHitStop = 8;
};  // namespace

namespace NrvSpinLeverSwitch {
    NEW_NERVE(SpinLeverSwitchNrvWait, SpinLeverSwitch, Wait);
    NEW_NERVE(SpinLeverSwitchNrvSwitchOn, SpinLeverSwitch, SwitchOn);
    NEW_NERVE(SpinLeverSwitchNrvEnd, SpinLeverSwitch, End);
};  // namespace NrvSpinLeverSwitch

SpinLeverSwitch::SpinLeverSwitch(const char* pName) : LiveActor(pName), mConnector() {
    mConnector = new MapObjConnector(this);
}

void SpinLeverSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpinLeverSwitch", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    MR::addHitSensorAtJointMapObj(this, "spin", "Spin", 4, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "SpinLeverSwitch", getSensor("body"), nullptr);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    bool isWait = MR::useStageSwitchWriteA(this, rIter);
    MR::setGroupClipping(this, rIter, 16);

    if (isWait) {
        initNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvWait::sInstance);
    } else {
        MR::startBck(this, "On", nullptr);
        MR::setBckFrameAndStop(this, MR::getBckFrameMax(this));
        MR::startBrk(this, "On");
        MR::setBrkFrameAndStop(this, MR::getBrkFrameMax(this));
        initNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvEnd::sInstance);
    }

    makeActorAppeared();
}

void SpinLeverSwitch::initAfterPlacement() {
    mConnector->attachToUnder();
}

void SpinLeverSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

void SpinLeverSwitch::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (pSender != (getSensor("spin"))) {
        return;
    }

    if (MR::sendMsgPush(pReceiver, pSender)) {
        return;
    }
}

bool SpinLeverSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvWait::sInstance)) {
        return false;
    }

    if (pReceiver != getSensor("spin")) {
        return false;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        setNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvSwitchOn::sInstance);
        MR::stopSceneForDefaultHit(::sStepForHitStop);
        return true;
    }

    return false;
}

void SpinLeverSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "On");
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}

void SpinLeverSwitch::exeSwitchOn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "On", nullptr);
        MR::startBrk(this, "On");

        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_HIT_W");
        } else {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_HIT");
        }

        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::sStepOnSE)) {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_ON_W");
        } else {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_ON");
        }
    }

    if (MR::isStep(this, ::sStepToOnSwitch)) {
        MR::onSwitchA(this);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvEnd::sInstance);
    }
}

void SpinLeverSwitch::exeEnd() {
}
