#include "Game/MapObj/CoinBox.hpp"

CoinBox::CoinBox(const char *pName) : LiveActor(pName) {

}

CoinBox::~CoinBox() {

}

void CoinBox::init(const JMapInfoIter &rIter) {
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

#ifdef NON_MATCHING
void CoinBox::exeHit() {
    if (!getNerveStep()) {
        getSensor("body")->invalidate();
        MR::emitEffect(this, "CoinGet");
    }

    if (getNerveStep() == 0xF) {
        TVec3f axis_y;
        MR::getRotatedAxisY(&axis_y, mRotation);
        MR::calcGravityOrZero(this);
        TVec3f stack_38;
        f32 val = MR::vecKillElement(axis_y, mGravity, &stack_38);
        TVec3f stack_8 = stack_38 * 10.0f;
        TVec3f stack_2C((mGravity * val) * 30.0f);
        
        //TVec3f stack_2C(stack_20);
        stack_2C.addInline(stack_8);
        MR::appearCoinToVelocity(this, mPosition, axis_y, 1);
        kill();
    }
}
#endif

bool CoinBox::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg - 0x31 < 2) {
        setNerve(&NrvCoinBox::CoinBoxNrvHit::sInstance);
        return 1;
    }

    return 0;
}

namespace NrvCoinBox {
    INIT_NERVE(CoinBoxNrvWait);
    INIT_NERVE(CoinBoxNrvHit);

    void CoinBoxNrvHit::execute(Spine *pSpine) const {
        CoinBox* box = reinterpret_cast<CoinBox*>(pSpine->mExecutor);
        box->exeHit();
    }

    void CoinBoxNrvWait::execute(Spine *pSpine) const {

    }
};