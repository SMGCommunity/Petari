#include "Game/MapObj/BallRail.hpp"
#include <JSystem/JMath/JMath.hpp>

BallRail::BallRail(const char *pName) : LiveActor(pName) {
    mRailPoints = nullptr;
    _90 = nullptr;
    mNumPoints = 0;
    mAcceleration = 1.5f;
    mDeceleration = 0.995f;
    _AC = 100.0f;
}

void BallRail::init(const JMapInfoIter &rIter) {
    if (MR::getJMapInfoArg0NoInit(rIter, &mAcceleration)) {
        mAcceleration /= 1000.0f;
    }
    
    if (MR::getJMapInfoArg1NoInit(rIter, &mDeceleration)) {
        mDeceleration /= 1000.0f;
    }

    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
    initRailRider(rIter);
    initRailPoints();
    MR::moveCoordToStartPos(this);
    MR::setRailCoordSpeed(this, 0.0f);
    initHitSensor(1);
    TVec3f offs;
    offs.y = _AC;
    offs.x = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensor(this, "bind", 0x7B, 8, _AC, offs);
    initNerve(&NrvBallRail::BallRailNrvWait::sInstance);
    appear();
}

void BallRail::control() {

}

bool BallRail::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 173 && isNerve(&NrvBallRail::BallRailNrvWait::sInstance)) {
        TVec3f v9(a2->mActor->mVelocity);
        TVec3f v8 = a2->mPosition - a3->mPosition;

        if (v8.dot(v9) > 0.0f) {
            return false;
        }

        if (mGravity.dot(v8) > 0.0f) {
            MR::normalizeOrZero(&v8);
            v8.dot(v9);
            MR::zeroVelocity(a2->mActor);
            return false;
        }
        else {
            _90 = a2;
            return true;
        }
    }

    return false;
}

// there is a minor regswap here but I'm marking it as done anyways
// TODO -- fix regswap
void BallRail::initRailPoints() {
    u32 numPoints = (u32)(MR::getRailTotalLength(this) / 100.0f) + 2;
    mNumPoints = numPoints;
    mRailPoints = new BallRailPoint[numPoints];
    MR::moveCoordToStartPos(this);
    MR::setRailCoordSpeed(this, 100.0f);

    for (u32 i = 0; i < mNumPoints - 1; i++) {
        mRailPoints[i]._0 = MR::getRailPos(this);
        mRailPoints[i]._24 = MR::getRailDirection(this);
        MR::moveRailRider(this);
    }

    MR::moveCoordToEndPos(this);
    mRailPoints[mNumPoints - 1]._0 = MR::getRailPos(this);
    mRailPoints[mNumPoints - 1]._24 = MR::getRailDirection(this);
    MR::moveCoordToStartPos(this);
    MR::setRailCoordSpeed(this, 0.0f);

    TVec3f v17(0, 1, 0);

    if (mNumPoints >= 2) {
        BallRailPoint* pnt = mRailPoints;
        PSVECCrossProduct(&pnt->_24, &v17, &pnt->_C);
        MR::normalizeOrZero(&pnt->_C);
    }

    for (u32 i = 0; i < mNumPoints; i++) {
        BallRailPoint* pnt = &mRailPoints[i];
        PSVECCrossProduct(&pnt->_24, &v17, &pnt->_C);
        MR::normalizeOrZero(&pnt->_C);
    }

    if (mNumPoints >= 2) {
        BallRailPoint* lastPnt = &mRailPoints[mNumPoints - 1];
        PSVECCrossProduct(&lastPnt->_24, &v17, &lastPnt->_C);
        MR::normalizeOrZero(&lastPnt->_C);
    }
}

void BallRail::exeWait() {
    MR::moveCoordToNearestPos(this, *MR::getPlayerPos());
    MR::moveTransToCurrentRailPos(this);

    if (_90 != nullptr) {
        setNerve(&NrvBallRail::BallRailNrvSetUp::sInstance);
    }
}

/*
void BallRail::exeSetUp() {
    if (MR::isFirstStep(this)) {
        _94 = _90->mPosition;
        MR::zeroVelocity(_90->mActor);
        MR::setRailCoordSpeed(this, 0.0f);
    }

    MR::moveCoord(this, 3.0f);
    MR::moveTransToCurrentRailPos(this);

    TVec3f v7;

    if (MR::isLessStep(this, 15)) {
        TVec3f v6;
        JMAVECScaleAdd(&mGravity, &mPosition, &v6, -_90->mRadius);
        f32 rate = MR::calcNerveEaseInRate(this, 15);
        MR::vecBlend(_94, v6, &v7, rate);
    }
    else {
        JMAVECScaleAdd(&mGravity, &mPosition, &v7, -_90->mRadius);
    }

    if (MR::isStep(this, 15)) {
        MR::tryRumblePadStrong(this, 0);
    }

    MR::subtractAndSet(_90->mActor->mVelocity, v7, &_90->mPosition);

    if (MR::isGreaterStep(this, 45)) {
        setNerve(&NrvBallRail::BallRailNrvRun::sInstance);
    }
}
*/

/*
void BallRail::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::setRailCoordSpeed(this, 0.0f);
    }

    TVec3f v14(MR::getRailDirection(this));
    TVec3f v13 = mGravity * mAcceleration;
    TVec3f v12;
    MR::accelerateRailCoordSpeed(this, v13.dot(v14));
    MR::slowDownRailCoordSpeed(this, mDeceleration);

    if (!MR::isRailGoingToEnd(this) || MR::getRailCoordSpeed(this) < 6.0f) {
        if (!MR::isRailGoingToEnd(this)) {
            MR::setRailDirectionToEnd(this);
        }

        MR::setRailCoordSpeed(this, 6.0f);
    }

    MR::moveRailRider(this);
    MR::moveTransToCurrentRailPos(this);
    JMAVECScaleAdd(&mGravity, &mPosition, &v12, -_90->mRadius);
    MR::subtractAndSet(_90->mActor->mVelocity, v12, &_90->mPosition);

    if (MR::isRailReachedGoal(this)) {
        LiveActor* actor = _90->mActor;
        TVec3f* vec = &actor->mVelocity;
        MR::multAndSet(vec, MR::getRailDirection(this), MR::getRailCoordSpeed(this));
        getSensor("bind")->receiveMessage(178, _90);
        _90 = nullptr;
        setNerve(&NrvBallRail::BallRailNrvNoBind::sInstance);
    }
}
*/

void BallRail::exeNoBind() {
    if (getNerveStep() > 60) {
        setNerve(&NrvBallRail::BallRailNrvWait::sInstance);
    }
}

BallRailPoint::BallRailPoint() : _0(0, 0, 0), _C(1, 0, 0), _18(0, 1, 0), _24(0, 0, 1) {

}

BallRail::~BallRail() {

}

namespace NrvBallRail {
    INIT_NERVE(BallRailNrvWait);
    INIT_NERVE(BallRailNrvSetUp);
    INIT_NERVE(BallRailNrvRun);
    INIT_NERVE(BallRailNrvNoBind);
};
