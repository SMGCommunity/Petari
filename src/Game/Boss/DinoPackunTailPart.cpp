#include "Game/Boss/DinoPackunTailPart.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvDinoPackunTailPart {
    NEW_NERVE(DinoPackunTailPartNrvWait, DinoPackunTailPart, Wait);
    NEW_NERVE(DinoPackunTailPartNrvLockPosition, DinoPackunTailPart, LockPosition);
};  // namespace NrvDinoPackunTailPart

DinoPackunTailPart::DinoPackunTailPart(const char* pName, DinoPackun* pParent) : DinoPackunTailNode(pName, pParent) {
    _D4 = 50.0f;
    _D8 = 0;
    _D9 = 1;
}

void DinoPackunTailPart::init(const JMapInfoIter& rIter) {
    initNerve(&NrvDinoPackunTailPart::DinoPackunTailPartNrvWait::sInstance);
    MR::onCalcGravity(this);
    initBinder(_D4, 0.0f, 0);
    MR::connectToSceneEnemyDecorationMovement(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 8, (1.25f + _D4), TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void DinoPackunTailPart::control() {
}

JointController* DinoPackunTailPart::createJointControllerOwn(LiveActor* pActor, const char* pJointName) {
    // we only do this because we already know it's a tail node type since it inherits
    return MR::createJointDelegator((DinoPackunTailNode*)this, pActor, &DinoPackunTailNode::calcJointScale, &DinoPackunTailNode::turnJointLocalXDir,
                                    pJointName);
}

void DinoPackunTailPart::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    mParent->attackSensorTail(pSender, pReceiver);
    bool push = MR::sendMsgPush(pReceiver, pSender);
    if (MR::isSensorPlayer(pReceiver) && push) {
        TVec3f v16;
        MR::calcSensorDirectionNormalize(&v16, pReceiver, pSender);

        if (mVelocity.dot(v16) < 3.0f) {
            mVelocity += v16.multInLine(1.0f);
        }
    }
}

bool DinoPackunTailPart::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::sendMsgAwayJump(pSender, pReceiver);
        return true;
    } else if (MR::isMsgPlayerTrample(msg)) {
        return true;
    }

    return mParent->receiveMsgPlayerAttackTail(msg, pSender, pReceiver);
}

void DinoPackunTailPart::requestLockPosition() {
    MR::zeroVelocity(this);
    setNerve(&NrvDinoPackunTailPart::DinoPackunTailPartNrvLockPosition::sInstance);
    MR::offBind(this);
}

void DinoPackunTailPart::requestUnLockPosition() {
    setNerve(&NrvDinoPackunTailPart::DinoPackunTailPartNrvWait::sInstance);
    MR::onBind(this);
}

void DinoPackunTailPart::exeLockPosition() {
}

void DinoPackunTailPart::exeWait() {
    MR::addVelocityToGravity(this, 0.69f);
    MR::attenuateVelocity(this, 0.88f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

DinoPackunTailPart::~DinoPackunTailPart() {
}
