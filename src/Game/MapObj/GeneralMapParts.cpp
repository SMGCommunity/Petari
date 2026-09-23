#include "Game/MapObj/GeneralMapParts.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailPosture.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Util.hpp"
#include <algorithm>

namespace NrvGeneralMapParts {
    NEW_NERVE(HostTypeWait, GeneralMapParts, Wait);
    NEW_NERVE(HostTypeWaitForPlayerOn, GeneralMapParts, WaitForPlayerOn);
    NEW_NERVE(HostTypeMoveStart, GeneralMapParts, MoveStart);
    NEW_NERVE(HostTypeMove, GeneralMapParts, Wait);
};  // namespace NrvGeneralMapParts

GeneralMapParts::GeneralMapParts(const char* pName)
    : MapParts(pName), mCameraInfo(), mMoveConditionType(), mSignMotionType(), mShadowType(), mFunctionArray(), mAppearController(), mRailRotator(),
      mRailMover(), mRotator(), mGuideDrawer(), mRailPosture(), mIsCameraEnded(), mIsPaused() {
}

void GeneralMapParts::init(const JMapInfoIter& rIter) {
    MapParts::init(rIter);
    MR::getMapPartsArgShadowType(&mShadowType, rIter);
    MR::initMapPartsShadow(this, rIter);
    MR::getMapPartsArgMoveConditionType(&mMoveConditionType, rIter);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);
    initMapPartsFunction(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(GET_NERVE(GeneralMapParts, HostTypeWait));
    initSensorType(rIter);
    initGravity(rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, &GeneralMapParts::receiveMsgSwitchBOn),
                                    MR::Functor(this, &GeneralMapParts::receiveMsgSwitchBOff));
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        appear();
    }

    MR::useStageSwitchSleep(this, rIter);
}

void GeneralMapParts::initAfterPlacement() {
    if (isFixed()) {
        if (!MR::isExistAnim(this, MR::getModelResName(this)) && !MR::isExistEffectTexMtx(this)) {
            MR::offCalcAnim(this);
        }

        if (MR::hasMapPartsShadow(mShadowType)) {
            MR::onCalcShadowOneTime(this, nullptr);
        }
    }
}

void GeneralMapParts::appear() {
    MapParts::appear();

    if (mGuideDrawer != nullptr) {
        mGuideDrawer->start();
    }

    if (MR::isValidSwitchB(this)) {
        setNerve(GET_NERVE(GeneralMapParts, HostTypeWait));
    } else if (!MR::isMoveStartTypeUnconditional(mMoveConditionType)) {
        setNerve(GET_NERVE(GeneralMapParts, HostTypeWaitForPlayerOn));
    } else {
        startMove();
    }
}

void GeneralMapParts::kill() {
    std::for_each(mFunctionArray.begin(), mFunctionArray.end(), std::mem_fun(&MapPartsFunction::end));
    LiveActor::kill();
}

bool GeneralMapParts::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mAppearController->receiveMsg(msg)) {
        return true;
    }

    if (msg == ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK) {
        broadcastMsgToAllFunctions(ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK);

        return true;
    }

    return MR::receiveMapPartsRotateMsg(this, msg, mRailMover, mRailRotator);
}

void GeneralMapParts::control() {
    if (!mIsPaused) {
        if (mAppearController != nullptr) {
            mAppearController->movement();
        }

        if (mRailMover != nullptr) {
            mRailMover->movement();

            if (MR::isValidSwitchDead(this) && mRailMover->isReachedEnd()) {
                MR::onSwitchDead(this);
            }
        }

        if (mRailPosture != nullptr) {
            mRailPosture->movement();
        }

        if (mRotator != nullptr) {
            mRotator->movement();
        }

        if (mRailRotator != nullptr) {
            mRailRotator->movement();
        }

        if (mGuideDrawer != nullptr) {
            mGuideDrawer->movement();
        }

        bool isRailMoving = false;
        if (mRailMover != nullptr && mRailMover->isWorking()) {
            isRailMoving = true;
        }

        if (isRailMoving) {
            mPosition.set(mRailMover->_28);
        }
    }
}

