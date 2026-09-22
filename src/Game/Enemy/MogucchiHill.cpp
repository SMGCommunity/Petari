#include "Game/Enemy/MogucchiHill.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void MogucchiHill_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
    (void)120.0f;
    (void)90000.0f;
    (void)100.0f;
    (void)150.0f;
    (void)0.6f;
    (void)(1.0f - 0.6f);
}

namespace {
    static const char* sPieceModelTable[] = {
        "MogucchiHillA",
        "MogucchiHillB",
        "MogucchiHillC",
    };
    static const f32 sPieceSensorRadius = 120.0f;
    // static const f32 sPieceLargeSensorRadius = _;
    static const f32 sMinScale = 0.6f;
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

MogucchiHillPiece::MogucchiHillPiece(const char* pName) : LiveActor(pName), mHost(), mModelName(), mJointName(), mAutoEffectName(), _CC(), _CD() {
    _9C.identity();
}

void MogucchiHillPiece::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(mModelName, nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    initHitSensor(1);
    MR::addHitSensorAtJointMapObj(this, "body", mJointName, 8, mScale.x * ::sPieceSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));

    if (mAutoEffectName != nullptr) {
        initEffectKeeper(0, mAutoEffectName, false);
    }

    if (mHasLightCtrl) {
        MR::initLightCtrl(this);
    }

    initSound(1, false);
    initNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceWait));
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
    setNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceAppear));
    MR::hideModel(this);

    _CC = false;

    MR::validateClipping(this);
}

void MogucchiHillPiece::initWithModelName(const char* pModelName, const char* pJointName, const char* pAutoEffectName, bool hasLightCtrl) {
    mModelName = pModelName;
    mJointName = pJointName;
    mAutoEffectName = pAutoEffectName;
    mHasLightCtrl = hasLightCtrl;

    initWithoutIter();
}

void MogucchiHillPiece::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Lead");
        MR::startBrk(this, "Normal");
        MR::startSound(this, "SE_EM_MOGHILL_APPEAR");
    }

    MR::setNerveAtBckStopped(this, GET_NERVE_ANON(MogucchiHillPieceNrvPieceWait));
}

void MogucchiHillPiece::exeWait() {
    if (mHost->isNerve(GET_NERVE_ANON(MogucchiHillNrvWait))) {
        return;
    }

    if (!_CD) {
        _CC = true;
    }

    _CD = false;
}

void MogucchiHillPiece::exeCrumble() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fade");
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

    if (isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceCrumble))) {
        return;
    }

    MR::invalidateClipping(this);
    setNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceCrumble));
}

void MogucchiHillPiece::destroy() {
    if (MR::isExistEffectKeeper(this)) {
        MR::emitEffect(this, "Break");
    }

    kill();
}

void MogucchiHillPiece::setSize(f32 size) {
    mScale.setAll< f32 >(size);
    HitSensor* pSensor = getSensor("body");
    pSensor->mRadius = ::sPieceSensorRadius * size;
}

void MogucchiHillPiece::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _9C);
}

void MogucchiHillPiece::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    bool isCrumbleEnd = isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceCrumble)) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (MR::isPlayerDamaging()) {
            destroy();
            return;
        }

        if (isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceAppear)) && mHost->notifyAppearAttackToPlayer(pSender, pReceiver)) {
            return;
        }

        if (isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceWait)) && mHost->notifyWaitAttackToPlayer(pSender, pReceiver)) {
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

    bool isCrumbleEnd = isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceCrumble)) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        destroy();

        return true;
    }

    bool isHitAll = false;

    if (MR::isMsgPlayerHitAll(msg)) {
        isHitAll = true;
    }

    if (isHitAll) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::startSound(this, "SE_EM_MOGHILL_BREAK");
        mHost->killPieces(this);

        return true;
    }

    return false;
}

bool MogucchiHillPiece::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isCrumbleEnd = isNerve(GET_NERVE_ANON(MogucchiHillPieceNrvPieceCrumble)) && MR::isGreaterEqualStep(this, ::sCrumbleFrame);

    if (isCrumbleEnd) {
        return false;
    }

    _CD = true;

    if (!_CC) {
        return false;
    }

    MR::tryRumblePadWeak(this, WPAD_CHAN0);
    MR::startSound(this, "SE_EM_MOGHILL_BREAK_SOFT");
    destroy();

    return true;
}

