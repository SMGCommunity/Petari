#include "Game/Enemy/BombBird.hpp"
#include "Game/Enemy/BombBirdBomb.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static BombBirdParam hFallParam = {0.0f, 1.2f, 0.98f, 0.0f};
    static BombBirdParam hOnGroundParam = {0.0f, 1.2f, 0.8f, 0.0f};
};  // namespace

namespace NrvBombBird {
    NEW_NERVE(HostTypeNrvFlyOnRail, BombBird, FlyOnRail);
    NEW_NERVE(HostTypeNrvFlyOnRailSearch, BombBird, FlyOnRailSearch);
    NEW_NERVE(HostTypeNrvFlyWithAttack, BombBird, FlyWithAttack);
    NEW_NERVE(HostTypeNrvBlow, BombBird, Blow);
    NEW_NERVE(HostTypeNrvEscape, BombBird, Escape);
    NEW_NERVE(HostTypeNrvDamage, BombBird, Damage);
    NEW_NERVE(HostTypeNrvFall, BombBird, Fall);
    NEW_NERVE(HostTypeNrvSwoonStart, BombBird, SwoonStart);
    NEW_NERVE(HostTypeNrvSwoon, BombBird, Swoon);
    NEW_NERVE(HostTypeNrvSwoonEnd, BombBird, SwoonEnd);
    NEW_NERVE(HostTypeNrvTrample, BombBird, Trample);
    NEW_NERVE(HostTypeNrvSinkDown, BombBird, SinkDown);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, BombBird, BindStarPointer, BindStarPointer);
};  // namespace NrvBombBird

BombBird::~BombBird() {}

void BombBird::makeActorDead() {
    LiveActor::makeActorDead();
    killAllBomb();
}

void BombBird::startClipped() {
    if (isNerve(&NrvBombBird::HostTypeNrvEscape::sInstance)) {
        kill();
    } else {
        LiveActor::startClipped();
    }
}

void BombBird::addVelocityToUpFromGround(f32 vel) {
    f32 proj = MR::getShadowNearProjectionLength(this);
    if (3.4028235e38 != proj && proj < 800.0f) {
        if ((800.0f - proj) < vel) {
            vel = (800.0f - proj);
        }

        MR::addVelocityLimit(this, mGravity * -vel);
    }
}

void BombBird::killAllBomb() {
    for (BombBirdBomb** it = mBombs.begin(); it < mBombs.end(); ++it) {
        if (!MR::isDead(*it)) {
            (*it)->kill();
        }
    }

    mBombs[3] = nullptr;
}

void BombBird::exeFlyWithAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");

        if (_A8 == 0) {
            MR::validateClipping(this);
        }
    }

    doFly();

    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        setNerve(&NrvBombBird::HostTypeNrvEscape::sInstance);
    } else {
        BombBirdBomb* bomb = mBombs[3];
        if (bomb != nullptr) {
            bomb->mVelocity.z = bomb->mVelocity.y = bomb->mVelocity.x = 0.0f;
        }

        if (MR::isStep(this, 130)) {
            MR::startSound(mBombs[3], "SE_EM_BOMBBIRD_DROP_BOMB", -1, -1);
            TVec3f v3(mGravity * -10.0f);
            TVec3f v4(_9C * 0.0f);
            TVec3f v5(v4);
            v5 -= v3;
            mBombs[3]->start(mPosition, v5);
            mBombs[3] = nullptr;
            _A9 = 0;
        }

        if (MR::isActionEnd(this)) {
            if (MR::calcDistanceToPlayerH(this) < 2000.0f) {
                setNerve(&NrvBombBird::HostTypeNrvFlyOnRailSearch::sInstance);
            } else {
                setNerve(&NrvBombBird::HostTypeNrvFlyOnRail::sInstance);
            }
        }
    }
}

void BombBird::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "DownSpin");
        MR::startSound(this, "SE_EV_BOMBBIRD_DEAD", -1, -1);
        MR::startSound(this, "SE_EM_BOMBBIRD_DAMAGE", -1, -1);
        MR::startBlowHitSound(this);
        MR::invalidateClipping(this);
    }

    if (mSpinHitController->execute(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        MR::emitEffect(this, "Death");
    }
}

