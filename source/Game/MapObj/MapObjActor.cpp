#include "Game/MapObj/MapObjActor.h"
#include "Game/MapObj/MapPartsRotator.h"
#include "Game/MapObj/MapPartsRailMover.h"
#include "Game/MapObj/MapPartsRailGuideDrawer.h"
#include "Game/MapObj/MapPartsRailPosture.h"
#include "Game/MapObj/MapPartsRailRotator.h"
#include "Game/LiveActor/MaterialCtrl.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/LiveActor/LodCtrl.h"
#include "Game/MapObj/StageEffectDataTable.h"
#include "Game/Util.h"

#include <stdio.h>

NrvMapObjActor::HostTypeDone NrvMapObjActor::HostTypeDone::sInstance;
NrvMapObjActor::HostTypeMove NrvMapObjActor::HostTypeMove::sInstance;
NrvMapObjActor::HostTypeWait NrvMapObjActor::HostTypeWait::sInstance;

MapObjActor::MapObjActor(const char *pName) : LiveActor(pName) {
    mObjectName = 0;
    mPlanetLodCtrl = 0;
    mBloomModel = 0;
    mModelObj = 0;
    mMatrixSetter = 0;
    mRailMover = 0;
    mRotator = 0;
    mRailRotator = 0;
    mRailPosture = 0;
    mRailGuideDrawer = 0;
    _B4 = 0;
    _B5 = 0;
    _B6 = 0;
    mWaitNrv = &NrvMapObjActor::HostTypeWait::sInstance;
    mMoveNrv = &NrvMapObjActor::HostTypeMove::sInstance;
    mDoneNrv = &NrvMapObjActor::HostTypeDone::sInstance;
}

MapObjActor::MapObjActor(const char *pName, const char *pObjName) : LiveActor(pName) {
    mObjectName = pObjName;
    mPlanetLodCtrl = 0;
    mBloomModel = 0;
    mModelObj = 0;
    mMatrixSetter = 0;
    mRailMover = 0;
    mRotator = 0;
    mRailRotator = 0;
    mRailPosture = 0;
    mRailGuideDrawer = 0;
    _B4 = 0;
    _B6 = 0;
    mWaitNrv = &NrvMapObjActor::HostTypeWait::sInstance;
    mMoveNrv = &NrvMapObjActor::HostTypeMove::sInstance;
    mDoneNrv = &NrvMapObjActor::HostTypeDone::sInstance;
}

void MapObjActor::init(const JMapInfoIter &rIter) {
    if (mObjectName) {
        return;
    }

    MR::getObjectName(&mObjectName, rIter);
}

void MapObjActor::initAfterPlacement() {
    if (!mMatrixSetter) {
        return;
    }

    if (!_B4) {
        return;
    }

    mMatrixSetter->updateMtxUseBaseMtx();
}

void MapObjActor::appear() {
    LiveActor::appear();

    if (mBloomModel) {
        mBloomModel->appear();
    }    

    if (MR::isExistEffectKeeper(this)) {
        const char* appearEffectName = cEffectNameAppear;
        if (MR::isRegisteredEffect(this, appearEffectName)) {
            MR::emitEffect(this, appearEffectName);
        }
    }

    if (!MR::isEqualString(mObjectName, "DarkHopperRotateStepA")) {
        const char* startSound = MR::StageEffect::getStartSe(mObjectName);

        if (startSound) {
            MR::startSound(this, startSound, -1, -1);
        }
    }

    if (_B6) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
    }
}

void MapObjActor::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (mModelObj) {
        mModelObj->kill();
    }

    if (mBloomModel) {
        mBloomModel->kill();
    }

    LiveActor::kill();
}

bool MapObjActor::isObjectName(const char *pName) const {
    return MR::isEqualString(pName, mObjectName);
}

void MapObjActor::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        if (MR::isExistCollisionResource(this, mObjectName)) {
            s32 type = rInfo._5C;

            if (type == 1) {
                MR::connectToSceneCollisionMapObjStrongLight(this);
            }
            else if (type == 2) {
                MR::connectToSceneCollisionMapObjWeakLight(this);
            }
            else {
                MR::connectToSceneCollisionMapObj(this);
            }
        }
        else if (rInfo._5C == 1) {
            MR::connectToSceneMapObjStrongLight(this);
        }
        else {
            MR::connectToSceneMapObj(this);
        }
    }
}