bool MogucchiHillPiece::isTargetGoingAway(HitSensor* pSender, HitSensor* pReceiver) const NO_INLINE {
    TVec3f toReceiver = pReceiver->mPosition - pSender->mPosition;

    return toReceiver.dot(pReceiver->mHost->mVelocity) >= 0.0f;
}

MogucchiHill::MogucchiHill(LiveActor* pHost, s32 pieceCount, const char* pName)
    : LiveActor(pName), mHost(pHost), _90(), _94(pieceCount), _98(), _CC(), _D0(100.0f), _D4(150.0f), _D8(3), mModelNameTable(::sPieceModelTable),
      mJointName("MogucchiHill"), mAutoEffectName("MogucchiHill"), mHasLightCtrl(), _EC(), _F0(), mAppearNum() {
    if (pieceCount > 100) {
        _94 = 100;
    }

    mAppearNum = _94;

    mReserveMtx.identity();
}

void MogucchiHill::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    initNerve(GET_NERVE_ANON(MogucchiHillNrvMove));
    createPieces();
    makeActorDead();
}

void MogucchiHill::start() {
    if (MR::isDead(this)) {
        MR::invalidateClipping(this);
        appear();
    }

    setNerve(GET_NERVE_ANON(MogucchiHillNrvMove));

    _F0 = 3;
}

void MogucchiHill::end() {
    setNerve(GET_NERVE_ANON(MogucchiHillNrvWait));
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
        index = (index + _94 + 1) % _94;

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

    s32 modelCount = _D8;
    s32 previousModel = MR::getRandom(0, modelCount);
    s32 secondPreviousModel = -2;
    s32 model;
    s32 thirdPreviousModel = -3;

    for (u32 i = 0; i < _94; i++) {
        s32 offset = MR::getRandom(0, modelCount - 1);
        model = previousModel + offset + 1;
        model = (model + modelCount) % modelCount;

        if (previousModel == thirdPreviousModel && secondPreviousModel == model) {
            s32 alternativeCount = modelCount - 1;
            offset = (offset + alternativeCount + 1) % alternativeCount;
            model = previousModel + offset + 1;
            model = (model + modelCount) % modelCount;
        }

        thirdPreviousModel = secondPreviousModel;
        secondPreviousModel = previousModel;
        previousModel = model;

        _90[i].mHost = this;
        _90[i].mScale.setAll< f32 >(mScale.x);
        _90[i].initWithModelName(mModelNameTable[model], mJointName, mAutoEffectName, mHasLightCtrl);
    }
}

void MogucchiHill::appearPiece() {
    killPieceOverAppear();
    killPieceIfAlive();

    switch (_EC) {
    case 0:
        break;
    case 1:
        _F0++;

        if (_F0 >= 3) {
            _F0 = 3;
            _EC = 0;
        }

        break;
    case 2:
        _F0--;

        if (_F0 <= 0) {
            _F0 = 0;
            setNerve(GET_NERVE_ANON(MogucchiHillNrvWait));
            _EC = 0;
            return;
        }

        break;
    }

    if (_F0 != 0) {
        _90[_98].setSize(mScale.x * (::sMinScale + (1.0f - ::sMinScale) * (_F0 - 1) / 2.0f));
        MogucchiHillPiece* pPiece = &_90[_98];
        pPiece->_9C.setInline(mReserveMtx);
        pPiece->_9C.getTrans(pPiece->mPosition);
        pPiece->appear();
        s32 count = _94;
        s32 index = _98;
        _CC = false;
        _98 = (index + count + 1) % count;
    }
}

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

f32 MogucchiHill::getDistanceFromReserveMtx() const NO_INLINE {
    TVec3f trans;
    mReserveMtx.getTrans(trans);

    return mHost->mPosition.distance(trans);
}

void MogucchiHill::reserveCurrentMtx() {
    mReserveMtx.setInline(mHost->getBaseMtx());
    _CC = true;
}
