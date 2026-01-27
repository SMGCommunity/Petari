#include "Game/MapObj/PhantomShipBridge.hpp"
#include "Game/Animation/BckCtrl.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvPhantomShipBridge {
    NEW_NERVE(HostTypeWait, PhantomShipBridge, Wait);
    NEW_NERVE(HostTypeMoveA, PhantomShipBridge, MoveA);
    NEW_NERVE(HostTypeMoveB, PhantomShipBridge, MoveB);
};  // namespace NrvPhantomShipBridge

PhantomShipBridge::PhantomShipBridge(const char* pName) : LiveActor(pName) {
    _8C = nullptr;
    mIsNutShipBridge = 0;
}

void PhantomShipBridge::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objName;
    MR::getObjectName(&objName, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    if (MR::isEqualString(objName, "NutShipFleetBridge")) {
        mIsNutShipBridge = 1;
    }

    MR::connectToSceneCollisionMapObj(this);
    initNerve(&NrvPhantomShipBridge::HostTypeWait::sInstance);
    initEffectKeeper(0, nullptr, false);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    MR::addMessageSensorMapObjMoveCollision(this, "move");
    _8C = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("move"), MR::getJointMtx(this, "Move"), (MR::CollisionScaleType)2);
    MR::validateCollisionParts(_8C);
    initSound(4, false);
    MR::setClippingTypeSphere(this, 2000.0f);
    makeActorAppeared();

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &PhantomShipBridge::startMoveB));
    }

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &PhantomShipBridge::startMoveA));
    } else {
        setStateMoveA();
    }
}

void PhantomShipBridge::calcAnim() {
    LiveActor::calcAnim();
    _8C->setMtx();
}

void PhantomShipBridge::startMoveA() {
    setNerve(&NrvPhantomShipBridge::HostTypeMoveA::sInstance);
}

void PhantomShipBridge::startMoveB() {
    setNerve(&NrvPhantomShipBridge::HostTypeMoveB::sInstance);
}

void PhantomShipBridge::setStateMoveA() {
    MR::startBck(this, "MoveA", nullptr);
    MR::setBckFrameAndStop(this, MR::getBckCtrl(this)->mEnd);
    MR::calcAnimDirect(this);
    _8C->forceResetAllMtxAndSetUpdateMtxOneTime();
    setNerve(&NrvPhantomShipBridge::HostTypeWait::sInstance);
}

void PhantomShipBridge::exeMoveA() {
    if (MR::isFirstStep(this)) {
        if (!MR::isPlayerDead()) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        }

        MR::startBck(this, "MoveA", nullptr);
    }

    if (MR::isLessStep(this, 55)) {
        if (mIsNutShipBridge == 1) {
            MR::startLevelSound(this, "SE_OJ_LV_NUT_SHIP_BRIDGE_MV", -1, -1, -1);
        } else {
            MR::startLevelSound(this, "SE_OJ_LV_FALL_BRIDGE_FALL", -1, -1, -1);
        }
    }

    if (MR::isStep(this, 55)) {
        if (mIsNutShipBridge == 1) {
            MR::startSound(this, "SE_OJ_NUT_SHIP_BRIDGE_ED", -1, -1);
        } else {
            MR::startSound(this, "SE_OJ_FALL_BRIDGE_LAND", -1, -1);
        }

        MR::shakeCameraNormal();
    }
}

void PhantomShipBridge::exeMoveB() {
    if (MR::isFirstStep(this)) {
        if (mIsNutShipBridge == 1) {
            MR::startSound(this, "SE_OJ_NUT_SHIP_BRIDGE_ST", -1, -1);
        } else {
            MR::startSound(this, "SE_OJ_PTM_SHIP_BRIDGE_FALL", -1, -1);
        }

        MR::startBck(this, "MoveB", nullptr);
    }

    if (MR::isLessStep(this, 55)) {
        if (mIsNutShipBridge == 1) {
            MR::startLevelSound(this, "SE_OJ_LV_NUT_SHIP_BRIDGE_MV", -1, -1, -1);
        } else {
            MR::startLevelSound(this, "SE_OJ_LV_FALL_BRIDGE_FALL", -1, -1, -1);
        }
    }

    if (MR::isStep(this, 55)) {
        if (mIsNutShipBridge == 1) {
            MR::startSound(this, "SE_OJ_NUT_SHIP_BRIDGE_ED", -1, -1);
        } else {
            MR::startSound(this, "SE_OJ_FALL_BRIDGE_LAND", -1, -1);
        }

        MR::shakeCameraNormal();
    }
}

void PhantomShipBridge::exeWait() {}

PhantomShipBridge::~PhantomShipBridge() {}
