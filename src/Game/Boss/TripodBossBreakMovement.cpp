#include "Game/Boss/TripodBossBreakMovement.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvTripodBossBreakMovement {
    NEW_NERVE(TripodBossBreakMovementNrvWait, TripodBossBreakMovement, Wait);
    NEW_NERVE(TripodBossBreakMovementNrvStartBreak, TripodBossBreakMovement, StartBreak);
    NEW_NERVE(TripodBossBreakMovementNrvBreak, TripodBossBreakMovement, Break);
};  // namespace NrvTripodBossBreakMovement

TripodBossBreakMovement::TripodBossBreakMovement(const char* pName) : LiveActor(pName), mSpring(nullptr), _CC(0, 0, 0), _D8(0, 0, 0), _E4(0, 1, 0) {
    _F0 = 0.3f;
    _F4 = 0.0f;
    _F8 = 0.9f;
    _FC = 0;
    _100 = -1;
    _8C.identity();
    initNerve(&NrvTripodBossBreakMovement::TripodBossBreakMovementNrvWait::sInstance);
    MR::invalidateClipping(this);
    mSpring = new SpringValue();
    mSpring->setParam(0.0f, 0.0f, 0.2f, 0.9f, 0.0f);
    setBreakDownLevel(1);
}

void TripodBossBreakMovement::start(const TPos3f& a1, s32 a2) {
    _8C.setInline(a1);
    _8C.getTrans(_C0);
    _CC.z = 0.0f;
    _CC.y = 0.0f;
    _CC.x = 0.0f;
    f32 v13 = MR::getRandom(-1.0f, 1.0f);
    f32 v14 = MR::getRandom(-1.0f, 1.0f);
    f32 v15 = MR::getRandom(-1.0f, 1.0f);
    _CC.set< f32 >(v15, v14, v13);
    MR::normalizeOrZero(&_CC);
    f32 v16 = MR::getRandom(0.01f, 0.04f);
    _CC *= v16;
    _F8 = MR::getRandom(0.995f, 0.997f);
    MR::zeroVelocity(this);
    _100 = a2;
    _FC = 0;
    TPos3f jointMtx;
    MR::getTripodBossJointMatrix(&jointMtx, _100);
    jointMtx.getYDir(_E4);

    f32 v17 = MR::getRandom(-1.0f, 1.0f);
    f32 v18 = MR::getRandom(-1.0f, 1.0f);
    f32 v19 = MR::getRandom(-1.0f, 1.0f);
    TVec3f v25;
    v25.set< f32 >(v19, v18, v17);
    f32 val = _F0;
    TVec3f v24(v25);
    v24 *= val;
    _E4 += v24;
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

/* stack issues */
void TripodBossBreakMovement::exeStartBreak() {
    TVec3f dir;
    _8C.getYDir(dir);
    f32 val = mSpring->mSpringValue;
    TVec3f v5(dir);
    v5 *= val;
    TVec3f v6(_C0);
    v6 += v5;
    _8C.setTrans(v6);
    mSpring->update();
    if (MR::isGreaterStep(this, 0)) {
        setNerve(&NrvTripodBossBreakMovement::TripodBossBreakMovementNrvBreak::sInstance);
    }
}

/* more stack issues */
void TripodBossBreakMovement::exeBreak() {
    MR::rotateMtxMoment(&_8C, _8C, _CC);
    if (MR::isFirstStep(this)) {
        _8C.getTrans(_C0);
        f32 val = _F4;
        TVec3f v8(_E4);
        v8 *= val;
        _D8 += v8;
    }

    _C0 += _D8;
    _8C.setTrans(_C0);
    TVec3f grav;
    MR::calcGravityVector(this, _C0, &grav, nullptr, MR::getTripodBossGravityHostID());
    TVec3f v7(grav);
    v7 *= 0.2f;
    _D8 += v7;
    _D8 *= _F8;
}

TripodBossBreakMovement::~TripodBossBreakMovement() {}
