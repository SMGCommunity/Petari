#include "Game/Enemy/Mogucchi.hpp"
#include "Game/Enemy/MogucchiHill.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    NEW_NERVE_ONEND(MogucchiNrvStroll, Mogucchi, Stroll, Stroll);
    NEW_NERVE(MogucchiNrvAppearDown, Mogucchi, AppearDown);
    NEW_NERVE(MogucchiNrvDown, Mogucchi, Down);
    NEW_NERVE(MogucchiNrvDive, Mogucchi, Dive);
    NEW_NERVE(MogucchiNrvScatter, Mogucchi, Scatter);
    NEW_NERVE(MogucchiNrvDie, Mogucchi, Die);
}  // namespace

namespace {
    Vec sBodyOffset;
    Vec sHeadOffset;
}  // namespace

Mogucchi::Mogucchi(const char* pName)
    : LiveActor(pName), mHill(nullptr), mHole(nullptr), mGrounded(false), mScatterPropulsionSpeed(0.0f), mScatterNormal(0.0f, 0.0f, 1.0f), _E4(0.0f),
      mStrollSpeed(0.0f), mMaxStrollSpeed(5.0f), mIsStoppedByP2(false) {
    mNewHolePos.identity();
}

Mogucchi::~Mogucchi() {
}

void Mogucchi::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mMaxStrollSpeed);
    initModelManagerWithAnm("Mogucchi", nullptr, false);
    MR::connectToSceneEnemy(this);
    initSensor();
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "PointerTouchManual");
    MR::setEffectHostMtx(this, "PointerTouchManual", MR::getJointMtx(this, "Nose"));
    initSound(4, false);
    initRailRider(rIter);
    MR::declareCoin(this, 1);
    initNerve(&MogucchiNrvStroll::sInstance);
    MR::useStageSwitchWriteDead(this, rIter);

    MR::initStarPointerTargetAtJoint(this, "Head", 83.0f, TVec3f(sHeadOffset));
    createMogucchiHill();
    createHole();
    MR::startBck(this, "Walk", nullptr);
    MR::startBtp(this, "EyeOpen");
    calcAnim();
    makeActorAppeared();
}

void Mogucchi::initAfterPlacement() {
    MR::moveCoordToNearestPos(this, mPosition);
    MR::calcGravityVector(this, MR::getRailPos(this), &mRailGravity, nullptr, nullptr);
    updatePosition();
    updateReferenceMtx();
}

void Mogucchi::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::showModel(this);
    setNerve(&MogucchiNrvStroll::sInstance);
    mHole->makeActorAppeared();
    MR::showModel(mHole);
    MR::validateClipping(this);
}

void Mogucchi::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    setNerve(&MogucchiNrvDie::sInstance);

    if (!MR::isDead(mHole)) {
        mHole->kill();
    }
}

void Mogucchi::control() {
    if (isNerve(&MogucchiNrvScatter::sInstance) || isNerve(&MogucchiNrvDie::sInstance)) {
        return;
    }

    updateReferenceMtx();
}

bool Mogucchi::checkHipDrop() const {
    // !MR::isPlayerHipDropLand() will not match
    if (MR::isPlayerHipDropLand() == false) {
        return false;
    }

    return !(MR::calcDistanceToPlayer(this) > 800.0f);
}

void Mogucchi::exeStroll() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
        MR::startBtp(this, "EyeOpen");
        MR::startBck(mHole, "Walk", nullptr);
        mHill->start();
        _E4 = 0.0f;
        mStrollSpeed = mMaxStrollSpeed;
        mIsStoppedByP2 = false;
    }

    MR::startLevelSound(this, "SE_EM_LV_MOGHILL_MOVE", -1, -1, -1);
    updateStrollSpeed();
    reflectStarPointer2P();
    MR::moveCoord(this, mStrollSpeed);
    MR::calcGravityVector(this, MR::getRailPos(this), &mRailGravity, nullptr, 0);
    updatePosition();
    makeEulerRotation();

    if (Mogucchi::checkHipDrop()) {
        setNerve(&MogucchiNrvAppearDown::sInstance);
    }
}

void Mogucchi::exeAppearDown() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "SwoonStart", nullptr);
        MR::startBtp(this, "EyeClose");
        MR::startBck(mHole, "SwoonStart", nullptr);
        MR::startSound(this, "SE_EM_MOGUCCHI_APPEAR", -1, -1);
        MR::startSound(this, "SE_EV_MOGUCCHI_SWOON", -1, -1);

        // stack swap with the inner TVec3f
        MR::setSensorOffset(this, "body", TVec3f(0.0f, 0.0f, 0.0f));
        MR::setSensorOffset(this, "spin", TVec3f(0.0f, 0.0f, 0.0f));
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
    MR::setNerveAtBckStopped(this, &MogucchiNrvDown::sInstance);
}

