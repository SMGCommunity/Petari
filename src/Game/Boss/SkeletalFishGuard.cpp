#include "Game/Boss/SkeletalFishGuard.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/Map/WaterInfo.hpp"
#include <JSystem/JMath.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>

// JGeometry functions are painful to match, so this just gives the gist
#ifdef NON_MATCHING
template<>
void TVec3f::cubic(const TVec3f &rv1, const TVec3f &rv2, const TVec3f &rv3, const TVec3f &rv4, f32 a) {
    set(
        rv3 * (a * a * a - a * a)
        + rv2 * (a + a * a * a - 2.0f * a * a)
        + rv1 * (1.0f + 2.0f * a * a * a - 3.0f * a * a)
        + rv4 * (3.0f * a * a - 2.0f * a * a * a)
    );
}
#endif

void SkeletalFishGuard::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("SkeletalFishGuard", nullptr, false);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::connectToSceneEnemy(this);
    initNerve(&::SkeletalFishGuardNrvWait::sInstance);
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

    if (!isNerve(&::SkeletalFishGuardNrvKill::sInstance)
        && !isNerve(&::SkeletalFishGuardNrvApart::sInstance)) {
        
        if (MR::isBinded(this)) {
            MR::isBindedGround(this);
            MR::isBindedWall(this);
            MR::isBindedRoof(this);
            setNerve(&::SkeletalFishGuardNrvKill::sInstance);
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
        MR::startBck(this, "Swim", nullptr);
        MR::startBrk(this, "Base");
        MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR", -1, -1);
        MR::validateHitSensors(this);
        MR::clearHitSensors(this);
        MR::offBind(this);
        _C8 = 0.0f;
        TVec3f v27;
        JGeometry::negateInternal((f32*)&mGravity, (f32*)&v27);
        _D0.set(v27);
        _10C.set(mPosition);
        _DC.set(MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f), MR::getRandom(-1.0f, 1.0f));
    }

    
    MR::calcGravityVector(this, _10C, &v32, nullptr, false);
    TVec3f v31 = _10C - v32 * 1000.0f;
    s32 v5 = 300 - getNerveStep();
    calcTarget(&_E8, &_F4, &_100, v5);
    TVec3f v30 = _DC;
    f32 v6 = v32.dot(v30);
    //JMAVECScaleAdd(v32.toCVec(), v30.toCVec(), v30.toVec(), -v6);
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
        mPosition.cubic<f32>(_10C, temp_vec, temp_vec2, v31, scaled);
    }
    else {
        f32 v9 = (getNerveStep() - 0x63);
        s32 max = 0xC9;
        f32 scaled = v9 / (f32)max;
        f32 v11 = MR::getRailCoordSpeed(mFishBoss->getCurrentBossRail());
        TVec3f temp_vec = v30 * 10.0f * (f32)max;
        TVec3f temp_vec2 = _100 * v11 * (f32)max;
        mPosition.cubic<f32>(v31, temp_vec, temp_vec2, _F4, scaled);
    }

    TVec3f v28 = mPosition - v29;

    if (!MR::isNearZero(v28, 0.001f)) {
        _D0.setInlinePS_2(v28);
        MR::normalize(&_D0);
    }

    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    MR::setNerveAtStep(this, &::SkeletalFishGuardNrvNormal::sInstance, 300);
}

void SkeletalFishGuard::exeNormal() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    calcTransAndFront();
    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    tryShiftApart();
}

