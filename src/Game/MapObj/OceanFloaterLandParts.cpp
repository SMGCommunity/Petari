#include "Game/MapObj/OceanFloaterLandParts.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util.hpp"

void OceanFloaterLandParts_FORCE_MATCH_SDATA2() {
    (void)-1.0f;
    (void)100.0f;
}

OceanFloaterLandParts::OceanFloaterLandParts(const char* pName)
    : LiveActor(pName), mRailMover(), mLodCtrl(), mRailEndPos(gZeroVec), mObjectName(), mIsMoving(), mIsRegisteredDemo(), mAlreadyDoneFlag(-1) {
}

namespace NrvOceanFloaterLandParts {
    NEW_NERVE(HostTypeWait, OceanFloaterLandParts, Wait);
    NEW_NERVE(HostTypeMove, OceanFloaterLandParts, Move);
    NEW_NERVE(HostTypeDone, OceanFloaterLandParts, Done);
};  // namespace NrvOceanFloaterLandParts

void OceanFloaterLandParts::init(const JMapInfoIter& rIter) {
    MR::getObjectName(&mObjectName, rIter);
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(mObjectName, nullptr, false);
    initEffectKeeper(0, nullptr, false);

    if (MR::isEqualString(mObjectName, "OceanFloaterChurch")) {
        MR::setEffectHostSRT(this, "Rise", &mRailEndPos, nullptr, nullptr);
    }

    initSound(4, false);
    initHitSensor(1);

    MR::initCollisionParts(this, mObjectName, MR::addBodyMessageSensorMapObj(this), nullptr);
    MR::connectToSceneCollisionMapObj(this);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    initLodCtrl(rIter);

    const char* pName = mObjectName;
    if (MR::isExistAnim(this, pName)) {
        MR::startAllAnim(this, pName);
    }

    bool notConnected = !MR::isConnectedWithRail(rIter);
    if (notConnected) {
        initNerve(&NrvOceanFloaterLandParts::HostTypeDone::sInstance);
    } else {
        initMoveType(rIter);
    }

    if (MR::isEqualString("OceanFloaterTypeU", mObjectName)) {
        u32 isDone = 0;
        mAlreadyDoneFlag = MR::setupAlreadyDoneFlag("フローター浮上", rIter, &isDone);

        if (isDone != 0) {
            MR::moveCoordToEndPos(this);
            MR::moveTransToCurrentRailPos(this);

            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            setNerve(&NrvOceanFloaterLandParts::HostTypeDone::sInstance);
        }
    }

    makeActorAppeared();
}

void OceanFloaterLandParts::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (mLodCtrl != nullptr) {
        mLodCtrl->validate();
    }
}

void OceanFloaterLandParts::makeActorDead() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->invalidate();
    }
    LiveActor::makeActorDead();
}

void OceanFloaterLandParts::initMoveType(const JMapInfoIter& rIter) {
    initRailMoveFunction(rIter);
    MR::calcRailEndPos(&mRailEndPos, this);
    initNerve(&NrvOceanFloaterLandParts::HostTypeWait::sInstance);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvOceanFloaterLandParts::HostTypeMove::sInstance, nullptr);
        mIsRegisteredDemo = true;
    }
    MR::useStageSwitchWriteA(this, rIter);
}

void OceanFloaterLandParts::initRailMoveFunction(const JMapInfoIter& rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);

        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();
    }
}

void OceanFloaterLandParts::initLodCtrl(const JMapInfoIter& rIter) {
    mLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
}

void OceanFloaterLandParts::control() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->update();
    }
}

bool OceanFloaterLandParts::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK && isNerve(&NrvOceanFloaterLandParts::HostTypeMove::sInstance)) {
        kill();
        return true;
    }

    return false;
}

void OceanFloaterLandParts::endFloatUpDemo() {
    if (!MR::isEqualString("OceanFloaterTypeU", mObjectName)) {
        return;
    }

    if (mAlreadyDoneFlag < 0) {
        return;
    }

    MR::updateAlreadyDoneFlag(mAlreadyDoneFlag, 1);
    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }
}

void OceanFloaterLandParts::exeWait() {
}

void OceanFloaterLandParts::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_HUGE_FLOATER_MOVE_ST");
        MR::emitEffect(this, "RiseBubble");

        if (MR::isEqualString(mObjectName, "OceanFloaterChurch")) {
            MR::emitEffect(this, "Rise");
        }

        MR::invalidateClipping(this);
        mIsMoving = false;
    }

    if (!mIsMoving) {
        MR::startLevelSound(this, "SE_OJ_LV_HUGE_FLOATER_MOVE");
    }

    mRailMover->movement();
    mPosition.set(mRailMover->_28);

    if (!mRailMover->isWorking() && !mIsMoving) {
        MR::startSound(this, "SE_OJ_HUGE_FLOATER_MOVE_ED");
        MR::deleteEffect(this, "RiseBubble");

        if (MR::isEqualString(mObjectName, "OceanFloaterChurch")) {
            MR::deleteEffect(this, "Rise");
        }

        mIsMoving = true;

        if (!mIsRegisteredDemo) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S");
            setNerve(&NrvOceanFloaterLandParts::HostTypeDone::sInstance);
            return;
        }
    }

    if (mIsRegisteredDemo && MR::isDemoLastStep()) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
        endFloatUpDemo();
        setNerve(&NrvOceanFloaterLandParts::HostTypeDone::sInstance);
    }
}

void OceanFloaterLandParts::exeDone() {
}
