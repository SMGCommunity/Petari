#include "Game/Enemy/Gesso.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    // static const f32 sReceivePushPower = ;
    // static const f32 sAttenuatePushRate = ;
    // static const f32 sWaterJudgeOffset = ;
    // static const f32 sOutBoxSpeedUp = ;
    // static const f32 sOutBoxSpeedFront = ;
    // static const a32 sStepForAppear = ;
    // static const f32 sDriftSpeedMax = ;
    // static const f32 sWaitTurnRate = ;
    // static const s32 sCountToInitPos = ;
    // static const f32 sSearchRadiusIn = ;
    // static const f32 sSearchRadiusOut = ;
    // static const f32 sSearchTurnRate = ;
    // static const f32 sHighSpeedRadius = ;
    // static const f32 sWalkSpeedFront = ;
    // static const f32 sWalkSpeedUpMax = ;
    // static const f32 sWalkSpeedSide = ;
    // static const f32 sWalkSinEndDegree = ;
    // static const f32 sUpVecTurnRate = ;
    // static const f32 sSinkSpeedUpMax = ;
    // static const f32 sSinkSinEndDegree = ;
    // static const s32 sStepForSink = ;
    // static const s32 sAttackReaction = ;
    // static const s32 sStepForStopReaction = ;
    // static const s32 sStepForSearchOff = ;
    // static const s32 sPunchDownReaction = ;
    // static const s32 sStepForPunchDown = ;
    // static const f32 sComeBackSpeed = ;
    // static const f32 sJudgeForComeBackRadius = ;
    // static const s32 sStepForWalkCharge = ;
    static const s32 sStepForWalk = 43;
};  // namespace

namespace NrvGesso {
    NEW_NERVE(GessoNrvComeFromBox, Gesso, ComeFromBox);
    NEW_NERVE(GessoNrvWait, Gesso, Wait);
    NEW_NERVE(GessoNrvSearch, Gesso, Search);
    NEW_NERVE(GessoNrvWalkCharge, Gesso, WalkCharge);
    NEW_NERVE(GessoNrvWalk, Gesso, Walk);
    NEW_NERVE(GessoNrvSink, Gesso, Sink);
    NEW_NERVE(GessoNrvLostPlayer, Gesso, LostPlayer);
    NEW_NERVE(GessoNrvAttack, Gesso, Attack);
    NEW_NERVE(GessoNrvPunchDown, Gesso, PunchDown);
    NEW_NERVE(GessoNrvComeBack, Gesso, ComeBack);
    NEW_NERVE_ONEND(GessoNrvDPDSwoon, Gesso, DPDSwoon, DPDSwoon);
    NEW_NERVE(GessoNrvRotate, Gesso, Rotate);

};  // namespace NrvGesso

Gesso::Gesso(const char* pName)
    : LiveActor(pName), mScaleController(nullptr), mStateBindStarPointer(nullptr), _94(0.0f, 0.0f, 0.0f), _A0(0.0f, 0.0f, 1.0f),
      _AC(0.0f, 1.0f, 0.0f), _B8(0.0f, 1.0f, 0.0f), _C4(0), _C8(0.0f, 0.0f, 0.0f), mIsMarioLeft(false), mIsHighSpeedMode(false) {
}

