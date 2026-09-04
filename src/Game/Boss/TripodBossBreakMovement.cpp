#include "Game/Boss/TripodBossBreakMovement.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SpringValue.hpp"

namespace {
    // static const s32 sMinBreakTimeLevel0 = _;
    // static const s32 sMaxBreakTimeLevel0 = _;
    // static const s32 sMinBreakTimeLevel1 = _;
    // static const s32 sMaxBreakTimeLevel1 = _;
    // static const s32 sMinBreakTimeLevel2 = _;
    // static const s32 sMaxBreakTimeLevel2 = _;
    // static const s32 sMinBreakTimeLevel3 = _;
    // static const s32 sMaxBreakTimeLevel3 = _;
    // static const f32 sMinBreakPower = _;
    // static const f32 sMaxBreakPower = _;
    static const f32 sBreakRandamaize = 1.0f;
    static const f32 sMinStartMoment = 0.01f;
    static const f32 sMaxStartMoment = 0.04f;
    static const f32 sGravityAccel = 0.2f;
    // static const f32 sMinAirFreq = _;
    // static const f32 sMaxAirFreq = _;
    // static const f32 sStartVibrationMinPower = _;
    // static const f32 sStartVibrationMaxPower = _;
};  // namespace

namespace NrvTripodBossBreakMovement {
    NEW_NERVE(TripodBossBreakMovementNrvWait, TripodBossBreakMovement, Wait);
    NEW_NERVE(TripodBossBreakMovementNrvStartBreak, TripodBossBreakMovement, StartBreak);
    NEW_NERVE(TripodBossBreakMovementNrvBreak, TripodBossBreakMovement, Break);
};  // namespace NrvTripodBossBreakMovement

TripodBossBreakMovement::TripodBossBreakMovement(const char* pName)
    : LiveActor(pName), mSpring(), _CC(0, 0, 0), _D8(0, 0, 0), _E4(0, 1, 0), _F0(0.3f), _F4(), _F8(0.9f), _FC(), _100(-1) {
    _8C.identity();
    initNerve(&NrvTripodBossBreakMovement::TripodBossBreakMovementNrvWait::sInstance);
    MR::invalidateClipping(this);
    mSpring = new SpringValue();
    mSpring->setParam(0.0f, 0.0f, 0.2f, 0.9f, 0.0f);
    setBreakDownLevel(1);
}

void TripodBossBreakMovement::start(const TPos3f& a1, s32 a2) {
    _8C.set(a1);
    _8C.getTrans(_C0);

    _CC.zero();
    _CC.set< f32 >(MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize), MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize),
                   MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize));
    MR::normalizeOrZero(&_CC);
    _CC *= MR::getRandom(::sMinStartMoment, ::sMaxStartMoment);

    _F8 = MR::getRandom(0.995f, 0.997f);

    MR::zeroVelocity(this);

    _100 = a2;
    _FC = 0;

    TPos3f jointMtx;
    MR::getTripodBossJointMatrix(&jointMtx, _100);
    jointMtx.getYDir(_E4);

    TVec3f v25;
    v25.set< f32 >(MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize), MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize),
                   MR::getRandom(-::sBreakRandamaize, ::sBreakRandamaize));
    _E4 += v25 * _F0;

    mSpring->mVelocity += MR::getRandom(20.0f, 25.0f);

    setNerve(&NrvTripodBossBreakMovement::TripodBossBreakMovementNrvStartBreak::sInstance);
    MR::invalidateClipping(this);
    appear();
}

void TripodBossBreakMovement::movement() {
    LiveActor::movement();
}

void TripodBossBreakMovement::setBreakDownLevel(s32 level) {
    if (level < 0) {
        level = 0;
    }

    if (level >= 3) {
        level = 3;
    }

    switch (level) {
    case 0:
    case 1:
    case 2:
        _F4 = MR::getRandom(50.0f, 90.0f);
        break;
    case 3:
        _F4 = 50.0f;
        _F0 = 0.0f;
        break;
    }
}

void TripodBossBreakMovement::exeWait() {
}

void TripodBossBreakMovement::exeStartBreak() {
    TVec3f yDir;
    _8C.getYDir(yDir);
    _8C.setTrans(_C0 + yDir * mSpring->mSpringValue);

    mSpring->update();

    if (MR::isGreaterStep(this, 0)) {
        setNerve(&NrvTripodBossBreakMovement::TripodBossBreakMovementNrvBreak::sInstance);
    }
}

void TripodBossBreakMovement::exeBreak() {
    MR::rotateMtxMoment(&_8C, _8C, _CC);

    if (MR::isFirstStep(this)) {
        _8C.getTrans(_C0);
        _D8 += _E4 * _F4;
    }

    _C0 += _D8;
    _8C.setTrans(_C0);

    TVec3f gravity;
    MR::calcGravityVector(this, _C0, &gravity, nullptr, MR::getTripodBossGravityHostID());

    _D8 += gravity * ::sGravityAccel;
    _D8 *= _F8;
}
