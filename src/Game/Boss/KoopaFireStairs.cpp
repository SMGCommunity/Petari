#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"

namespace NrvKoopaFireStairs {
    NEW_NERVE(KoopaFireStairsNrvFly, KoopaFireStairs, Fly);
    NEW_NERVE(KoopaFireStairsNrvBreak, KoopaFireStairs, Break);
};  // namespace NrvKoopaFireStairs

KoopaFireStairs::~KoopaFireStairs() {
    return;
}

// KoopaFireStairs::KoopaFireStairs
// KoopaFireStairs::init

void KoopaFireStairs::appear() {
    LiveActor::appear();
    MR::showModel(this);
    MR::emitEffect(this, "MeteorStrike");

    if (_8C) {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_METEORSHOT_F", -1, -1);
    } else {
        MR::startSound(this, "SE_OJ_KOOPA_FIRE_SHOT", -1, -1);
    }

    setNerve(&NrvKoopaFireStairs::KoopaFireStairsNrvFly::sInstance);
}

void KoopaFireStairs::makeActorDead() {
    LiveActor::makeActorDead();
    mBreakModel->makeActorDead();
}

void KoopaFireStairs::setInfo(const KoopaBattleMapStair* pStair, const TVec3f* a2) {
    mStair = pStair;
    _A0.set< f32 >(mPosition);
    MR::calcGravity(this);

    if (a2 != nullptr) {
        TVec3f v13 = mStair->mPosition - _A0;
        TVec3f v12 = mStair->_AC - _A0;
        TVec3f v11;
        PSVECCrossProduct(&v13, &v12, &v11);

        f32 deg;

        if (v11.dot(mGravity) > 0.0f) {
            deg = MR::getRandom(0.0f, 45.0f);
            MR::rotateVecDegree(&_AC, *a2, deg);
        } else {
            deg = MR::getRandom(-45.0f, 0.0f);
            MR::rotateVecDegree(&_AC, *a2, deg);
        }
    }

    if (_8C) {
        mVelocity.zero();
        _94.set< f32 >(mStair->_AC - _A0);
        MR::normalize(&_94);
    } else {
        s32 breakTime = mStair->calcRemainTimeToBreak();
        TVec3f v10 = mStair->_AC - mPosition;
        f32 v8 = v10.length();
        mVelocity.set< f32 >(v10);
        MR::normalize(&mVelocity);
        _94.set< f32 >(mVelocity);
        mVelocity.scale(v8 / (breakTime - 1));
    }
}

// KoopaFireStairs::exeFly

void KoopaFireStairs::exeBreak() {
    if (MR::isFirstStep(this)) {
        mBreakModel->appear();
        MR::startAction(mBreakModel, "Break");
    }

    if (MR::isActionEnd(mBreakModel)) {
        kill();
    }
}

void KoopaFireStairs::calcAndSetBaseMtx() {
    TPos3f v5;
    v5.identity();
    TVec3f v4;
    JMathInlineVEC::PSVECNegate(&mGravity, &v4);
    MR::makeMtxFrontUpPos(&v5, _94, v4, mPosition);
    MR::rotateMtxLocalXDegree(v5, mRotation.x);
    MR::setBaseTRMtx(this, v5);
}
