#include "Game/MapObj/BallRail.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void BallRail_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)3.0f;
}
namespace NrvBallRail {
    NEW_NERVE(BallRailNrvWait, BallRail, Wait);
    NEW_NERVE(BallRailNrvSetUp, BallRail, SetUp);
    NEW_NERVE(BallRailNrvRun, BallRail, Run);
    NEW_NERVE(BallRailNrvNoBind, BallRail, NoBind);
};  // namespace NrvBallRail

BallRail::BallRail(const char* pName)
    : LiveActor(pName), mRailPoints(), _90(), mNumPoints(), mAcceleration(1.5f), mDeceleration(0.995f), _AC(100.0f) {
}

void BallRail::init(const JMapInfoIter& rIter) {
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
    MR::addHitSensor(this, "bind", ATYPE_BALL_RAIL, 8, _AC, offs);
    initNerve(GET_NERVE(BallRail, BallRailNrvWait));
    appear();
}

void BallRail::control() {
}

bool BallRail::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED && isNerve(GET_NERVE(BallRail, BallRailNrvWait))) {
        TVec3f v9(pSender->mHost->mVelocity);
        TVec3f v8 = pSender->mPosition - pReceiver->mPosition;

        if (v8.dot(v9) > 0.0f) {
            return false;
        }

        if (mGravity.dot(v8) > 0.0f) {
            MR::normalizeOrZero(&v8);
            v8.dot(v9);
            MR::zeroVelocity(pSender->mHost);

            return false;
        } else {
            _90 = pSender;

            return true;
        }
    }

    return false;
}

namespace {
    inline void initPointSide(BallRailPoint* pPoint, const TVec3f& rUp) {
        pPoint->_C.cross(pPoint->_24, rUp);
        MR::normalizeOrZero(&pPoint->_C);
    }
}  // namespace

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

    BallRailPoint* pLastPoint;
    BallRailPoint* pPoint;
    TVec3f up(0, 1, 0);

    if (mNumPoints >= 2U) {
        ::initPointSide(mRailPoints, up);
    }

    for (u32 i = 0; i < mNumPoints; i++) {
        pPoint = &mRailPoints[i];
        pPoint->_C.cross(pPoint->_24, up);
        MR::normalizeOrZero(&pPoint->_C);
    }

    if (mNumPoints >= 2U) {
        pLastPoint = &mRailPoints[mNumPoints - 1];
        pLastPoint->_C.cross(pLastPoint->_24, up);
        MR::normalizeOrZero(&pLastPoint->_C);
    }
}

void BallRail::exeWait() {
    MR::moveCoordToNearestPos(this, *MR::getPlayerPos());
    MR::moveTransToCurrentRailPos(this);

    if (_90 != nullptr) {
        setNerve(GET_NERVE(BallRail, BallRailNrvSetUp));
    }
}

void BallRail::exeSetUp() {
    if (MR::isFirstStep(this)) {
        _94 = _90->mPosition;
        MR::zeroVelocity(_90->mHost);
        MR::setRailCoordSpeed(this, 0.0f);
    }

    MR::moveCoord(this, 3.0f);
    MR::moveTransToCurrentRailPos(this);

    TVec3f v7;

    if (MR::isLessStep(this, 15)) {
        TVec3f v6;
        v6.scaleAdd(-_90->mRadius, mGravity, mPosition);
        f32 rate = MR::calcNerveEaseInRate(this, 15);
        MR::vecBlend(_94, v6, &v7, rate);
    } else {
        v7.scaleAdd(-_90->mRadius, mGravity, mPosition);
    }

    if (MR::isStep(this, 15)) {
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
    }

    TVec3f& rVelocity = _90->mHost->mVelocity;
    rVelocity.set(v7 - _90->mPosition);

    if (MR::isGreaterStep(this, 45)) {
        setNerve(GET_NERVE(BallRail, BallRailNrvRun));
    }
}

void BallRail::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::setRailCoordSpeed(this, 0.0f);
    }

    TVec3f v14 = MR::getRailDirection(this);

    TVec3f acceleration = mGravity * mAcceleration;
    MR::accelerateRailCoordSpeed(this, acceleration.dot(v14));
    TVec3f v12;
    MR::slowDownRailCoordSpeed(this, mDeceleration);

    if (!MR::isRailGoingToEnd(this) || MR::getRailCoordSpeed(this) < 6.0f) {
        if (!MR::isRailGoingToEnd(this)) {
            MR::setRailDirectionToEnd(this);
        }

        MR::setRailCoordSpeed(this, 6.0f);
    }

    MR::moveRailRider(this);
    MR::moveTransToCurrentRailPos(this);
    v12.scaleAdd(-_90->mRadius, mGravity, mPosition);
    TVec3f& rVelocity = _90->mHost->mVelocity;
    rVelocity.set(v12 - _90->mPosition);

    if (MR::isRailReachedGoal(this)) {
        TVec3f& rVelocity = _90->mHost->mVelocity;
        rVelocity.set(MR::getRailDirection(this) * MR::getRailCoordSpeed(this));
        _90->receiveMessage(ACTMES_END_BALL_RAIL, getSensor("bind"));
        _90 = nullptr;
        setNerve(GET_NERVE(BallRail, BallRailNrvNoBind));
    }
}

void BallRail::exeNoBind() {
    if (getNerveStep() > 60) {
        setNerve(GET_NERVE(BallRail, BallRailNrvWait));
    }
}

BallRailPoint::BallRailPoint() : _0(0, 0, 0), _C(1, 0, 0), _18(0, 1, 0), _24(0, 0, 1) {
}

BallRail::~BallRail() {
}
