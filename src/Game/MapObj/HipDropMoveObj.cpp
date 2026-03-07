#include "Game/MapObj/HipDropMoveObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util.hpp"

namespace NrvHipDropMoveObj {
    NEW_NERVE(HostTypeWait, HipDropMoveObj, Wait);
    NEW_NERVE(HostTypeMove, HipDropMoveObj, Move);
    NEW_NERVE(HostTypeDone, HipDropMoveObj, Done);
};  // namespace NrvHipDropMoveObj

namespace {
    static const char* cMoveJointName = "Move";
    static const char* cSwitchJointName = "Switch";
    static const char* cMoveAnimName = "Move";
};  // namespace

HipDropDemoMoveObj::~HipDropDemoMoveObj() {}

HipDropMoveObj::HipDropMoveObj(const char* pName) : LiveActor(pName) {
    mObjectName = 0;
}

void HipDropMoveObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&mObjectName, rIter);
    initModelManagerWithAnm(mObjectName, 0, false);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    initHitSensor(1);
    TVec3f sensor;
    sensor.x = 0.0f;
    sensor.y = 0.0f;
    sensor.z = 0.0f;
    HitSensor* jointSensor = MR::addHitSensorAtJointMapObj(this, "body", cSwitchJointName, 0, 225.0f, sensor);
    MtxPtr mtx = MR::getJointMtx(this, cMoveJointName);
    MR::initCollisionParts(this, mObjectName, jointSensor, mtx);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setGroupClipping(this, rIter, 8);
    MR::useStageSwitchWriteA(this, rIter);
    initNerve(&NrvHipDropMoveObj::HostTypeWait::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    makeActorAppeared();
}

bool HipDropMoveObj::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        if (!isNerve(&NrvHipDropMoveObj::HostTypeWait::sInstance)) {
            return false;
        } else {
            f32 radius = pReceiver->mRadius;
            f32 dist = MR::calcDistanceToPlayer(pReceiver->mPosition);
            if (radius < dist) {
                return false;
            }

            setNerve(&NrvHipDropMoveObj::HostTypeMove::sInstance);
            return true;
        }
    }

    return false;
}

bool HipDropMoveObj::isEndMove() const {
    return MR::isBckStopped(this);
}

void HipDropMoveObj::exeWait() {}

void HipDropMoveObj::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, cMoveAnimName);
        const char* startSe = MR::StageEffect::getStartSe(mObjectName);

        if (startSe) {
            MR::startSound(this, startSe, -1, -1);
        }

        MR::StageEffect::rumblePadStart(this, mObjectName);
        MR::StageEffect::shakeCameraMoving(this, mObjectName);
        MR::tryStartDemoRegistered(this, 0);
        moveStart();
    }

    s32 steps = MR::StageEffect::getStopSeSteps(mObjectName);
    const char* movingSe = MR::StageEffect::getMovingSe(mObjectName);

    if (movingSe) {
        if (steps >= 0) {
            if (MR::isLessStep(this, steps)) {
                MR::startLevelSound(this, movingSe, -1, -1, -1);
            }
        } else {
            MR::startLevelSound(this, movingSe, -1, -1, -1);
        }
    }

    if (steps >= 0) {
        if (MR::isStep(this, steps)) {
            const char* stopSe = MR::StageEffect::getStopSe(mObjectName);

            if (stopSe) {
                MR::startSound(this, stopSe, -1, -1);
            }

            if (MR::StageEffect::isRiddleSeTypeStop(mObjectName)) {
                MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
            }
        }
    }

    moving();

    if (isEndMove()) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        MR::StageEffect::rumblePadStop(this, mObjectName);
        MR::StageEffect::stopShakingCameraMoving(this, mObjectName);

        if (steps < 0) {
            const char* stopSe = MR::StageEffect::getStopSe(mObjectName);

            if (stopSe) {
                MR::startSound(this, stopSe, -1, -1);
            }
        }

        moveEnd();
        setNerve(&NrvHipDropMoveObj::HostTypeDone::sInstance);
    }
}

void HipDropMoveObj::exeDone() {}

HipDropDemoMoveObj::HipDropDemoMoveObj(const char* pName) : HipDropMoveObj(pName) {
    mMtx.identity();
}

HipDropMoveObj::~HipDropMoveObj() {}

void HipDropDemoMoveObj::moveStart() {
    MR::startBckPlayer("Wait", (char*)nullptr);
    MtxPtr mtx = MR::getPlayerDemoActor()->getBaseMtx();
    TMtx34f stack_38;
    stack_38.set(mtx);
    MtxPtr jointMtx = MR::getJointMtx(this, cMoveJointName);
    TMtx34f stack_8;
    stack_8.set(jointMtx);
    stack_8.invert(stack_8);
    mMtx.concat(stack_8, stack_38);
}

void HipDropDemoMoveObj::moving() {
    if (MR::isDemoActive()) {
        MtxPtr jointMtx = MR::getJointMtx(this, cMoveJointName);
        TMtx34f stack_8;
        stack_8.set(jointMtx);
        stack_8.concat(stack_8, mMtx);
        TVec3f vec;
        LiveActor* demoActor = MR::getPlayerDemoActor();
        f32 z = stack_8.mMtx[2][3];
        f32 y = stack_8.mMtx[1][3];
        f32 x = stack_8.mMtx[0][3];
        demoActor->mPosition.set< f32 >(x, y, z);
        MR::setPlayerBaseMtx((MtxPtr)&stack_8);
    }
}

void HipDropMoveObj::moveStart() {}

void HipDropMoveObj::moving() {}

void HipDropMoveObj::moveEnd() {}
