#include "Game/Map/SeaGull.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvSeaGull {
    NEW_NERVE(SeaGullNrvHoverFront, SeaGull, HoverFront);
    NEW_NERVE(SeaGullNrvHoverLeft, SeaGull, HoverLeft);
    NEW_NERVE(SeaGullNrvHoverRight, SeaGull, HoverRight);
};  // namespace NrvSeaGull

namespace {
    static const f32 sRailPosInterval = 500.0f;
    static const s32 sRailMoveTime = 180;
    static const f32 sFlySpeedMax = 10.0f;
    // static const f32 sFlySpeedAccel;
    static const f32 sFrictionRate = 0.99f;
    // static const f32 sDistanceToTurn;
    // static const f32 sDistanceToHoverUp;
    // static const f32 sStepHoverFrontInvalidMax;
    // static const f32 sHoverUpAccel;
    static const f32 sGravity = 0.005f;
    // static const f32 sStepToHoverUpMin;
    // static const f32 sStepToHoverUpMax;
    // static const f32 sStepHoverUpMin;
    // static const f32 sStepHoverUpMax;
    static const f32 sTurnAngleFriction = 0.995f;
    static const f32 sTurnAngleAccel = 0.1f;
    static const f32 sTurnAngleMax = 30.0f;
    static const f32 sTurnAngleMin = -30.0f;
    static const f32 sTurnAngleToRotate = 0.01f;
    // static const f32 sStepTurnMin;
    // static const f32 sStepTurnMax;
    static const s32 sChirpStepMin = 60;
    static const s32 sChirpStepMax = 480;
};  // namespace

void FORCE_INLINE() {
    TVec3f vec;
    vec.sub(vec);
}

void SeaGull_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

SeaGull::SeaGull(SeaGullGroup* pGroup)
    : LiveActor("カモメ"), mSeaGullGroup(pGroup), _90(), _94(), mRailMoveTimer(MR::getRandom(0, ::sRailMoveTime)), _9C(), _A0(0.0f, 1.0f, 0.0f),
      _AC(0.0f, 0.0f, 1.0f), _B8(0.0f, 1.0f, 0.0f), _C4(1.0f, 0.0f, 0.0f), _D0(), mTurnAngle(), _D8(), _DC(), mChirpTimer() {
}

void SeaGull::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::calcActorAxis(&_C4, &_B8, &_AC, this);
    MR::calcActorAxisY(&_A0, this);
    initModelManagerWithAnm("SeaGull", nullptr, false);
    MR::startBck(this, "Fly");
    MR::setBckFrameAtRandom(this);
    MR::connectToSceneEnvironment(this);

    f32 rnd = MR::getRandom(1.0f, MR::getRailTotalLength(mSeaGullGroup) - 1.0f);
    _90 = rnd / ::sRailPosInterval;
    MR::calcRailPosAtCoord(&mPosition, mSeaGullGroup, rnd);
    _94 = MR::isHalfProbability();
    _9C = mSeaGullGroup->updatePosInfo(&_90, _94);

    TVec3f stack_14(_C4);
    TVec3f stack_8(_AC);
    f32 scale = MR::getRandom(-1.0f, 1.0f);
    stack_14.scale(scale);
    scale = MR::getRandom(-1.0f, 1.0f);
    stack_8.scale(scale);
    _AC.set(stack_14);
    _AC.add(stack_8);

    if (MR::isNearZero(_AC)) {
        _AC.set(0.0f, 0.0f, 1.0f);
    } else {
        MR::normalize(&_AC);
    }

    initNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    initSound(4, false);
    mChirpTimer = MR::getRandom((s32)::sChirpStepMin, (s32)::sChirpStepMax);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void SeaGull::exeHoverFront() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)0, (s32)60);
    }

    mTurnAngle *= ::sTurnAngleFriction;

    if (_D0 >= 0) {
        _D0--;
        return;
    }
    TVec3f stack_8(*_9C - mPosition);

    if (mPosition.distance(*_9C) > ::sRailPosInterval) {
        if (_C4.dot(stack_8) > 0.0f) {
            setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverLeft));
        } else {
            setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverRight));
        }
    }
}

void SeaGull::exeHoverLeft() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)60, (s32)120);
    }

    mTurnAngle -= ::sTurnAngleAccel;

    if (MR::isStep(this, _D0)) {
        setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    }
}