void MapObjActor::initCaseUseSwitchA(const MapObjActorInitInfo &) {
    setNerve(mWaitNrv);
}

void MapObjActor::initCaseNoUseSwitchA(const MapObjActorInitInfo &) {

}

void MapObjActor::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    void (MapObjActor::*end)(void) = &MapObjActor::endMapPartsFunctions;
    void (MapObjActor::*start)(void) = &MapObjActor::startMapPartsFunctions;
    MR::listenStageSwitchOnOffB(this, MR::Functor(this, end), MR::Functor(this, start));
}

void MapObjActor::initCaseNoUseSwitchB(const MapObjActorInitInfo &rInfo) {
    MapObjActorUtil::startAllMapPartsFunctions(this);
}

void MapObjActor::control() {
    if (!tryEmitWaitEffect()) {
        tryDeleteWaitEffect();
    }

    if (mPlanetLodCtrl) {
        mPlanetLodCtrl->update();
    }

    if (mRailPosture) {
        mRailPosture->movement();
    }

    if (mRailMover) {
        mRailMover->movement();

        if (mRailMover->isWorking()) {
            mPosition.set(mRailMover->_28);
            mRailMover->tryResetPositionRepeat();
        }
    }

    if (mRotator) {
        mRotator->movement();
        if (mRotator->isOnReverse()) {
            const char* startSound = MR::StageEffect::getStartSe(mObjectName);

            if (startSound) {
                MR::startSound(this, startSound, -1, -1);
            }
        }
    }

    if (mRailRotator) {
        mRailRotator->movement();
    }

    if (!mRailMover && !mRotator && !mRailRotator && !MR::isEqualString(mObjectName, "OceanRingRuinsMove")) {
        const char* movingSound = MR::StageEffect::getMovingSe(mObjectName);
        if (movingSound) {
            MR::startLevelSound(this, movingSound, -1, -1, -1);
        }
    }

    if (mRailGuideDrawer) {
        mRailGuideDrawer->movement();
    }
}

void MapObjActor::startClipped() {
    tryEmitWaitEffect();
    LiveActor::startClipped();
}

void MapObjActor::endClipped() {
    LiveActor::endClipped();
    tryDeleteWaitEffect();
}

bool MapObjActor::tryCreateBreakModel(const MapObjActorInitInfo &rInfo) {
    char buf[0x100];

    if (rInfo._80) {
        snprintf(buf, 0x100, "%s", rInfo._80);
    }
    else {
        snprintf(buf, 0x100, "%sBreak", mObjectName);
    }

    if (!MR::isExistModel(buf)) {
        return false;
    }

    if (MR::isEqualString(mObjectName, "SandUpDownTowerBreakableWallB")) {
        MtxPtr baseMtx = getBaseMtx();
        mModelObj = MR::createModelObjMapObj("壊れモデル", buf, baseMtx);
    }
    else {
        MtxPtr baseMtx = getBaseMtx();
        mModelObj = MR::createModelObjMapObjStrongLight("壊れモデル", buf, baseMtx);
    }

    mModelObj->makeActorDead();

    return true;
}

bool MapObjActor::tryEmitWaitEffect() {
    if (!MR::isExistEffectKeeper(this)) {
        return false;
    }

    if (!MR::isRegisteredEffect(this, mObjectName)) {
        return false;
    }

    if (MR::calcCameraDistanceZ(mPosition) > 4000.0f) {
        return false;
    }

    if (MR::isEffectValid(this, mObjectName)) {
        return false;
    }

    MR::emitEffect(this, mObjectName);
    return true;
}

bool MapObjActor::tryDeleteWaitEffect() {
    if (!MR::isExistEffectKeeper(this)) {
        return false;
    }

    if (!MR::isRegisteredEffect(this, mObjectName)) {
        return false;
    }

    if (MR::calcCameraDistanceZ(mPosition) <= 4000.0f) {
        return false;
    }

    if (MR::isNearPlayer(this, 4000.0f)) {
        return false;
    }

    if (!MR::isEffectValid(this, mObjectName)) {
        return false;
    }

    MR::deleteEffect(this, mObjectName);
    return true;
}

void MapObjActor::startMapPartsFunctions() {
    MapObjActorUtil::startAllMapPartsFunctions(this);
}

