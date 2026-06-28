#include "Game/Enemy/OtaRock.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/Enemy/FireBall.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/CocoNut.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

namespace {
    const Vec cSensorOffset = {0.0f, 0.0f, 0.0f};
};  // namespace

namespace NrvOtaRock {
    NEW_NERVE(OtaRockNrvWait, OtaRock, Wait);
    NEW_NERVE(OtaRockNrvThrowCocoNut, OtaRock, ThrowCocoNut);
    NEW_NERVE(OtaRockNrvThrowFireBall, OtaRock, ThrowFireBall);
    NEW_NERVE(OtaRockNrvDown, OtaRock, Down);
    NEW_NERVE(OtaRockNrvHide, OtaRock, Hide);
    NEW_NERVE(OtaRockNrvHideWait, OtaRock, HideWait);
    NEW_NERVE(OtaRockNrvShow, OtaRock, Show);
};  // namespace NrvOtaRock

OtaRock::OtaRock(const char* pName)
    : LiveActor(pName), _8C(false), mCocoNutArray(nullptr), mFireBallArray(nullptr), mFixedPosition(nullptr), _9C(nullptr), _A0(0), _A4(),
      _D4(0.0f, 0.0f, 1.0f), mAnimScaleController(nullptr) {
    _A4.identity();
}
OtaRock::~OtaRock() {
}

void OtaRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_8C);
    MR::calcGravity(this);
    initModel();
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(0, nullptr, false);
    initSound(8, false);
    MR::addToAttributeGroupSearchTurtle(this);
    mAnimScaleController = new AnimScaleController(nullptr);
    initNerve(&NrvOtaRock::OtaRockNrvWait::sInstance);
}

void OtaRock::appear() {
    LiveActor::appear();
    if (_8C) {
        setNerve(&NrvOtaRock::OtaRockNrvWait::sInstance);
    } else {
        setNerve(&NrvOtaRock::OtaRockNrvHideWait::sInstance);
    }
}

void OtaRock::kill() {
    MR::emitEffect(this, "OtaRockDeath");
    MR::startSound(this, "SE_EM_OTAROCK_DIE");
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    LiveActor::kill();
}

void OtaRock::control() {
    mAnimScaleController->updateNerve();
}

void OtaRock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A4);
    TVec3f scale;
    JMathInlineVEC::PSVECMultiply(&mAnimScaleController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

void OtaRock::initSensor() {
    initHitSensor(1);
    MR::addHitSensorMtxEnemy(this, "body", 8, 300.0f, MR::getJointMtx(this, "body"), static_cast< TVec3f >(::cSensorOffset));
}

void OtaRock::updateBaseMtx() {
    TVec3f up = mGravity.negateInline();
    TVec3f front;
    if (MR::isStageStateScenarioOpeningCamera()) {
        front.set(_D4);
    } else {
        front.sub(*MR::getPlayerPos(), mPosition);
        MR::normalize(&front);
    }
    MR::makeMtxUpFrontPos(&_A4, up, front, mPosition);
}

CocoNutBall* OtaRock::getDisappearedCocoNut() {
    for (s32 i = 0; i < 4; i++) {
        if (MR::isDead(mCocoNutArray[i])) {
            return mCocoNutArray[i];
        }
    }
    return nullptr;
}

FireBall* OtaRock::getDisappearedFireBall() {
    for (s32 i = 0; i < 3; i++) {
        if (MR::isDead(mFireBallArray[i])) {
            return mFireBallArray[i];
        }
    }
    return nullptr;
}

s32 OtaRock::getDisappearedCocoNutNum() const {
    for (s32 i = 0; i < 4; i++) {
        if (MR::isDead(mCocoNutArray[i])) {
            return i;
        }
    }
    return 0;
}

s32 OtaRock::getDisappearedFireBallNum() const {
    for (s32 i = 0; i < 3; i++) {
        if (MR::isDead(mFireBallArray[i])) {
            return i;
        }
    }
    return 0;
}

void OtaRock::throwCocoNut() {
    CocoNutBall* pCocoNut = OtaRock::getDisappearedCocoNut();
    mFixedPosition->calc();
    mFixedPosition->copyTrans(&pCocoNut->mPosition);
    f32 f = MR::getRandom(5.0f, 10.0f);
    pCocoNut->appearAndThrow(mFixedPosition->mLocalTrans, MR::isHalfProbability() ? 5.0f : -5.0f);
}

void OtaRock::throwFireBall() {
    FireBall* fireBall = getDisappearedFireBall();
    mFixedPosition->calc();
    mFixedPosition->copyTrans(&fireBall->mPosition);
    fireBall->appearAndThrow(fireBall->mPosition, 15.0f, 0.0f);
}

bool OtaRock::tryToHide() {
    bool canHide;

    if (MR::isValidSwitchA(this)) {
        canHide = MR::isOnSwitchA(this);
    } else {
        canHide = MR::isNearPlayer(this, 4000.0f);
    }

    if (_8C || getDisappearedCocoNutNum() != 4 || canHide) {
        setNerve(&NrvOtaRock::OtaRockNrvHide::sInstance);
        return true;
    }
    return false;
}

void OtaRock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            mAnimScaleController->startHitReaction();
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool OtaRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        mAnimScaleController->startHitReaction();
        return true;
    } else if (MR::isMsgJetTurtleAttack(msg)) {
        setNerve(&NrvOtaRock::OtaRockNrvDown::sInstance);
        return true;
    }
    return true;
}