void GeneralMapParts::calcAndSetBaseMtx() {
    bool isRotating = false;
    if (mRotator != nullptr && mRotator->isWorking()) {
        isRotating = true;
    }

    bool isRailRotating = false;
    if (mRailRotator != nullptr && mRailRotator->isWorking()) {
        isRailRotating = true;
    }

    bool isRailPostureWorking = false;
    if (mRailPosture != nullptr && mRailPosture->isWorking()) {
        isRailPostureWorking = true;
    }

    if (!(isRailRotating || isRotating || isRailPostureWorking)) {
        LiveActor::calcAndSetBaseMtx();
    } else {
        TPos3f mtx;
        mtx.identity();

        if (isRailPostureWorking) {
            mtx.concat(mRailPosture->_18);
        }

        if (isRailRotating) {
            mtx.concat(mRailRotator->_5C);
        }

        if (isRotating) {
            mtx.concat(mRotator->getRotateMtx());
        }

        mtx.setTrans(mPosition);
        MR::setBaseTRMtx(this, mtx);
    }
}

void GeneralMapParts::initMapPartsFunction(const JMapInfoIter& rIter) {
    mAppearController = new MapPartsAppearController(this);
    mAppearController->init(rIter);
    mFunctionArray.push_back(mAppearController);

    f32 rotateSpeed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotateSpeed, rIter);
    bool hasRotateSpeed = 0.0f != rotateSpeed;

    if (hasRotateSpeed) {
        mRotator = new MapPartsRotator(this);
        mRotator->init(rIter);
        mFunctionArray.push_back(mRotator);
    }

    if (MR::isConnectedWithRail(rIter)) {
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mFunctionArray.push_back(mRailMover);
        mRailRotator = new MapPartsRailRotator(this);
        mRailRotator->init(rIter);
        mFunctionArray.push_back(mRailRotator);
        mGuideDrawer = MR::createMapPartsRailGuideDrawer(this, "RailPoint", rIter);
        mFunctionArray.push_back(mGuideDrawer);
        mRailPosture = new MapPartsRailPosture(this);
        mRailPosture->init(rIter);
        mFunctionArray.push_back(mRailPosture);
    }
}

void GeneralMapParts::initSensorType(const JMapInfoIter& rIter) {
    if (MR::isMapPartsPressOn(rIter)) {
        MR::setBodySensorTypePress(this);
    } else if (!isFixed()) {
        MR::setBodySensorTypeMoveCollision(this);
    } else {
        MR::setBodySensorTypeMapObj(this);
    }
}

void GeneralMapParts::initGravity(const JMapInfoIter& rIter) {
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);

    if (!MR::isMapPartsShadowTypeNone(mShadowType)) {
        if (isFixed()) {
            MR::calcGravity(this);
        } else {
            MR::onCalcGravity(this);
        }
    }
}

namespace {
    inline bool isWaiting(const LiveActor* pActor) {
        return pActor->isNerve(GET_NERVE(GeneralMapParts, HostTypeWait)) || pActor->isNerve(GET_NERVE(GeneralMapParts, HostTypeWaitForPlayerOn));
    }

    inline bool isOtherGroupMembersMoving(const GeneralMapParts* pActor) {
        if (!::isWaiting(pActor)) {
            return false;
        }

        LiveActor* pMember;
        LiveActorGroup* pGroup = MR::getGroupFromArray(pActor);
        if (pGroup == nullptr) {
            return true;
        }

        for (s32 i = 0; i < pGroup->getObjNum(); i++) {
            pMember = pGroup->getActor(i);
            if (pMember == pActor) {
                continue;
            }

            if (!MR::isBodySensorTypeMapObj(pMember)) {
                return false;
            }

            if (!MR::isEqualString(MR::getModelResName(pActor), MR::getModelResName(pMember))) {
                return false;
            }

            if (::isWaiting(pMember)) {
                return false;
            }
        }

        return true;
    }

