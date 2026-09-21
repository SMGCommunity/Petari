#include "Game/MapObj/HipDropMoveObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util.hpp"

namespace {
    static const char* cMoveJointName = "Move";
    static const char* cSwitchJointName = "Switch";
    static const char* cMoveAnimName = "Move";
};  // namespace

namespace NrvHipDropMoveObj {
    NEW_NERVE(HostTypeWait, HipDropMoveObj, Wait);
    NEW_NERVE(HostTypeMove, HipDropMoveObj, Move);
    NEW_NERVE(HostTypeDone, HipDropMoveObj, Done);
};  // namespace NrvHipDropMoveObj

HipDropDemoMoveObj::~HipDropDemoMoveObj() {
}

HipDropMoveObj::HipDropMoveObj(const char* pName) : LiveActor(pName), mModelName() {
}

void HipDropMoveObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&mModelName, rIter);
    initModelManagerWithAnm(mModelName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initHitSensor(1);
    HitSensor* pSensor = MR::addHitSensorAtJointMapObj(this, "body", ::cSwitchJointName, 0, 225.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, mModelName, pSensor, MR::getJointMtx(this, ::cMoveJointName));
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setGroupClipping(this, rIter, 8);
    MR::useStageSwitchWriteA(this, rIter);
    initNerve(GET_NERVE(HipDropMoveObj, HostTypeWait));
    MR::tryRegisterDemoCast(this, rIter);
    makeActorAppeared();
}

bool HipDropMoveObj::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        if (!isNerve(GET_NERVE(HipDropMoveObj, HostTypeWait))) {
            return false;
        }

        if (pReceiver->getRadius() < MR::calcDistanceToPlayer(pReceiver->mPosition)) {
            return false;
        }

        setNerve(GET_NERVE(HipDropMoveObj, HostTypeMove));

        return true;
    }

    return false;
}

bool HipDropMoveObj::isEndMove() const {
    return MR::isBckStopped(this);
}

void HipDropMoveObj::exeWait() {
}

void HipDropMoveObj::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, ::cMoveAnimName);
        const char* startSe = MR::StageEffect::getStartSe(mModelName);

        if (startSe != nullptr) {
            MR::startSound(this, startSe);
        }

        MR::StageEffect::rumblePadStart(this, mModelName);
        MR::StageEffect::shakeCameraMoving(this, mModelName);
        MR::tryStartDemoRegistered(this, nullptr);
        moveStart();
    }

    s32 stopSeSteps = MR::StageEffect::getStopSeSteps(mModelName);
    const char* movingSe = MR::StageEffect::getMovingSe(mModelName);

    if (movingSe) {
        if (stopSeSteps >= 0) {
            if (MR::isLessStep(this, stopSeSteps)) {
                MR::startLevelSound(this, movingSe);
            }
        } else {
            MR::startLevelSound(this, movingSe);
        }
    }

    if (stopSeSteps >= 0) {
        if (MR::isStep(this, stopSeSteps)) {
            const char* stopSe = MR::StageEffect::getStopSe(mModelName);

            if (stopSe != nullptr) {
                MR::startSound(this, stopSe);
            }

            if (MR::StageEffect::isRiddleSeTypeStop(mModelName)) {
                MR::startSystemSE("SE_SY_READ_RIDDLE_S");
            }
        }
    }

    moving();

    if (isEndMove()) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        MR::StageEffect::rumblePadStop(this, mModelName);
        MR::StageEffect::stopShakingCameraMoving(this, mModelName);

        if (stopSeSteps < 0) {
            const char* stopSe = MR::StageEffect::getStopSe(mModelName);

            if (stopSe != nullptr) {
                MR::startSound(this, stopSe);
            }
        }

        moveEnd();
        setNerve(GET_NERVE(HipDropMoveObj, HostTypeDone));
    }
}

void HipDropMoveObj::exeDone() {
}

HipDropDemoMoveObj::HipDropDemoMoveObj(const char* pName) : HipDropMoveObj(pName) {
    mMtx.identity();
}

HipDropMoveObj::~HipDropMoveObj() {
}

void HipDropDemoMoveObj::moveStart() {
    MR::startBckPlayer("Wait");
    TMtx34f playerBaseMtx;
    playerBaseMtx.set(MR::getPlayerDemoActor()->getBaseMtx());

    TMtx34f invJointMtx;
    invJointMtx.set(MR::getJointMtx(this, ::cMoveJointName));
    invJointMtx.invert(invJointMtx);
    mMtx.concat(invJointMtx, playerBaseMtx);
}

void HipDropDemoMoveObj::moving() {
    if (MR::isDemoActive()) {
        TPos3f stack_8;
        stack_8.set(MR::getJointMtx(this, ::cMoveJointName));
        stack_8.concat(stack_8, mMtx);
        stack_8.getTrans(MR::getPlayerDemoActor()->mPosition);
        MR::setPlayerBaseMtx((MtxPtr)&stack_8);
    }
}
