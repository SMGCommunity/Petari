#include "Game/MapObj/HipDropRock.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

namespace NrvHipDropRock {
    NEW_NERVE(HipDropRockNrvWait, HipDropRock, Wait);
    NEW_NERVE(HipDropRockNrvBreak, HipDropRock, Break);
    NEW_NERVE(HipDropRockNrvWreck, HipDropRock, Wreck);
};  // namespace NrvHipDropRock

HipDropRock::HipDropRock(const char* pName) : LiveActor(pName) {
    mModel = nullptr;
    _C0 = -1;
    _C4 = -1;
    _C8 = false;
    _90.identity();
}

void HipDropRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropRock", nullptr, false);
    initBreakModel();
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "HipDropRock", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    HipDropRock::initItem(rIter);
    MR::getJMapInfoArg7NoInit(rIter, &_C8);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mModel, rIter);
    }
    initNerve(&NrvHipDropRock::HipDropRockNrvWait::sInstance);
    makeActorAppeared();
}

void HipDropRock::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

void HipDropRock::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ROCK_BREAK", -1, -1);
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);
        if (_C8) {
            MR::hideModel(this);
        } else {
            MR::startBva(this, "BreakLevel");
        }
        mModel->makeActorAppeared();
        MR::invalidateClipping(mModel);
        MR::startBck(mModel, "HipDropRockBreak", nullptr);
        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
        HipDropRock::appearItem();
    }
    if (MR::isStep(this, 30) && MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    if (MR::isBckStopped(mModel) && MR::isGreaterStep(this, 30)) {
        mModel->kill();
        if (_C8) {
            kill();
        } else {
            setNerve(&NrvHipDropRock::HipDropRockNrvWreck::sInstance);
        }
    }
}

void HipDropRock::exeWreck() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

bool HipDropRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvHipDropRock::HipDropRockNrvBreak::sInstance)) {
        return false;
    }
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        setNerve(&NrvHipDropRock::HipDropRockNrvBreak::sInstance);
        return true;
    }
    return false;
}

void HipDropRock::initBreakModel() {
    MtxPtr baseMtx = getBaseMtx();
    _90.setInline(baseMtx);
    TVec3f v1;
    TVec3f v2;
    v1.x = 0.0f;
    v1.y = 1.0f;
    v1.z = 0.0f;
    v2.scale(MR::getRandom(0.0f, 180.0f), v1);
    TPos3f v3;
    MR::makeMtxMoment(&v3, v2);
    _90.concat(v3);
    mModel = MR::createModelObjMapObj("ヒビ石壊れモデル", "HipDropRockBreak", _90);
    mModel->initWithoutIter();
    MR::invalidateClipping(mModel);
    mModel->makeActorDead();
}

void HipDropRock::initItem(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &_C0);
    MR::getJMapInfoArg1NoInit(rIter, &_C4);
    if (_C0 == 1 && _C4 == -1) {
        _C4 = 6;
    }
    if (!_C0) {
        MR::declareCoin(this, 1);
    } else if (_C0 == 1) {
        MR::declareStarPiece(this, _C4);
    }
}

void HipDropRock::appearItem() {
    if (_C0 != -1) {
        if (!_C0) {
            MR::appearCoinPop(this, mPosition, 1);
        } else if (_C0 == 1) {
            MR::appearStarPiece(this, mPosition, _C4, 10.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }
    }
}
