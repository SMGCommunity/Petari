#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/Boss/TripodBossKillerGenerator.hpp"
#include "Game/Enemy/HomingKiller.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

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
};

TripodBossKillerGenerater::TripodBossKillerGenerater(const char *pName) : TripodBossFixPartsBase(pName) {
    mKiller = nullptr;
    _118 = 2000.0f;
    mActiveLabel = false;
    mHasCollision = true;
    _121 = 0;
    _E4.identity();
}

void TripodBossKillerGenerater::init(const JMapInfoIter &rIter) {
    TripodBossFixPartsBase::init(rIter);
    MR::makeMtxTR(_E4, this);
    initModelManagerWithAnm("TripodBossKillerCannon", nullptr, false);
    MR::connectToScene(this, 0x23, 0xB, 0x1F, -1);
    MR::getJMapInfoArg1NoInit(rIter, &_118);
    initClippingSphere();
    mKiller = new HomingKiller("ホーミングキラー");
    mKiller->initWithoutIter();
    s32 arg;
    MR::getJMapInfoArg2WithInit(rIter, &arg);

    if (arg == 0) {
        mKiller->_15C = 1;
    }
    
    if (mHasCollision) {
        initHitSensor(1);
        MR::addHitSensor(this, "body", 45, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "TripodBossKillerCannon", getSensor("body"), nullptr);
    }

    initEffectKeeper(0, "TripodBoss", false);
    initSound(4, false);
    initNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvNonActive::sInstance);
    MR::useStageSwitchReadAppear(this, rIter);
    if (mActiveLabel > 0) {
        MR::startBck(this, "Shoot", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
    }
    else {
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

bool TripodBossKillerGenerater::receiveMsgEnemyAttack(u32 msg, HitSensor *a2,  HitSensor *a3) {
    if (MR::isMsgExplosionAttack(msg)) {
        return requestBreak();
    }

    return false;
}

void TripodBossKillerGenerater::calcTripodLocalMatrix(TPos3f *pPos) {
    pPos->set(_E4);
}

void TripodBossKillerGenerater::activateTripodBoss() {
    MR::onCalcAnim(this);
    if (mActiveLabel > 0) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvHide::sInstance);
        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    }
    else {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance);
        if (mHasCollision) {
            MR::validateCollisionParts(this);
        }
    }

    _E0 = 1;
    _E1 = 1;
}

void TripodBossKillerGenerater::setLocalMatrix(const TPos3f &pMtx) {
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

    if (MR::isSteppingTripodBossJointID(_CC)) {
        return false;
    }

    TVec3f trans;
    _8C.getTrans(trans);
    if (MR::isDead(mKiller)) {
        if (PSVECDistance(MR::getPlayerPos(), &trans) < _118) {
            TVec3f front;
            MR::calcFrontVec(&front, this);
            TVec3f v4;
            _8C.getTrans(v4);
            mKiller->appear(v4, front);
            setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShootSetting::sInstance);
            return true;
        }
    }

    return false;
}

bool TripodBossKillerGenerater::tryShoot() {
    if (mKiller->isMoveStart()) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShoot::sInstance);
        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryCoolDown() {
    if (MR::isDead(mKiller)) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryReady() {
    if (MR::isGreaterStep(this, 0)) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance);
        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryBreak() {
    if (MR::isEndBreakDownDemoTripodBoss()) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvBreak::sInstance);
        return true;
    }

    return false;
}

bool TripodBossKillerGenerater::tryAbort() {
    if (!MR::isStartedTripodBoss() || MR::isDemoTripodBoss()) {
        if (!MR::isDead(mKiller)) {
            mKiller->kill();
        }

        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance);
        return true;
    }
    else {
        return false;
    }
}

bool TripodBossKillerGenerater::requestBreak() {
    if (isNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance) 
        || isNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShootSetting::sInstance)
        || isNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShoot::sInstance)
        || isNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvCoolDown::sInstance)) {
        
        if (isNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShootSetting::sInstance) && !MR::isDead(mKiller)) {
            mKiller->kill();
        }
    
        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    
        MR::startBck(this, "2ndDemo", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvStop::sInstance);
        return true;
    }

    return false;
}

void TripodBossKillerGenerater::exeShowDemo() {
    if (MR::isStep(this, 90)) {
        if (PSVECDistance(MR::getPlayerPos(), &mPosition) < 230.0f) {
            setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvRestart::sInstance);
            return;
        }

        MR::startBck(this, "2ndDemo", nullptr);

        if (mHasCollision) {
            MR::validateCollisionParts(this);
        }
    }

    updateTripodMatrix();
    MR::isStep(this, 110);
    if (!MR::isDamageDemoTripodBoss()) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance);
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
    killer->_B8.set<f32>(trans);
    killer->_C4.set<f32>(front);
    if (!tryBreak() && !tryAbort() && !tryShoot()) {
        return;
    }
}

void TripodBossKillerGenerater::exeShoot() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shoot", nullptr);
        PSVECDistance(&mPosition, MR::getPlayerPos());
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
        MR::startSound(this, "SE_BM_TRIPOD_CANNON_BREAK", -1, -1);
        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    }
    updateTripodMatrix();

    if (MR::isGreaterStep(this, 300)) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvRestart::sInstance);
    }
}

void TripodBossKillerGenerater::exeRestart() {
    if (MR::isFirstStep(this)) {
        _121 = 1;
    }

    if (_121) {
        if (PSVECDistance(MR::getPlayerPos(), &mPosition) >= 230.0f) {
            MR::startBck(this, "2ndDemo", nullptr);
            MR::startSound(this, "SE_BM_TRIPOD_CANNON_RESTART", -1, -1);
            if (mHasCollision) {
                MR::validateCollisionParts(this);
            }

            _121 = 0;
        }
    }

    updateTripodMatrix();
    if (!_121) {
        if (MR::isBckStopped(this)) {
            setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvReady::sInstance);
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

void TripodBossKillerGenerater::exeHide() {
    if (MR::isDamageDemoTripodBoss()) {
        setNerve(&NrvTripodBossKillerGenerater::TripodBossKillerGeneraterNrvShowDemo::sInstance);
    }

    updateTripodMatrix();
}

void TripodBossKillerGenerater::exeNonActive() {
    
}

TripodBossKillerGenerater::~TripodBossKillerGenerater() {
    
}