void Gesso::init(const JMapInfoIter& rIter) {
    // FIXME: TVec ctor inlining
    // https://decomp.me/scratch/TEB6E
    MR::initDefaultPos(this, rIter);
    _94 = mPosition;
    MR::useStageSwitchReadAppear(this, rIter);
    initModelManagerWithAnm("Gesso", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_KILLER_TARGET_ENEMY, 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(150.0f, 0.0f, 0);
    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    mScaleController = new AnimScaleController(nullptr);
    mStateBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    MR::initShadowVolumeSphere(this, 90.0f);
    MR::calcFrontVec(&_A0, this);
    MR::calcUpVec(&_AC, this);
    _B8.set(_AC);
    MR::declareCoin(this, 1);
    MR::addToAttributeGroupSearchTurtle(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        s32 initialBehavior;
        MR::getJMapInfoArg0NoInit(rIter, &initialBehavior);
        if (initialBehavior == 0) {
            initNerve(GET_NERVE(Gesso, GessoNrvComeFromBox));
        } else {
            initNerve(GET_NERVE(Gesso, GessoNrvWait));
        }
        makeActorDead();
    } else {
        initNerve(GET_NERVE(Gesso, GessoNrvWait));
        makeActorAppeared();
    }
}

void Gesso::kill() {
    MR::emitEffect(this, "Death");
    MR::appearCoinPop(this, mPosition, 1);
    LiveActor::kill();
}

void Gesso::control() {
    // FIXME: killElement inline issues
    // https://decomp.me/scratch/cCHgO
    mScaleController->updateNerve();
    if (tryDPDSwoon()) {
        return;
    }
    if (clipAndInitPos()) {
        return;
    }

    TVec3f stack_08;
    TVec3f diffpos = *MR::getPlayerPos() - mPosition;
    MR::normalize(diffpos, &stack_08);
    MR::turnVecToVecCosOnPlane(&_AC, _B8, _A0, MR::cosDegree(0.5f));
    if (mVelocity.dot(_C8) < 0.0f) {
        _C8.killElement(mVelocity);
    }
    mVelocity.add(_C8);
    _C8 *= 0.7f;
    if (MR::isNearZero(_C8, 0.001f)) {
        _C8.zero();
    }
    stack_08.negate(mGravity);
    diffpos.scale(50.0f, stack_08);
    if (!MR::isInWater(this, diffpos) && mVelocity.dot(stack_08) > 0.0f) {
        stack_08.killElement(mVelocity);
    }
}

void Gesso::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, _AC, _A0, mPosition);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void Gesso::exeComeFromBox() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "FromBox");
        MR::startSound(this, "SE_EM_GESSO_SWIM_L");
        MR::invalidateHitSensors(this);
        mPosition.scaleAdd(-100.0f, mGravity, mPosition);
        _B8.set(0.0f, 1.0f, 0.3f);
        MR::normalize(&_B8);
        _AC.set(_B8);
    }
    if (MR::isLessStep(this, 10)) {
        calcAndSetVelocity(0.0f, 10.0f, 5.0f);
    } else {
        mVelocity.zero();
    }
    if (MR::isActionEnd(this)) {
        mVelocity.zero();
        MR::validateHitSensors(this);
        setNerve(GET_NERVE(Gesso, GessoNrvWait));
    }
}

void Gesso::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        calcAndSetUpVecTarget(0.0f, 0.0f, 0.0f);
    }
    if (MR::isStep(this, 2)) {
        MR::invalidateClipping(this);
    }
    mVelocity.scale(1.5f * (MR::sinDegree(45.0f + (2.0f * getNerveStep()))), mGravity);
    MR::rotateVecDegree(&_A0, mGravity, 0.3f);
    if (MR::isNearPlayer(this, 1400.0f) && MR::isPlayerInWaterMode()) {
        MR::validateClipping(this);
        setNerve(GET_NERVE(Gesso, GessoNrvSearch));
    }
}

void Gesso::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Search");
        MR::startSound(this, "SE_EM_GESSO_FIND");
        calcAndSetUpVecTarget(0.0f, 0.0f, 0.0f);
    }

    mVelocity.scale(1.5f * (MR::sinDegree(45.0f + (2.0f * getNerveStep()))), mGravity);
    MR::turnDirectionToPlayerDegree(this, &_A0, 1.5f);
    if (MR::isFaceToPlayerHorizontalDegree(this, _A0, 1.5f) && MR::isActionEnd(this)) {
        setNerve(GET_NERVE(Gesso, GessoNrvWalkCharge));
    }
}

