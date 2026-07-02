#include "Game/MapObj/CoinBox.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvCoinBox {
    NEW_NERVE(CoinBoxNrvWait, CoinBox, Wait);
    NEW_NERVE(CoinBoxNrvHit, CoinBox, Hit);
};  // namespace NrvCoinBox

CoinBox::CoinBox(const char* pName) : LiveActor(pName) {
}

CoinBox::~CoinBox() {
}

void CoinBox::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    initSound(6, false);
    initNerve(&NrvCoinBox::CoinBoxNrvWait::sInstance);
    initHitSensor(1);
    TVec3f box;
    box.x = 0.0f;
    box.y = 0.0f;
    box.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 8, 5.0f, box);
    makeActorAppeared();
    setNerve(&NrvCoinBox::CoinBoxNrvWait::sInstance);
    MR::declareCoin(this, 1);
    MR::setClippingFar50m(this);
    initEffectKeeper(1, "Coin", false);
}

void CoinBox::exeWait() {
}

// https://decomp.me/scratch/2bcHZ
void CoinBox::exeHit() {
    if (!getNerveStep()) {
        getSensor("body")->invalidate();
        MR::emitEffect(this, "CoinGet");
    }

    if (getNerveStep() == 0xF) {
        TVec3f axis_y;
        MR::getRotatedAxisY(&axis_y, mRotation);
        MR::calcGravityOrZero(this);
        TVec3f horiz;
        f32 val = MR::vecKillElement(axis_y, mGravity, &horiz);
        axis_y = (mGravity * val) * 30.0f + horiz * 10.0f;
        MR::appearCoinToVelocity(this, mPosition, axis_y, 1);
        kill();
    }
}

bool CoinBox::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_TORNADO_ATTACK || msg == ACTMES_TORNADO_STORM_RANGE || msg == ACTMES_SPIN_STORM_RANGE) {
        setNerve(&NrvCoinBox::CoinBoxNrvHit::sInstance);

        return true;
    }

    return false;
}
