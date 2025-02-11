#include "Game/MapObj/AnmModelObj.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"

namespace {
    const char* cFollowjointName = "Move";
    const char* cAnimFileName = "Move";
    const char* cEndLoopEffectName = "EndLoop";
};

AnmModelObj::AnmModelObj(const char *pName) : MapObjActor(pName) {
    mJointPos.x = 0.0f;
    mJointPos.y = 0.0f;
    mJointPos.z = 0.0f;
}

void AnmModelObj::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(&NrvAnmModelObj::HostTypeWait::sInstance);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    initialize(rIter, info);
    
    if (MR::isExistJoint(this, cFollowjointName)) {
        MR::copyJointPos(this, cFollowjointName, &mJointPos);
    }
    else {
        mJointPos.set<f32>(mPosition);
    }

    f32 boundRadius;
    MR::calcModelBoundingRadius(&boundRadius, this);
    MR::setClippingTypeSphere(this, boundRadius, &mJointPos);
}

bool AnmModelObj::isDone() const {
    return MR::isBckOneTimeAndStopped(this);
}

void AnmModelObj::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, cAnimFileName);
        MR::StageEffect::tryStageEffectStart(this, mObjectName);
        MR::StageEffect::shakeCameraMoving(this, mObjectName);
        startInner();
    }

    s32 steps = MR::StageEffect::getStopSeSteps(mObjectName);
    const char* movingSE = MR::StageEffect::getMovingSe(mObjectName);
    if (movingSE != nullptr) {
        if (steps > 0) {
            if (MR::isLessStep(this, steps)) {
                MR::startLevelSound(this, movingSE, -1, -1, -1);
            }
        }
        else {
            MR::startLevelSound(this, movingSE, -1, -1, -1);
        }
    }

    const char* stopSE = MR::StageEffect::getStopSe(mObjectName);

    if (stopSE != nullptr) {
        if (steps >= 0) {
            if (MR::isStep(this, steps)) {
                MR::startSound(this, stopSE, -1, -1);
                if (MR::StageEffect::isRiddleSeTypeStop(mObjectName)) {
                    MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
                }
            }
        }
    }

    MR::StageEffect::rumblePadMoving(this, mObjectName);
    moveInner();
    if (MR::isExistJoint(this, cFollowjointName)) {
        MR::copyJointPos(this, cFollowjointName, &mJointPos);
    }
    else {
        mJointPos.set<f32>(mPosition);
    }

    if (isDone()) {
        MR::StageEffect::rumblePadStop(this, mObjectName);
        MR::StageEffect::shakeStopCamera(this, mObjectName);
        MR::StageEffect::stopShakingCameraMoving(this, mObjectName);
        stopInner();

        if (isKilledAtMoveDone()) {
            kill();
        }
        else if (isRepeat()) {
            setNerve(&NrvAnmModelObj::HostTypeWait::sInstance);
        }
        else {
            setNerve(&NrvAnmModelObj::HostTypeDone::sInstance);
        }
    }
}

void AnmModelObj::exeDone() {
    if (MR::isFirstStep(this) && MR::isRegisteredEffect(this, cEndLoopEffectName)) {
        MR::emitEffect(this, cEndLoopEffectName);
    }

    if (MR::isEqualString(mObjectName, "HeavenlyBeachUnderRock")) {
        const char* movingSE = MR::StageEffect::getMovingSe(mObjectName);

        if (movingSE) {
            MR::startLevelSound(this, movingSE, -1, -1, -1);
        }
    }
}

void AnmModelObj::exeWait() {
    if (isOnStartAnmTrigger()) {
        setNerve(&NrvAnmModelObj::HostTypeMove::sInstance);
    }
}

AnmModelSwitchMove::AnmModelSwitchMove(const char *pName) : AnmModelObj(pName) {

}

AnmModelObj::~AnmModelObj() {

}

void AnmModelSwitchMove::init(const JMapInfoIter &rIter) {
    AnmModelObj::init(rIter);

    if (MR::isDemoCast(this, nullptr)) {
        if (MR::tryRegisterDemoActionNerve(this, &NrvAnmModelObj::HostTypeMove::sInstance, nullptr)) {
            setNerve(&NrvAnmModelObj::HostTypeWait::sInstance);
        }
    }
}

bool AnmModelSwitchMove::isOnStartAnmTrigger() const {
    if (MR::isDemoCast(this, nullptr) && MR::isRegisteredDemoActionNerve(this)) {
        return false;
    }

    if (!MR::isValidSwitchA(this)) {
        return true;
    }

    return MR::isOnSwitchA(this);
}

