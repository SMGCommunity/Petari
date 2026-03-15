#include "Game/Enemy/JellyfishElectric.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

namespace NrvJellyfishElectric {
    NEW_NERVE(JellyfishElectricNrvWait, JellyfishElectric, Wait);
    NEW_NERVE(JellyfishElectricNrvWaitWithRightTurn, JellyfishElectric, WaitWithRightTurn);
    NEW_NERVE(JellyfishElectricNrvWaitWithLeftTurn, JellyfishElectric, WaitWithLeftTurn);
    NEW_NERVE(JellyfishElectricNrvDamage, JellyfishElectric, Damage);
    NEW_NERVE(JellyfishElectricNrvDeath, JellyfishElectric, Death);
    NEW_NERVE(JellyfishElectricNrvAttack, JellyfishElectric, Attack);
    NEW_NERVE(JellyfishElectricNrvRailGoal, JellyfishElectric, RailGoal);
    NEW_NERVE(JellyfishElectricNrvDPDSwoon, JellyfishElectric, DPDSwoon);

};  // namespace NrvJellyfishElectric

namespace {
    static Color8 sPointLightColor(0x9C, 0xFF, 0xFF, 0xFF);
};

JellyfishElectric::JellyfishElectric(const char* pName) : LiveActor(pName) {
    mController = nullptr;
    mBindStarPtr = nullptr;
    _94 = 0;
    _98.set(0.0f, 0.0f, 1.0f);
    _A4 = false;
    mIsConnectedRail = 0;
    _A8 = 5.0f;
    _AC = 60;
    _B0 = 0;
    _B4.set(0.0f, 0.0f, 0.0f);
}

void JellyfishElectric::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JellyfishElectric", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::addToAttributeGroupSearchTurtle(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.set(0.0f, -60.0f, 0.0f);
    MR::addHitSensorEnemy(this, "body", 8, 350.0f, sensorOffs);
    initBinder(400.0f, 0.0f, 0);
    MR::offBind(this);
    mController = new AnimScaleController(nullptr);
    mController->setParamTight();
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    mController = new AnimScaleController(nullptr);
    mBindStarPtr = new WalkerStateBindStarPointer(this, mController);
    MR::initShadowVolumeSphere(this, 230.0f);
    MR::declareCoin(this, 1);

    if (MR::isConnectedWithRail(rIter)) {
        mIsConnectedRail = true;
        initRailRider(rIter);
        MR::initAndSetRailClipping(&_B4, this, 100.0f, 500.0f);
        MR::moveCoordToNearestPos(this, mPosition);
        MR::getJMapInfoArg0NoInit(rIter, &_A8);
        MR::getJMapInfoArg1NoInit(rIter, &_AC);
        s32 arg2 = 0;
        MR::getJMapInfoArg2NoInit(rIter, &arg2);
        _B0 = arg2 == 1;
    }

    MR::calcFrontVec(&_98, this);
    MR::startBrk(this, "Wait");
    initNerve(&NrvJellyfishElectric::JellyfishElectricNrvWait::sInstance);
    makeActorAppeared();
}

void JellyfishElectric::kill() {
    MR::appearCoinPop(this, mPosition, 1);
    LiveActor::kill();
}

void JellyfishElectric::calcAndSetBaseMtx() {
    TVec3f scale;
    TVec3f v7 = -mGravity;
    TPos3f pos;
    MR::makeMtxFrontUpPos(&pos, _98, v7, mPosition);
    MR::setBaseTRMtx(this, pos);
    JMathInlineVEC::PSVECMultiply(&mController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

JellyfishElectric::~JellyfishElectric() {
}
