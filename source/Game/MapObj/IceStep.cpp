#include "Game/MapObj/IceStep.hpp"

IceStep::IceStep(const char *pName) : LiveActor(pName) {

}

IceStep::~IceStep() {

}

void IceStep::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("IceStep", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    initSound(4, false);
    initEffectKeeper(1, nullptr, false);
    f32 scale = 90.0f * mScale.x;
    initHitSensor(1);
    TVec3f offs;
    offs.y = scale;
    offs.x = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 8, scale, offs);
    PSMTXIdentity(_8C);
    MR::initCollisionParts(this, "IceStep", getSensor("body"), nullptr);
    MR::validateClipping(this);
    initNerve(&NrvIceStep::IceStepNrvWait::sInstance);
    kill();
    mTimer = 0;
}

void IceStep::exeWait() {
    if (mTimer != 0) {
        mTimer--;
    }

    switch (mTimer) {
        case 0x12C:
            MR::startBpk(this, "TimeOutLong");
            break;
        case 0x78:
            MR::startBpk(this, "TimeOutShort");
            break;
        case 1:
            destroy();
            break;
        case 0:
            if (MR::isBckStopped(this)) {
                kill();
            }
            break;
    }
}

/* matching except for assembly floats not being correct but whatever*/
void IceStep::setOn(u32 stepIdx, const TVec3f &rPosition, const TVec3f &rRotation) {
    mPosition = rPosition;
    mRotation = rRotation;
    MR::offUpdateCollisionParts(this);
    MR::onUpdateCollisionPartsOnetimeImmediately(this);
    s32 v8 = stepIdx >> 1;

    if (stepIdx & 1) {
        v8 -= 10;
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this); 
    TVec3f multVec(MR::multVecNoCtor(MR::multVecNoCtor(upVec, v8), 0.2f));
    mPosition = MR::addVec(multVec, rPosition);
  
    MR::startBck(this, "Start", nullptr);
    MR::startBpk(this, "Start");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_FREEZE", -1, -1); 
    mTimer = 0x4B0;
    MR::invalidateClipping(this); 
    MR::resetPosition(this); 
    setNerve(&NrvIceStep::IceStepNrvWait::sInstance);
    MR::validateCollisionParts(this);
    MR::showModel(this);
    appear(); 
}

void IceStep::destroy() {
    MR::startBck(this, "End", nullptr);
    MR::startBpk(this, "End");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_MELT", -1, -1);
    mTimer = 0;
}

void IceStep::doHit() {
    MR::invalidateCollisionParts(this);
    setNerve(&NrvIceStep::IceStepNrvHit::sInstance);
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_ICE_FLOOR_BREAK", -1, -1);
    MR::startBck(this, "End", nullptr);
    mTimer = 0;
}

bool IceStep::receiveMsgPlayerAttack(u32 , HitSensor *, HitSensor *) {
    return false;
}

bool IceStep::receiveMsgEnemyAttack(u32 msg, HitSensor *, HitSensor *) {
    if (MR::isOnPlayer(this)) {
        return false;
    }

    if (isNerve(&NrvIceStep::IceStepNrvWait::sInstance)) {
        doHit();
        return true;
    }
    
    return false;
}

void IceStep::control() {
    
}

namespace NrvIceStep {
    INIT_NERVE(IceStepNrvHit);
    INIT_NERVE(IceStepNrvWait);
}; 