#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace NrvKoopaFireStairs {
    NEW_NERVE(KoopaFireStairsNrvFly, KoopaFireStairs, Fly);
    NEW_NERVE(KoopaFireStairsNrvBreak, KoopaFireStairs, Break);
};  // namespace NrvKoopaFireStairs

KoopaFireStairs::~KoopaFireStairs() {
    return;
}

KoopaFireStairs::KoopaFireStairs(const char* pName, bool a2) : LiveActor(pName) {
    _8C = a2;
    mStair = nullptr;
    _94.set(0.0f, 0.0f, 1.0f);
    _A0.set(0.0f);
    _AC.set(0.0f, 1.0f, 0.0f);
    mBreakModel = nullptr;
}

void KoopaFireStairs::init(const JMapInfoIter& rIteR) {
    initModelManagerWithAnm("MeteorStrike", nullptr, false);
    MR::startBrk(this, "MeteorStrike");
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f offs;
    offs.set(0.0f);
    MR::addHitSensorEnemyAttack(this, "Fire", 8, 100.0f, offs);
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, "Hit");
    MR::setEffectBaseScale(this, "Hit", 2.0f);
    initSound(4, false);
    MR::initShadowVolumeSphere(this, 60.0f);
    initNerve(&NrvKoopaFireStairs::KoopaFireStairsNrvFly::sInstance);
    MR::invalidateClipping(this);
    mBreakModel = MR::createModelObjEnemy("クッパメテオ（壊れ）", "MeteorStrikeBreak", getBaseMtx());
    mBreakModel->kill();
    MR::invalidateClipping(mBreakModel);
    makeActorDead();
}

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

void KoopaFireStairs::exeFly() {
    if (mStair->isBreak()) {
        TVec3f v15(mVelocity);
        MR::normalizeOrZero(&v15);
        v15.scale(100.0f);
        v15.add(mPosition);
        MR::emitEffectHit(this, v15, "Hit");
        MR::emitEffect(this, "MeteorStrikeBreak");
        mVelocity.zero();
        MR::startSound(this, "SE_OJ_KOOPA_FIRE_EXPLODE", -1, -1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraWeak();
        MR::hideModel(this);
        setNerve(&NrvKoopaFireStairs::KoopaFireStairsNrvBreak::sInstance);
    } else {
        if (_8C) {
            TVec3f v14(mPosition);
            f32 rate = mStair->calcTimeRate();
            TVec3f v10(mStair->_AC);
            v10.scale(rate);
            TVec3f v11(_A0);
            v11.scale(1.0f - rate);
            TVec3f v12(v11);
            v12.add(v10);
            mPosition.setPS2(v12);

            f32 v5 = (100.0f * mStair->calcTimeRate());
            f32 v6 = JMath::sSinCosTable.sinLap(v5);

            TVec3f v8(_AC);
            v8.scale(v6);
            TVec3f v9(v8);
            v9.scale(1500.0f);
            mPosition.add(v9);

            TVec3f v13 = mPosition - v14;

            if (!MR::isNearZero(v13)) {
                MR::normalize(&v13);
                _94.set< f32 >(v13);
            }

            MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_METEOR", -1, -1, -1);
        }

        mRotation.x = MR::repeatDegree(mRotation.x + 15.0f);
    }
}

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
