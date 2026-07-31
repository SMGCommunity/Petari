#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    // static const f32 sFlyUpHeight = _;
    static const f32 sFlyUpDirDegreeMax = 45.0f;
    static const f32 sRotateSpeed = 15.0f;
};  // namespace

namespace NrvKoopaFireStairs {
    NEW_NERVE(KoopaFireStairsNrvFly, KoopaFireStairs, Fly);
    NEW_NERVE(KoopaFireStairsNrvBreak, KoopaFireStairs, Break);
};  // namespace NrvKoopaFireStairs

KoopaFireStairs::~KoopaFireStairs() {
}

KoopaFireStairs::KoopaFireStairs(const char* pName, bool a2)
    : LiveActor(pName), mIsKoopaJr(a2), mStair(), _94(0.0f, 0.0f, 1.0f), _A0(0.0f, 0.0f, 0.0f), _AC(0.0f, 1.0f, 0.0f), mBreakModel() {
}

void KoopaFireStairs::init(const JMapInfoIter& rIteR) {
    initModelManagerWithAnm("MeteorStrike", nullptr, false);
    MR::startBrk(this, "MeteorStrike");

    MR::connectToSceneEnemy(this);

    initHitSensor(1);
    MR::addHitSensorEnemyAttack(this, "Fire", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
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

    if (mIsKoopaJr) {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_METEORSHOT_F");
    } else {
        MR::startSound(this, "SE_OJ_KOOPA_FIRE_SHOT");
    }

    setNerve(&NrvKoopaFireStairs::KoopaFireStairsNrvFly::sInstance);
}

void KoopaFireStairs::makeActorDead() {
    LiveActor::makeActorDead();
    mBreakModel->makeActorDead();
}

void KoopaFireStairs::setInfo(const KoopaBattleMapStair* pStair, const TVec3f* a2) {
    mStair = pStair;

    _A0.set(mPosition);

    MR::calcGravity(this);

    if (a2 != nullptr) {
        TVec3f vec1 = mStair->mPosition - _A0;
        TVec3f vec2 = mStair->_AC - _A0;
        TVec3f vec;
        vec.cross(vec1, vec2);

        if (vec.dot(pStair->mGravity) > 0.0f) {
            MR::rotateVecDegree(&_AC, *a2, MR::getRandom(0.0f, ::sFlyUpDirDegreeMax));
        } else {
            MR::rotateVecDegree(&_AC, *a2, MR::getRandom(-::sFlyUpDirDegreeMax, 0.0f));
        }
    }

    if (mIsKoopaJr) {
        mVelocity.zero();

        _94.set(mStair->_AC - _A0);
        MR::normalize(&_94);
    } else {
        s32 breakTime = mStair->calcRemainTimeToBreak();
        TVec3f v10 = mStair->_AC - mPosition;
        f32 length = v10.length();

        mVelocity.set(v10);
        MR::normalize(&mVelocity);
        _94.set(mVelocity);
        mVelocity.scale(length / (breakTime - 1));
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
        MR::startSound(this, "SE_OJ_KOOPA_FIRE_EXPLODE");
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
        MR::hideModel(this);
        setNerve(&NrvKoopaFireStairs::KoopaFireStairsNrvBreak::sInstance);
    } else {
        if (mIsKoopaJr) {
            TVec3f v14(mPosition);
            f32 rate = mStair->calcTimeRate();

            mPosition = _A0 * (1.0f - rate) + mStair->_AC * rate;

            f32 v5 = (100.0f * mStair->calcTimeRate());
            f32 v6 = MR::sinDegree(v5);

            mPosition.add(_AC * v6 * 1500.0f);

            TVec3f v13 = mPosition - v14;

            if (!MR::isNearZero(v13)) {
                MR::normalize(&v13);
                _94.set< f32 >(v13);
            }

            MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_METEOR");
        }

        mRotation.x += ::sRotateSpeed;
        mRotation.x = MR::repeatDegree(mRotation.x);
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
    MR::makeMtxFrontUpPos(&v5, _94, -mGravity, mPosition);
    MR::rotateMtxLocalXDegree(v5, mRotation.x);
    MR::setBaseTRMtx(this, v5);
}
