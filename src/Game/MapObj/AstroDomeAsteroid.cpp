#include "Game/MapObj/AstroDomeAsteroid.hpp"
#include "Game/Map/SphereSelector.hpp"

AstroDomeAsteroid::AstroDomeAsteroid(const char *pName) : LiveActor(pName) {
    mRotationMtx.identity();
}

void AstroDomeAsteroid::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("AstroDomeAsteroid", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::invalidateClipping(this);
    initNerve(&NrvAstroDomeAsteroid::AstroDomeAsteroidNrvWait::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoSimpleCastAll(this);
    SphereSelectorFunction::registerTarget(this);
    makeActorDead();
}

void AstroDomeAsteroid::appear() {
    LiveActor::appear();
    mRotationMtx.identity();
}

void AstroDomeAsteroid::control() {
    SphereSelectorFunction::calcHandledTrans(TVec3f(0.0f, 0.0f, 0.0f), &mPosition);
    SphereSelectorFunction::calcHandledRotateMtx(TVec3f(0.0f, 0.0f, 0.0f), &mRotationMtx);
}

void AstroDomeAsteroid::calcAndSetBaseMtx() {
    TPos3f other;
    other = mRotationMtx;
    other.mMtx[0][3] = mPosition.x;
    other.mMtx[1][3] = mPosition.y;
    other.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, other);
}

bool AstroDomeAsteroid::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    return SphereSelectorFunction::trySyncAppearMsgSelectStart(this, msg);
}

AstroDomeAsteroid::~AstroDomeAsteroid() {

}

namespace NrvAstroDomeAsteroid {
    INIT_NERVE(AstroDomeAsteroidNrvWait);
};