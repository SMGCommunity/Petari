#include "Game/MapObj/UFOBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"
#include <cstddef>

UFOBase::UFOBase(const char *pName) : LiveActor(pName) {
    mCollisionParts = nullptr;
    mLODCtrl = nullptr;
    mRailMover2 = nullptr;
    mRailMover1 = nullptr;
    _9C.x = 0.0f;
    _9C.y = 0.0f;
    _9C.z = 1.0f;
    _A8 = 0.0f;
    _AC = 0;
    _B0 = false;
    _B4 = nullptr;
}

void UFOBase::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    const char *name;
    MR::getObjectName(&name, rIter);
    initModelManagerWithAnm(name, nullptr, false);
    _B4 = name;
    MR::connectToSceneCollisionMapObj(this);
    initHitSensor(1);
    //
    MR::initCollisionParts(this, name, getSensor(nullptr), nullptr);
    mCollisionParts = MR::tryCreateCollisionMoveLimit(this, getBaseMtx(), getSensor(nullptr));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover1 = new MapPartsRailMover(this);
        mRailMover1->init(rIter);
    }
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setGroupClipping(this, rIter, 32);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    initSubModel(rIter, name);
    MR::tryRegisterDemoCast(this, rIter);
    MR::calcFrontVec(&_9C, this);
    MR::getMapPartsArgRotateSpeed(&_A8, rIter);
    _A8 *= 0.0099999998f;
    MR::getMapPartsArgMoveConditionType(&_AC, rIter);
    MR::tryStartAllAnim(this, name);
    if (MR::isMoveStartTypeUnconditional(_AC)) {
        initNerve(&NrvUFOBase::UFOBaseNrvWait::sInstance);
    }
    else {
        initNerve(&NrvUFOBase::UFOBaseNrvWaitForPlayerOn::sInstance);
    }
    makeActorAppeared();
}

void UFOBase::exeWaitForPlayerOn() {
    if (MR::isOnPlayer(getSensor(nullptr)) && (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this))) {
        setNerve(&NrvUFOBase::UFOBaseNrvMove::sInstance);
    }
}

void UFOBase::exeMove() {
    if (MR::isFirstStep(this)) {
        if (mRailMover1) {
            mRailMover1->start();
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startSound(this, "SE_OJ_UFO_BTL_C_START", -1, -1);
            }
        }
    }
    MR::rotateVecDegree(&_9C, mGravity, _A8);
}

void UFOBase::initSensorType() {
    MR::addBodyMessageSensorMapObj(this);
}

void UFOBase::makeActorDead() {
    if (mRailMover2) {
        u32 msg;
        mRailMover2->receiveMsg(msg);
    }
    LiveActor::makeActorDead();
}

void UFOBase::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    LiveActor::kill();
}

void UFOBase::control() {
    if (mCollisionParts) {
        mCollisionParts->setMtx();
    }
    mLODCtrl->update();
    if (MR::isEqualString("UFONormalB", _B4)) {
        MR::startLevelSound(this, "SE_OJ_LV_UFO_NORM_B_WORK", -1, -1, -1);
    }
    else if (MR::isEqualString("UFOBattleStageD", _B4)) {
        MR::startLevelSound(this, "SE_OJ_LV_UFO_BTL_D_ROTATE", -1, -1, -1);
    }
    if (mRailMover1) {
        mRailMover1->movement();
        mPosition.x = mRailMover1->_28.x;
        mPosition.y = mRailMover1->_28.y;
        mPosition.z = mRailMover1->_28.z;
        if (mRailMover1->isWorking()) {
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startLevelSound(this, "SE_OJ_LV_UFO_BTL_C_MOVE", -1, -1, -1);
            }
            else {
                MR::startLevelSound(this, "SE_OJ_LV_UFO_MOVE", -1, -1, -1);
            }
        }
        if (mRailMover1->isDone() && !_B0) {
            MR::tryRumblePadMiddle(this, 0);
            MR::shakeCameraNormal();
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startSound(this, "SE_OJ_UFO_BTL_C_STOP", -1, -1);
            }
            else {
                MR::startSound(this, "SE_OJ_UFO_MOVE_END", -1, -1);
            }
            _B0 = true;
        }
    }
}

void UFOBase::calcAndSetBaseMtx() {
    TVec3f vec;
    TPos3f mtx;
    mtx.identity();
    MR::calcUpVec(&vec, this);
    MR::makeMtxUpFrontPos(&mtx, vec, _9C, mPosition);
    MR::setBaseTRMtx(this, mtx);
}

void UFOBase::initSubModel(const JMapInfoIter &riter, const char *name) {
    mLODCtrl = MR::createLodCtrlMapObj(this, riter, 100.0f);
    if (MR::isExistSubModel(name, "Bloom")) {
        char pChar;
        //snprintf(pChar, 0x100u, "sBloom", name);
    }
}

void UFOBase::exeWait() {
    if (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this)) {
        setNerve(&NrvUFOBase::UFOBaseNrvMove::sInstance);
    }
}

namespace NrvUFOBase {
    INIT_NERVE(UFOBaseNrvWait);   
    INIT_NERVE(UFOBaseNrvWaitForPlayerOn);   
    INIT_NERVE(UFOBaseNrvMove);   
    INIT_NERVE(UFOBaseNrvBreak);   

	void UFOBaseNrvWait::execute(Spine *pSpine) const {
		UFOBase *pActor = (UFOBase*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void UFOBaseNrvWaitForPlayerOn::execute(Spine *pSpine) const {
		UFOBase *pActor = (UFOBase*)pSpine->mExecutor;
		pActor->exeWaitForPlayerOn();
	}    
    
	void UFOBaseNrvMove::execute(Spine *pSpine) const {
		UFOBase *pActor = (UFOBase*)pSpine->mExecutor;
		pActor->exeMove();
	}    
    
	void UFOBaseNrvBreak::execute(Spine *pSpine) const {
		UFOBase *pActor = (UFOBase*)pSpine->mExecutor;
		pActor->kill();
	}        
};