void MapObjActor::endMapPartsFunctions() {
    MapObjActorUtil::endAllMapPartsFunctions(this);
}

void MapObjActor::pauseMapPartsFunctions() {
    MapObjActorUtil::pauseAllMapPartsFunctions(this);
}

void MapObjActor::setStateWait() {
    setNerve(mWaitNrv);
}

void MapObjActor::updateProjmapMtx() {
    if (!mMatrixSetter) {
        return;
    }

    if (!_B4) {
        return;
    }

    mMatrixSetter->updateMtxUseBaseMtx();
}

void MapObjActor::exeWait() {
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        MapObjActorUtil::startAllMapPartsFunctions(this);
        setNerve(mMoveNrv);
    }
}

void MapObjActor::exeMove() {
    if (MR::isFirstStep(this)) {
        const char* moveName = cBckNameMove;
        if (MR::isExistBck(this, moveName)) {
             MR::startBck(this, moveName, 0);
        }   
    }

    if (MR::isExistBck(this, cBckNameMove) && MR::isBckStopped(this)) {
        setNerve(mDoneNrv);
    }
}

void MapObjActorUtil::startRotator(const MapObjActor *pActor) {
    pActor->mRotator->start();
}

void MapObjActorUtil::startRailMover(const MapObjActor *pActor) {
    pActor->mRailMover->start();
}

void MapObjActorUtil::endRotator(const MapObjActor *pActor) {
    pActor->mRotator->end();
}

void MapObjActorUtil::pauseRotator(const MapObjActor *pActor) {
    pActor->mRotator->_14 = 0;
}

void MapObjActorUtil::resetRailMoverToInitPos(const MapObjActor *pActor) {
    pActor->mRailMover->resetToInitPos();
}

void MapObjActorUtil::startBreak(MapObjActor *pActor) {
    if (!MapObjActorUtil::tryStartBreak(pActor)) {
        pActor->kill();
    }
}

bool MapObjActorUtil::tryStartBreak(MapObjActor *pActor) {
    const char* stopSe = MR::StageEffect::getStopSe(pActor->mObjectName);
    if (stopSe) {
        MR::startSound(pActor, stopSe, -1, -1);
    }

    const char* breakEffect = cEffectNameBreak;
    if (MR::isRegisteredEffect(pActor, breakEffect)) {
        MR::emitEffect(pActor, breakEffect);
    }

    ModelObj* modelObj = pActor->mModelObj;
    if (modelObj) {
        pActor->mModelObj->appear();
        const char* breakName = (const char *)cBckNameBreak;
        MR::startAllAnim(modelObj, breakName);

        if (MR::isExistBva(pActor, breakName)) {
            MR::startBva(pActor, breakName);
            MR::setBvaFrameAndStop(pActor, 1.0f);
        }
        else {
            MR::hideModel(pActor);
        }

        MR::invalidateClipping(modelObj);
        return true;
    }
    else {
        const char* breakName = cBckNameBreak;
        if (MR::isExistBck(pActor, breakName)) {
            MR::startAllAnim(pActor, breakName);
            MR::invalidateClipping(pActor);
            return true;
        }
    }

    return false;
}

bool MapObjActorUtil::isBreakStopped(const MapObjActor *pActor) {
    const LiveActor* actor = pActor->mModelObj;

    if (!pActor->mModelObj && MR::isExistBck(pActor, cBckNameBreak)) {
        actor = pActor;
    }

    if (!actor) {
        return false;
    }

    return MR::isBckOneTimeAndStopped(actor);
}

void MapObjActorUtil::killBloomModel(MapObjActor *pActor) {
    pActor->mBloomModel->kill();
}

void MapObjActorUtil::appearBloomModel(MapObjActor *pActor) {
    pActor->mBloomModel->appear();
    char buf[0x100];
    snprintf(buf, 0x100, "%sBloom", pActor->mObjectName);
    MR::tryStartAllAnim(pActor->mBloomModel, buf);
}

namespace NrvMapObjActor {
    void HostTypeDone::execute(Spine *pSpine) const {

    }

    void HostTypeMove::execute(Spine *pSpine) const {
        MapObjActor* actor = reinterpret_cast<MapObjActor*>(pSpine->mExecutor);
        actor->exeMove();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        MapObjActor* actor = reinterpret_cast<MapObjActor*>(pSpine->mExecutor);
        actor->exeWait();
    }
};