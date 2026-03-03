#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/MapObj/CocoNut.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    const f32 cReboundVelocity[] = {0.0f, 15.0f, 5.0f};
}

namespace NrvCocoNutBall {
    NEW_NERVE(CocoNutBallNrvThrow, CocoNutBall, Throw);
    NEW_NERVE(CocoNutBallNrvHitBackToHost, CocoNutBall, HitBackToHost);
    NEW_NERVE(CocoNutBallNrvHitBackToPlayer, CocoNutBall, HitBackToPlayer);
    NEW_NERVE(CocoNutBallNrvRebound, CocoNutBall, Rebound);
    NEW_NERVE(CocoNutBallNrvFreeze, CocoNutBall, Freeze);
    NEW_NERVE(CocoNutBallNrvFreezeRelease, CocoNutBall, FreezeRelease);
}  // namespace NrvCocoNutBall

CocoNutBall::CocoNutBall(const char* pName)
    : LiveActor(pName), _8C(nullptr), _90(0.0f, -1.0f, 0.0f), _9C(0), _A0(0), _A4(gZeroVec), _B0(gZeroVec), _BC(false), _BD(false), _BE(false),
      _C0(450.0f), _C4(0.0f), _C8(0.0f, 1.0f, 0.0f), _D4(10000.0f), _D8(false) {
}

void CocoNutBall::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(CocoNut::getModelName(), nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensor(this, "body", ATYPE_COCO_NUT, 8, 40.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "bind", ATYPE_COCO_NUT, 8, 500.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(40.0f, 0.0f, 0);
    initEffectKeeper(0, "CocoNut", false);
    // some weirdness with this TVec here
    MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(4, false);
    MR::initShadowVolumeCylinder(this, 60.0f);
    MR::invalidateClipping(this);
    initNerve(&NrvCocoNutBall::CocoNutBallNrvThrow::sInstance);
    makeActorDead();
}

void CocoNutBall::appear() {
    LiveActor::appear();
    _9C = 0;
    MR::onBind(this);
    setNerve(&NrvCocoNutBall::CocoNutBallNrvThrow::sInstance);
}

void CocoNutBall::kill() {
    MR::forceDeleteEffect(this, "CocoNutBlur");
    MR::forceDeleteEffect(this, "CocoNutLight");
    MR::startSound(this, "SE_OJ_COCONUT_BALL_BREAK", -1, -1);
    if (_9C > 0) {
        MR::sendArbitraryMsg(ACTMES_RUSH_END, _8C->getSensor("body"), getSensor("body"));
    }
    LiveActor::kill();
    MR::emitEffect(this, CocoNut::getBreakEffectName());
}

void CocoNutBall::appearAndThrow(const TVec3f& appearPos, f32 f1) {
    mPosition.set(appearPos);
    MR::calcGravity(this);
    _C8.negate(mGravity);
    setVelocityToPlayer(15.0f, f1);
    _BD = MR::isHalfProbability();
    appear();
}

void CocoNutBall::hitBackToPlayer() {
    f32 rand = MR::getRandom(12.5f, 17.5f);

    if (_BD) {
        rand = -rand;
        _BD = false;
    } else {
        _BD = true;
    }

    setVelocityToPlayer(3.0f * _9C + 15.0f, rand);
    setNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToPlayer::sInstance);
}

void CocoNutBall::demoBreak(const TVec3f& pos) {
    mPosition.set(pos);
    mRotation.zero();
    mVelocity.zero();

    TPos3f TRMtx;
    MR::makeMtxTR(TRMtx.toMtxPtr(), this);
    MR::setBaseTRMtx(this, TRMtx);

    MR::emitEffect(this, "Hit");
    MR::startSound(this, "SE_OJ_COCONUT_BALL_BREAK", -1, -1);
    MR::emitEffect(this, CocoNut::getBreakEffectName());
}

void CocoNutBall::calcAndSetBaseMtx() {
    if (!MR::isNearZero(mVelocity)) {
        TVec3f velocityDirection;
        MR::normalize(mVelocity, &velocityDirection);
        TPos3f baseTRMtx;
        MR::makeMtxFrontUpPos(&baseTRMtx, velocityDirection, _C8, mPosition);
        MR::setBaseTRMtx(this, baseTRMtx);
    }
}

bool CocoNutBall::isSensorBody(HitSensor* pSensor) const {
    return pSensor == getSensor("body");
}

void CocoNutBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensorBody(pSender) && MR::isSensorPlayer(pReceiver)) {
        if (isNerveTrowToOrFreeze() && MR::sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK, pReceiver, pSender)) {
            setNerve(&NrvCocoNutBall::CocoNutBallNrvRebound::sInstance);
            return;
        }
    }

    if (MR::isSensorEnemy(pReceiver) && isSensorBody(pSender)) {
        if (pReceiver->mHost == _8C) {
            if (isNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToHost::sInstance)) {
                if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                    MR::emitEffect(this, "Hit");
                    kill();
                }
            }
            return;
        }

        if (isNerve(&NrvCocoNutBall::CocoNutBallNrvThrow::sInstance) || isNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToHost::sInstance) ||
            isNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToPlayer::sInstance)) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                kill();
            }
        }
    }
}

bool CocoNutBall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(MR::isMsgPlayerSpinAttack(msg)) {
        if(isValidReceivePunch()) {
            setNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToHost::sInstance);
            return true;
        }
    } else if(isSensorBody(pReceiver) && MR::isMsgStarPieceReflect(msg)){
        return true;
    }

    return false;
}

HitSensor* CocoNutBall::isBindedAny() const {
    if (MR::isBindedGround(this)) {
        return MR::getGroundSensor(this);
    }

    if (MR::isBindedWall(this)) {
        return MR::getWallSensor(this);
    }

    if (MR::isBindedRoof(this)) {
        return MR::getRoofSensor(this);
    }

    return nullptr;
}

bool CocoNutBall::isNerveTrowToOrFreeze() const {
    return isNerve(&NrvCocoNutBall::CocoNutBallNrvThrow::sInstance) || isNerve(&NrvCocoNutBall::CocoNutBallNrvHitBackToPlayer::sInstance) ||
           isNerve(&NrvCocoNutBall::CocoNutBallNrvFreeze::sInstance) || isNerve(&NrvCocoNutBall::CocoNutBallNrvFreezeRelease::sInstance);
}

bool CocoNutBall::isValidReceivePunch() const {
    if (MR::isDead(this) || (!isNerveTrowToOrFreeze() && !isNerve(&NrvCocoNutBall::CocoNutBallNrvRebound::sInstance))) {
        return false;
    } else {
        return PSVECDistance(mPosition, MR::getPlayerCenterPos()) < 400.0f;
    }
}

void CocoNutBall::calcHitBackVelocitAndGravity() {
    bool hitBackRight = isHitBackRight();
    bool hitBackFront = isHitBackFront();
    TVec3f hitBackDstPos;
    calcHitBackDstPos(&hitBackDstPos, hitBackRight, hitBackFront);

    TVec3f dir;
    dir.subInline(hitBackDstPos, mPosition);
    TVec3f scaled;
    scaled.scale(_C8.dot(dir), _C8);
    MR::vecKillElement(dir, _C8, &dir);
    f32 f1 = dir.length() / 42.0f;
    MR::normalize(&dir);
    TVec3f cross;
    PSVECCrossProduct(_C8, dir, &cross);
    MR::normalize(&cross);
    _90.scale(2.2f, mGravity);
    TVec3f scaled2;
    scaled2.scale(42.0f, dir);
    TVec3f scaled3(_90.scaleInline(f1).scaleInline(f1));
    scaled.subInline(scaled.scaleInline(2.0f));
    mVelocity.add(scaled2, scaled.scaleInline(1.0f / (2.0f * f1)));

    if (!hitBackFront) {
        f32 scaleFactor = (hitBackRight ? 1.2f : -1.2f);
        TVec3f scaled4;
        scaled4.scale(scaleFactor * f1 * f1 / (f1 * 2.0f), cross);
        mVelocity.addInline(scaled4);
        scaled4.scale(scaleFactor, cross);
        _90.subInline(scaled4);
    }

    _BC = hitBackRight;
}

bool CocoNutBall::isHitBackRight() const {
    TVec3f vec1;
    vec1.subInline(_8C->mPosition, *MR::getPlayerPos());

    TVec3f vec2;
    vec2.subInline(mPosition, *MR::getPlayerPos());

    MR::vecKillElement(vec1, _C8, &vec1);
    MR::vecKillElement(vec2, _C8, &vec2);

    MR::normalize(&vec1);
    MR::normalize(&vec2);

    TVec3f cross;
    PSVECCrossProduct(vec1, vec2, cross);

    return 0.0f < _C8.dot(cross);
}

