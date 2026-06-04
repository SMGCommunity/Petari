#include "Game/Enemy/MogucchiHill.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const char* sPieceModelTable[] = {
        "MogucchiHillA",
        "MogucchiHillB",
        "MogucchiHillC",
    };
    static const f32 sPieceSensorRadius = 120.0f;
    // static const f32 sPieceLargeSensorRadius = _;
    // static const f32 sMinScale = _;
    static const s32 sCrumbleFrame = 40;
    // static const s32 sPieceInterval = _;
    static const f32 sAppearDist = 300.0f;
    // static const s32 sNaturallyNum = _;
};  // namespace

namespace {
    NEW_NERVE(MogucchiHillPieceNrvPieceAppear, MogucchiHillPiece, Appear);
    NEW_NERVE(MogucchiHillPieceNrvPieceWait, MogucchiHillPiece, Wait);
    NEW_NERVE(MogucchiHillPieceNrvPieceCrumble, MogucchiHillPiece, Crumble);
};  // namespace

namespace {
    NEW_NERVE(MogucchiHillNrvMove, MogucchiHill, Move);
    NEW_NERVE(MogucchiHillNrvWait, MogucchiHill, Wait);
};  // namespace

MogucchiHillPiece::MogucchiHillPiece(const char* pName) : LiveActor(pName), mHost(), _90(), _94(), _98(), _CC(), _CD() {
    _9C.identity();
}

void MogucchiHillPiece::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(_90, nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    initHitSensor(1);
    MR::addHitSensorAtJointMapObj(this, "body", _94, 8, mScale.x * ::sPieceSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));

    if (_98 != nullptr) {
        initEffectKeeper(0, _98, false);
    }

    if (_CE) {
        MR::initLightCtrl(this);
    }

    initSound(1, false);
    initNerve(&::MogucchiHillPieceNrvPieceWait::sInstance);
    makeActorDead();
}

void MogucchiHillPiece::control() {
    f32 sqrDist = (mPosition - *MR::getPlayerCenterPos()).squared();

    if (MR::isPlayerHipDropLand() && sqrDist < ::sAppearDist * ::sAppearDist) {
        if (MR::isExistEffectKeeper(this)) {
            MR::emitEffect(this, "Break");
        }

        kill();
    }
}

void MogucchiHillPiece::appear() {
    LiveActor::appear();
    setNerve(&::MogucchiHillPieceNrvPieceAppear::sInstance);
    MR::hideModel(this);

    _CC = false;

    MR::validateClipping(this);
}

void MogucchiHillPiece::initWithModelName(const char* pParam1, const char* pParam2, const char* pParam3, bool param4) {
    _90 = pParam1;
    _94 = pParam2;
    _98 = pParam3;
    _CE = param4;

    initWithoutIter();
}

void MogucchiHillPiece::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Lead", nullptr);
        MR::startBrk(this, "Normal");
        MR::startSound(this, "SE_EM_MOGHILL_APPEAR");
    }

    MR::setNerveAtBckStopped(this, &::MogucchiHillPieceNrvPieceWait::sInstance);
}

void MogucchiHillPiece::exeWait() {
    if (mHost->isNerve(&::MogucchiHillNrvWait::sInstance)) {
        return;
    }

    if (!_CD) {
        _CC = true;
    }

    _CD = false;
}

void MogucchiHillPiece::exeCrumble() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fade", nullptr);
        MR::startBrk(this, "Fade");
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void MogucchiHillPiece::crumble() {
    if (MR::isDead(this)) {
        return;
    }

    if (MR::isClipped(this)) {
        kill();
        return;
    }

    if (isNerve(&::MogucchiHillPieceNrvPieceCrumble::sInstance)) {
        return;
    }

    MR::invalidateClipping(this);
    setNerve(&::MogucchiHillPieceNrvPieceCrumble::sInstance);
}

void MogucchiHillPiece::destroy() {
    if (MR::isExistEffectKeeper(this)) {
        MR::emitEffect(this, "Break");
    }

    kill();
}

void MogucchiHillPiece::setSize(f32 size) {
    mScale.setAll< f32 >(size);
    getSensor("body")->mRadius = size * ::sPieceSensorRadius;
}

void MogucchiHillPiece::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _9C);
}

void MogucchiHillPiece::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    bool isCrumbleEnd = isNerve(&::MogucchiHillPieceNrvPieceCrumble::sInstance) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (MR::isPlayerDamaging()) {
            destroy();
            return;
        }

        if (isNerve(&::MogucchiHillPieceNrvPieceAppear::sInstance) && mHost->notifyAppearAttackToPlayer(pSender, pReceiver)) {
            return;
        }

        if (isNerve(&::MogucchiHillPieceNrvPieceWait::sInstance) && mHost->notifyWaitAttackToPlayer(pSender, pReceiver)) {
            return;
        }

        if (MR::isPlayerHipDropFalling()) {
            return;
        }

        if (isTargetGoingAway(pSender, pReceiver)) {
            return;
        }
    }

    MR::sendMsgPush(pReceiver, pSender);
}

