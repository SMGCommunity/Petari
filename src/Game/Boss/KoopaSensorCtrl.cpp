#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaSensorCtrl.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

KoopaSensorCtrl::KoopaSensorCtrl(Koopa* pHost) :
    mHost(pHost)
{}

void KoopaSensorCtrl::update(HitSensor* pSensor) {
    
}

void KoopaSensorCtrl::initSensor() {
    MR::addHitSensorAtJoint(mHost, "Body", "Spine", ATYPE_KOOPA_PUSH, 8, 150.0f, TVec3f(-70.0f, -30.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "Face", "Face", ATYPE_KOOPA_PUSH, 8, 110.0f, TVec3f(80.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "WristL", "WristL", ATYPE_KOOPA_PUSH, 8, 60.0f, TVec3f(30.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "WristR", "WristR", ATYPE_KOOPA_PUSH, 8, 60.0f, TVec3f(30.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "ArmL", "ArmL2", ATYPE_KOOPA_PUSH, 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "ArmR", "ArmR2", ATYPE_KOOPA_PUSH, 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "LegL", "LegL2", ATYPE_KOOPA_PUSH, 8, 80.0f, TVec3f(30.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "LegR", "LegR2", ATYPE_KOOPA_PUSH, 8, 80.0f, TVec3f(30.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "FootL", "FootL2", ATYPE_KOOPA_PUSH, 8, 70.0f, TVec3f(-50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "FootR", "FootR2", ATYPE_KOOPA_PUSH, 8, 70.0f, TVec3f(-50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "Tail", "Tail2", ATYPE_KOOPA_PUSH, 8, 60.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "TailTop", "Tail3", ATYPE_KOOPA_PUSH, 8, 50.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(mHost, "GuardBody", "Spine", 8, 130.0f, TVec3f(0.0f, -150.0f, 0.0f));
    MR::invalidateHitSensor(mHost, "GuardBody");
    MR::addHitSensorAtJointEnemyAttack(mHost, "AttackShell", "Spine", 8, 180.0f, TVec3f(-50.0f, 100.0f, 0.0f));
    MR::addHitSensorAtJointEnemyAttack(mHost, "AttackFootL", "FootL2", 8, 50.0f, TVec3f(-50.0f, 40.0f, 0.0f));
    MR::addHitSensorAtJointEnemyAttack(mHost, "AttackFootR", "FootR2", 8, 50.0f, TVec3f(-50.0f, 40.0f, 0.0f));
    MR::addHitSensor(mHost, "AttackHipDrop", ATYPE_KOOPA_ATTACK, 8, 200.0f, TVec3f(0.0f, 250.0f, 0.0f));
    MR::invalidateHitSensor(mHost, "AttackHipDrop");
    MR::addHitSensorAtJointEnemyAttack(mHost, "ChaseRollAttack", "Spine", 8, 260.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "ChaseRollFace", "Face", ATYPE_KOOPA_RECEIVER, 16, 200.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "ChaseRollStarPiece", "Spine", ATYPE_KOOPA_PUSH, 8, 280.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(mHost, "ChaseRollAttack");
    MR::invalidateHitSensor(mHost, "ChaseRollFace");
    MR::invalidateHitSensor(mHost, "ChaseRollStarPiece");
    MR::addHitSensorAtJointEnemyAttack(mHost, "AttackRoll", "Spine", 8, 180.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "AttackRollReceiver", "Spine", ATYPE_KOOPA_RECEIVER, 8, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "AttackRollStarPiece", "Spine", ATYPE_KOOPA_PUSH, 8, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(mHost, "AttackRoll");
    MR::invalidateHitSensor(mHost, "AttackRollReceiver");
    MR::invalidateHitSensor(mHost, "AttackRollStarPiece");
    MR::addHitSensorAtJoint(mHost, "ReceiverTail", "Tail2", ATYPE_KOOPA_RECEIVER, 8, 180.0f, TVec3f(50.0f, 30.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "ReceiverTailTop", "Tail3", ATYPE_KOOPA_RECEIVER, 8, 150.0f, TVec3f(100.0f, 30.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "DamageEscapeBody", "Spine", ATYPE_KOOPA_RECEIVER, 8, 250.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::addHitSensorAtJoint(mHost, "DamageEscapePush", "Spine", 0x41, 8, 180.0f, TVec3f(-70.0f, -30.0f, 0.0f));
}