bool CocoNutBall::isHitBackFront() const {
    TVec3f vec1;
    vec1.subInline(*MR::getPlayerPos(), _8C->mPosition);

    TVec3f vec2;
    vec2.subInline(mPosition, *MR::getPlayerPos());

    MR::vecKillElement(vec1, _C8, &vec1);
    MR::vecKillElement(vec2, _C8, &vec2);

    MR::normalize(&vec1);
    MR::normalize(&vec2);

    return vec1.dot(vec2) < 0.0f;
}

void CocoNutBall::calcHitBackDstPos(TVec3f* pOut, bool a1, bool a2) {
    TVec3f vec1(0.0f, 0.0f, 0.0f);
    if (!a2) {
        TVec3f cross;
        TVec3f vec2;

        vec2.subInline(_8C->mPosition, *MR::getPlayerPos());

        MR::vecKillElement(vec2, _C8, &vec2);
        MR::normalize(&vec2);

        PSVECCrossProduct(_C8, vec2, &cross);

        MR::vecKillElement(cross, _C8, &cross);
        MR::normalize(&cross);

        vec1.scale(a1 ? 150.0f : -150.0f, cross);
    }
    TVec3f scaled;

    scaled.scale(100.0f, _C8);
    vec1.addInline(scaled);

    pOut->add(_8C->getSensor("body")->mPosition, vec1);
}

bool CocoNutBall::tryToKill(bool alwaysKill) {
    HitSensor* bindedSensor = isBindedAny();

    if (bindedSensor != nullptr) {
        if (bindedSensor->isType(ATYPE_PUNCH_BOX) && !isNerve(&NrvCocoNutBall::CocoNutBallNrvRebound::sInstance)) {
            MR::sendMsgEnemyAttack(bindedSensor, getSensor("body"));
        }
        kill();
        return true;
    }

    if (alwaysKill) {
        kill();
        return true;
    }

    return false;
}

void CocoNutBall::setVelocityToPlayer(f32 f1, f32 f2) {
    TVec3f vec1;

    if (!_BE) {
        vec1.scale(120.0f, _C8);
        vec1.addInline(*MR::getPlayerPos());
    } else {
        vec1.set(*MR::getPlayerPos());
    }

    TRot3f rotate;
    f32 angle = PI_180 * f2;
    rotate.makeRotateInline(_C8, angle);
    TVec3f vec2;
    vec2.subInline(vec1, mPosition);
    rotate.mult33(vec2);
    vec1.add(mPosition, vec2);

    if (_BE) {
        // inline max function?
        f32 val = vec1.y;
        f32 val2 = _C0 + _8C->mPosition.y;
        if (val >= _C4 + _8C->mPosition.y) {
            val = val;
        } else {
            val = _C4 + _8C->mPosition.y;
        }
        vec1.y = val;

        f32 flt = 120.0f;
        bool v1 = false;
        while (vec1.y < val2 - flt) {
            vec2.subInline(vec1, mPosition);
            vec2.scale(1.5f);

            if (!MR::getFirstPolyOnLineToMap(nullptr, nullptr, mPosition, vec2)) {
                v1 = true;
                vec1.y += flt;
                break;
            }

            vec1.y += 60.0f;
        }

        if (!v1) {
            vec1.y = val2;
        }
    }
    vec2.subInline(vec1, mPosition);
    mVelocity.setLength(vec2, f1);
}

void CocoNutBall::freeze() {
    _A0 = 0;
    _A4.set(mPosition);
    _B0.set(mVelocity);
    setNerve(&NrvCocoNutBall::CocoNutBallNrvFreeze::sInstance);
}

bool CocoNutBall::isFreezable() {
    return !MR::isHiddenModel(this) && MR::isStarPointerPointing2POnPressButton(this, "弱", true, false);
}

void CocoNutBall::processApproachToPlayer() {
    if (MR::changeShowModelFlagSyncNearClipping(this, 300.0f)) {
        MR::emitEffect(this, "CocoNutLight");
    } else {
        MR::deleteEffect(this, "CocoNutLight");
    }

    if (isNerve(&NrvCocoNutBall::CocoNutBallNrvFreezeRelease::sInstance) || MR::isGreaterStep(this, 20)) {
        if (isFreezable()) {
            freeze();
            return;
        }
    }

    if (tryToKill(!MR::isNear(this, _8C, _D4))) {
        return;
    }
}

