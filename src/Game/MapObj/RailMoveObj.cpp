#include "Game/MapObj/RailMoveObj.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"

RailMoveObj::RailMoveObj(const char *pName) : MapObjActor(pName) {
    _C4.setZero();
    _D0 = 0;
}

void RailMoveObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupSound(0x6);
    info.setupSoundPos(getSoundCalcPos());
    info.setupGroupClipping(0x20);
    info.setupSeAppear();
    info.setupRailMover();
    info.setupRailPosture();
    info.setupShadow(0);
    info.setupBaseMtxFollowTarget();
    info.setupNerve(&NrvRailMoveObj::HostTypeMove::sInstance);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    setupInitInfo(rIter, &info);
    initialize(rIter, info);

    if (MR::isConnectedWithRail(rIter)) {
        f32 radius = 0.0f;
        MR::calcModelBoundingRadius(&radius, this);
        MR::initAndSetRailClipping(&_C4, this, 100.0f, radius);
    }
    else {
        setNerve(&NrvRailMoveObj::HostTypeDone::sInstance);
    }

    s32 condition_type = 0;
    MR::getMapPartsArgMoveConditionType(&condition_type, rIter);

    if (!MR::isMoveStartTypeUnconditional(condition_type)) {
        setNerve(&NrvRailMoveObj::HostTypeWaitForPlayerOn::sInstance);
    }

    if (MR::isDemoCast(this, 0) && MR::tryRegisterDemoActionNerve(this, &NrvRailMoveObj::HostTypeMove::sInstance, 0)) {
        setNerve(&NrvRailMoveObj::HostTypeWait::sInstance);
    }
}

TVec3f* RailMoveObj::getSoundCalcPos() {
    return 0;
}

void RailMoveObj::startClipped() {
    MapObjActor::startClipped();

    if (mRailMover) {
        if (MR::StageEffect::isExistStageEffectData(mObjectName)) {
            MR::StageEffect::stopShakingCameraMoving(this, mObjectName);
        }
    }
}

void RailMoveObj::endClipped() {
    MapObjActor::endClipped();

    if (mRailMover) {
        if (MapObjActorUtil::isRailMoverWorking(this)) {
            if (!isNerve(&NrvRailMoveObj::HostTypeDone::sInstance)) {
                if (MR::StageEffect::isExistStageEffectData(mObjectName)) {
                    MR::StageEffect::shakeCameraMoving(this, mObjectName);
                }
            }
        }
    }
}

void RailMoveObj::initCaseUseSwitchB(const MapObjActorInitInfo &rInitInfo) {
    MapObjActor::initCaseUseSwitchB(rInitInfo);
    setNerve(&NrvRailMoveObj::HostTypeWait::sInstance);
}

void RailMoveObj::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void RailMoveObj::startMoveInner() {
    MR::StageEffect::tryStageEffectStart(this, mObjectName);

    if (MR::isExistBck(this, cMoveBckName)) {
        MR::startBck(this, cMoveBckName, 0);
    }

    if (MR::StageEffect::isExistStageEffectData(mObjectName)) {
        MR::StageEffect::shakeCameraMoving(this, mObjectName);
    }
}

void RailMoveObj::move() {
    if (MapObjActorUtil::isRailMoverWorking(this)) {
        MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    }
}

void RailMoveObj::doAtEndPoint() {
    MR::StageEffect::tryStageEffectStop(this, mObjectName);

    if (MR::isExistBck(this, cMoveBckName) && MR::isBckPlaying(this, cMoveBckName)) {
        MR::stopBck(this);
    }

    if (MR::StageEffect::isExistStageEffectData(mObjectName)) {
        MR::StageEffect::stopShakingCameraMoving(this, mObjectName);
    }
}

bool RailMoveObj::endMove() {
    doAtEndPoint();
    return true;
}

bool RailMoveObj::isMoving() const {
    return MapObjActorUtil::isRailMoverWorking(this);
}