#ifdef NON_MATCHING
void SkeletalFishGuard::exeApart() {
    if (MR::isFirstStep(this)) {
        TVec3f* grav = &mGravity;
        f32 dot = grav->dot(_A4);
        TVec3f v15;
        JMAVECScaleAdd(grav->toCVec(), _A4.toCVec(), v15.toVec(), -dot);
        _B0 = grav->dot(_A4 - v15);
        _A4.set(v15);
        MR::startBck(this, "Turn", nullptr);
        MR::startBrk(this, "Attack");
        mAttackDelay = 0;
    }

    f32 v3 = (f32)getNerveStep() / 80.0f;
    TVec3f v14(_A4);

    if (!MR::isNearZero(v14, 0.001f)) {
        MR::normalize(&v14);
        f32 sqr = v14.squared();
        if (sqr > 0.0000038146973f) {
            f32 inv = JGeometry::TUtil<f32>::inv_sqrt(sqr);
            v14.scale(inv * 0.30000001f);
        }

        f32 v6 = _A4.squared();
        if (v14.squared() >= v6) {
            _A4.z = 0.0f;
            _A4.y = 0.0f;
            _A4.x = 0.0f;
        }
        else {
            TVec3f v10 = v14 * 0.30000001f;
            JMathInlineVEC::PSVECSubtract(_A4.toCVec(), v10.toCVec(), _A4.toVec());
        }
    }

    f32 v7 = (_B0 * v3);
    TVec3f grav(mGravity);
    MR::calcGravity(this);
    TQuat4f quat;
    quat.setRotate(grav, mGravity);
    quat.transform(_D0);
    quat.transform(_A4);
    lookToPlayer((v3 * (v3 * 18.849556f)) / 180.0f, ((v3 * (v3 * 18.849556f)) / 180.0f));
    mPosition = (_A4 + (mGravity * v7)) + mPosition;
    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this), -1, -1);
    MR::setNerveAtStep(this, &::SkeletalFishGuardNrvFollow::sInstance, 80);
}
#endif

#ifdef NON_MATCHING
void SkeletalFishGuard::exeFollow() {
    if (MR::isFirstStep(this)) {
        _C4 = 0.0f;
        MR::startBck(this, "Attack", nullptr);
        MR::startBrk(this, "Attack");
        MR::startSound(this, "SE_BM_SKL_GUARD_ATTACK_START", -1, -1);
    }

    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_ATTACK", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this), -1, -1);
    mPosition += _D0 * _C4;
    _C4 += 0.80000001f;
    if (_C4 >= 22.0f) {
        _C4 = 22.0f;
    }

    f32 dist = PSVECDistance(MR::getPlayerCenterPos()->toCVec(), mPosition.toCVec());
    f32 v6 = 0.0f;
    f32 v7 = (dist - 1000.0f) / 500.0f;
    if (v7 >= 0.0f) {
        v6 = 1.0f;

        if (v7 <= 1.0f) {
            v6 = ((dist - 1000.0f) / 500.0f);
        }
    }

    lookToPlayer((((1.5f * (1.0f - ((1.0f - v6) * (1.0f - v6)))) * 3.1415927f) / 180.0f), (((1.0f - ((1.0f - v6) * (1.0f - v6))) * 3.1415927f) / 180.0f));
    MR::calcGravity(this);

    if (!tryShiftNumb(&::SkeletalFishGuardNrvFollow::sInstance)) {
        tryShiftStraight();
    }
}
#endif

void SkeletalFishGuard::exeStraight() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "AttackNear");
    }

    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_ATTACK", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_ALARM", MR::calcDistanceToPlayer(this), -1, -1);
    mPosition += _D0 * _C4;

    _C4 += 0.80000001f;
    if (_C4 >= 22.0f) {
        _C4 = 22.0f;
    }

    MR::calcGravity(this);
    if (!tryShiftNumb(&::SkeletalFishGuardNrvStraight::sInstance)) {
        tryShiftKill();
    }
}

void SkeletalFishGuard::exeKill() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateShadow(this, nullptr);
        MR::startBck(this, "Break", nullptr);
        MR::startSound(this, "SE_BM_SKL_GUARD_EXPLODE", -1, -1);
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

void SkeletalFishGuard::appearNaturally() {
    appear();
    setNerve(&::SkeletalFishGuardNrvWait::sInstance);
    MR::hideModel(this);
}

void SkeletalFishGuard::appearForce() {
    makeActorAppeared();
    MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR", -1, -1);
    setNerve(&::SkeletalFishGuardNrvNormal::sInstance);
    MR::showModel(this);
}

void SkeletalFishGuard::killNaturally() {
    if (!MR::isDead(this)) {
        if (isNerve(&::SkeletalFishGuardNrvWait::sInstance)) {
            makeActorDead();
        }
        else if (!isNerve(&::SkeletalFishGuardNrvKill::sInstance)) {
            setNerve(&::SkeletalFishGuardNrvKill::sInstance);
        }
    }
}

