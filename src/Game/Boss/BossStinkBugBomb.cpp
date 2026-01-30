#include "Game/Boss/BossStinkBugBomb.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvBossStinkBugBomb {
    NEW_NERVE(BossStinkBugBombNrvWait, BossStinkBugBomb, Wait);
    NEW_NERVE(BossStinkBugBombNrvExplosion, BossStinkBugBomb, Explosion);
}  // namespace NrvBossStinkBugBomb

BossStinkBugBomb::BossStinkBugBomb(const char* pName) : LiveActor(pName), _8C(0.0f, 0.0f, 0.0f, 1.0f), _9C(0.0f, 0.0f, 0.0f) {}

void BossStinkBugBomb::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("BossStinkBugBomb", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::onCalcGravity(this);
    initHitSensor(2);

    MR::addHitSensorEnemy(this, "body", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "bomb", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(this, "bomb");

    initBinder(60.0f, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(5, false);
    MR::initShadowVolumeSphere(this, 60.0f);
    
    initNerve(&NrvBossStinkBugBomb::BossStinkBugBombNrvWait::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void BossStinkBugBomb::control() {
    _9C.mult(0.96f);
    MR::rotateQuatMoment(&_8C, _9C);
}

void BossStinkBugBomb::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
    MR::setBaseScale(this, mScale);
}

void BossStinkBugBomb::start(const TVec3f& startPos, const TVec3f& startVelocity) {
    mPosition.set(startPos);
    MR::showModel(this);

    MR::validateHitSensor(this, "body");
    MR::invalidateHitSensor(this, "bomb");

    setNerve(&NrvBossStinkBugBomb::BossStinkBugBombNrvWait::sInstance);
    appear();
    mVelocity.set(startVelocity);
}

void BossStinkBugBomb::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if(MR::isSensorEnemyAttack(pSender)) {
        MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
    } else if(MR::isSensorPlayer(pReceiver)) {
        if(MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
            MR::emitEffect(this, "Explosion");
            kill();
        }
    }  
}

bool BossStinkBugBomb::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if(MR::isMsgStarPieceAttack(msg)) {
            MR::emitEffect(this, "Explosion");
            kill();
            return true;
    }

    return false;
}

bool BossStinkBugBomb::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void BossStinkBugBomb::exeWait() {
    if(MR::isFirstStep(this)) {
        MR::startBrk(this, "Signal");
    }

    MR::addVelocityToGravity(this, 1.2f);
    f32 dot = mGravity.dot(mVelocity);

    if(MR::reboundVelocityFromCollision(this, 0.8f, 5.0f, 1.0f) && dot >= 9.5f) {
        f32 liner = MR::getLinerValueFromMinMax(dot, 10.5f, 30.0f, 0.3f, 1.0f);
        MR::startSound(this, "SE_BM_BOSS_BUG_BOMB_GROUND", 100.0f * liner, -1);
        if(MR::isBindedGroundWater(this)) {
            MR::startSound(this, "SE_BM_BOSS_BUG_BOMB_GROUND_WATER", 100.0f*liner, -1);
        }
    }

    if(MR::isLessStep(this, 112)) {
        MR::startLevelSound(this, "SE_BM_BOSS_BUG_BOMB_COUNT1", -1, -1, -1);
    } else {
        MR::startLevelSound(this, "SE_BM_BOSS_BUG_BOMB_COUNT2", -1, -1, -1);
    }

    MR::attenuateVelocity(this, MR::isBinded(this) ? 0.9f :0.98f);

    if(MR::isGreaterStep(this, 150)) {
        setNerve(&NrvBossStinkBugBomb::BossStinkBugBombNrvExplosion::sInstance);
    }
}

void BossStinkBugBomb::exeExplosion() {
    if(MR::isFirstStep(this)) {
        MR::hideModel(this);

        MR::validateHitSensor(this, "bomb");
        MR::invalidateHitSensor(this, "body");

        MR::emitEffect(this, "Explosion");
        MR::startSound(this, "SE_BM_BOSS_BUG_BOMB_BANG", -1, -1);

        if(MR::isBindedGroundWater(this)) {
            MR::startSound(this, "SE_BM_BOSS_BUG_BOMB_BANG_WATER", -1, -1);
        }

        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
        MR::zeroVelocity(this);
    }

    MR::setSensorRadius(this, "bomb", MR::calcNerveEaseOutValue(this, 10, -0.0f, 280.0f));

    if(MR::isGreaterStep(this, 20)) {
        kill();
    }
}
