#include "Game/Boss/TripodBossKillerGenerater.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/Enemy/HomingKiller.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void TripodBossKillerGenerater_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    // static const s32 sForceKillTime = _;
    static const s32 sDemoAnimStartDelayTime = 90;
    static const s32 sStopTime = 300;
    static const f32 sCanRestartPlayerDistance = 230.0f;
    // static const f32 sStrongPadRumbleDistance = _;
    // static const f32 sMiddlePadRumbleDistance = _;
    // static const s32 sDemoAnimTime = _;
    // static const s32 sCoolDownTime = _;
};  // namespace

namespace NrvTripodBossKillerGenerater {
    NEW_NERVE(TripodBossKillerGeneraterNrvNonActive, TripodBossKillerGenerater, NonActive);
    NEW_NERVE(TripodBossKillerGeneraterNrvHide, TripodBossKillerGenerater, Hide);
    NEW_NERVE(TripodBossKillerGeneraterNrvShowDemo, TripodBossKillerGenerater, ShowDemo);
    NEW_NERVE(TripodBossKillerGeneraterNrvReady, TripodBossKillerGenerater, Ready);
    NEW_NERVE(TripodBossKillerGeneraterNrvShootSetting, TripodBossKillerGenerater, ShootSetting);
    NEW_NERVE(TripodBossKillerGeneraterNrvShoot, TripodBossKillerGenerater, Shoot);
    NEW_NERVE(TripodBossKillerGeneraterNrvCoolDown, TripodBossKillerGenerater, CoolDown);
    NEW_NERVE(TripodBossKillerGeneraterNrvBreak, TripodBossKillerGenerater, Break);
    NEW_NERVE(TripodBossKillerGeneraterNrvStop, TripodBossKillerGenerater, Stop);
    NEW_NERVE(TripodBossKillerGeneraterNrvRestart, TripodBossKillerGenerater, Restart);
};  // namespace NrvTripodBossKillerGenerater

TripodBossKillerGenerater::TripodBossKillerGenerater(const char* pName)
    : TripodBossFixPartsBase(pName), mKiller(), mCanShootSettingPlayerDistance(2000.0f), mActiveLabel(), mHasCollision(true), _121() {
    _E4.identity();
}

void TripodBossKillerGenerater::init(const JMapInfoIter& rIter) {
    TripodBossFixPartsBase::init(rIter);
    MR::makeMtxTR(_E4, this);
    initModelManagerWithAnm("TripodBossKillerCannon", nullptr, false);
    MR::connectToScene(this, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_TripodBoss, MR::DrawType_None);
    MR::getJMapInfoArg1NoInit(rIter, &mCanShootSettingPlayerDistance);
    initClippingSphere();

    mKiller = new HomingKiller("ホーミングキラー");
    mKiller->initWithoutIter();

    s32 arg2;
    MR::getJMapInfoArg2WithInit(rIter, &arg2);

    if (arg2 == 0) {
        mKiller->mDisableChase = true;
    }

    if (mHasCollision) {
        initHitSensor(1);
        MR::addHitSensor(this, "body", ATYPE_TRIPODBOSS_KILLER_GENERATER, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "TripodBossKillerCannon", getSensor("body"), nullptr);
    }

    initEffectKeeper(0, "TripodBoss", false);
    initSound(4, false);
    initNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvNonActive));
    MR::useStageSwitchReadAppear(this, rIter);

    if (mActiveLabel > 0) {
        MR::startBck(this, "Shoot", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
    } else {
        MR::startBck(this, "2ndDemo", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
    }

    if (mHasCollision) {
        MR::invalidateCollisionParts(this);
    }

    makeActorAppeared();
}

void TripodBossKillerGenerater::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "ExplosionM");
}

bool TripodBossKillerGenerater::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        return requestBreak();
    }

    return false;
}

void TripodBossKillerGenerater::calcTripodLocalMatrix(TPos3f* pMtx) {
    pMtx->set(_E4);
}

void TripodBossKillerGenerater::activateTripodBoss() {
    MR::onCalcAnim(this);

    if (mActiveLabel > 0) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvHide));

        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    } else {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady));

        if (mHasCollision) {
            MR::validateCollisionParts(this);
        }
    }

    _E0 = true;
    _E1 = true;
}

void TripodBossKillerGenerater::setLocalMatrix(const TPos3f& pMtx) {
    _E4.set(pMtx);
}

void TripodBossKillerGenerater::setActiveLebel(s32 label) {
    mActiveLabel = label;
}

void TripodBossKillerGenerater::setHasCollision(bool hasCollision) {
    mHasCollision = hasCollision;
}