void SeaGull::exeHoverRight() {
    if (MR::isFirstStep(this)) {
        _D0 = MR::getRandom((s32)60, (s32)120);
    }

    mTurnAngle += ::sTurnAngleAccel;

    if (MR::isStep(this, _D0)) {
        setNerve(GET_NERVE(SeaGull, SeaGullNrvHoverFront));
    }
}

void SeaGull::control() {
    updateHover();

    mVelocity.mult(::sFrictionRate);

    if (mVelocity.length() > ::sFlySpeedMax) {
        MR::normalize(&mVelocity);
        mVelocity.mult(::sFlySpeedMax);
    }
    _C4.cross(_B8, _AC);
    MR::normalize(&_C4);
    _B8.cross(_AC, _C4);
    MR::normalize(&_B8);
    mRailMoveTimer--;

    if (mRailMoveTimer <= 0) {
        _9C = mSeaGullGroup->updatePosInfo(&_90, _94);
        mRailMoveTimer = ::sRailMoveTime;
    }

    if (mChirpTimer <= 0) {
        MR::startSound(this, "SE_OJ_SEAGULL_CHIRP");
        mChirpTimer = MR::getRandom(::sChirpStepMin, ::sChirpStepMax);
    } else {
        mChirpTimer--;
    }

    if (MR::isJudgedToClipFrustum300m(mPosition, 200.0f)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
    } else {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }
    }
}

void SeaGull::updateHover() {
    if (MR::abs(mTurnAngle) > 0.01f) {
        _A0.set(mGravity);
        _A0.mult(-1.0f);
        mTurnAngle = MR::clamp(mTurnAngle, ::sTurnAngleMin, ::sTurnAngleMax);

        TPos3f mtx;
        mtx.identity();
        mtx.setRotate(_AC, MR::toRadian(mTurnAngle));
        _B8.set(_A0);
        mtx.mult(_B8, _B8);

        mtx.setRotate(_A0, -MR::toRadian(::sTurnAngleToRotate * mTurnAngle));
        mtx.mult(_AC, _AC);
    }

    mVelocity += (_AC * 0.05f);

    if (_DC > 0) {
        mVelocity += (_B8 * 0.04f);

        _DC--;
        if (_DC <= 0) {
            _D8 = MR::getRandom((s32)60, (s32)300);
        }
        return;
    }

    mVelocity.y -= ::sGravity;
    f32 f1 = (*_9C - mPosition).dot(_A0);
    if (f1 < ::sRailPosInterval) {
        _D8 = 300;
        return;
    }
    _D8--;

    if (f1 > ::sRailPosInterval || _D8 <= 0) {
        _DC = MR::getRandom((s32)30, (s32)180);
    }
}

void SeaGull::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    mtx.setTR(_C4, _B8, _AC, mPosition);
    MR::setBaseTRMtx(this, mtx);
}

SeaGullGroup::SeaGullGroup(const char* pName) : LiveActor(pName), mArraySize(), _90() {
}

void SeaGullGroup::init(const JMapInfoIter& rIter) {
    s32 numSeaGulls = 10;
    MR::getJMapInfoArg0NoInit(rIter, &numSeaGulls);
    initRailRider(rIter);
    mPosition.set(MR::getRailPos(this));
    mArraySize = MR::getRailTotalLength(this) / ::sRailPosInterval;
    mArraySize++;

    f32 temp = MR::getRailTotalLength(this) / mArraySize;
    _90 = new TVec3f[mArraySize];

    for (int i = 0; i < mArraySize; i++) {
        MR::calcRailPosAtCoord(&_90[i], this, temp * i);
    }

    for (int i = 0; i < numSeaGulls; i++) {
        SeaGull* gull = new SeaGull(this);
        gull->init(rIter);
    }

    MR::invalidateClipping(this);
    makeActorAppeared();
}

TVec3f* SeaGullGroup::updatePosInfo(s32* a1, bool a2) const {
    if (a2) {
        if (--(*a1) <= 0) {
            *a1 = mArraySize - 1;
        }
    } else {
        if (++(*a1) >= mArraySize) {
            *a1 = 0;
        }
    }

    return &_90[*a1];
}