AnmModelGroundOnMove::AnmModelGroundOnMove(const char *pName) : AnmModelObj(pName) {
    
}

void AnmModelGroundOnMove::init(const JMapInfoIter &rIter) {
    AnmModelObj::init(rIter);
    MR::useStageSwitchWriteB(this, rIter);
}

void AnmModelGroundOnMove::control() {
    if (MR::isValidSwitchB(this) && isNerve(&NrvAnmModelObj::HostTypeMove::sInstance) && MR::isFirstStep(this) && !MR::isOnSwitchB(this)) {
        MR::onSwitchB(this);
    }
}

bool AnmModelGroundOnMove::isOnStartAnmTrigger() const {
    return MR::isOnPlayer(getSensor("body"));
}

AnmModelBindMove::AnmModelBindMove(const char *pName) : AnmModelObj(pName) {

}

void AnmModelBindMove::init(const JMapInfoIter &rIter) {
    AnmModelObj::init(rIter);
    MR::useStageSwitchWriteB(this, rIter);
}

void AnmModelBindMove::control() {
    if (MR::isValidSwitchB(this) && isNerve(&NrvAnmModelObj::HostTypeMove::sInstance) && MR::isFirstStep(this) && !MR::isOnSwitchB(this)) {
        MR::onSwitchB(this);
    }
}

bool AnmModelBindMove::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    return msg == 196;
}

bool AnmModelBindMove::isOnStartAnmTrigger() const {
    LiveActor* rush = MR::getCurrentRushActor();

    if (rush != nullptr) {
        return MR::isBinded(rush, getSensor("body"));
    }

    return false;
}

AnmModelSwitchMoveEventCamera::AnmModelSwitchMoveEventCamera(const char *pName) : AnmModelSwitchMove(pName) {
    mCameraInfo = nullptr;
}

AnmModelSwitchMove::~AnmModelSwitchMove() {

}

void AnmModelSwitchMoveEventCamera::init(const JMapInfoIter &rIter) {
    AnmModelObj::init(rIter);
    if (MR::isDemoCast(this, nullptr) && MR::tryRegisterDemoActionNerve(this, &NrvAnmModelObj::HostTypeMove::sInstance, nullptr)) {
        setNerve(&NrvAnmModelObj::HostTypeWait::sInstance);
    }

    MR::initActorCamera(this, rIter, &mCameraInfo);
}

bool AnmModelSwitchMoveEventCamera::isDone() const {
    ActorCameraInfo* info = mCameraInfo;
    if (info == nullptr) {
        return MR::isBckOneTimeAndStopped(this);
    }

    s32 frames = MR::getActorCameraFrames(this, info);
    if (frames <= 0) {
        return MR::isBckOneTimeAndStopped(this);
    }
    else {
        return MR::isGreaterEqualStep(this, frames);
    }
}

void AnmModelSwitchMoveEventCamera::startInner() {
    if (mCameraInfo != nullptr) {
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);
    }
}

void AnmModelSwitchMoveEventCamera::stopInner() {
    MR::endActorCamera(this, mCameraInfo, false, -1);
}

AnmModelGroundOnMove::~AnmModelGroundOnMove() {

}

AnmModelBindMove::~AnmModelBindMove() {

}

AnmModelSwitchMoveEventCamera::~AnmModelSwitchMoveEventCamera() {

}

namespace NrvAnmModelObj {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMove);
    INIT_NERVE(HostTypeDone);
};

void AnmModelObj::startInner() {

}

void AnmModelObj::moveInner() {
    
}

void AnmModelObj::stopInner() {

}

bool AnmModelObj::isKilledAtMoveDone() const {
    return false;
}

bool AnmModelObj::isRepeat() const {
    return false;
}

void AnmModelObj::control() {

}

void AnmModelObj::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void AnmModelObj::initCaseUseSwitchB(const MapObjActorInitInfo &) {

}

void AnmModelObj::initCaseNoUseSwitchA(const MapObjActorInitInfo &) {

}

void AnmModelObj::initCaseUseSwitchA(const MapObjActorInitInfo &) {

}

bool AnmModelBindMove::isRepeat() const {
    return true;
}

bool AnmModelBindMove::isKilledAtMoveDone() const {
    return false;
}

bool AnmModelGroundOnMove::isKilledAtMoveDone() const {
    return true;
}
