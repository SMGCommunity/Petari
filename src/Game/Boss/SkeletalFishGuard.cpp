#include "Game/Boss/SkeletalFishGuard.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    NEW_NERVE(SkeletalFishGuardNrvWait, SkeletalFishGuard, Wait);
    NEW_NERVE(SkeletalFishGuardNrvAppear, SkeletalFishGuard, Appear);
    NEW_NERVE(SkeletalFishGuardNrvNormal, SkeletalFishGuard, Normal);
    NEW_NERVE(SkeletalFishGuardNrvApart, SkeletalFishGuard, Apart);
    NEW_NERVE(SkeletalFishGuardNrvFollow, SkeletalFishGuard, Follow);
    NEW_NERVE(SkeletalFishGuardNrvStraight, SkeletalFishGuard, Straight);
    NEW_NERVE(SkeletalFishGuardNrvDefence, SkeletalFishGuard, Defence);
    NEW_NERVE(SkeletalFishGuardNrvKill, SkeletalFishGuard, Kill);
    NEW_NERVE_ONEND(SkeletalFishGuardNrvNumb, SkeletalFishGuard, Numb, Numb);
};  // namespace

SkeletalFishGuard::SkeletalFishGuard(SkeletalFishBoss* pFishBoss, const char* pName)
    : LiveActor(pName), _8C(), mFishBoss(pFishBoss), _94(0.0f, 0.0f, 0.0f), _A0(), _A4(0.0f, 0.0f, 0.0f), _B0(), _B4(), _B8(), _BC(), mAttackDelay(),
      _C4(), _C8(), _CC(), _D0(0.0f, 0.0f, 0.0f), _DC(0.0f, 0.0f, 0.0f), _E8(0.0f, 0.0f, 0.0f), _F4(0.0f, 0.0f, 0.0f), _100(0.0f, 0.0f, 1.0f),
      _10C(0.0f, 0.0f, 0.0f) {
    mScaleController = new AnimScaleController(nullptr);
}

void SkeletalFishGuard::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("SkeletalFishGuard", nullptr, false);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::connectToSceneEnemy(this);
    initNerve(GET_NERVE_ANON(SkeletalFishGuardNrvWait));
    initSensor();
    MR::initLightCtrl(this);
    MR::initShadowVolumeFlatModel(this, "SkeletalFishGuardShadow");
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "PointerTouchManual");
    MR::setEffectHostMtx(this, "PointerTouchManual", MR::getJointMtx(this, "Head"));
    initSound(6, false);
    initBinder(120.0f, 0.0f, 0);
    MR::invalidateClipping(this);
    MR::setBinderExceptActor(this, mFishBoss->getHeadActor());
    MR::initStarPointerTargetAtJoint(this, "Joint00", 170.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::declareStarPiece(this, 128);
}

void SkeletalFishGuard::initAfterPlacement() {
    calcTransAndFront();
    makeActorDead();
}

void SkeletalFishGuard::appear() {
    LiveActor::appear();
    MR::validateShadow(this, nullptr);
}

void SkeletalFishGuard::control() {
    mScaleController->updateNerve();

    if (mAttackDelay > 0) {
        mAttackDelay--;
    }

    if (!isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill)) && !isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvApart))) {
        if (MR::isBinded(this)) {
            if (MR::isBindedGround(this)) {
            }

            if (MR::isBindedWall(this)) {
            }

            if (MR::isBindedRoof(this)) {
            }

            setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill));
        }
    }
}

/* the stack on this function is not correct
    but the layers of inlines are extremely complex
*/
void SkeletalFishGuard::exeAppear() {
    TVec3f v32;
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Swim");
        MR::startBrk(this, "Base");
        MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR");
        MR::validateHitSensors(this);
        MR::clearHitSensors(this);
        MR::offBind(this);
        _C8 = 0.0f;
        _D0.set(-mGravity);
        _10C.set(mPosition);
        _DC.set< f32 >(MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f));
    }

    MR::calcGravityVector(this, _10C, &v32, nullptr, false);
    TVec3f v31 = _10C - v32 * 1000.0f;
    calcTarget(&_E8, &_F4, &_100, 300 - getNerveStep());
    TVec3f v30 = _DC;
    v30.orthogonalize(v32);
    MR::normalizeOrZero(&v30);
    v31 += v30 * 500.0f;
    v30 += TVec3f(0.0f, 1.0f, 0.0f);
    MR::normalize(&v30);
    TVec3f v29 = mPosition;

    if (getNerveStep() < 99) {
        s32 v7 = getNerveStep();
        s32 max = 0x63;
        f32 scaled = v7 / (f32)max;
        TVec3f temp_vec = TVec3f(0.0f, 1.0f, 0.0f) * 0.0f * (f32)max;
        TVec3f temp_vec2 = v30 * 10.0f * (f32)max;
        mPosition.cubic(_10C, temp_vec, temp_vec2, v31, scaled);
    } else {
        f32 v9 = (getNerveStep() - 0x63);
        s32 max = 0xC9;
        f32 scaled = v9 / (f32)max;
        f32 v11 = MR::getRailCoordSpeed(mFishBoss->getCurrentRail());
        TVec3f temp_vec = v30 * 10.0f * (f32)max;
        TVec3f temp_vec2 = _100 * v11 * (f32)max;
        mPosition.cubic(v31, temp_vec, temp_vec2, _F4, scaled);
    }

    TVec3f v28 = mPosition - v29;

    if (!MR::isNearZero(v28)) {
        _D0 = v28;
        MR::normalize(&_D0);
    }

    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL");
    MR::setNerveAtStep(this, GET_NERVE_ANON(SkeletalFishGuardNrvNormal), 300);
}

