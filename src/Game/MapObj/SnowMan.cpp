#include "Game/MapObj/SnowMan.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "revolution/types.h"

namespace NrvSnowMan {
    NEW_NERVE(SnowManNrvWait, SnowMan, Wait);
    NEW_NERVE(SnowManNrvWaitBody, SnowMan, WaitBody);
    NEW_NERVE(SnowManNrvWaitHead, SnowMan, WaitHead);
    NEW_NERVE(SnowManNrvMeltHead, SnowMan, MeltHead);
    NEW_NERVE(SnowManNrvMeltBody, SnowMan, MeltBody);
    NEW_NERVE(SnowManNrvDownHead, SnowMan, DownHead);
    NEW_NERVE(SnowManNrvDownBody, SnowMan, DownBody);
}

SnowMan::SnowMan(const char* pName) : LiveActor(pName) {
    mHeadCollisionParts = nullptr;
    mBodyCollisionParts = nullptr;
    mAnimScaleCtrl = nullptr;
    mAnimScaleParam = nullptr;
 }
SnowMan::~SnowMan() { }

void SnowMan::init(const JMapInfoIter &rIfter) {
    MR::initDefaultPos(this,rIfter);
    initModelManagerWithAnm("SnowMan", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(2);
    MR::addMessageSensorMapObj(this, "head");
    MR::addMessageSensorMapObj(this, "body");
    mHeadCollisionParts =  MR::createCollisionPartsFromLiveActor(this, "HeadCol", getSensor("head"), MR::getJointMtx(this, "Head"), (MR::CollisionScaleType)1);
    mBodyCollisionParts = MR::createCollisionPartsFromLiveActor(this, "BodyCol", getSensor("body"), MR::getJointMtx(this, "Body"), (MR::CollisionScaleType)1);
    MR::validateCollisionParts(mHeadCollisionParts);
    MR::validateCollisionParts(mBodyCollisionParts);
    mAnimScaleParam = new AnimScaleParam();
    mAnimScaleParam->_8 = 0.94999999f;
    mAnimScaleParam->_C = 1.05f;
    mAnimScaleParam->_24 = 0.60000002f;
    mAnimScaleParam->_28 = 0.80000001f;
    mAnimScaleCtrl = new AnimScaleController(mAnimScaleParam);
    initEffectKeeper(0, nullptr, false);
    initSound(4,false);
    MR::useStageSwitchWriteA(this, rIfter);
    MR::useStageSwitchWriteB(this, rIfter);
    MR::useStageSwitchWriteDead(this, rIfter);
    initNerve(&NrvSnowMan::SnowManNrvWait::sInstance);
    makeActorAppeared();
}

void SnowMan::control() {
    if (mHeadCollisionParts->_CC) {
        mHeadCollisionParts->setMtx();
    } 
    if (mBodyCollisionParts->_CC) {
        mBodyCollisionParts->setMtx();
    }
    mAnimScaleCtrl->updateNerve();
}

void SnowMan::kill() {
    if (MR::isValidSwitchDead(this)){
        MR::onSwitchDead(this);
    }
    LiveActor::kill();
}

bool SnowMan::receiveMsgPlayerAttack(u32 a1, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgFireBallAttack(a1)) {
        if (a3 == getSensor("head")) {
            damageHead();
        } else {
            damageBody();
        }
        return true;
    }
    else if (MR::isMsgPlayerSpinAttack(a1)) {
        if (mAnimScaleCtrl->isHitReaction(-1)) {
            return false;
        }
        else {
            mAnimScaleCtrl->startHitReaction();
            return true;
        }
    }

    return false;
}

void SnowMan::damageHead() {
    if (isNerve(&NrvSnowMan::SnowManNrvWait::sInstance)) {
        setNerve(&NrvSnowMan::SnowManNrvMeltHead::sInstance);
    } else if (isNerve(&NrvSnowMan::SnowManNrvWaitHead::sInstance)) {
        setNerve(&NrvSnowMan::SnowManNrvDownHead::sInstance);
    }
}

void SnowMan::damageBody() {
    if (isNerve(&NrvSnowMan::SnowManNrvWait::sInstance)) {
        setNerve(&NrvSnowMan::SnowManNrvMeltBody::sInstance);
    } else if (isNerve(&NrvSnowMan::SnowManNrvWaitBody::sInstance)) {
        setNerve(&NrvSnowMan::SnowManNrvDownBody::sInstance);
    }
}

void SnowMan::calcAndSetBaseMtx() {
    TVec3f mtx;
    LiveActor::calcAndSetBaseMtx();
    mtx.multPS(mScale, mAnimScaleCtrl->_C);
    MR::setBaseScale(this, mtx);
}

void SnowMan::exeWait() { }
void SnowMan::exeWaitBody() { }
void SnowMan::exeWaitHead() { }

void SnowMan::exeMeltHead() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(mHeadCollisionParts);
        MR::startBck(this, "MeltHead", nullptr);
        MR::startSound(this, "SE_OJ_SNOW_MAN_MELT", -1, -1);
        MR::shakeCameraWeak();
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    }
    if (MR::isStep(this, 20)) {
        MR::hideMaterial(this, "SnowManBucketMat_v");
        setNerve(&NrvSnowMan::SnowManNrvWaitBody::sInstance);
    }
}

void SnowMan::exeMeltBody() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(mBodyCollisionParts);
        MR::startBck(this, "MeltBody", nullptr);
        MR::startSound(this, "SE_OJ_SNOW_MAN_MELT", -1, -1);
        MR::shakeCameraWeak();
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
    }
    if (MR::isStep(this, 20)) {
        setNerve(&NrvSnowMan::SnowManNrvWaitHead::sInstance);
    }
}

void SnowMan::exeDownHead() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(mHeadCollisionParts);
        MR::invalidateCollisionParts(mBodyCollisionParts);
        MR::startBck(this, "DownHead", nullptr);
        MR::startSound(this, "SE_OJ_SNOW_MAN_MELT", -1, -1);
        MR::shakeCameraWeak();
    }
    if (MR::isBckStopped(this)) {
        kill();
    }
}

void SnowMan::exeDownBody() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(mHeadCollisionParts);
        MR::invalidateCollisionParts(mBodyCollisionParts);
        MR::startBck(this, "DownBody", nullptr);
        MR::startSound(this, "SE_OJ_SNOW_MAN_MELT", -1, -1);
        MR::shakeCameraWeak();
        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
    }
    if (MR::isBckStopped(this)) {
        kill();
    }
}