bool RailMoveObj::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg == 0xCF && isNerve(&NrvRailMoveObj::HostTypeMove::sInstance)) {
        kill();
        return true;
    }

    return false;
}

void RailMoveObj::exeWait() {
    if (!isNerve(&NrvRailMoveObj::HostTypeDone::sInstance) && (!MR::isDemoCast(this, 0) || !MR::isRegisteredDemoActionNerve(this)) && (!MR::isValidSwitchB(this) || MR::isOnSwitchB(this)) && (!isNerve(&NrvRailMoveObj::HostTypeWaitForPlayerOn::sInstance) || MR::isOnPlayer(this))) {
        tryStartMove();
    }
}

bool RailMoveObj::tryStartMove() {
    setNerve(&NrvRailMoveObj::HostTypeMove::sInstance);
    return true;
}

void RailMoveObj::exeMove() {
    if (MR::isFirstStep(this)) {
        startMapPartsFunctions();
    }

    if (!_D0 && MapObjActorUtil::isRailMoverWorking(this)) {
        startMoveInner();
    }

    _D0 = MapObjActorUtil::isRailMoverWorking(this);
    move();

    if (mRailMover->isReachedEnd()) {
        if (mRailMover->isDone() && endMove()) {
            setNerve(&NrvRailMoveObj::HostTypeDone::sInstance);
        }
        else {
            doAtEndPoint();
        }
    }
}

RailMoveObjPress::RailMoveObjPress(const char *pName) : RailMoveObj(pName) {

}

void RailMoveObjPress::init(const JMapInfoIter &rIter) {
    RailMoveObj::init(rIter);
    MR::setBodySensorType(this, 0x76);
}

RailMoveObjBreakAtEnd::RailMoveObjBreakAtEnd(const char *pName) : RailMoveObj(pName) {

}

void RailMoveObjBreakAtEnd::doAtEndPoint() {
    RailMoveObj::doAtEndPoint();

    if (MR::isRegisteredEffect(this, "Break")) {
        MR::emitEffect(this, "Break");
    }

    kill();
}

void RailMoveIndirectObj::connectToScene(const MapObjActorInitInfo &rInfo) {
    MR::connectToSceneIndirectMapObj(this);
}

void RailMoveObjSwitchShadow::init(const JMapInfoIter &rIter) {
    RailMoveObj::init(rIter);
    MR::invalidateShadow(this, 0);
}

void RailMoveObjSwitchShadow::initCaseUseSwitchA(const MapObjActorInitInfo &rInfo) {
    void (RailMoveObjSwitchShadow::*startFunc)(void) = &RailMoveObjSwitchShadow::startOnShadow;
    MR::listenStageSwitchOnA(this, MR::Functor(this, startFunc));
}

void RailMoveObjSwitchShadow::startOnShadow() {
    MR::validateShadow(this, 0);
}

RailDemoMoveObj::RailDemoMoveObj(const char *pName) : RailMoveObj(pName) {
    mDemoControl = 0;
}

void RailDemoMoveObj::init(const JMapInfoIter &rIter) {
    RailMoveObj::init(rIter);
    mDemoControl = new DemoCtrlBase(this, mObjectName);
    mDemoControl->init(rIter);
    makeActorAppeared();
}

bool RailDemoMoveObj::tryStartMove() {
    if (!mDemoControl->tryStart()) {
        return false;
    }

    setNerve(&NrvRailMoveObj::HostTypeMove::sInstance);
    return true;
}

void RailDemoMoveObj::startMoveInner() {
    const char* start_sound = MR::StageEffect::getStartSe(mObjectName);

    if (start_sound) {
        MR::startSound(this, start_sound, -1, -1);
    }
}

void RailDemoMoveObj::move() {
    mDemoControl->update();

    if (MapObjActorUtil::isRailMoverWorking(this)) {
        const char* moving_sound = MR::StageEffect::getMovingSe(mObjectName);

        if (moving_sound) {
            MR::startLevelSound(this, moving_sound, -1, -1, -1);
        }
    }
}