void CocoNutBall::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinX", nullptr);
        MR::emitEffect(this, "CocoNutLight");
    }

    if (_D8) {
        MR::startSound(this, "SE_BM_OTAKING_SPIT_RALLY_BALL", -1, -1);
    } else {
        MR::startSound(this, "SE_OJ_COCONUT_BALL_SPIT_OUT", -1, -1);
    }

    processApproachToPlayer();
}

void CocoNutBall::exeHitBackToHost() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0);
        MR::deleteEffect(this, "Touch");
        MR::emitEffect(this, "CocoNutBlur");
        MR::emitEffect(this, "CocoNutLight");
        MR::emitEffect(this, "SpinHitMark");
        MR::offBind(this);
        MR::startSound(this, "SE_OJ_COCONUT_BALL_VOLLEY", -1, -1);
        MR::startSpinHitSound(this);

        if (_D8 && MR::isPlayingStageBgm()) {
            if (_9C == 0) {
                MR::startSystemME("ME_RALLY_COMBO_FIRST");
            } else {
                MR::startSystemME("ME_RALLY_COMBO_SECOND");
            }
        }

        _9C++;
        calcHitBackVelocitAndGravity();
    }

    if (MR::isStep(this, 1)) {
        MR::stopScene(6);
    }

    if (MR::isStep(this, 2)) {
        MR::shakeCameraWeak();
    }

    if (MR::isGreaterEqualStep(this, 1)) {
        mVelocity.addInline(_90);
    }

    if (tryToKill(!MR::isNearPlayer(this, 5000.0f))) {
        return;
    }
}

void CocoNutBall::exeHitBackToPlayer() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Hit");
        MR::startSound(this, "SE_OJ_COCONUT_BALL_VOLLEY", -1, -1);

        if (_D8 && MR::isPlayingStageBgm()) {
            MR::startSystemME("ME_RALLY_COMBO_SECOND");
        }

        MR::onBind(this);
    }

    processApproachToPlayer();
}

void CocoNutBall::exeRebound() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.25f);
        MR::deleteEffect(this, "CocoNutBlur");
        MR::deleteEffect(this, "CocoNutLight");

        TVec3f vec1;
        vec1.subInline(mPosition, *MR::getPlayerPos());

        TPos3f pos;
        pos.identity();

        MR::makeMtxUpFront(&pos, _C8, vec1);

        mVelocity.x = ::cReboundVelocity[0];
        mVelocity.y = ::cReboundVelocity[1];
        mVelocity.z = ::cReboundVelocity[2];

        pos.mult33(mVelocity);
    }

    TVec3f twoGravity;
    twoGravity.scale(2.0f, mGravity);
    mVelocity.addInline(twoGravity);

    if (tryToKill(!MR::isNearPlayer(this, 5000.0f))) {
        return;
    }
}

void CocoNutBall::exeFreeze() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();

        MR::deleteEffect(this, "CocoNutBlur");
        MR::deleteEffect(this, "CocoNutLight");
        MR::emitEffect(this, "Touch");

        if (_A0 == 0) {
            MR::startDPDHitSound();
        }
    }
    _A0++;
    MR::startDPDFreezeLevelSound(this);

    f32 cos = JMath::sSinCosTable.cosLap(MR::repeatDegree(_A0 * 75.0f));

    f32 scaleFactor = ((7.5f * cos) * (20 - getNerveStep())) / 20.0f;

    TVec3f camXDirScaled;
    camXDirScaled.set(MR::getCamXdir());
    camXDirScaled.scale(scaleFactor);
    mPosition.add(_A4, camXDirScaled);

    if (MR::changeShowModelFlagSyncNearClipping(this, 300.0)) {
        MR::emitEffect(this, "Touch");
    } else {
        MR::deleteEffect(this, "Touch");
    }

    if (isFreezable()) {
        // resets nerve step every frame i guess
        setNerve(&NrvCocoNutBall::CocoNutBallNrvFreeze::sInstance);
    } else if (MR::isStep(this, 20)) {
        setNerve(&NrvCocoNutBall::CocoNutBallNrvFreezeRelease::sInstance);
    }
}

void CocoNutBall::exeFreezeRelease() {
    if (MR::isFirstStep(this)) {
        mPosition.set(_A4);
        mVelocity.set(_B0);
        MR::deleteEffect(this, "Touch");
        MR::emitEffect(this, "CocoNutLight");
    }

    processApproachToPlayer();
}