void SkeletalFishGuard::exeNormal() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    calcTransAndFront();
    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL");
    tryShiftApart();
}

void SkeletalFishGuard::exeApart() {
    if (MR::isFirstStep(this)) {
        TVec3f v15 = _A4.killElement(mGravity);
        _B0 = mGravity.dot(_A4 - v15);
        _A4.set(v15);
        MR::startBck(this, "Turn");
        MR::startBrk(this, "Attack");
        mAttackDelay = 0;
    }

    f32 v3 = (f32)getNerveStep() / 80.0f;
    TVec3f v14 = _A4;

    if (!MR::isNearZero(v14)) {
        MR::normalize(&v14);
        v14.setLength(0.3f);

        f32 v6 = _A4.squared();
        if (v14.squared() >= v6) {
            _A4.zero();
        } else {
            _A4.sub(v14 * 0.3f);
        }
    }

    f32 v7 = _B0 * v3;
    TVec3f grav(mGravity);
    MR::calcGravity(this);
    TQuat4f quat;
    quat.setRotate(grav, mGravity);
    quat.transform(_D0);
    quat.transform(_A4);
    lookToPlayer((v3 * (v3 * (6 * MR::pi()))) / 180.0f, ((v3 * (v3 * (6 * MR::pi()))) / 180.0f));
    mPosition += mGravity * v7 + _A4;
    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL");
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this));
    MR::setNerveAtStep(this, GET_NERVE_ANON(SkeletalFishGuardNrvFollow), 80);
}

void SkeletalFishGuard::exeFollow() {
    if (MR::isFirstStep(this)) {
        _C4 = 0.0f;
        MR::startBck(this, "Attack");
        MR::startBrk(this, "Attack");
        MR::startSound(this, "SE_BM_SKL_GUARD_ATTACK_START");
    }

    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_ATTACK");
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL");
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this));
    mPosition += _D0 * _C4;
    _C4 += 0.8f;
    if (_C4 >= 22.0f) {
        _C4 = 22.0f;
    }

    f32 dist = MR::getPlayerCenterPos()->distance(mPosition);
    f32 v6 = 0.0f;
    f32 v7 = (dist - 1000.0f) / 500.0f;
    if (v7 >= 0.0f) {
        v6 = 1.0f;

        if (v7 <= 1.0f) {
            v6 = ((dist - 1000.0f) / 500.0f);
        }
    }

    lookToPlayer((((1.5f * (1.0f - ((1.0f - v6) * (1.0f - v6)))) * 3.1415927f) / 180.0f),
                 (((1.0f - ((1.0f - v6) * (1.0f - v6))) * 3.1415927f) / 180.0f));
    MR::calcGravity(this);

    if (!tryShiftNumb(GET_NERVE_ANON(SkeletalFishGuardNrvFollow))) {
        tryShiftStraight();
    }
}

void SkeletalFishGuard::exeStraight() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "AttackNear");
    }

    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_ATTACK");
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL");
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this));
    mPosition += _D0 * _C4;

    _C4 += 0.8f;
    if (_C4 >= 22.0f) {
        _C4 = 22.0f;
    }

    MR::calcGravity(this);
    if (!tryShiftNumb(GET_NERVE_ANON(SkeletalFishGuardNrvStraight)) && tryShiftKill()) {
        return;
    }
}

void SkeletalFishGuard::exeDefence() {
}

void SkeletalFishGuard::exeKill() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateShadow(this, nullptr);
        MR::startBck(this, "Break");
        MR::startSound(this, "SE_BM_SKL_GUARD_EXPLODE");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::hideModel(this);
        kill();
    }
}