void Mogucchi::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon", nullptr);
        MR::startBtp(this, "EyeClose");
        MR::startBck(mHole, "Swoon", nullptr);
    }
    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
    MR::setNerveAtStep(this, &MogucchiNrvDive::sInstance, 30);
}

void Mogucchi::exeDive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hide", nullptr);
        MR::startBtp(this, "EyeOpen");
        MR::startBck(mHole, "Hide", nullptr);
        MR::startSound(this, "SE_EM_MOGUCCHI_HIDE", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&MogucchiNrvStroll::sInstance);
    }
}

void Mogucchi::exeScatter() {
    if (MR::isFirstStep(this)) {
        mHill->end();
        MR::startBck(this, "PunchDown", nullptr);
        MR::startBtp(this, "EyeClose");
        MR::startBck(mHole, "PunchDown", nullptr);
        MR::startBlowHitSound(this);
        mScatterPropulsionSpeed = 50.0f;
        MR::invalidateClipping(this);
    }

    TVec3f* railGravity = &mRailGravity;

    JMAVECScaleAdd(railGravity, &mScatterNormal, &mScatterNormal, -mRailGravity.dot(mScatterNormal));
    MR::normalizeOrZero(&mScatterNormal);

    if (!MR::isNearZero(mScatterNormal)) {
        TVec3f v2;
        PSVECCrossProduct(railGravity, mScatterNormal, &v2);

        TRot3f mtx;
        mtx.setXDirInline(v2);
        mtx.setYDirInline(-mRailGravity);
        mtx.setZDirInline(-mScatterNormal);
        mtx.getEulerXYZ(mRotation);
        mRotation.mult(_180_PI);
    }

    mPosition.add(mRailGravity.scaleInline(-mScatterPropulsionSpeed).addOperatorInLine(mScatterNormal.multInLine(23.0f)));
    mScatterPropulsionSpeed -= 1.2f;

    if (MR::isGreaterEqualStep(this, 15)) {
        setNerve(&MogucchiNrvDie::sInstance);
    }
}

void Mogucchi::exeDie() {
    if (MR::isFirstStep(this)) {
        MR::appearCoinPop(this, mPosition, 1);
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        MR::emitEffect(this, "Death");
        MR::hideModel(this);
        MR::hideModel(mHole);
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        return kill();
    }
}

void Mogucchi::endStroll() {
    MR::deleteEffect(this, "PointerTouchManual");
}

void Mogucchi::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver) && (MR::isSensor(pSender, "body") || MR::isSensor(pSender, "head"))) {
        if (!isNerve(&MogucchiNrvStroll::sInstance) || !MR::isOnGroundPlayer()) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        TVec3f attackDir;
        calcAttackDir(&attackDir, pSender->mPosition, pReceiver->mPosition);
        if (MR::sendMsgEnemyAttackStrongToDir(pReceiver, pSender, attackDir)) {
            MR::shakeCameraNormal();
            return;
        }
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Mogucchi::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, "spin")) {
        return receiveAttackBySpinSensor(msg, pSender, pReceiver);
    }

    return receiveAttackByBodySensor(msg, pSender, pReceiver);
}

