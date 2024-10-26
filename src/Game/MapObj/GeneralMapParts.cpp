#include "Game/MapObj/GeneralMapParts.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

GeneralMapParts::GeneralMapParts(const char *pName) : MapParts(pName) {
    mCameraInfo = nullptr;
    mMoveConditionType = 0;
    mSignMotionType = 0;
    mShadowType = 0;
    mFunctionArray.mCount = 0;
    mAppearController = nullptr;
    mRailRotator = nullptr;
    mRailMover = nullptr;
    mRotator = nullptr;
    mGuideDrawer = nullptr;
    mRailPosture = nullptr;
    _E4 = 0;
    _E5 = 0;
}

void GeneralMapParts::init(const JMapInfoIter &rIter) {
    MapParts::init(rIter);
    MR::getMapPartsArgShadowType(&mShadowType, rIter);
    MR::initMapPartsShadow(this, rIter);
    MR::getMapPartsArgMoveConditionType(&mMoveConditionType, rIter);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);
    initMapPartsFunction(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(&NrvGeneralMapParts::HostTypeWait::sInstance);
    initSensorType(rIter);
    initGravity(rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    
    if (MR::useStageSwitchReadB(this, rIter)) {
        void (GeneralMapParts::*switchOff)(void) = &GeneralMapParts::receiveMsgSwitchBOff;
        void (GeneralMapParts::*switchOn)(void) = &GeneralMapParts::receiveMsgSwitchBOn;
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, switchOff), MR::Functor(this, switchOn));
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
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

    if (mGuideDrawer) {
        mGuideDrawer->start();
    }

    if (MR::isValidSwitchB(this)) {
        setNerve(&NrvGeneralMapParts::HostTypeWait::sInstance);
    }
    else if (!MR::isMoveStartTypeUnconditional(mMoveConditionType)) {
        setNerve(&NrvGeneralMapParts::HostTypeWaitForPlayerOn::sInstance);
    }
    else {
        startMove();
    }
}

// GeneralMapParts::kill

bool GeneralMapParts::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (mAppearController->receiveMsg(msg)) {
        return true;
    }

    if (msg == 207) {
        broadcastMsgToAllFunctions(207);
        return true;
    }

    return MR::receiveMapPartsRotateMsg(this, msg, mRailMover, mRailRotator);
}

void GeneralMapParts::control() {
    if (!_E5) {
        if (mAppearController) {
            mAppearController->movement();
        }

        if (mRailMover) {
            mRailMover->movement();
            if (MR::isValidSwitchDead(this) && mRailMover->isReachedEnd()) {
                MR::onSwitchDead(this);
            }
        }

        if (mRailPosture) {
            mRailPosture->movement();
        }

        if (mRotator) {
            mRotator->movement();
        }

        if (mRailRotator) {
            mRailRotator->movement();
        }

        if (mGuideDrawer) {
            mGuideDrawer->movement();
        }

        bool v9 = false;
        if (mRailMover && mRailMover->isWorking()) {
            v9 = true;
        }

        if (v9) {
            mPosition.setInline(mRailMover->_28);
        }
    }
}

void GeneralMapParts::calcAndSetBaseMtx() {
    bool v3 = false;
    if (mRotator && mRotator->isWorking()) {
        v3 = true;
    }

    bool v5 = false;
    if (mRailRotator && mRailRotator->isWorking()) {
        v5 = true;
    }

    bool v7 = false;
    if (mRailPosture && mRailPosture->isWorking()) {
        v7 = true;
    }

    if (!(v5 || v3 || v7)) {
        LiveActor::calcAndSetBaseMtx();
    }
    else {
        TPos3f mtx;
        mtx.identity();

        if (v7) {
            mtx.concat(mRailPosture->_18);
        }

        if (v5) {
            mtx.concat(mRailRotator->_5C);
        }

        if (v3) {
            mtx.concat(mRotator->getRotateMtx());
        }

        mtx.mMtx[0][3] = mPosition.x;
        mtx.mMtx[1][3] = mPosition.y;
        mtx.mMtx[2][3] = mPosition.z;
        MR::setBaseTRMtx(this, mtx);
    }
}

void GeneralMapParts::initMapPartsFunction(const JMapInfoIter &rIter) {
    mAppearController = new MapPartsAppearController(this);
    mAppearController->init(rIter);
    mFunctionArray.push_back(mAppearController);
    f32 rotate_speed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotate_speed, rIter); 
    bool hasRotateSpeed = 0.0f != rotate_speed;

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