void Gesso::exeWalkCharge() {
    if (MR::isFirstStep(this)) {
        if (tryChangeHighSpeedMode()) {
            MR::startAction(this, "WalkFast");
            _B8.set(0.0f, 1.0f, 1.0f);
            MR::normalize(&_B8);
        } else {
            MR::startAction(this, "Walk");
            _B8.set(0.0f, 1.0f, 1.0f);
            MR::normalize(&_B8);
        }
    }
    mVelocity.mult(0.995f);
    if (MR::isStep(this, sStepForWalk)) {
        mIsMarioLeft = isMarioLeft();
        setNerve(GET_NERVE(Gesso, GessoNrvWalk));
    }
}

void Gesso::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_GESSO_SWIM_L");
    }
    MR::turnDirectionToTargetDegree(this, &_A0, *MR::getPlayerPos(), 1.5f);
    if (calcWalkMove(getNerveStep())) {
        selectNextNerve();
    }
}

void Gesso::exeSink() {
    if (MR::isFirstStep(this)) {
        if (MR::isNearPlayer(this, 1000.0f)) {
            MR::startAction(this, "SinkFast");
        } else {
            MR::startAction(this, "Sink");
        }
    }
    MR::turnDirectionToTargetDegree(this, &_A0, *MR::getPlayerPos(), 1.5f);
    if (MR::isBindedGround(this)) {
        setNerve(GET_NERVE(Gesso, GessoNrvWalkCharge));
    } else if (calcSinkMove(getNerveStep())) {
        selectNextNerve();
    }
}

void Gesso::exeLostPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "CoolDown");
        calcAndSetUpVecTarget(0.0f, 0.0f, 0.0f);
    }
    mVelocity.scale(1.5f * (MR::sinDegree(45.0f + (2.0f * getNerveStep()))), mGravity);
    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(Gesso, GessoNrvWait));
    }
}

void Gesso::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
        MR::startSound(this, "SE_EM_GESSO_INK");
        calcAndSetUpVecTarget(0.0f, 0.0f, 0.0f);
    }
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startAction(this, "Wait");
    }
    if (MR::isBckPlaying(this, "Attack")) {
        mVelocity.mult(0.9f);
    } else {
        mVelocity.scale(1.5f * (MR::sinDegree(45.0f + (2.0f * getNerveStep()))), mGravity);
    }
    if (MR::isStep(this, 240)) {
        setNerve(GET_NERVE(Gesso, GessoNrvWait));
    }
}

void Gesso::exePunchDown() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::stopScene(5);
        MR::startAction(this, "PunchDown");
        MR::startSound(this, "SE_EM_GESSO_HIT_PUNCH");
        MR::startBlowHitSound(this);
    }
    MR::turnDirectionToTargetDegree(this, &_A0, *MR::getPlayerPos(), 1.5f);
    if (MR::isStep(this, 20) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_GESSO_DEAD");
        kill();
    }
}

void Gesso::exeComeBack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "CoolDown");
    }

    if (MR::isBckPlaying(this, "CoolDown")) {
        mVelocity.scale(1.5f * (MR::sinDegree(45.0f + (2.0f * getNerveStep()))), mGravity);
    } else {
        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startAction(this, "Sink");
        }
        if (MR::isPlayerInWaterMode()) {
            setNerve(GET_NERVE(Gesso, GessoNrvWait));
        } else {
            MR::turnDirectionToTargetDegree(this, &_A0, *MR::getPlayerPos(), 1.5f);
            TVec3f stack_14;
            TVec3f diff = _94 - mPosition;
            MR::normalize(diff, &stack_14);
            mVelocity.scale(3.0f, stack_14);
            if (MR::isNear(this, _94, 100.0f)) {
                setNerve(GET_NERVE(Gesso, GessoNrvWait));
            }
        }
    }
}

void Gesso::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StarPiece");
        mVelocity.zero();
    }
    MR::startLevelSound(this, "SE_EM_LV_GESSO_STAR_PIECE_HIT");
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Gesso, GessoNrvWait));
    }
}

void Gesso::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mStateBindStarPointer, GET_NERVE(Gesso, GessoNrvWait));
}

void Gesso::endDPDSwoon() {
    mStateBindStarPointer->kill();
}