void SkeletalFishGuard::exeNumb() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::stopBck(this);
        mEffectKeeper->forceDeleteEmitterAll();
        MR::emitEffectWithScale(this, "PointerTouchManual", 1.5f, -1);
        mScaleController->startDpdHitVibration();
    }

    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(_CC);
    }
}

void SkeletalFishGuard::endNumb() {
    MR::deleteEffect(this, "PointerTouchManual");
    mScaleController->startAnim();
}

void SkeletalFishGuard::appearNaturally() {
    appear();
    setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvWait));
    MR::hideModel(this);
}

void SkeletalFishGuard::appearForce() {
    makeActorAppeared();
    MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR");
    setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvNormal));
    MR::showModel(this);
}

void SkeletalFishGuard::killNaturally() {
    if (!MR::isDead(this)) {
        if (isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvWait))) {
            makeActorDead();
        } else if (!isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill))) {
            setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill));
        }
    }
}

void SkeletalFishGuard::waitAttack(s32 time) {
    mAttackDelay = time;
}

void SkeletalFishGuard::calcAndSetBaseMtx() {
    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);

    if (isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvAppear))) {
        TVec3f gravityVec;
        MR::calcGravityVector(this, _F4, &gravityVec, nullptr, 0);
        gravityVec.negate();
        TVec3f stack_4C = gravityVec.cross(_100);
        MR::normalize(&stack_4C);
        gravityVec.cross(_100, stack_4C);
        TQuat4f quat;
        quat.setRotate(_100, _D0);
        quat.transform(gravityVec);
        quat.transform(stack_4C);
        TPos3f stack_D0;
        stack_D0.setXDir(stack_4C);
        stack_D0.setYDir(gravityVec);
        stack_D0.setZDir(_D0);
        stack_D0.setTrans(mPosition);
        MR::setBaseTRMtx(this, stack_D0);
    } else {
        TVec3f stack_30 = -mGravity;
        TVec3f stack_24 = stack_30.cross(_D0);

        if (MR::isNearZero(stack_24)) {
            MtxPtr mtx = getBaseMtx();
            TPos3f stack_A0;
            f32 x, y, z;
            stack_A0.setInline(mtx);
            stack_A0.getXDir(stack_24);
            stack_A0.getYDir(stack_30);
            TVec3f stack_18;
            stack_A0.getZDir(stack_18);
            TQuat4f stack_8;
            stack_8.setRotate(stack_18, _D0);
            stack_8.transform(stack_24);
            stack_8.transform(stack_30);
        } else {
            MR::normalize(&stack_24);
            stack_30.cross(_D0, stack_24);
        }

        TPos3f stack_70;
        stack_70.setXDir(stack_24);
        stack_70.setYDir(stack_30);
        stack_70.setZDir(_D0);
        stack_70.setTrans(mPosition);
        MR::setBaseTRMtx(this, stack_70);
    }
}

void SkeletalFishGuard::exeWait() {
    MR::setNerveAtStep(this, GET_NERVE_ANON(SkeletalFishGuardNrvAppear), _A0);
}

void SkeletalFishGuard::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill)) && (!isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvNormal)) || getNerveStep() >= 2)) {
        if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill));
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool SkeletalFishGuard::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill))) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (isNerve(GET_NERVE_ANON(SkeletalFishGuardNrvNormal)) && getNerveStep() < 2) {
        return false;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W_F");
        setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill));
        return true;
    }

    return false;
}

void SkeletalFishGuard::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "head", "Joint00", 16, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "body", "Joint02", 16, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

void SkeletalFishGuard::calcTransAndFront() {
    calcTarget(&_E8, &_F4, &_100, 0);
    _D0.set(_100);
    _A4.sub(_F4, mPosition);
    mPosition.set(_F4);
}

void SkeletalFishGuard::rotateHorizontal(const TVec3f& a2, f32 scalar) {
    TVec3f v10 = a2.killElement(mGravity);
    if (!MR::isNearZero(v10)) {
        MR::normalize(&v10);
        // TODO: this looks like an inline. Possible fakematch.
        TVec3f* v11 = &_D0;
        TVec3f* g = &mGravity;
        TVec3f v9 = _D0.killElement(*g);
        if (!MR::isNearZero(v9)) {
            MR::normalize(&v9);
            turn(v11, v9, v10, scalar);
        }
    }
}

