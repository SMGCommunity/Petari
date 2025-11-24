#include "Game/MapObj/SnowplowSwitch.hpp"

SnowplowSwitch::SnowplowSwitch(const char* pName) : LiveActor(pName), mSnowDiscovered(false) {}

bool SnowplowSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_ASK_HIDDEN_BY_SNOW) {
        return mSnowDiscovered == false;
    }

    if (msg == ACTMES_NOTIFY_DISCOVER_SNOW && !mSnowDiscovered) {
        mSnowDiscovered = true;

        MR::onSwitchA(this);

        return true;
    }

    return false;
}

SnowplowSwitch::~SnowplowSwitch() {}

void SnowplowSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "Body", 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::needStageSwitchWriteA(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}