void Gesso::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(Gesso, GessoNrvPunchDown)) && MR::isSensorEnemy(pSender) && MR::isSensorPlayer(pReceiver) &&
        !isNerve(GET_NERVE(Gesso, GessoNrvDPDSwoon)) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
        TVec3f stack_14;
        TVec3f diff = pSender->mPosition - pReceiver->mPosition;
        MR::normalize(diff, &stack_14);
        mVelocity.scale(25.0f, stack_14);
        setNerve(GET_NERVE(Gesso, GessoNrvAttack));
    }
}

bool Gesso::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Gesso, GessoNrvPunchDown))) {
        return false;
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(GET_NERVE(Gesso, GessoNrvRotate));
        return true;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgPlayerHitAll(msg)) {
        knockOut(pSender, pReceiver);
        return true;
    }

    return false;
}

bool Gesso::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Gesso, GessoNrvPunchDown))) {
        return false;
    }
    if (MR::isMsgExplosionAttack(msg)) {
        knockOut(pSender, pReceiver);
        return true;
    }
    if (pSender->mType == ACTMES_RUSHDROP) {
        knockOut(pSender, pReceiver);
        return false;
    }

    return false;
}

bool Gesso::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        TVec3f stack_08;
        stack_08.sub(mPosition, pSender->mHost->mPosition);
        MR::normalizeOrZero(&stack_08);
        _C8.scaleAdd(0.2f, _C8, stack_08);
        return true;
    }

    return false;
}

void Gesso::selectNextNerve() {
    if (!MR::isPlayerInWaterMode()) {
        setNerve(GET_NERVE(Gesso, GessoNrvComeBack));
    } else if (!MR::isNearPlayer(this, 2400.0f)) {
        setNerve(GET_NERVE(Gesso, GessoNrvLostPlayer));
    } else if (isMarioUp()) {
        setNerve(GET_NERVE(Gesso, GessoNrvWalkCharge));
    } else if (!isNerve(GET_NERVE(Gesso, GessoNrvSink))) {
        setNerve(GET_NERVE(Gesso, GessoNrvSink));
    }
}

void Gesso::knockOut(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f stack_14;
    TVec3f diff = pReceiver->mPosition - pSender->mPosition;
    MR::normalize(diff, &stack_14);
    mVelocity.scale(30.0f, stack_14);
    MR::turnDirectionToTarget(this, &_A0, pSender->mPosition, -1.0f);
    setNerve(GET_NERVE(Gesso, GessoNrvPunchDown));
}

bool Gesso::clipAndInitPos() {
    if (!MR::isNearPlayer(this, 10000.0f) || MR::isJudgedToClipFrustum(mPosition, getSensor(nullptr)->getRadius())) {
        if (_C4 >= 300) {
            mPosition.set(_94);
            setNerve(GET_NERVE(Gesso, GessoNrvWait));
            MR::validateClipping(this);
            _C4 = 0;
            return true;
        }
        _C4++;
        return false;
    }

    _C4 = 0;
    return false;
}

bool Gesso::calcWalkMove(s32 step) {
    // FIXME: float regswaps
    // https://decomp.me/scratch/LSPgj
    f32 a;
    if (mIsMarioLeft) {
        a = 1.0f;
    } else {
        a = -1.0f;
    }
    f32 b;
    f32 c = 0.990099f * step;

    if (mIsHighSpeedMode) {
        MR::cosDegree(c);
        TVec3f* vel = &mVelocity;
        TVec3f upVec;
        TVec3f sideVec;
        b = c * 7.0f;
        MR::calcUpVec(&upVec, this);
        MR::calcSideVec(&sideVec, this);
        mVelocity.scale(b, upVec);
        vel->scaleAdd((a * 3.0f) / 5.0f, sideVec, *vel);
        vel->scaleAdd(2.3f, _A0, *vel);
    } else {
        MR::cosDegree(c);
        TVec3f* vel = &mVelocity;
        TVec3f upVec;
        TVec3f sideVec;
        b = c * 1.5f;
        MR::calcUpVec(&upVec, this);
        MR::calcSideVec(&sideVec, this);
        mVelocity.scale(b, upVec);
        vel->scaleAdd(a * 3.0f, sideVec, *vel);
        vel->scaleAdd(2.3f, _A0, *vel);
    }

    if (MR::isFirstStep(this)) {
        if (mIsHighSpeedMode) {
            calcAndSetUpVecTarget((3.0f * a) / 5.0f, MR::cosDegree(c) * 7.0f, 4.6f);
        } else {
            calcAndSetUpVecTarget((3.0f * a) * 0.5f, MR::cosDegree(c) * 7.0f, 4.6f);
        }
    }

    if (c >= 70.0f) {
        _B8.set(0.0f, 1.0f, 1.0f);
        MR::normalize(&_B8);
    }

    return a >= 100.0f;
}