void BombBird::exeEscape() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::validateClipping(this);
    }

    TVec3f v6(mPosition);
    v6 -= *MR::getPlayerPos();
    MR::vecKillElement(v6, mGravity, &v6);
    MR::normalizeOrZero(&v6);

    if (!MR::isNearZero(v6)) {
        MR::moveAndTurnToDirection(this, &_9C, v6, 0.3f, 1.0f, 0.959f, 4.4836625f);
    }

    TVec3f v3(mGravity * 5.0f);
    TVec3f v4(_9C * 5.0f);
    TVec3f v5(v4);
    v5 -= v3;
    mVelocity.set< f32 >(v5);

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBombBird::HostTypeNrvEscape::sInstance);
    }
}

void BombBird::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Damage");
        MR::startSound(this, "SE_EV_BOMBBIRD_DAMAGE", -1, -1);
        MR::startSound(this, "SE_EM_BOMBBIRD_DAMAGE", -1, -1);
        MR::invalidateClipping(this);
    }

    BombBirdParam* param = &hFallParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBombBird::HostTypeNrvFall::sInstance);
    }
}

void BombBird::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Fall");
        MR::invalidateClipping(this);
    }

    BombBirdParam* param = &hFallParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);

    if (MR::isOnGround(this)) {
        MR::startAction(this, "SwoonStart");
        setNerve(&NrvBombBird::HostTypeNrvSwoonStart::sInstance);
    }
}

void BombBird::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
    if (MR::isOnGround(this)) {
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    }

    BombBirdParam* param = &hOnGroundParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);
    TVec3f area(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&area, this)) {
        mVelocity.set< f32 >(area * 1.0f);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBombBird::HostTypeNrvSwoon::sInstance);
    }
}

void BombBird::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Swoon");
        MR::invalidateClipping(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

    if (MR::isOnGround(this)) {
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    }

    MR::moveAndTurnToPlayer(this, &_9C, hOnGroundParam._0, hOnGroundParam._4, hOnGroundParam._8, hOnGroundParam._C);
    TVec3f area(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&area, this)) {
        mVelocity.set< f32 >(area * 1.0f);
    }

    if (MR::isGreaterStep(this, 240)) {
        setNerve(&NrvBombBird::HostTypeNrvSwoonEnd::sInstance);
    }
}

void BombBird::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SwoonEnd");
        MR::startSound(this, "SE_EV_BOMBBIRD_RECOVER", -1, -1);
        MR::invalidateClipping(this);
    }

    if (MR::isOnGround(this)) {
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    }

    BombBirdParam* param = &hOnGroundParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);

    TVec3f area(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&area, this)) {
        mVelocity.set< f32 >(area * 1.0f);
    }

    if (MR::isGreaterEqualStep(this, 30)) {
        if (MR::isStep(this, 30)) {
            MR::addVelocityJump(this, 10.0f);
        }

        doFly();
        addVelocityToUpFromGround(5.0f);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBombBird::HostTypeNrvFlyOnRail::sInstance);
    }
}

void BombBird::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "DownStep");
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::invalidateClipping(this);
    }

    if (MR::isOnGround(this)) {
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    }

    BombBirdParam* param = &hOnGroundParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);

    if (MR::isStep(this, 36)) {
        TVec3f pos = mGravity * getSensor("body")->mRadius;
        pos -= mPosition;
        // TVec3f pos(mGravity * getSensor("body")->mRadius);
        // pos -= mPosition;
        MR::appearCoinPop(this, pos, 1);
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        MR::emitEffect(this, "Death");
        kill();
    }
}

void BombBird::exeSinkDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SinkDown");
        MR::invalidateShadow(this, nullptr);
        MR::invalidateClipping(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_SINK", -1, -1, -1);
    MR::startLevelSound(this, "SE_EV_LV_BOMBBIRD_SINK", -1, -1, -1);
    BombBirdParam* param = &hOnGroundParam;
    MR::moveAndTurnToPlayer(this, &_9C, param->_0, param->_4, param->_8, param->_C);

    if (MR::isActionEnd(this)) {
        MR::emitEffect(this, "SinkDeath");
        kill();
    }
}

void BombBird::exeBindStarPointer() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    killAllBomb();
    if (MR::updateActorStateAndNextNerve(this, mStarPointer, &NrvBombBird::HostTypeNrvFlyOnRail::sInstance)) {
        return;
    }
}
