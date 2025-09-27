#include "Game/Player/FireMarioBall.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    f32 cSensorRadius = 80.0f;
    f32 cBinderRadius = 50.0f;
    f32 cThrowSpeed = 30.0f;
    f32 cForceKillDistance = 3000.0f;
    f32 cBoundReduction = 0.9f;
    f32 cGravityAcc = 2.0f;
}

namespace NrvFireMarioBall {
    NEW_NERVE(FireMarioBallNrvThrow, FireMarioBall, Throw);
}

FireMarioBall::~FireMarioBall() {}
 
FireMarioBall::FireMarioBall(const char* pName) : LiveActor(pName) {
    _8C = 0;
}

void FireMarioBall::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("MarioFireBall", 0, false);
    MR::connectToSceneEnemyDecoration(this);
    initSensor();
    initBinder(cBinderRadius, 0.0f, 0);
    initEffectKeeper(4, "MarioFireBall", false);
    initSound(4, false);
    MR::initShadowVolumeCylinder(this, 25.0f);
    MR::onShadowVolumeCutDropLength(this, 0);
    MR::setShadowVolumeEndDropOffset(this, 0, 50.0f);
    MR::invalidateClipping(this);
    initNerve(&NrvFireMarioBall::FireMarioBallNrvThrow::sInstance);
    _90 = 0;
    makeActorDead();
}

void FireMarioBall::appear() {
    LiveActor::appear();
    setNerve(&NrvFireMarioBall::FireMarioBallNrvThrow::sInstance);
    MR::onCalcGravity(this);
    MR::onCalcShadow(this, 0);
}

void FireMarioBall::kill() {
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_MARIO_FIRE_BALL_BREAK", -1, -1);
    LiveActor::kill();
}

void FireMarioBall::appearAndThrow(const TVec3f& v1 , const TVec3f& v2) {
    mPosition.set<f32>(v1);
    mVelocity.set<f32>(v2 * cThrowSpeed);
    mRotation.set<f32>(0.0f, 57.295776f * JMath::sAtanTable.atan2_(mVelocity.x, mVelocity.z), 0.0f);
    MR::tryRumblePadWeak(this, 0);
    appear();
}

void FireMarioBall::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        attackFire(pReceiver);
    }
}

bool FireMarioBall::attackFire(HitSensor* pReceiver) {
    if (MR::sendArbitraryMsg(ACTMES_FIREBALL_ATTACK, pReceiver, getSensor("body"))) {
        MR::tryRumblePadMiddle(this, 0);
        kill();

        return true;
    }

    return false;
}

void FireMarioBall::initSensor() {
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 8, cSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
}

HitSensor* FireMarioBall::isBindedAny() const {
    if (MR::isBindedGround(this)) {
        return MR::getGroundSensor(this);
    }
    else if (MR::isBindedWall(this)) {
        return MR::getWallSensor(this);
    }
    else if (MR::isBindedRoof(this)) {
        return MR::getRoofSensor(this);
    }
    else 
        return nullptr;
}

bool FireMarioBall::tryToKill() {
    HitSensor* v1 = isBindedAny();
    if (v1) {
        if (attackFire((v1))) {
            kill();
            return true;
        }
        if (_90) {
            _90 -= 1;
            if (!_90 && MR::isBindedWall(this)) {
                kill();
                return true;
            }
        }
        else {
            _90 = 3;
        } 
    }
    else {
        _90 = 0;
    }
    if (!MR::isNearPlayer(this, cForceKillDistance)) {
        kill();
        return true;
    }
    if (MR::isInWater(mPosition)) {
        kill();
        return true;
    }
    return false;
}

void FireMarioBall::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Roll", 0);
        MR::startBtk(this, "Roll");
        MR::startBrk(this, "Roll");
        MR::startSound(this, "SE_OJ_MARIO_FIRE_BALL_OUT", -1, -1);
        MR::startCSSound("CS_FIRE_BALL", 0, 0);
    }
    if (MR::isBindedGround(this)) {
        MR::startSound(this, "SE_OJ_MARIO_FIRE_BALL_BOUND", -1, -1);
        TVec3f* v1 = MR::getGroundNormal(this);
        mVelocity += -*v1 * MR::vecKillElement(mVelocity, *v1, &mVelocity) * cBoundReduction;

    }
    else if (MR::isBindedRoof(this)) {
        MR::startSound(this, "SE_OJ_MARIO_FIRE_BALL_BOUND", -1, -1);
        TVec3f* v2 = MR::getRoofNormal(this);
        mVelocity += -*v2 * MR::vecKillElement(mVelocity, *v2, &mVelocity) * cBoundReduction;
    }
    else {
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);
        mVelocity += grav * cGravityAcc;
    }
    if (!tryToKill() && getNerveStep() >= 300) {
        kill();
    }
}