bool RailDemoMoveObj::endMove() {
    MR::validateClipping(this);

    if (!mDemoControl->isExistEndFrame()) {
        mDemoControl->end();
    }

    const char* stop_sound = MR::StageEffect::getStopSe(mObjectName);
    if (stop_sound) {
        MR::startSound(this, stop_sound, -1, -1);
    }

    return mDemoControl->isDone();
}

RailRotateMoveObj::RailRotateMoveObj(const char *pName) : RailMoveObj(pName) {

}

void RailRotateMoveObj::setupInitInfo(const JMapInfoIter &rIter, MapObjActorInitInfo *pInfo) {
    pInfo->setupRailRotator();
}

bool RailRotateMoveObj::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 0xCB) {
        return tryStartRotateAtPoint();
    }

    if (msg == 0xCC) {
        mRailMover->endRotateAtPoint();
        return true;
    }

    if (msg == 0xCD) {
        return tryStartRotateBetweenPoints();
    }

    return RailMoveObj::receiveOtherMsg(msg, a2, a3);
}

void RailRotateMoveObj::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    setNerve(&NrvRailMoveObj::HostTypeWait::sInstance);
    void (RailRotateMoveObj::*pauseFunc)(void) = &MapObjActor::pauseMapPartsFunctions;
    MR::listenStageSwitchOffB(this, MR::Functor(this, pauseFunc));
}

bool RailRotateMoveObj::tryStartRotateAtPoint() {
    s32 cur_point = MR::getCurrentRailPointNo(this);
    if (!mRailRotator->hasRotation(cur_point)) {
        return false;
    }

    if (mRailRotator->hasRotationBetweenPoints(cur_point)) {
        return false;
    }

    mRailRotator->rotateAtPoint(cur_point);
    return true;
}

bool RailRotateMoveObj::tryStartRotateBetweenPoints() {
    MapPartsRailRotator* rail_rotator = mRailRotator;
    s32 cur_point = MR::getCurrentRailPointNo(this);

    if (!rail_rotator->hasRotationBetweenPoints(cur_point)) {
        return false;
    }

    f32 next_time = 0.0f;
    mRailMover->calcTimeToNextRailPoint(&next_time);
    MapPartsRailRotator* uhm = mRailRotator;
    s32 new_point = MR::getCurrentRailPointNo(this);
    uhm->rotateBetweenPoints(new_point, next_time);
    return true;
}

namespace NrvRailMoveObj {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeWaitForPlayerOn);
    INIT_NERVE(HostTypeMove);
    INIT_NERVE(HostTypeDone);

    void HostTypeDone::execute(Spine *pSpine) const {
        RailMoveObj* obj = reinterpret_cast<RailMoveObj*>(pSpine->mExecutor);
        obj->exeWait();
    }

    void HostTypeMove::execute(Spine *pSpine) const {
        RailMoveObj* obj = reinterpret_cast<RailMoveObj*>(pSpine->mExecutor);
        obj->exeMove();
    }

    void HostTypeWaitForPlayerOn::execute(Spine *pSpine) const {
        RailMoveObj* obj = reinterpret_cast<RailMoveObj*>(pSpine->mExecutor);
        obj->exeWait();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        RailMoveObj* obj = reinterpret_cast<RailMoveObj*>(pSpine->mExecutor);
        obj->exeWait();
    }
};

RailMoveObjPress::~RailMoveObjPress() {

}

RailMoveObjBreakAtEnd::~RailMoveObjBreakAtEnd() {

}

RailMoveIndirectObj::~RailMoveIndirectObj() {

}

RailMoveObjSwitchShadow::~RailMoveObjSwitchShadow() {

}

RailDemoMoveObj::~RailDemoMoveObj() {

}

RailRotateMoveObj::~RailRotateMoveObj() {

}
