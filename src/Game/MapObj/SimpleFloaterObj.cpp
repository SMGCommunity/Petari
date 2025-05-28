#include "Game/MapObj/SimpleFloaterObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapPartsFloatingForce.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "revolution/types.h"

namespace NrvSimpleFloaterObj {
    NEW_NERVE(SimpleFloaterObjNrvWait, SimpleFloaterObj, Wait);
}; //Namespace NrvSimpleFloaterObj

namespace {
    const char* cEffectName = "Ripple";
}

SimpleFloaterObj::SimpleFloaterObj(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90 = nullptr; 
    _94.set(0.0f, 0.0f, 0.0f);
 }
SimpleFloaterObj::~SimpleFloaterObj() { }

void SimpleFloaterObj::init(const JMapInfoIter & rIfter) {
    MR::initDefaultPos(this, rIfter);
    MR::getObjectName(&_8C, rIfter);
    initModelManagerWithAnm(_8C, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    HitSensor* mSensor =  getSensor(0);
    MR::initCollisionParts(this, _8C, mSensor, nullptr);
    initEffectKeeper(0, nullptr, false);
    _94.set(mPosition);
    MR::setEffectHostSRT(this, cEffectName, &_94, &mRotation, nullptr);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    _90 = new MapPartsFloatingForce(this);
    _90->init(rIfter);
    _90->setObjectName(_8C);
    _90->start();
    initNerve(&NrvSimpleFloaterObj::SimpleFloaterObjNrvWait::sInstance);
    makeActorAppeared();
}

void SimpleFloaterObj::exeWait() {
    MR::isFirstStep(this);
    if(!isVelocityClose() && !MR::isEffectValid(this, cEffectName)) {
        MR::emitEffect(this, cEffectName);
    } else if(isVelocityClose() && MR::isEffectValid(this, cEffectName)) {
        MR::deleteEffect(this, cEffectName);            
    }
    _90->movement();
}