void GeneralMapParts::initSensorType(const JMapInfoIter &rIter) {
    if (MR::isMapPartsPressOn(rIter)) {
        MR::setBodySensorTypePress(this);
    }
    else if (!isFixed()) {
        MR::setBodySensorTypeMoveCollision(this);
    }
    else {
        MR::setBodySensorTypeMapObj(this);
    }
}

void GeneralMapParts::initGravity(const JMapInfoIter &rIter) {
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);

    if (!MR::isMapPartsShadowTypeNone(mShadowType)) {
        if (isFixed()) {
            MR::calcGravity(this);
        }
        else {
            MR::onCalcGravity(this);
        }
    }
}

/*
void GeneralMapParts::receiveMsgSwitchBOn() {
    if (!MR::isValidSwitchB(this)) {
        return;
    }

    if (!isNerve(&NrvGeneralMapParts::HostTypeWait::sInstance)) {
        _E5 = 0;
    }

    if (MR::isExistActorCamera(mCameraInfo) && !_E4) {
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);
    }

    if (MR::getGroupFromArray(this)) {
        MR::invalidateClipping(this);
        bool flag = false;
        if (isNerve(&NrvGeneralMapParts::HostTypeWait::sInstance) || isNerve(&NrvGeneralMapParts::HostTypeWaitForPlayerOn::sInstance)) {
            flag = true;
        }
    }
}
*/

void GeneralMapParts::receiveMsgSwitchBOff() {
    if (MR::isValidSwitchB(this)) {
        _E5 = 1;
    }
}

// GeneralMapParts::broadcastMsgToAllFunctions

bool GeneralMapParts::isFixed() const {
    if (!mRailMover && !mRotator) {
        return true;
    }

    return false;
}

void GeneralMapParts::startMove() {
    if (!MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
        setNerve(&NrvGeneralMapParts::HostTypeMoveStart::sInstance);
    }
    else {
        if (mRailPosture) {
            mRailPosture->start();
        }

        if (mRotator) {
            mRotator->start();
        }

        if (mRailMover) {
            mRailMover->start();
        }

        if (mRailRotator) {
            mRailRotator->start();
        }

        setNerve(&NrvGeneralMapParts::HostTypeMove::sInstance);
    }
}

void GeneralMapParts::exeWait() {
    if (isNerve(&NrvGeneralMapParts::HostTypeMove::sInstance) && MR::isExistActorCamera(mCameraInfo) && !_E4) {
        if (MR::isStep(this, MR::getActorCameraFrames(this, mCameraInfo))) {
            MR::endActorCamera(this, mCameraInfo, false, -1);
            _E4 = 1;
        }
    }
}

void GeneralMapParts::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        if (mRotator) {
            mRotator->startWithSignalMotion();
        }

        if (mRailMover) {
            mRailMover->startWithSignalMotion();
        }
    }

    if (MR::isMapPartsSignMotionTypeMoveWait(mSignMotionType) && MR::isOnPlayer(MR::getBodySensor(this))) {
        if (mRotator) {
            mRotator->cancelSignalMotion();
        }

        if (mRailMover) {
            mRailMover->cancelSignalMotion();
        }

        setNerve(&NrvGeneralMapParts::HostTypeWaitForPlayerOn::sInstance);
    }
    else {
        if (MR::isStep(this, MapParts::getMoveStartSignalTime())) {
            if (mRailMover) {
                mRailMover->start();
            }

            if (mRailRotator) {
                mRailRotator->start();
            }

            setNerve(&NrvGeneralMapParts::HostTypeMove::sInstance);
        }
    }
}

namespace NrvGeneralMapParts {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeWaitForPlayerOn);
    INIT_NERVE(HostTypeMoveStart);
    INIT_NERVE(HostTypeMove);

    void HostTypeMove::execute(Spine *pSpine) const {
        GeneralMapParts* part = reinterpret_cast<GeneralMapParts*>(pSpine->mExecutor);
        part->exeWait();
    }

    void HostTypeMoveStart::execute(Spine *pSpine) const {
        GeneralMapParts* part = reinterpret_cast<GeneralMapParts*>(pSpine->mExecutor);
        part->exeMoveStart();
    }

    void HostTypeWaitForPlayerOn::execute(Spine *pSpine) const {
        GeneralMapParts* part = reinterpret_cast<GeneralMapParts*>(pSpine->mExecutor);
        if (MR::isOnPlayer(MR::getBodySensor(part))) {
            part->broadcastMsgToAllFunctions(0xCA);
            part->startMove();   
        }
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        GeneralMapParts* part = reinterpret_cast<GeneralMapParts*>(pSpine->mExecutor);
        part->exeWait();
    }
};

