#include "Game/Enemy/Mogucchi.hpp"
#include "Game/Enemy/MogucchiHill.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
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
    TVec3f sHeadOffset;
}

Mogucchi::Mogucchi(const char* pName)
    : LiveActor(pName), mHill(nullptr), _90(nullptr), _D0(false), _D4(0.0f), _D8(0.0f, 0.0f, 1.0f), _E4(0.0f), _E8(0.0f), _EC(5.0f), _F0(false) {
    _94.identity();
}

Mogucchi::~Mogucchi() {
}

void Mogucchi::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_EC);
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
    // TODO: figure out _C4
    MR::calcGravityVector(this, MR::getRailPos(this), &mGravity, nullptr, nullptr);
    updatePosition();
    updateReferenceMtx();
}

void Mogucchi::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::showModel(this);
    setNerve(&MogucchiNrvStroll::sInstance);
    _90->makeActorAppeared();
    MR::showModel(_90);
    MR::validateClipping(this);
}

void Mogucchi::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    setNerve(&MogucchiNrvDie::sInstance);

    if (!MR::isDead(_90)) {
        _90->kill();
    }
}

void Mogucchi::control() {
    // TODO: figure out inline
    if (!isNerve(&MogucchiNrvScatter::sInstance) && isNerve(&MogucchiNrvDie::sInstance)) {
        updateReferenceMtx();
    }
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
        MR::startBck(_90, "Walk", nullptr);
        mHill->start();
        _E4 = 0.0f;
        _E8 = _EC;
        _F0 = false;
    }

    MR::startLevelSound(this, "SE_EM_LV_MOGHILL_MOVE", -1, -1, -1);
    updateStrollSpeed();
    reflectStarPointer2P();
    MR::moveCoord(this, _E8);
    MR::calcGravityVector(this, MR::getRailPos(this), &_C4, nullptr, 0);
    updatePosition();
    makeEulerRotation();

    if (Mogucchi::checkHipDrop()) {
        setNerve(&MogucchiNrvAppearDown::sInstance);
    }
}

void Mogucchi::exeDie() {
    if (MR::isFirstStep(this)) {
        MR::appearCoinPop(this, mPosition, 1);
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        MR::emitEffect(this, "Death");
        MR::hideModel(this);
        MR::hideModel(_90);
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        return kill();
    }
}

void Mogucchi::exeDive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
        MR::startBtp(this, "EyeOpen");
        MR::startBck(_90, "Walk", nullptr);
        MR::startSound(this, "SE_EM_MOGUCCHI_HIDE", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&MogucchiNrvStroll::sInstance);
    }
}

void Mogucchi::exeAppearDown() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "SwoonStart", nullptr);
        MR::startBtp(this, "EyeClose");
        MR::startBck(_90, "SwoonStart", nullptr);
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
        MR::startBck(_90, "Swoon", nullptr);
    }
    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
    MR::setNerveAtStep(this, &MogucchiNrvDive::sInstance, 30);
}

void Mogucchi::endStroll() {
    MR::deleteEffect(this, "PointerTouchManual");
}

void Mogucchi::exeScatter() {
    if (MR::isFirstStep(this)) {
        mHill->end();
        MR::startBck(this, "PunchDown", nullptr);
        MR::startBtp(this, "EyeClose");
        MR::startBck(_90, "PunchDown", nullptr);
        MR::startBlowHitSound(this);
        _D4 = 50.0f;
        MR::invalidateClipping(this);
    }

    TVec3f* c4 = &_C4;

    JMAVECScaleAdd(c4, &_D8, &_D8, -_C4.dot(_D8));
    MR::normalizeOrZero(&_D8);

    if (!MR::isNearZero(_D8)) {
        TVec3f v2;
        PSVECCrossProduct(c4, _D8, &v2);

        TRot3f mtx;
        mtx.setXDirInline(v2);
        mtx.setYDirInline(-_C4);
        mtx.setZDirInline(-_D8);
        mtx.getEulerXYZ(mRotation);
        mRotation.mult(_180_PI);
    }

    mPosition.add(_C4.scaleInline(-_D4).addOperatorInLine(_D8.multInLine(23.0f)));
    _D4 -= 1.2f;

    if (MR::isGreaterEqualStep(this, 15)) {
        setNerve(&MogucchiNrvDie::sInstance);
    }
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
}
