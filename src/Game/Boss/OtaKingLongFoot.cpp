#include "Game/Boss/OtaKingLongFoot.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"

namespace NrvOtaKingLongFoot {
    NEW_NERVE(OtaKingLongFootNrvWait, OtaKingLongFoot, Wait)
    NEW_NERVE(OtaKingLongFootNrvDamage, OtaKingLongFoot, Damage)
    NEW_NERVE(OtaKingLongFootNrvAppearDemo, OtaKingLongFoot, AppearDemo)
    NEW_NERVE(OtaKingLongFootNrvDownDemo, OtaKingLongFoot, DownDemo)
}

namespace {

}

OtaKingLongFoot::OtaKingLongFoot(LiveActor* pHost, s32 a1, const char* pChar) : PartsModel(pHost, pChar, "OtaKingLongFoot", nullptr, MR::DrawBufferType_Enemy, false), _9C(a1), _A0(nullptr){

}

void OtaKingLongFoot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::initLightCtrl(this);
    initSensor();
   _A0 = new AnimScaleController(nullptr);
   initNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
   PartsModel::init(rIter);
}

void OtaKingLongFoot::hide() {
    MR::startBckNoInterpole(this, "Appear");
    MR::setBckFrameAndStop(this, 0.0f);
    MR::hideModel(this);
}

void OtaKingLongFoot::startAppearDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvAppearDemo::sInstance);
}

void OtaKingLongFoot::startDownDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDownDemo::sInstance);
}

void OtaKingLongFoot::endDemo() {
    setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
}

void OtaKingLongFoot::control() {
    _A0->updateNerve();
}

void OtaKingLongFoot::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    MR::setBaseScale(this, _A0->_C.mult(mScale));
}

void OtaKingLongFoot::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if(MR::isSensorPlayer(pReceiver)) {
        if(isNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
            setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDamage::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool OtaKingLongFoot::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(MR::isMsgStarPieceReflect(msg)) {
        _A0->startHitReaction();
        return true;
    }
    return false;
}

bool OtaKingLongFoot::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(isNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance)) {
        setNerve(&NrvOtaKingLongFoot::OtaKingLongFootNrvDamage::sInstance);
    }
    return true;
}


void OtaKingLongFoot::exeWait() {
    if(MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::setBckFrameAtRandom(this);
    }
}

void OtaKingLongFoot::exeDamage() {
    if(MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Damage");
    }
    
    MR::setNerveAtBckStopped(this, &NrvOtaKingLongFoot::OtaKingLongFootNrvWait::sInstance);
}

void OtaKingLongFoot::exeAppearDemo() {
    if(MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Appear");
        MR::setBckFrameAndStop(this, 0.0f);
        MR::showModel(this);
    }

    //delay animating for _9C (+1?) frames
    if(MR::isStep(this, _9C)) {
        MR::setBckRate(this, 1.0f);
    }
}

void OtaKingLongFoot::exeDownDemo() {
    
}
