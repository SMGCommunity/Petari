#include "Game/MapObj/BattleShipElevator.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/DemoUtil.hpp"

BattleShipElevator::BattleShipElevator(const char *pName) : MapObjActor(pName) {

}

BattleShipElevator::~BattleShipElevator() {
    
}

void BattleShipElevator::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupRailMover();
    info.setupNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance);
    MapObjActor::initialize(rIter, info);
}

void BattleShipElevator::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_B_SHIP_ELEV_START", -1, -1);
        MapObjActor::startMapPartsFunctions();
    }
    MR::startLevelSound(this, "SE_OJ_LV_B_SHIP_ELEV_MOVE", -1, -1, -1);
    if (!MapObjActorUtil::isRailMoverWorking(this)) {
        MR::startSound(this, "SE_OJ_B_SHIP_ELEV_STOP", -1, -1);
        setNerve(&NrvBattleShipElevator::BattleShipElevatorNrvEnd::sInstance);
    }
}

void BattleShipElevator::control() {
    if (!isNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance)) {
        MapObjActor::control();
    }
}

bool BattleShipElevator::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (!isNerve(&NrvBattleShipElevator::BattleShipElevatorNrvWait::sInstance)) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isOnPlayer(getSensor("body"))) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "エレベーター上昇", &NrvBattleShipElevator::BattleShipElevatorNrvMove::sInstance, nullptr, nullptr);
        }
    }

    return false;
}

namespace NrvBattleShipElevator {
    INIT_NERVE(BattleShipElevatorNrvWait);
    INIT_NERVE(BattleShipElevatorNrvMove);
    INIT_NERVE(BattleShipElevatorNrvEnd);

	void BattleShipElevatorNrvWait::execute(Spine *pSpine) const {
	}    

	void BattleShipElevatorNrvMove::execute(Spine *pSpine) const {
        BattleShipElevator* planet = reinterpret_cast<BattleShipElevator*>(pSpine->mExecutor);
        planet->exeMove();
	}    

	void BattleShipElevatorNrvEnd::execute(Spine *pSpine) const {
	}        
};