#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Util.hpp"

#ifdef NON_MATCHING
// floating reg order on the inlined matrix set, but oh well
MapPartsRotator::MapPartsRotator(LiveActor *pActor) : MapPartsFunction(pActor, "自身回転") {
    _18 = 0.0f;
    mRotateAngle = 0.0f;
    mRotateStopTime = 0;
    mTargetAngle = 0.0f;
    mRotateSpeed = 0.0f;
    mAngle = 0.0f;
    mRotateAxis = 0;
    mRotateAccelType = 0;
    mRotateType = 1;
    mSignMotionType = 0;
    _A0 = 0.0f;
    mIsOnReverse = false;
    _40.identity();
    _70.identity();
    updateBaseHostMtx();
    _70.setInline(_40);
}
#endif

void MapPartsRotator::init(const JMapInfoIter &rIter) {
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    MR::getMapPartsArgRotateAxis(&mRotateAxis, rIter);
    MR::getMapPartsArgRotateAccelType(&mRotateAccelType, rIter);
    initRotateSpeed(rIter);
    MR::getMapPartsArgRotateStopTime(&mRotateStopTime, rIter);
    MR::getMapPartsArgRotateType(&mRotateType, rIter);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);
    
    if (MR::isNearZero(_18, 0.001f)) {
        initNerve(&NrvMapPartsRotator::HostTypeNeverMove::sInstance);
    }
    else {
        initNerve(&NrvMapPartsRotator::HostTypeWait::sInstance);
    }

    bool cond = 0.0f < _18;
    f32 angle;

    if (cond) {
        angle = mRotateAngle;
    }
    else {
        angle = -mRotateAngle;   
    }

    mTargetAngle = angle;
}

void MapPartsRotator::start() {
    updateTargetAngle();
    updateRotateMtx((AxisType)mRotateAxis, mAngle);
    setNerve(&NrvMapPartsRotator::HostTypeRotate::sInstance);
}

void MapPartsRotator::end() {
    setNerve(&NrvMapPartsRotator::HostTypeWait::sInstance);
}

bool MapPartsRotator::isWorking() const {
    if (!isNerve(&NrvMapPartsRotator::HostTypeNeverMove::sInstance)) {
        return !isNerve(&NrvMapPartsRotator::HostTypeWait::sInstance);
    }

    return false;
}

void MapPartsRotator::startWithSignalMotion() {
    updateBaseHostMtx();
    updateTargetAngle();
    setNerve(&NrvMapPartsRotator::HostTypeRotateStart::sInstance);
}

void MapPartsRotator::cancelSignalMotion() {
    mAngle = _A0;
    updateRotateMtx((AxisType)mRotateAxis, _A0);
    setNerve(&NrvMapPartsRotator::HostTypeWait::sInstance);
}

bool MapPartsRotator::isMoving() const {
    return isNerve(&NrvMapPartsRotator::HostTypeRotate::sInstance);
}

void MapPartsRotator::restartAtEnd() {
    if (mRotateType != 0) {
        if (mRotateType == 1) {
            _18 *= -1.0f;
        }

        updateTargetAngle();

        if (MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
            setNerve(&NrvMapPartsRotator::HostTypeRotateStart::sInstance);
        }
        else {
            setNerve(&NrvMapPartsRotator::HostTypeRotate::sInstance);
        }
    }
}

void MapPartsRotator::initRotateSpeed(const JMapInfoIter &rIter) {
    if (mRotateAccelType == 2) {
        s32 rotate_time = 0;
        MR::getMapPartsArgRotateTime(&rotate_time, rIter);
        _18 = mRotateAngle / rotate_time;
    }
    else {
        MR::getMapPartsArgRotateSpeed(&mRotateSpeed, rIter);
        _18 *= 0.0099999998f;
    }

    if (mRotateAngle < 0.0f) {
        _18 = 0.0f;
    }
}

void MapPartsRotator::calcRotateAxisDir(AxisType type, TVec3f *pDir) const {
    f32 x, y, z;
    switch (type) {
        case 0:
            z = _40.mMtx[2][0];
            y = _40.mMtx[1][0];
            x = _40.mMtx[0][0];
            pDir->set(x, y, z);
            break;
        case 1:
            z = _40.mMtx[2][1];
            y = _40.mMtx[1][1];
            x  = _40.mMtx[0][1];
            pDir->set(x, y, z);
            break;
        case 2:
            z = _40.mMtx[2][2];
            y = _40.mMtx[1][2];
            x = _40.mMtx[0][2];
            pDir->set(x, y, z);
            break;
    }
}

MapPartsRotator::~MapPartsRotator() {

}

namespace NrvMapPartsRotator {
    HostTypeNeverMove HostTypeNeverMove::sInstance;
    HostTypeWait HostTypeWait::sInstance;
    HostTypeRotateStart HostTypeRotateStart::sInstance;
    HostTypeRotate HostTypeRotate::sInstance;
    HostTypeStopAtEnd HostTypeStopAtEnd::sInstance;

    void HostTypeStopAtEnd::execute(Spine *pSpine) const {
        MapPartsRotator* rotator = reinterpret_cast<MapPartsRotator*>(pSpine->mExecutor);
        if (rotator->isStep(rotator->mRotateStopTime)) {
            rotator->restartAtEnd();
        }
    }

    void HostTypeRotate::execute(Spine *pSpine) const {
        MapPartsRotator* rotator = reinterpret_cast<MapPartsRotator*>(pSpine->mExecutor);
        rotator->exeRotate();
    }

    void HostTypeRotateStart::execute(Spine *pSpine) const {
        MapPartsRotator* rotator = reinterpret_cast<MapPartsRotator*>(pSpine->mExecutor);
        rotator->exeRotateStart();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        
    }

    void HostTypeNeverMove::execute(Spine *pSpine) const {

    }
};

bool MapPartsRotator::isOnReverse() const {
    return mIsOnReverse;
}

f32 MapPartsRotator::getRotateSpeed() const {
    return mRotateSpeed;
}