    inline void validateGroupClipping(const GeneralMapParts* pActor) {
        s32 i;
        LiveActor* pMember;
        LiveActorGroup* pGroup = MR::getGroupFromArray(pActor);
        if (pGroup == nullptr) {
            return;
        }

        for (i = 0; i < pGroup->getObjNum(); i++) {
            pMember = pGroup->getActor(i);
            if (MR::isBodySensorTypeMapObj(pMember) && MR::isEqualString(MR::getModelResName(pActor), MR::getModelResName(pMember))) {
                MR::validateClipping(pMember);
            }
        }
    }
}  // namespace

void GeneralMapParts::receiveMsgSwitchBOn() {
    if (!MR::isValidSwitchB(this)) {
        return;
    }

    if (!isNerve(GET_NERVE(GeneralMapParts, HostTypeWait))) {
        mIsPaused = 0;
        return;
    }

    if (MR::isExistActorCamera(mCameraInfo) && !mIsCameraEnded) {
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);
    }

    if (MR::getGroupFromArray(this) != nullptr) {
        MR::invalidateClipping(this);

        if (::isOtherGroupMembersMoving(this)) {
            ::validateGroupClipping(this);
        }
    }

    startMove();
}

void GeneralMapParts::receiveMsgSwitchBOff() {
    if (MR::isValidSwitchB(this)) {
        mIsPaused = 1;
    }
}

void GeneralMapParts::broadcastMsgToAllFunctions(u32 msg) {
    std::for_each(mFunctionArray.begin(), mFunctionArray.end(),
                  std::binder2nd< std::mem_fun1_t< bool, MapPartsFunction, u32 >, u32 >(std::mem_func(&MapPartsFunction::receiveMsg), msg));
}

bool GeneralMapParts::isFixed() const {
    if (mRailMover == nullptr && mRotator == nullptr) {
        return true;
    }

    return false;
}

void GeneralMapParts::startMove() {
    if (MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
        setNerve(GET_NERVE(GeneralMapParts, HostTypeMoveStart));
    } else {
        if (mRailPosture != nullptr) {
            mRailPosture->start();
        }

        if (mRotator != nullptr) {
            mRotator->start();
        }

        if (mRailMover != nullptr) {
            mRailMover->start();
        }

        if (mRailRotator != nullptr) {
            mRailRotator->start();
        }

        setNerve(GET_NERVE(GeneralMapParts, HostTypeMove));
    }
}

void GeneralMapParts::exeWait() {
    if (isNerve(GET_NERVE(GeneralMapParts, HostTypeMove)) && MR::isExistActorCamera(mCameraInfo) && !mIsCameraEnded) {
        if (MR::isStep(this, MR::getActorCameraFrames(this, mCameraInfo))) {
            MR::endActorCamera(this, mCameraInfo, false, -1);
            mIsCameraEnded = 1;
        }
    }
}

void GeneralMapParts::exeWaitForPlayerOn() {
    if (MR::isOnPlayer(MR::getBodySensor(this))) {
        broadcastMsgToAllFunctions(ACTMES_MAPPARTS_ON_PLAYER);
        startMove();
    }
}

void GeneralMapParts::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        if (mRotator != nullptr) {
            mRotator->startWithSignalMotion();
        }

        if (mRailMover != nullptr) {
            mRailMover->startWithSignalMotion();
        }
    }

    if (MR::isMapPartsSignMotionTypeMoveWait(mSignMotionType) && !MR::isOnPlayer(MR::getBodySensor(this))) {
        if (mRotator != nullptr) {
            mRotator->cancelSignalMotion();
        }

        if (mRailMover != nullptr) {
            mRailMover->cancelSignalMotion();
        }

        setNerve(GET_NERVE(GeneralMapParts, HostTypeWaitForPlayerOn));
    } else {
        if (MR::isStep(this, MapParts::getMoveStartSignalTime())) {
            if (mRailMover != nullptr) {
                mRailMover->start();
            }

            if (mRailRotator != nullptr) {
                mRailRotator->start();
            }

            setNerve(GET_NERVE(GeneralMapParts, HostTypeMove));
        }
    }
}

bool GeneralMapParts_FORCE_MATCH(MapPartsFunction* pFunction, u32 msg) {
    return pFunction->MapPartsFunction::receiveMsg(msg);
}
