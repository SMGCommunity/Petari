#include "Game/Boss/DinoPackunBall.hpp"
#include "Game/Util/JointController.hpp"

namespace NrvDinoPackunBall {
    NEW_NERVE(DinoPackunBallNrvWait, DinoPackunBall, Wait);
    NEW_NERVE(DinoPackunBallNrvShoot, DinoPackunBall, Shoot);
    NEW_NERVE(DinoPackunBallNrvCharge, DinoPackunBall, Charge);
    NEW_NERVE(DinoPackunBallNrvReverse, DinoPackunBall, Reverse);
    NEW_NERVE(DinoPackunBallNrvRebound, DinoPackunBall, Rebound);
    NEW_NERVE(DinoPackunBallNrvLock, DinoPackunBall, Lock);
};  // namespace NrvDinoPackunBall

DinoPackunBall::DinoPackunBall(const char* pName, DinoPackun* pParent) : DinoPackunTailNode(pName, pParent) {
    _104.x = 0.0f;
    _104.y = 0.0f;
    _104.z = 0.0f;
    _104.w = 1.0f;
    mWeakSensor = nullptr;
    _118.x = 0.0f;
    _118.y = 0.0f;
    _118.z = 0.0f;
    _124 = 0;
    _125 = 0;
    _126 = 0;
    _128 = -1;
    _D4.identity();
}

DinoPackunTailNode::~DinoPackunTailNode() {
}

void DinoPackunBall::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyDecorationMovement(this);
    initBinder(135.0f, 0.0f, 0);
    initSound(6, false);
    initEffectKeeper(0, "DinoPackunBall", false);
    MR::setEffectHostMtx(this, "TailDrag", _D4);
    MR::setEffectHostMtx(this, "TailDragBlack", _D4);
    initNerve(&NrvDinoPackunBall::DinoPackunBallNrvWait::sInstance);
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorEnemy(this, "body", 8, 135.0f, offs);
    makeActorAppeared();
}

void DinoPackunBall::control() {
    MR::makeMtxUpNoSupport(&_D4, -mGravity);
    TVec3f v3 = mGravity * 135.0f;
    TVec3f v4(mPosition);
    v4.add(v3);
    _D4.setPos(v4);

    if (_128 > 0) {
        MR::startLevelSound(this, "SE_BM_LV_D_PAKKUN_TAIL_DRAG", -1, -1, -1);
        _128--;
    } else {
        _128 = -1;
    }

    if (_125) {
        MR::startLevelSound(this, "SE_BM_LV_D_PAKKUN_TAIL_FIRE", -1, -1, -1);
    }
}

void DinoPackunBall::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _104);
    MR::setBaseScale(this, mScale);
}

void DinoPackunBall::addNodeVelocity(const TVec3f& rOther) {
    f32 v4;

    if (isNerve(&NrvDinoPackunBall::DinoPackunBallNrvWait::sInstance)) {
        if (_126) {
            v4 = 0.0f;
        } else {
            v4 = 1.0f;
        }
    } else {
        if (isNerve(&NrvDinoPackunBall::DinoPackunBallNrvRebound::sInstance)) {
            v4 = MR::calcNerveRate(this, 180);
        } else {
            return;
        }
    }

    mVelocity.add(rOther * v4);
}

void DinoPackunBall::setWeakSensor(HitSensor* pSensor) {
    mWeakSensor = pSensor;
}

typedef bool (DinoPackunTailNode::*calcFunc)(TPos3f*, const JointControllerInfo&);

JointController* DinoPackunBall::createJointControllerOwn(LiveActor* pActor, const char* pJointName) {
    return MR::createJointDelegator((DinoPackunTailNode*)this, pActor, &DinoPackunTailNode::turnJointLocalXDir, (calcFunc)0, pJointName);
}

void DinoPackunBall::requestLockPosition() {
    MR::zeroVelocity(this);
    setNerve(&NrvDinoPackunBall::DinoPackunBallNrvLock::sInstance);
}

void DinoPackunBall::requestUnLockPosition() {
    mVelocity.set< f32 >(_118);
    setNerve(&NrvDinoPackunBall::DinoPackunBallNrvRebound::sInstance);
}

void DinoPackunBall::addDodgeTargetVelocity() {
    const TVec3f* weakPos = &MR::getSensorPos(mWeakSensor);
    TVec3f v12(mPosition);
    TVec3f v13;
    v12.sub(*weakPos);
    v13.set< f32 >(v12);
    TVec3f* grav = &MR::getSensorHost(mWeakSensor)->mGravity;
    f32 v4 = grav->dot(v13);
    JMAVECScaleAdd(grav, &v13, &v13, -v4);
    f32 v7;
    MR::separateScalarAndDirection(&v7, &v13, v13);
    f32 v5 = mVelocity.dot(v13);

    if (MR::isNearZero(v7)) {
        MR::addRandomVector(&mVelocity, mVelocity, 2.0f);
    } else {
        if (v5 < 0.0f) {
            f32 v6 = (1.0f - MR::normalize(v7, 500.0f, 1500.0f));
            mVelocity.add((-v13 * v6) * v5);
        }

        if (v7 < 500.0f) {
            mVelocity.add(v13 * 1.0f);
        }
    }
}