bool OtaRock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    setNerve(&NrvOtaRock::OtaRockNrvDown::sInstance);
    return true;
}

// void OtaRock::initModel() {}

void OtaRock::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        if (_8C) {
            _A0 = MR::getRandom((s32)120, (s32)180);
        } else {
            _A0 = MR::getRandom((s32)30, (s32)90);
        }
    }
    updateBaseMtx();
    if (tryToHide() && MR::isGreaterStep(this, _A0)) {
        if (!_8C && _9C == 2) {
            if (getDisappearedCocoNutNum() != 0) {
                setNerve(&NrvOtaRock::OtaRockNrvThrowCocoNut::sInstance);
            }
        } else {
            if (getDisappearedFireBallNum() != 1) {
                _9C = 0;
                setNerve(&NrvOtaRock::OtaRockNrvThrowFireBall::sInstance);
            }
        }
    }
}

void OtaRock::exeThrowCocoNut() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EM_OTAROCK_ATTACK");
    }
    OtaRock::updateBaseMtx();
    if (MR::isStep(this, 40)) {
        throwCocoNut();
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvOtaRock::OtaRockNrvWait::sInstance);
    }
}

void OtaRock::exeThrowFireBall() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EM_OTAROCK_ATTACK");
    }
    OtaRock::updateBaseMtx();
    if (MR::isStep(this, 40)) {
        MR::startSound(this, "SE_EM_OTAROCK_ATTAC");
        throwFireBall();
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvOtaRock::OtaRockNrvWait::sInstance);
    }
}

void OtaRock::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "OtaRockDeath");
        MR::startSound(this, "SE_EM_OTAROCK_DIE");
        MR::startSound(this, "SE_EM_OTAROCK_DIE");
        MR::invalidateHitSensors(this);
        MR::tryRumblePadVeryStrong(this, 0);

        if (_8C) {
            for (s32 i = 0; i < 4; i++) {
                CocoNutBall* pCocoNut = mCocoNutArray[i];
                if (!MR::isDead(pCocoNut))
                    pCocoNut->kill();
            }
            for (s32 i = 0; i < 3; i++) {
                FireBall* pFireBall = mFireBallArray[i];
                if (!MR::isDead(pFireBall))
                    pFireBall->kill();
            }
        }
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(16);
    }
    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormal();
    }
    if (MR::isBckStopped(this)) {
        kill();
    }
}

void OtaRock::exeHideWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "HideWait");
    }
    updateBaseMtx();
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this) || MR::isNearPlayer(this, 3800.0f)) {
        setNerve(&NrvOtaRock::OtaRockNrvShow::sInstance);
    }
}

void OtaRock::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hide", nullptr);
        MR::startSound(this, "SE_EM_OTAROCK_HIDE");
    }
    updateBaseMtx();
    MR::setNerveAtBckStopped(this, &NrvOtaRock::OtaRockNrvHideWait::sInstance);
}

void OtaRock::exeShow() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Show");
        MR::startSound(this, "SE_EM_OTAROCK_APPEAR");
    }
    updateBaseMtx();
    MR::setNerveAtBckStopped(this, &NrvOtaRock::OtaRockNrvWait::sInstance);
}

void OtaRock::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool noCocoNut = false;
    MR::getJMapInfoArg0NoInit(rIter, &noCocoNut);

    if (!noCocoNut) {
        pArchiveList->addArchive(CocoNut::getModelName());
    }
}