void SkeletalFishGuard::rotateVertical(const TVec3f& a2, f32 a3) {
    TVec3f v12 = mGravity.cross(_D0);

    if (!MR::isNearZero(v12)) {
        MR::normalize(&v12);
        f32 angle = mGravity.angle(a2);
        f32 v7 = (angle - mGravity.angle(_D0));

        if (v7 > 0.0f) {
            if (v7 > a3) {
                v7 = a3;
            }
        } else {
            if (v7 < -a3) {
                v7 = -a3;
            }
        }

        TQuat4f v10;
        v10.setRotate(v12, v7);
        v10.transform(_D0);
    }
}

bool SkeletalFishGuard::tryShiftApart() {
    if (mAttackDelay > 0) {
        return false;
    }

    if (!isInScreen()) {
        return false;
    }

    if (!isPlayerInAttackRange()) {
        return false;
    }

    if (!isLineOfSightClear()) {
        return false;
    }

    mFishBoss->notifyAttack(this);
    setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvApart));
    return true;
}

bool SkeletalFishGuard::tryShiftStraight() {
    if (_D0.dot(*MR::getPlayerCenterPos() - mPosition) < 0.0f) {
        setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvStraight));
        return true;
    }

    return false;
}

bool SkeletalFishGuard::tryShiftKill() {
    WaterInfo info;
    if (MR::getWaterAreaObj(&info, mPosition)) {
        return false;
    }

    setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvKill));
    return true;
}

void SkeletalFishGuard::turn(TVec3f* a1, const TVec3f& a2, const TVec3f& a3, f32 a4) {
    TQuat4f quat;

    f32 angle = a2.angle(a3);
    f32 v10 = 1.0f;

    if (angle > a4) {
        v10 = (a4 / angle);
    }

    quat.setRotate(a2, a3, v10);
    quat.transform(*a1);
}

void SkeletalFishGuard::lookToPlayer(f32 a2, f32 a3) {
    TVec3f pos(*MR::getPlayerCenterPos());
    pos -= mPosition;

    if (!MR::isNearZero(pos)) {
        MR::normalize(&pos);
        rotateHorizontal(pos, a2);
        rotateVertical(pos, a3);
    }
}

// functionally equiv. just needs stack work
void SkeletalFishGuard::calcTarget(TVec3f* a1, TVec3f* a2, TVec3f* a3, s32 a4) {
    f32 v11 = MR::getRailCoord(mFishBoss->getCurrentRail());
    f32 v13 = (_94.z + (v11 + (MR::getRailCoordSpeed(mFishBoss->getCurrentRail()) * a4)));
    MR::calcRailPosAtCoord(a1, mFishBoss->getCurrentRail(), v13);
    MR::calcRailDirectionAtCoord(a3, mFishBoss->getCurrentRail(), v13);
    TVec3f v22;
    MR::calcGravityVector(this, *a1, &v22, nullptr, 0);
    TVec3f v21 = a3->cross(v22);
    MR::normalizeOrZero(&v21);
    a2->add(*a1, v21 * _94.x + v22 * -_94.y);
}

bool SkeletalFishGuard::isInScreen() const {
    TVec2f screen;
    return MR::calcScreenPosition(&screen, mPosition);
}

bool SkeletalFishGuard::isPlayerInAttackRange() const {
    // FIXME: is there some inline that calculates vector to player?

    TVec3f v7 = *MR::getPlayerCenterPos() - mPosition;

    if (v7.length() > 5000.0f) {
        return false;
    }

    if (MR::isNearZero(v7)) {
        return false;
    }

    MR::normalize(&v7);
    if (v7.dot(_D0) < MR::cosDegree(180.0f)) {
        return false;
    }

    return true;
}

bool SkeletalFishGuard::isLineOfSightClear() const {
    TVec3f v12 = *MR::getPlayerCenterPos() - mPosition;

    if (Collision::checkStrikeLineToMap(mPosition, v12, 0, nullptr, nullptr)) {
        return false;
    }

    if (Collision::checkStrikeLineToMap(mPosition, MR::getCamPos() - mPosition, 0, nullptr, nullptr)) {
        return false;
    }

    TVec3f v11 = _E8 - mPosition;
    v11.orthogonalize2(_100);
    v12.orthogonalize2(_100);

    bool ret = true;

    if (!MR::isNearZero(v11) && !MR::isNearZero(v12)) {
        f64 angle = MR::asin(mFishBoss->getBodyThickness() / v11.length());
        if (v11.angle(v12) < angle) {
            return false;
        }
    }

    return true;
}

bool SkeletalFishGuard::tryShiftNumb(const Nerve* pNerve) {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _CC = pNerve;
        setNerve(GET_NERVE_ANON(SkeletalFishGuardNrvNumb));
        return true;
    }

    return false;
}
