#include "Game/Player/MarioBump.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

void Mario::checkBump() {
    if (!mMovementStates.jumping && !mMovementStates._A && mMovementStates._1) {
        if (!isStatusActive(5) && !isStatusActive(1) && !isStatusActive(6) && !isStatusActive(29)) {
            if (!isDamaging() && getPlayerMode() != 5 && !isSwimming()) {
                if (isStickOn()) {
                    if (!_750 && (mMovementStates._23 == 0)) {
                        if (_4DC->isValid()) {
                            const char* floor = MR::getFloorCodeString(_4DC);

                            if (!isSlipPolygon(_4DC) || !strcmp(floor, "NoSlip") || !strcmp(floor, "Sand")) {
                                startBump(_518);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Mario::startBump(const TVec3f& rVec) {
    TVec3f v18 = getTrans() - rVec;
    if (v18.length() >= 100.0f) {
        return;
    }

    TVec3f v21;
    const TVec3f* grav = &getAirGravityVec();
    const TVec3f* norm = _4DC->getNormal(0);
    MR::vecKillElement(*norm, *grav, &v21);

    if (MR::normalizeOrZero(&v21)) {
        return;
    }

    f32 diff = MR::diffAngleAbsHorizontal(mFrontVec, -v21, getAirGravityVec());

    if (diff > 0.52359879f) {
        TVec3f v20;
        if (MR::vecKillElement(getTrans() - rVec, getAirGravityVec(), &v20) > 0.0f) {
        } else {
            if (getCurrentStatus() == 30) {
                closeStatus(mBump);
            }

            setTrans(rVec, "段差");
            changeStatus(mBump);
            mVelocity.zero();
            _3D0 = 0;
            mMovementStates._10 = 0;
            _790 = -(*_4DC->getNormal(0));
            TVec3f v19;
            MR::vecKillElement(-(*_4DC->getNormal(0)), mSideVec, &v19);

            if (!MR::normalizeOrZero(&v19)) {
                _790 = v19;
            }
        }
    }
}

MarioBump::MarioBump(MarioActor* pActor) : MarioState(pActor, 0x1E) {
    _12 = 0;
    _14 = 0.0f;
    _18.zero();
}

bool MarioBump::start() {
    _14 = getPlayer()->_278;

    if (_14 >= 0.0f) {
        if (_14 < 1.0f) {
        }
    }

    s32 blendTimer = 0x14 - static_cast< s32 >(16.0f * _14);

    if (blendTimer < 4) {
        blendTimer = 4;
    }

    mActor->setBlendMtxTimer(static_cast< u16 >(blendTimer));
    _12 = static_cast< u16 >(blendTimer);

    if (isAnimationRun(static_cast< const char* >(nullptr))) {
        stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
    }

    _18 = getFrontVec();
    return true;
}

bool MarioBump::update() {
    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    } else {
        clearVelocity();
        f32 val = getPlayer()->_278;

        if (val < 0.1f) {
            val = 0.1f;
        }

        getPlayer()->_278 = val;
        getPlayer()->updateWalkSpeed();

        if (_12) {
            --_12;
        }
    }

    if (_12 == 0) {
        return false;
    }

    return true;
}

bool MarioBump::close() {
    return true;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