bool MogucchiHillPiece::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isDead(this)) {
        return false;
    }

    bool isCrumbleEnd = isNerve(&::MogucchiHillPieceNrvPieceCrumble::sInstance) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumblePadWeak(this, 0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::tryRumblePadWeak(this, 0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        MR::tryRumblePadMiddle(this, 0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        mHost->killPieces(this);

        return true;
    }

    return false;
}

bool MogucchiHillPiece::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isCrumbleEnd = isNerve(&::MogucchiHillPieceNrvPieceCrumble::sInstance) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return false;
    }

    _CD = true;

    if (!_CC) {
        return false;
    }

    MR::tryRumblePadWeak(this, 0);
    MR::startSound(this, "SE_EM_MOGHILL_BREAK_SOFT");
    destroy();

    return true;
}

bool MogucchiHillPiece::isTargetGoingAway(HitSensor* pSensor1, HitSensor* pSensor2) const {
    TVec3f deltaTrans = pSensor2->mPosition - pSensor1->mPosition;
    f32 dotP = deltaTrans.dot(pSensor2->mHost->mVelocity);

    return dotP >= 0.0f;
}

MogucchiHill::MogucchiHill(LiveActor* pHost, s32 param2, const char* pName)
    : LiveActor(pName), mHost(pHost), _90(), _94(param2), _98(), _CC(), _D0(100.0f), _D4(150.0f), _D8(3), _DC(::sPieceModelTable), _E0("MogucchiHil"),
      _E4("MogucchiHil"), _E8(), _EC(), _F0(), mAppearNum() {
    if (param2 > 100) {
        _94 = 100;
    }

    mAppearNum = _94;

    mReserveMtx.identity();
}

void MogucchiHill::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    initNerve(&::MogucchiHillNrvMove::sInstance);
    createPieces();
    makeActorDead();
}

void MogucchiHill::start() {
    if (MR::isDead(this)) {
        MR::invalidateClipping(this);
        appear();
    }

    setNerve(&::MogucchiHillNrvMove::sInstance);

    _F0 = 3;
}

void MogucchiHill::end() {
    setNerve(&::MogucchiHillNrvWait::sInstance);
}

void MogucchiHill::startNaturally() {
    _EC = 1;

    start();
}

void MogucchiHill::endNaturally() {
    _EC = 2;
}

void MogucchiHill::killPieces(MogucchiHillPiece* pIter) {
    s32 index = pIter - _90 - 2;

    for (s32 i = 0; i < 3; i++) {
        index = (_94 + index + 1) % index;

        if (MR::isDead(&_90[index])) {
            continue;
        }

        _90[index].destroy();
    }
}

void MogucchiHill::pauseOff() {
    MR::requestMovementOn(this);

    for (u32 i = 0; i < _94; i++) {
        MR::requestMovementOn(&_90[i]);
    }
}

void MogucchiHill::killAllPieces() {
    for (u32 i = 0; i < _94; i++) {
        _90[i].makeActorDead();
    }
}

void MogucchiHill::setAppearNum(s32 appearNum) {
    mAppearNum = appearNum;
}

void MogucchiHill::exeMove() {
    if (MR::isFirstStep(this)) {
        reserveCurrentMtx();
    }

    if (_CC && getDistanceFromReserveMtx() >= _D0) {
        appearPiece();
    }

    if (getDistanceFromReserveMtx() >= _D4) {
        reserveCurrentMtx();
    }
}

void MogucchiHill::exeWait() {
    _F0 = 0;
}

void MogucchiHill::createPieces() {
    _90 = new MogucchiHillPiece[_94]();

    for (u32 i = 0; i < _94; i++) {
        _90[i].mHost = this;
        _90[i].mScale.setAll< f32 >(mPosition.x);
        // _90[i].initWithModelName(_DC[..], _E0, _E4, _E8);
    }
}

// MogucchiHill::appearPiece

void MogucchiHill::killPieceOverAppear() {
    s32 index = (_94 + (_98 - mAppearNum)) % _94;

    _90[index].crumble();
}

void MogucchiHill::killPieceIfAlive() {
    if (MR::isDead(&_90[_98])) {
        return;
    }

    _90[_98].kill();
}

f32 MogucchiHill::getDistanceFromReserveMtx() const {
    TVec3f trans;
    mReserveMtx.getTrans(trans);

    return mHost->mPosition.distance(trans);
}

void MogucchiHill::reserveCurrentMtx() {
    mReserveMtx.setInline(getBaseMtx());
    _CC = true;
}