bool TripodBossKillerGenerater::tryShootSetting() {
    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this)) {
        return false;
    }

    if (MR::isSteppingTripodBossJointID(mJointID)) {
        return false;
    }

    TVec3f trans;
    _8C.getTrans(trans);

    if (MR::isDead(mKiller)) {
        if (MR::getPlayerPos()->distance(trans) < mCanShootSettingPlayerDistance) {
            TVec3f front;
            MR::calcFrontVec(&front, this);

            TVec3f trans;
            _8C.getTrans(trans);

            mKiller->appear(trans, front);

            setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShootSetting));

            return true;
        }
    }

    return false;
}

bool TripodBossKillerGenerater::tryShoot() {
    if (mKiller->isMoveStart()) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShoot));

        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryCoolDown() {
    if (MR::isDead(mKiller)) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvCoolDown));

        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryReady() {
    if (MR::isGreaterStep(this, 0)) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady));

        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryBreak() {
    if (MR::isEndBreakDownDemoTripodBoss()) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvBreak));

        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryAbort() {
    if (!MR::isStartedTripodBoss() || MR::isDemoTripodBoss()) {
        if (!MR::isDead(mKiller)) {
            mKiller->kill();
        }

        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady));

        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::requestBreak() {
    if (isNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady)) ||
        isNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShootSetting)) ||
        isNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShoot)) ||
        isNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvCoolDown))) {
        if (isNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShootSetting)) && !MR::isDead(mKiller)) {
            mKiller->kill();
        }

        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }

        MR::startBck(this, "2ndDemo", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvStop));

        return true;
    }

    return false;
}

void TripodBossKillerGenerater::exeNonActive() {
}

void TripodBossKillerGenerater::exeHide() {
    if (MR::isDamageDemoTripodBoss()) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvShowDemo));
    }

    updateTripodMatrix();
}

void TripodBossKillerGenerater::exeShowDemo() {
    if (MR::isStep(this, ::sDemoAnimStartDelayTime)) {
        if (MR::getPlayerPos()->distance(mPosition) < ::sCanRestartPlayerDistance) {
            setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvRestart));
            return;
        }

        MR::startBck(this, "2ndDemo", nullptr);

        if (mHasCollision) {
            MR::validateCollisionParts(this);
        }
    }

    updateTripodMatrix();

    if (MR::isStep(this, 110)) {
    }

    if (!MR::isDamageDemoTripodBoss()) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady));
    }
}

void TripodBossKillerGenerater::exeReady() {
    updateTripodMatrix();

    if (!tryBreak() && !tryAbort() && !tryShootSetting()) {
        return;
    }
}

void TripodBossKillerGenerater::exeShootSetting() {
    updateTripodMatrix();

    TVec3f front;
    MR::calcFrontVec(&front, this);

    TVec3f trans;
    _8C.getTrans(trans);

    HomingKiller* killer = mKiller;
    killer->mBasePos.set< f32 >(trans);
    killer->mBaseFront.set< f32 >(front);

    if (!tryBreak() && !tryAbort() && !tryShoot()) {
        return;
    }
}

void TripodBossKillerGenerater::exeShoot() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shoot", nullptr);
        mPosition.distance(*MR::getPlayerPos());
    }

    updateTripodMatrix();

    if (!tryBreak() && !tryAbort() && !tryCoolDown()) {
        return;
    }
}

void TripodBossKillerGenerater::exeCoolDown() {
    updateTripodMatrix();

    if (!tryBreak() && !tryAbort() && !tryReady()) {
        return;
    }
}

void TripodBossKillerGenerater::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "ExplosionS");
        MR::startSound(this, "SE_BM_TRIPOD_CANNON_BREAK");

        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    }

    updateTripodMatrix();

    if (MR::isGreaterStep(this, ::sStopTime)) {
        setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvRestart));
    }
}

void TripodBossKillerGenerater::exeRestart() {
    if (MR::isFirstStep(this)) {
        _121 = true;
    }

    if (_121) {
        if (MR::getPlayerPos()->distance(mPosition) >= ::sCanRestartPlayerDistance) {
            MR::startBck(this, "2ndDemo", nullptr);
            MR::startSound(this, "SE_BM_TRIPOD_CANNON_RESTART");

            if (mHasCollision) {
                MR::validateCollisionParts(this);
            }

            _121 = false;
        }
    }

    updateTripodMatrix();

    if (!_121) {
        if (MR::isBckStopped(this)) {
            setNerve(GET_NERVE(TripodBossKillerGenerater, TripodBossKillerGeneraterNrvReady));
        }
    }
}

void TripodBossKillerGenerater::exeBreak() {
    if (MR::isFirstStep(this)) {
        if (!MR::isDead(mKiller)) {
            mKiller->kill();
        }

        kill();
    }
}