void Mogucchi::initSensor() {
    LiveActor::initHitSensor(3);
    MR::addHitSensorAtJointEnemy(this, "head", "Head", 32, 83.0f, TVec3f(sHeadOffset));
    MR::addHitSensorAtJointEnemy(this, "body", "Spine", 32, 83.0f, TVec3f(sBodyOffset));
    MR::addHitSensorEnemy(this, "spin", 16, 180.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

void Mogucchi::createMogucchiHill() {
    s32 railLength = MR::getRailTotalLength(this) / 150.0f;
    if (railLength > 20) {
        railLength = 20;
    }

    // "Mogucchi hill"
    mHill = new MogucchiHill(this, railLength, "モグッチ塚");
    mHill->initWithoutIter();
    if (railLength > 5) {
        mHill->setAppearNum(mHill->_94 - 5);
    }

    MogucchiHill* hill = mHill;
    hill->_D0 = 80.0f;
    hill->_D4 = 100.0f;
}

void Mogucchi::updateStrollSpeed() {
    if (!(mStrollSpeed < mMaxStrollSpeed)) {
        return;
    }

    mStrollSpeed += 0.1f;
    if (mStrollSpeed > mMaxStrollSpeed) {
        mStrollSpeed = mMaxStrollSpeed;
    }
}

void Mogucchi::reflectStarPointer2P() {
    // "Weak"
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        if (!mIsStoppedByP2) {
            mIsStoppedByP2 = true;
            MR::startDPDHitSound();
            MR::emitEffectWithScale(this, "PointerTouchManual", 1.4f, -1);
        }

        mStrollSpeed = 0.0f;
        MR::startDPDFreezeLevelSound(this);
        return;
    }

    if (!mIsStoppedByP2) {
        return;
    }

    mIsStoppedByP2 = false;
    MR::deleteEffect(this, "PointerTouchManual");
}

void Mogucchi::updatePosition() {
    Triangle triangle = Triangle();

    TVec3f v1;
    mGrounded = MR::getFirstPolyOnLineToMap(&v1, &triangle, MR::getRailPos(this), mRailGravity.scaleInline(1000.0f));

    if (mGrounded) {
        mPosition.set(v1);
    } else {
        mPosition.set(MR::getRailPos(this));
    }
}

void Mogucchi::createHole() {
    // "Mogucchi hole"
    mHole = new ModelObj("モグッチ穴", "MogucchiHole", mNewHolePos, 10, -2, -2, false);
    mHole->initWithoutIter();
}

void Mogucchi::calcAttackDir(TVec3f* pDir, const TVec3f& senderPos, const TVec3f& receiverPos) const {
    pDir->sub(receiverPos, senderPos);
    const TVec3f* railGravity = &mRailGravity;
    JMAVECScaleAdd(railGravity, pDir, pDir, -railGravity->dot(*pDir));
    MR::normalizeOrZero(pDir);

    if (MR::isNearZero(*pDir, 0.001f)) {
        pDir->set< f32 >(getBaseMtx()[0][1], getBaseMtx()[1][1], getBaseMtx()[2][1]);
    }

    pDir->add(-mRailGravity);
    MR::normalize(pDir);
}

void Mogucchi::makeEulerRotation() {
    TPos3f mtx;
    MR::makeMtxUpFront(&mtx, -mRailGravity, MR::getRailDirection(this));
    mtx.getEulerXYZ(mRotation);
    mRotation.mult(_180_PI);
}

void Mogucchi::calcScatterVec(const TVec3f& p1, const TVec3f& p2) {
    mScatterNormal.sub(p2, p1);
    const TVec3f* railGravity = &mRailGravity;
    JMAVECScaleAdd(railGravity, mScatterNormal, mScatterNormal, -railGravity->dot(mScatterNormal));
    MR::normalizeOrZero(&mScatterNormal);
}

bool Mogucchi::receiveAttackBySpinSensor(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    if (isNerve(&MogucchiNrvStroll::sInstance)) {
        MR::startSound(this, "SE_EM_MOGUCCHI_REFRECT", -1, -1);
        MR::sendMsgEnemyAttackCounterSpin(pSender, pReceiver);
        return true;
    }

    bool isDown = false;
    if (isNerve(&MogucchiNrvDown::sInstance) || isNerve(&MogucchiNrvAppearDown::sInstance) || isNerve(&MogucchiNrvDive::sInstance)) {
        isDown = true;
    }

    if (!isDown) {
        return false;
    }

    if (isNerve(&MogucchiNrvAppearDown::sInstance) && MR::isLessStep(this, 10)) {
        return false;
    }

    MR::stopScene(8);
    MR::tryRumblePadMiddle(this, 0);
    calcScatterVec(pSender->mPosition, pReceiver->mPosition);
    setNerve(&MogucchiNrvScatter::sInstance);

    return true;
}

bool Mogucchi::receiveAttackByBodySensor(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    if (!isNerve(&MogucchiNrvScatter::sInstance) && !isNerve(&MogucchiNrvDie::sInstance) && MR::isMsgStarPieceAttack(msg)) {
        setNerve(&MogucchiNrvAppearDown::sInstance);
        return true;
    }

    bool isDown = isNerve(&MogucchiNrvDown::sInstance) || isNerve(&MogucchiNrvAppearDown::sInstance) || isNerve(&MogucchiNrvDive::sInstance);

    if (!isDown) {
        if (MR::isMsgPlayerTrample(msg)) {
            MR::startSound(this, "SE_EM_MOGUCCHI_REFRECT", -1, -1);
            return true;
        }

        return false;
    }

    if (isNerve(&MogucchiNrvAppearDown::sInstance) && MR::isLessStep(this, 10)) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        MR::startBck(this, "Bounce", nullptr);
        MR::startBtp(this, "EyeOpen");
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        MR::stopScene(8);
        MR::tryRumblePadMiddle(this, 0);
        calcScatterVec(pSender->mPosition, pReceiver->mPosition);
        setNerve(&MogucchiNrvScatter::sInstance);
        return true;
    }

    return false;
}

void Mogucchi::updateReferenceMtx() {
    TVec3f v1(mRotation);
    // Using PI_180 will mismatch the float value by 1 least significant bit
    v1.scale(57.295776);

    mNewHolePos.makeMatrixFromRotAxesInline(v1.x, v1.y, v1.z);
    mNewHolePos.setTransInline(mPosition);

    mHole->mPosition.set(mPosition);
}