bool Gesso::calcSinkMove(s32 step) {
    // FIXME: float regswaps
    // https://decomp.me/scratch/SpPn3
    f32 a;
    if (mIsMarioLeft) {
        a = 1.0f;
    } else {
        a = -1.0f;
    }

    f32 b = 100.0f + 3.0f * step;
    if (b >= 180.0f) {
        b = 180.0f;
    }

    f32 cosMult = MR::cosDegree(b) * 3.0f;
    TVec3f* vel = &mVelocity;
    TVec3f upVec;
    TVec3f sideVec;
    MR::calcUpVec(&upVec, this);
    MR::calcSideVec(&sideVec, this);

    mVelocity.scale(cosMult, upVec);
    vel->scaleAdd(a * 3.0f, sideVec, *vel);
    vel->scaleAdd(2.3f, _A0, *vel);
    _B8.set(0.0f, 1.0f, 1.0f);
    MR::normalize(&_B8);

    return b >= 100.0f;
}

void Gesso::calcAndSetVelocity(f32 f1, f32 f2, f32 f3) {
    TVec3f* vel = &mVelocity;
    TVec3f sideVec;
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    MR::calcSideVec(&sideVec, this);
    mVelocity.scale(f2, upVec);
    vel->scaleAdd(f1, sideVec, *vel);
    vel->scaleAdd(f3, _A0, *vel);
}

void Gesso::calcAndSetUpVecTarget(f32 f1, f32 f2, f32 f3) {
    if (f1 == 0.0f && f2 == 0.0f && f3 == 0.0f) {
        _B8.set(0.0f, 1.0f, 0.0f);
        return;
    }
    _B8.set(f1, f2, f3);
    MR::normalize(&_B8);
    return;
}

bool Gesso::isMarioUp() const {
    TVec3f* pos = MR::getPlayerPos();
    TVec3f playerDist;
    playerDist.sub(*pos, mPosition);
    TVec3f upVec;
    upVec.negate(mGravity);
    return playerDist.dot(upVec) > 0.0f;
}

bool Gesso::isMarioLeft() const {
    TVec3f* pos = MR::getPlayerPos();
    TVec3f playerDist;
    playerDist.sub(*pos, mPosition);
    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);
    return playerDist.dot(sideVec) > 0.0f;
}

bool Gesso::tryChangeHighSpeedMode() {
    bool change = false;
    if (MR::isNearPlayer(this, 1000.0f) && !MR::isPlayerInWaterMode()) {
        change = true;
    }
    mIsHighSpeedMode = change;
    return change;
}

bool Gesso::tryDPDSwoon() {
    if (isNerve(GET_NERVE(Gesso, GessoNrvDPDSwoon))) {
        return false;
    }
    if (isNerve(GET_NERVE(Gesso, GessoNrvPunchDown))) {
        return false;
    }
    if (isNerve(GET_NERVE(Gesso, GessoNrvRotate))) {
        return false;
    }
    if (!mStateBindStarPointer->tryStartPointBind()) {
        return false;
    }
    setNerve(GET_NERVE(Gesso, GessoNrvDPDSwoon));
    return true;
}

Gesso::~Gesso() {
}