void DinoPackunBall::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (_125) {
        MR::sendMsgEnemyAttackFire(a2, a1);
    } else if (isNerve(&NrvDinoPackunBall::DinoPackunBallNrvWait::sInstance) || isNerve(&NrvDinoPackunBall::DinoPackunBallNrvRebound::sInstance)) {
        if (MR::sendMsgPush(a2, a1)) {
            TVec3f v13;
            MR::calcSensorDirectionNormalize(&v13, a2, a1);
            f32 v6 = mVelocity.dot(v13);

            if (MR::isSensorPlayer(a2)) {
                if (v6 < 5.0f) {
                    mVelocity.add(v13 * 1.0f);
                }
            } else if (v6 < 0.0f) {
                mVelocity.sub(v13 * v6);
            }
        }

    } else if (a2 == mWeakSensor && isNerve(&NrvDinoPackunBall::DinoPackunBallNrvReverse::sInstance) && MR::sendArbitraryMsg(186, a2, a1)) {
        TVec3f v12;
        MR::calcSensorDirection(&v12, a2, a1);
        MR::normalizeOrZero(&v12);
        f32 v7 = mVelocity.length();
        mVelocity.set< f32 >((v12 * v7) * 0.30000001f);
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvRebound::sInstance);
    }
}

bool DinoPackunBall::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }
    if (_125) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestPunch(a3, a2);
    }

    return false;
}

bool DinoPackunBall::receiveMsgEnemyAttack(u32 msg, HitSensor*, HitSensor*) {
    MR::isMsgEnemyAttackFire(msg);
    return false;
}

bool DinoPackunBall::requestPunch(HitSensor* a1, HitSensor* a2) {
    if (isNerve(&NrvDinoPackunBall::DinoPackunBallNrvWait::sInstance) && MR::sendArbitraryMsg(184, mWeakSensor, a1)) {
        MR::addVelocitySeparateHV(this, a2, a1, 120.0f, 40.0f);
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvShoot::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBall::setDamageFire() {
    if (!_125) {
        _125 = 1;
    }
}

void DinoPackunBall::setDamageNone() {
    if (_125) {
        _125 = 0;
    }
}

bool DinoPackunBall::tryRebound() {
    if (MR::sendMsgToBindedSensor(186, getSensor(nullptr))) {
        TVec3f v6(*MR::getBindedFixReactionVector(this));
        MR::normalizeOrZero(&v6);
        _118.set< f32 >(v6 * 40.0f);
        MR::zeroVelocity(this);
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvRebound::sInstance);
        return true;
    }

    return false;
}

void DinoPackunBall::exeWait() {
    MR::addVelocityToGravity(this, 1.0f);
    MR::attenuateVelocity(this, 0.098f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    if (MR::isBindedGround(this) && mVelocity.length() > 5.0f) {
        if (!_125) {
            MR::emitEffect(this, _124 ? "TailDragBlack" : "TailDrag");
        }

        _128 = 10;
    } else {
        MR::deleteEffect(this, _124 ? "TailDragBlack" : "TailDrag");
    }
}

void DinoPackunBall::exeShoot() {
    if (MR::isFirstStep(this)) {
        MR::stopScene(3);
        MR::tryRumblePadStrong(this, 0);
        MR::startBlowHitSound(this);
        MR::deleteEffect(this, _124 ? "TailDragBlack" : "TailDrag");
    }

    addDodgeTargetVelocity();
    MR::attenuateVelocity(this, 1.0f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    f32 v4 = PSVECDistance(MR::getSensorPos(mWeakSensor), &mPosition);
    if (MR::isGreaterStep(this, 30) || v4 >= 1500.0f) {
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvCharge::sInstance);
    }
}

void DinoPackunBall::exeCharge() {
    if (MR::isFirstStep(this)) {
        MR::sendArbitraryMsg(185, mWeakSensor, getSensor(nullptr));
        MR::startSound(this, "SE_BM_D_PAKKUN_TAIL_CRG_ST", -1, -1);
    }

    addDodgeTargetVelocity();
    MR::attenuateVelocity(this, 0.88f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::startLevelSound(this, "SE_BM_LV_D_PAKKUN_TAIL_CRG", -1, -1, -1);

    if (MR::isGreaterStep(this, 35)) {
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvReverse::sInstance);
    }
}

void DinoPackunBall::exeReverse() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_D_PAKKUN_TAIL_REV", -1, -1);
    }

    TVec3f v5(MR::getSensorPos(mWeakSensor));
    v5.sub(mPosition);
    TVec3f v6;
    v6.set< f32 >(v5);
    MR::normalizeOrZero(&v6);
    mVelocity.add(v6 * 8.0f);
    MR::attenuateVelocity(this, 0.98000002f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryRebound()) {
        return;
    }
}

void DinoPackunBall::exeRebound() {
    MR::addVelocityToGravity(this, 1.0f);
    MR::attenuateVelocity(this, 0.89999998f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    if (MR::isGreaterStep(this, 180)) {
        setNerve(&NrvDinoPackunBall::DinoPackunBallNrvWait::sInstance);
    }
}

void DinoPackunBall::exeLock() {
    return;
}

DinoPackunBall::~DinoPackunBall() {
}
