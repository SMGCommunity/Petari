#include "Game/Enemy/Hanachan.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvHanachan {
    NEW_NERVE(HanachanPartsNrvWalk, HanachanParts, Walk);
    NEW_NERVE(HanachanPartsNrvBecomeAngry, HanachanParts, BecomeAngry);
    NEW_NERVE(HanachanPartsNrvAngryPursue, HanachanParts, AngryPursue);
    NEW_NERVE(HanachanPartsNrvAngryEnd, HanachanParts, AngryEnd);
    NEW_NERVE(HanachanPartsNrvWallHitEnd, HanachanParts, WallHitEnd);
    NEW_NERVE(HanachanPartsNrvTrample, HanachanParts, Trample);
    NEW_NERVE(HanachanPartsNrvOverturn, HanachanParts, Overturn);
    NEW_NERVE(HanachanPartsNrvOverturnBound, HanachanParts, OverturnBound);
    NEW_NERVE(HanachanPartsNrvOverturnWait, HanachanParts, OverturnWait);
    NEW_NERVE(HanachanPartsNrvRecover, HanachanParts, Recover);
    NEW_NERVE(HanachanPartsNrvHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvOverturnHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvBlow, HanachanParts, Blow);
    NEW_NERVE(HanachanPartsNrvStarPointerBind, HanachanParts, StarPointerBind);

    NEW_NERVE(HanachanNrvHanachanWalk, Hanachan, Walk);
    NEW_NERVE(HanachanNrvHanachanBecomeAngry, Hanachan, BecomeAngry);
    NEW_NERVE(HanachanNrvHanachanAngryPursue, Hanachan, AngryPursue);
    NEW_NERVE(HanachanNrvHanachanAngryEnd, Hanachan, AngryEnd);
    NEW_NERVE(HanachanNrvHanachanWallHitEnd, Hanachan, WallHitEnd);
    NEW_NERVE(HanachanNrvHanachanTrample, Hanachan, Trample);
    NEW_NERVE(HanachanNrvHanachanOverturn, Hanachan, Overturn);
    NEW_NERVE(HanachanNrvHanachanOverturnBound, Hanachan, OverturnBound);
    NEW_NERVE(HanachanNrvHanachanOverturnWait, Hanachan, OverturnWait);
    NEW_NERVE(HanachanNrvHanachanRecover, Hanachan, Recover);
    NEW_NERVE(HanachanNrvHanachanHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanOverturnHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanBlow, Hanachan, Blow);
    NEW_NERVE(HanachanNrvHanachanStarPointerBind, Hanachan, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEnd, Hanachan, WallHitEnd);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindOverturn, Hanachan, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEndOverturn, Hanachan, WallHitEnd);
};  // namespace NrvHanachan

HanachanParts::HanachanParts(Hanachan* pParent, s32 a2, const char* pType, const char* pName) : LiveActor(pType) {
    mParent = pParent;
    _90.set(0.0f);
    _9C.set(0.0f);
    _A8 = 1;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 0.0f;
    _AC.w = 1.0f;
    _BC = a2;
    _C0 = -1;
    _C4 = 0;

    initModelManagerWithAnm(pName, nullptr, false);

    if (MR::isEqualString(pName, "HanachanBody")) {
        _A8 = 1;
    } else if (MR::isEqualString(pName, "HanachanBodyS")) {
        _A8 = 2;
    } else if (MR::isEqualString(pName, "HanachanHead")) {
        _A8 = 0;
    }
}

void HanachanParts::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, 0xB, 0x12, -1);
    initNerve(GET_NERVE(Hanachan, HanachanPartsNrvWalk));
    initHitSensor(1);

    if (_A8 == 1) {
        MR::addHitSensorEnemy(this, "body", 0x20, 85.0f, TVec3f(0.0f, 100.0f, 0.0f));
    } else if (_A8 == 2) {
        MR::addHitSensorEnemy(this, "body", 0x20, 85.0f, TVec3f(0.0f, 85.0f, 0.0f));
    } else if (_A8 == 0) {
        MR::addHitSensorEnemy(this, "body", 0x20, 85.0f, TVec3f(0.0f, 100.0f, 0.0f));
    }

    initBinder(100.0f, 100.0f, 0);

    initEffectKeeper(1, _A8 == 0 ? "HanachanHead" : "HanachanBody", false);
    MR::addEffect(this, "PointerTouch");
    MR::initLightCtrl(this);
    initSound(2, false);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 70.0f);
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::startBrk(this, "Normal");

    if (_A8 == 0) {
        MR::startBva(this, "normal");
    }
}

const TVec3f* HanachanParts::getCommonGravity() const {
    if (mParent->isNerve(GET_NERVE(Hanachan, HanachanNrvHanachanTrample)) || mParent->isNerve(GET_NERVE(Hanachan, HanachanNrvHanachanBecomeAngry)) ||
        mParent->isNerve(GET_NERVE(Hanachan, HanachanNrvHanachanOverturn)) ||
        mParent->isNerve(GET_NERVE(Hanachan, HanachanNrvHanachanOverturnBound)) ||
        mParent->isNerve(GET_NERVE(Hanachan, HanachanNrvHanachanRecover))) {
        return &mParent->mBodyParts[2]->mGravity;
    }

    return &mGravity;
}

void HanachanParts::kill() {
    LiveActor::kill();

    MR::emitEffect(this, _A8 == 0 ? "HeadDeath" : "BodyDeath");

    if (_A8 <= 1) {
        MR::appearStarPiece(mParent, mPosition, 2, 10.0f, 40.0f, false);
        MR::startSound(mParent, "SE_OJ_STAR_PIECE_BURST");
    }
}

// HanachanParts::attackSensor
// HanachanParts::receiveMsgPlayerAttack

bool HanachanParts::receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*) {
    return true;
}

bool HanachanParts::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return false;
    }

    TVec3f v13(pReceiver->mPosition);
    v13 -= pSender->mPosition;

    f32 length;

    if (!mParent->isOwnSensor(pSender)) {
        length = (((pReceiver->mRadius) + (pSender->mRadius)) - v13.length());

    } else {
        length = (((pReceiver->mRadius / 2.0f) + (pSender->mRadius / 2.0f)) - v13.length());

        if (length < 0.0f) {
            return false;
        }
    }

    v13.setLength(0.25f * length);
    _90.add(v13);
    return true;
}

void HanachanParts::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.makeQuat(getRot());
    mtx.setTrans(mPosition);

    MR::setBaseTRMtx(this, mtx);
    MR::setBaseScale(this, mParent->mScaleCtrl->_C.mult(mScale));
}

void HanachanParts::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
        f32 max = MR::getBckFrameMax(this, "Walk");
        s32 v = 8 * _BC;
        s32 s = (s32)(max - 1 + v) % (s32)(max - 1);
        MR::setBckFrameAndStop(this, v);
        MR::setBckRate(this, 1.0f);
    }

    mVelocity.set< f32 >(_9C);
    mVelocity.add(_90);

    if (MR::isOnGround(this)) {
        _9C.zero();
    } else {
        TVec3f v7(*getCommonGravity());
        v7.scale(0.5f);
        _9C.add(v7);
        _9C.x *= 0.98f;
        _9C.y *= 0.98f;
        _9C.z *= 0.98f;
    }
}

Hanachan::Hanachan(const char* pName) : LiveActor(pName) {
}
Hanachan::~Hanachan() {
}
