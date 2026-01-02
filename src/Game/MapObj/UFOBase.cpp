#include "Game/MapObj/UFOBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
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
#include <cstdio>

namespace NrvUFOBase {
    NEW_NERVE(UFOBaseNrvWait, UFOBase, Wait);
    NEW_NERVE(UFOBaseNrvWaitForPlayerOn, UFOBase, WaitForPlayerOn);
    NEW_NERVE(UFOBaseNrvMove, UFOBase, Move);
    NEW_NERVE(UFOBaseNrvBreak, UFOBase, Break);
};  // namespace NrvUFOBase

UFOBase::UFOBase(const char* pName) : LiveActor(pName) {
    mCollisionParts = nullptr;
    mLODCtrl = nullptr;
    mModel = nullptr;
    mRailMover = nullptr;
    f32 v1 = 1.0f;  // ???
    _9C.x = 0.0f;
    _9C.y = 0.0f;
    _9C.z = 1.0f;
    _A8 = 0.0f;
    _AC = 0;
    _B0 = false;
    _B4 = nullptr;
}

void UFOBase::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* name;
    MR::getObjectName(&name, rIter);
    initModelManagerWithAnm(name, nullptr, false);
    _B4 = name;
    MR::connectToSceneCollisionMapObj(this);
    initHitSensor(1);
    initSensorType();
    MR::initCollisionParts(this, name, getSensor(nullptr), nullptr);
    mCollisionParts = MR::tryCreateCollisionMoveLimit(this, getBaseMtx(), getSensor(nullptr));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
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
    _A8 *= 0.01f;
    MR::getMapPartsArgMoveConditionType(&_AC, rIter);
    MR::tryStartAllAnim(this, name);

    if (MR::isMoveStartTypeUnconditional(_AC)) {
        initNerve(&NrvUFOBase::UFOBaseNrvWait::sInstance);
    } else {
        initNerve(&NrvUFOBase::UFOBaseNrvWaitForPlayerOn::sInstance);
    }
    makeActorAppeared();
}

void UFOBase::exeWait() {
    if (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this)) {
        setNerve(&NrvUFOBase::UFOBaseNrvMove::sInstance);
    }
}

void UFOBase::exeWaitForPlayerOn() {
    if (MR::isOnPlayer(getSensor(nullptr)) && (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this))) {
        setNerve(&NrvUFOBase::UFOBaseNrvMove::sInstance);
    }
}

void UFOBase::exeMove() {
    if (MR::isFirstStep(this)) {
        if (mRailMover) {
            mRailMover->start();
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startSound(this, "SE_OJ_UFO_BTL_C_START", -1, -1);
            }
        }
    }
    MR::rotateVecDegree(&_9C, mGravity, _A8);
}

void UFOBase::exeBreak() {
    kill();
};

void UFOBase::initSensorType() {
    MR::addBodyMessageSensorMapObj(this);
}

void UFOBase::makeActorDead() {
    if (mModel) {
        mModel->makeActorDead();
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
    } else if (MR::isEqualString("UFOBattleStageD", _B4)) {
        MR::startLevelSound(this, "SE_OJ_LV_UFO_BTL_D_ROTATE", -1, -1, -1);
    }

    if (mRailMover) {
        mRailMover->movement();
        mPosition.x = mRailMover->_28.x;
        mPosition.y = mRailMover->_28.y;
        mPosition.z = mRailMover->_28.z;
        if (mRailMover->isWorking()) {
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startLevelSound(this, "SE_OJ_LV_UFO_BTL_C_MOVE", -1, -1, -1);
            } else {
                MR::startLevelSound(this, "SE_OJ_LV_UFO_MOVE", -1, -1, -1);
            }
        }
        if (mRailMover->isDone() && !_B0) {
            MR::tryRumblePadMiddle(this, 0);
            MR::shakeCameraNormal();
            if (MR::isEqualString("UFOBattleStageC", _B4)) {
                MR::startSound(this, "SE_OJ_UFO_BTL_C_STOP", -1, -1);
            } else {
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

void UFOBase::initSubModel(const JMapInfoIter& rIter, const char* name) {
    mLODCtrl = MR::createLodCtrlMapObj(this, rIter, 100.0f);
    if (MR::isExistSubModel(name, "Bloom")) {
        char buf[0x100];
        snprintf(buf, sizeof(buf), "%sBloom", name);
        const char* name2 = mName;
        mModel = MR::createModelObjBloomModel(name2, buf, getBaseMtx());
        f32 num = 500.0f;
        MR::calcModelBoundingRadius(&num, this);
        MR::setClippingFarMax(mModel);
        MR::setClippingTypeSphere(mModel, num);
        MR::startBpk(mModel, buf);
    }
}

UFOSolid::UFOSolid(const char* pName) : UFOBase(pName) {}

UFOBreakable::UFOBreakable(const char* pName) : UFOBase(pName) {}

void UFOBreakable::initSensorType() {
    MR::addHitSensor(this, "body", ATYPE_KILLER_TARGET_MAPOBJ, 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

bool UFOBreakable::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && !isNerve(&NrvUFOBase::UFOBaseNrvBreak::sInstance)) {
        setNerve(&NrvUFOBase::UFOBaseNrvBreak::sInstance);
        return true;
    }
    return false;
}