void SkeletalFishGuard::waitAttack(s32 time) {
    mAttackDelay = time;
}

void SkeletalFishGuard::calcAndSetBaseMtx() {
    TVec3f stack_64;
    stack_64.multPS(mScale, mScaleController->_C);
    MR::setBaseScale(this, stack_64);

    if (isNerve(&::SkeletalFishGuardNrvAppear::sInstance)) {
        TVec3f gravityVec;
        MR::calcGravityVector(this, _F4, &gravityVec, nullptr, 0);
        JGeometry::negateInternal((f32*)&gravityVec, (f32*)&gravityVec);
        TVec3f stack_4C;
        PSVECCrossProduct(gravityVec.toCVec(), _100.toCVec(), stack_4C.toVec());
        MR::normalize(&stack_4C);
        PSVECCrossProduct(_100.toCVec(), stack_4C.toCVec(), gravityVec.toVec());
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
    }
    else {
        TVec3f stack_30;
        TVec3f stack_24;
        JGeometry::negateInternal((f32*)&mGravity, (f32*)&stack_30);
        PSVECCrossProduct(stack_30.toCVec(), _D0.toCVec(), stack_24.toVec());

        if (MR::isNearZero(stack_24, 0.001f)) {
            MtxPtr mtx = getBaseMtx();
            TPos3f stack_A0;
            f32 x, y, z;
            stack_A0.setInline_2(mtx);
            x = stack_A0.mMtx[0][0];
            y = stack_A0.mMtx[1][0];
            z = stack_A0.mMtx[2][0];
            
            stack_24.set(x, y, z);
            z = stack_A0.mMtx[2][1];
            y = stack_A0.mMtx[1][1];
            x = stack_A0.mMtx[0][1];
            stack_30.set(x, y, z);
            TVec3f stack_18;
            z = stack_A0.mMtx[2][2];
            y = stack_A0.mMtx[1][2];
            x = stack_A0.mMtx[0][2];
            stack_18.set(x, y, z);
            TQuat4f stack_8;
            stack_8.setRotate(stack_18, _D0);
            stack_8.transform(stack_24);
            stack_8.transform(stack_30);
        }
        else {
            MR::normalize(&stack_24);
            PSVECCrossProduct(_D0.toCVec(), stack_24.toCVec(), stack_30.toVec());
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
    MR::setNerveAtStep(this, &::SkeletalFishGuardNrvAppear::sInstance, _A0);
}

void SkeletalFishGuard::attackSensor(HitSensor *a2, HitSensor *a3) {
    if (!isNerve(&::SkeletalFishGuardNrvKill::sInstance) && (!isNerve(&::SkeletalFishGuardNrvNormal::sInstance) || getNerveStep() >= 2)) {
        if (MR::isSensorPlayer(a3) && MR::sendMsgEnemyAttackStrong(a3, a2)) {
            setNerve(&::SkeletalFishGuardNrvKill::sInstance);
        }
        else {
            MR::sendMsgPush(a3, a2);
        }
    }
}

bool SkeletalFishGuard::receiveMsgPlayerAttack(u32 msg, HitSensor *a3, HitSensor *a4) {
    if (isNerve(&::SkeletalFishGuardNrvKill::sInstance)) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (isNerve(&::SkeletalFishGuardNrvNormal::sInstance) && getNerveStep() < 2) {
        return false;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W_F", -1, -1);
        setNerve(&::SkeletalFishGuardNrvKill::sInstance);
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
    JMathInlineVEC::PSVECSubtract(_F4.toCVec(), mPosition.toCVec(), _A4.toVec());
    mPosition.set(_F4);
}

void SkeletalFishGuard::rotateHorizontal(const TVec3f &a2, f32 scalar) {
    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(a2);
    TVec3f v10;
    JMAVECScaleAdd(grav->toCVec(), a2.toCVec(), v10.toVec(), -dot);
    if (!MR::isNearZero(v10, 0.001f)) {
        MR::normalize(&v10);
        TVec3f* v11 = &_D0;
        TVec3f* g = &mGravity;
        f32 v8 = g->dot(*v11);
        TVec3f v9;
        JMAVECScaleAdd(g->toCVec(), _D0.toCVec(), v9.toVec(), -v8);
        if (!MR::isNearZero(v9, 0.001f)) {
            MR::normalize(&v9);
            turn(v11, v9, v10, scalar);
        }
    }
}

void SkeletalFishGuard::rotateVertical(const TVec3f &a2, f32 a3) {
    TVec3f v12;
    PSVECCrossProduct(mGravity.toCVec(), _D0.toCVec(), v12.toVec());

    if (!MR::isNearZero(v12, 0.001f)) {
        MR::normalize(&v12);
        f32 angle = mGravity.angle(a2);
        f32 v7 = (angle - mGravity.angle(_D0));

        if (v7 > 0.0f) {
            if (v7 > a3) {
                v7 = a3;
            }
        }
        else {
            if (v7 < -a3) {
                v7 = -a3;
            }
        }

        f32 v8 = (0.5f * v7);
        f32 v9 = sin(v8);
        TQuat4f v10;
        v10.scale(v9, v12);
        v10.w = cos(v8);
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
    setNerve(&::SkeletalFishGuardNrvApart::sInstance);
    return true;
}

bool SkeletalFishGuard::tryShiftStraight() {
    TVec3f pos(*MR::getPlayerCenterPos());
    JMathInlineVEC::PSVECSubtract(pos.toCVec(), mPosition.toCVec(), pos.toVec());
    if (_D0.dot(pos) < 0.0f) {
        setNerve(&::SkeletalFishGuardNrvStraight::sInstance);
        return true;
    }

    return false;
}

bool SkeletalFishGuard::tryShiftKill() {
    WaterInfo info;
    if (MR::getWaterAreaObj(&info, mPosition)) {
        return false;
    }

    setNerve(&::SkeletalFishGuardNrvKill::sInstance);
    return true;
}

#ifdef NON_MATCHING
// very close. small instruction swap.
void SkeletalFishGuard::turn(TVec3f *a1, const TVec3f &a2, const TVec3f &a3, f32 a4) {
    TQuat4f quat;

    f32 angle = a2.angle(a3);
    f32 v10 = 1.0f;

    if (angle > a4) {
        v10 = (a4 / angle);
    }

    TVec3f v15;
    PSVECCrossProduct(a2.toCVec(), a3.toCVec(), v15.toVec());
    f32 v11 = PSVECMag(v15.toCVec());
    f32 factor = 1.0f / 262144.0f;

    if (v11 <= factor) {
        quat.set(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        f32 v12 = a2.dot(a3);
        f32 v13 = (v10 * (0.5f * JMath::sAtanTable.atan2_(v11, v12)));
        f32 v14 = sin(v13);
        quat.scale((v14 / v11), v15);
        quat.w = cos(v13);
    }

    quat.transform(*a1);
}
#endif

void SkeletalFishGuard::lookToPlayer(f32 a2, f32 a3) {
    TVec3f pos(*MR::getPlayerCenterPos());
    pos -= mPosition;

    if (!MR::isNearZero(pos, 0.001f)) {
        MR::normalize(&pos);
        rotateHorizontal(pos, a2);
        rotateVertical(pos, a3);
    }
}

#ifdef NON_MATCHING
// functionally equiv. just needs stack work
void SkeletalFishGuard::calcTarget(TVec3f *a1, TVec3f *a2, TVec3f *a3, s32 a4) {
    f32 v11 = MR::getRailCoord(mFishBoss->getCurrentRail());
    f32 v13 = (_94.z + (v11 + (MR::getRailCoordSpeed(mFishBoss->getCurrentRail()) * a4)));
    MR::calcRailPosAtCoord(a1, mFishBoss->getCurrentRail(), v13);
    MR::calcRailDirectionAtCoord(a3, mFishBoss->getCurrentRail(), v13);
    TVec3f v22;
    MR::calcGravityVector(this, *a1, &v22, nullptr, 0);
    TVec3f v21;
    PSVECCrossProduct(a3->toCVec(), v22.toCVec(), v21.toVec());
    MR::normalizeOrZero(&v21);
    f32 v16 = -_94.y;
    TVec3f v18(v22);
    v18.scale(v16);
    f32 v17 = _94.x;
    TVec3f v19(v21);
    v19.scale(v17);
    TVec3f v20(v19);
    JMathInlineVEC::PSVECAdd(v20.toCVec(), v18.toCVec(), v20.toVec());
    JMathInlineVEC::PSVECAdd(a1->toCVec(), v20.toCVec(), a2->toVec());
}
#endif

bool SkeletalFishGuard::isInScreen() const {
    TVec2f screen;
    return MR::calcScreenPosition(&screen, mPosition);
}

//#ifdef NON_MATCHING
bool SkeletalFishGuard::isPlayerInAttackRange() const {
    //const Vec* posPtr = mPosition.toCVec();
    //TVec3f v7(*MR::getPlayerCenterPos());
    //JMathInlineVEC::PSVECSubtract(v7.toCVec(), posPtr, v7.toVec());

    TVec3f v7 = MR::subVec(mPosition, *MR::getPlayerCenterPos());

    if (PSVECMag(v7.toCVec()) > 5000.0f) {
        return false;
    }

    if (MR::isNearZero(v7, 0.001f)) {
        return false;
    }

    MR::normalize(&v7);
    f32 lap = JMath::sSinCosTable.cosLap(180.0f);
    f32 dot = v7.dot(_D0);
    if (dot < lap) {
        return false;
    }
    
    return true; 
}
//#endif

#ifdef NON_MATCHING
// pretty close
bool SkeletalFishGuard::isLineOfSightClear() const {
    TVec3f v12(*MR::getPlayerCenterPos());
    JMathInlineVEC::PSVECSubtract(v12.toCVec(), mPosition.toCVec(), v12.toVec());
    
    if (Collision::checkStrikeLineToMap(mPosition, v12, 0, nullptr, nullptr)) {
        return false;
    }

    TVec3f v10 = MR::getCamPos();
    JMathInlineVEC::PSVECSubtract(v10.toCVec(), mPosition.toCVec(), v10.toVec());
    
    if (Collision::checkStrikeLineToMap(mPosition, v10, 0, nullptr, nullptr)) {
        return false; 
    }

    TVec3f v11(_E8);
    JMathInlineVEC::PSVECSubtract(v11.toCVec(), mPosition.toCVec(), v11.toVec());
    f32 v4 = _100.dot(v11);
    JMAVECScaleAdd(_100.toCVec(), v11.toCVec(), v11.toVec(), -v4);
    f32 v5 = _100.dot(v12);
    JMAVECScaleAdd(_100.toCVec(), v12.toCVec(), v12.toVec(), -v5);
    bool ret = true;

    if (!MR::isNearZero(v11, 0.001f) && !MR::isNearZero(v12, 0.001f)) {
        f32 v6 = PSVECMag(v11.toCVec());
        f32 thick = mFishBoss->getBodyThickness();
        f64 angle = JMAAsinRadian(thick / v6);
        if (v11.angle(v12) < angle) {
            return false;
        }
    }

    return true;
}
#endif

bool SkeletalFishGuard::tryShiftNumb(const Nerve *pNerve) {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _CC = pNerve;
        setNerve(&::SkeletalFishGuardNrvNumb::sInstance);
        return true;
    }

    return false;
}

SkeletalFishGuard::~SkeletalFishGuard() {

}

namespace {
    INIT_NERVE(SkeletalFishGuardNrvWait);
    INIT_NERVE(SkeletalFishGuardNrvAppear);
    INIT_NERVE(SkeletalFishGuardNrvNormal);
    INIT_NERVE(SkeletalFishGuardNrvApart);
    INIT_NERVE(SkeletalFishGuardNrvFollow);
    INIT_NERVE(SkeletalFishGuardNrvStraight);
    INIT_NERVE(SkeletalFishGuardNrvDefence);
    INIT_NERVE(SkeletalFishGuardNrvKill);
    INIT_NERVE(SkeletalFishGuardNrvNumb);
};