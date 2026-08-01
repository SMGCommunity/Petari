#include "Game/MapObj/Sandstorm.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static const f32 sRailSpeed = 3.0f;
    static const f32 sTurnRate = 2.0f;
    static const f32 sActiveRadius = 7000.0f;
    // static const f32 sSpiralInterpoleStep = _;
    // static const f32 sFixUpDownLength = _;
    // static const f32 sFixUpDownSpeed = _;
    static const s32 sStepToEnableSpin = 15;
    static const f32 sTrapOffsetY = 200.0f;
    static const f32 sTrapOffsetZ = 220.0f;
    static const f32 sTrapRotateSpeed = 4.0f;
    // static const f32 sTrapBoundSpeed = _;
    // static const f32 sTrapSpinSpeed = _;
    // static const f32 sTrapOrbitAmpMax = _;
    static const char* const sParamFileName = "SandstormParam.bcsv";
};  // namespace

namespace NrvSunakazeKunTrap {
    NEW_NERVE(SunakazeKunTrapNrvTrap, SunakazeKunTrap, Trap);
};  // namespace NrvSunakazeKunTrap

SunakazeKunTrap::SunakazeKunTrap(LiveActor* pHost, const TVec3f& rParam2)
    : ModelObj("スナカゼ用トラップ", "SunakazeKunTrap", nullptr, -2, -2, -2, false), mHost(pHost), _94(rParam2) {
}

void SunakazeKunTrap::init(const JMapInfoIter& rIter) {
    initHitSensor(1);
    MR::addHitSensorEnemyAttack(this, "attack", 4, 40.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initShadowVolumeCylinder(this, 40.0f);

    TVec3f up;
    MR::calcUpVec(&up, mHost);
    up.negate();

    MR::setShadowDropDirection(this, nullptr, up);
    MR::setShadowDropLength(this, nullptr, 500.0f);
    PSMTXMultVec(mHost->getBaseMtx(), &_94, &mPosition);
    MR::invalidateClipping(this);
    initNerve(&NrvSunakazeKunTrap::SunakazeKunTrapNrvTrap::sInstance);
    makeActorAppeared();
}

void SunakazeKunTrap::exeTrap() {
    TVec3f axis = TVec3f(0.0f, 1.0f, 0.0f);

    MR::rotateVecDegree(&_94, axis, ::sTrapRotateSpeed);
    PSMTXMultVec(mHost->getBaseMtx(), &_94, &mPosition);
    f32 value = MR::getEaseInValue(getNerveStep() % 100, 0.5f, 1.0f, 30.0f);
    f32 angle = MR::sinDegree(getNerveStep() * 5.0f);

    mPosition.scaleAdd(value * angle * 40.0f, axis, mPosition);
    mRotation += TVec3f(5.0f);
}

void SunakazeKunTrap::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
    }
}

namespace NrvSandstorm {
    NEW_NERVE(SandstormNrvNonActive, Sandstorm, NonActive);
    NEW_NERVE(SandstormNrvWait, Sandstorm, Wait);
    NEW_NERVE(SandstormNrvStorm, Sandstorm, Storm);
    NEW_NERVE(SandstormNrvStormAdjust, Sandstorm, StormAdjust);
    NEW_NERVE(SandstormNrvStormFix, Sandstorm, StormFix);
    NEW_NERVE(SandstormNrvStormEnd, Sandstorm, StormEnd);
};  // namespace NrvSandstorm

Sandstorm::Sandstorm(const char* pName)
    : LiveActor(pName), _8C(), mBindActor(), _94(0.0f, 0.0f, 1.0f), _A0(0.0f, 0.0f, 0.0f), mType(), mSpiralRot(), mSpiralRadius(), _B8(), _BC(),
      _C0(), _C4(), _C8(), _CC(), mSensorHeight(), mSizeUpRate(), mSensorMin(), mStepStorm(), mStepAdjust(), mStepFix(), mStepEnd(), mBindHeight(),
      mTrapModel() {
}

void Sandstorm::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool useTrapModel = false;
    MR::getJMapInfoArg0NoInit(rIter, &useTrapModel);

    if (useTrapModel) {
        pArchiveList->addArchive("SunakazeKunTrap");
    }
}

void Sandstorm::init(const JMapInfoIter& rIter) {
    const char* objName;

    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString("TwisterSan", objName)) {
        mType = Type_TwisterSan;
    } else if (MR::isEqualString("SunakazeKun", objName)) {
        mType = Type_SunakazeKun;
    } else if (MR::isEqualString("TwisterSanSlim", objName)) {
        mType = Type_TwisterSanSlim;
    }

    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneNoShadowedMapObj(this);
    initSandstormParam();
    initHitSensor(2);
    MR::addHitSensorCallbackPriorBinder(this, "binder", 4, 0.0f);
    MR::addHitSensorMapObjSimple(this, "star_piece", 4, mSensorHeight * 0.5f + 200.0f, TVec3f(0.0f, mSensorHeight * 0.5f + 200.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    MR::initShadowVolumeCylinder(this, 100.0f);
    MR::setShadowVolumeStartDropOffset(this, nullptr, -100.0f);
    MR::setShadowDropLength(this, nullptr, 300.0f);
    MR::calcGravity(this);
    MR::initActorCamera(this, rIter, &_8C);

    bool useTrapModel = false;
    MR::getJMapInfoArg0NoInit(rIter, &useTrapModel);

    if (useTrapModel) {
        initTrapModel();
    }

    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);

    _94 = *MR::getRailDirection(this);

    f32 boundingRadius;
    MR::calcModelBoundingRadius(&boundingRadius, this);

    f32 railBoundingRadius;
    MR::calcRailClippingInfo(&_A0, &railBoundingRadius, this, 100.0f, 500.0f);

    MR::setClippingTypeSphere(this, boundingRadius + railBoundingRadius, &_A0);
    MR::setGroupClipping(this, rIter, 8);
    MR::useStageSwitchWriteA(this, rIter);

    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }

    initNerve(&NrvSandstorm::SandstormNrvWait::sInstance);
    makeActorAppeared();
}

void Sandstorm::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::offCalcShadow(this, nullptr);
        MR::invalidateHitSensors(this);
        MR::deleteEffectAll(this);
    }

    if (MR::isNearPlayerAnyTime(this, ::sActiveRadius)) {
        MR::onCalcShadow(this, nullptr);
        MR::validateHitSensors(this);
        setNerve(&NrvSandstorm::SandstormNrvWait::sInstance);
    }
}

void Sandstorm::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Wait");
    }

    MR::turnDirectionToPlayerDegree(this, &_94, ::sTurnRate);
    tryNonActive();
}

void Sandstorm::exeStorm() {
    if (MR::isFirstStep(this)) {
        if (isSunakazeKun()) {
            MR::startBck(this, "Spin", nullptr);
        }
    }

    MR::tryRumblePadWeak(this, 0);
    updateSpiral();

    if ((!MR::isGreaterStep(this, ::sStepToEnableSpin) || !tryTornadoJumpPlayer()) && MR::isStep(this, mStepStorm)) {
        setNerve(&NrvSandstorm::SandstormNrvStormAdjust::sInstance);
    }
}

void Sandstorm::exeStormAdjust() {
    if (MR::isFirstStep(this)) {
        _C0 = _BC;
    }

    MR::tryRumblePadVeryWeak(this, 0);
    updateSpiral();

    if (!tryTornadoJumpPlayer() && MR::isStep(this, mStepAdjust)) {
        setNerve(&NrvSandstorm::SandstormNrvStormFix::sInstance);
    }
}

void Sandstorm::exeStormFix() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("StormWait", static_cast< const char* >(nullptr));

        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }
    }

    MR::tryRumblePadVeryWeak(this, 0);

    if (isSunakazeKun()) {
        MR::startLevelSound(this, "SE_OJ_LV_SANDSTORM_S_CHANCE");
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_SANDSTORM_CHANCE");
    }

    tryTornadoJumpPlayer();
}

void Sandstorm::exeStormEnd() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        if (isSunakazeKun) {
            MR::startSound(this, "SE_OJ_SANDSTORM_S_BIND_END");
        } else {
            MR::startSound(this, "SE_OJ_SANDSTORM_BIND_END");
        }
    }

    if (MR::isGreaterStep(this, mStepEnd)) {
        MR::validateHitSensors(this);
        setNerve(&NrvSandstorm::SandstormNrvWait::sInstance);
    }
}

void Sandstorm::control() {
    MR::moveCoordAndFollowTrans(this, ::sRailSpeed);

    if (isNerve(&NrvSandstorm::SandstormNrvNonActive::sInstance)) {
        return;
    }

    if (isSunakazeKun()) {
        MR::startLevelSound(this, "SE_OJ_LV_SANDSTORM_S_WAIT");

        if (mTrapModel != nullptr) {
            MR::startLevelSound(this, "SE_OJ_LV_SANDSTORM_S_HAMMER");
        }
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_SANDSTORM_WAIT");
    }
}

void Sandstorm::calcAndSetBaseMtx() {
    TVec3f vec;
    MR::calcUpVec(&vec, this);
    vec.negate();

    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, vec, _94);
    MR::setBaseTRMtx(this, mtx);
}

void Sandstorm::startClipped() {
    if (mTrapModel != nullptr) {
        for (s32 i = 0; i < 2; i++) {
            if (mTrapModel[i] != nullptr) {
                mTrapModel[i]->startClipped();
            }
        }
    }

    LiveActor::startClipped();
}

void Sandstorm::endClipped() {
    if (mTrapModel != nullptr) {
        for (s32 i = 0; i < 2; i++) {
            if (mTrapModel[i] != nullptr) {
                mTrapModel[i]->endClipped();
            }
        }
    }

    LiveActor::endClipped();
}

bool Sandstorm::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("star_piece")) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        TVec3f up;
        MR::calcUpVec(&up, this);

        f32 verticalDistance = MR::calcDistanceVertical(this, pSender->mPosition, up);

        if (verticalDistance > mSensorHeight + mSizeUpRate * mSensorHeight) {
            return false;
        }

        f32 horizontalDistance = MR::calcDistanceHorizontal(this, pSender->mPosition, up);
        bool ret = horizontalDistance <= MR::max(mSensorMin, mSizeUpRate * verticalDistance);

        if (ret) {
            MR::limitedStarPieceHitSound();
        }

        return ret;
    }

    return false;
}

bool Sandstorm::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvSandstorm::SandstormNrvStormEnd::sInstance)) {
        return false;
    }

    if (pReceiver == getSensor("star_piece")) {
        return false;
    }

    if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pSender)) {
        if (tryStartBind(pSender)) {
            if (isSunakazeKun()) {
                MR::startSound(this, "SE_OJ_SANDSTORM_BIND_S_IN");
            } else {
                MR::startSound(this, "SE_OJ_SANDSTORM_BIND_IN");
            }

            setNerve(&NrvSandstorm::SandstormNrvStorm::sInstance);

            return true;
        }

        return false;
    }

    if (MR::isMsgUpdateBaseMtx(msg) && mBindActor != nullptr) {
        updateBindActorMtx();

        return true;
    }

    return false;
}

void Sandstorm::updateHitSensor(HitSensor* pSensor) {
    TVec3f up;
    MR::calcUpVec(&up, this);

    TVec3f vec;
    vec.scaleAdd(mSensorHeight, up, mPosition);

    f32 val = MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerCenterPos(), mPosition, vec);
    pSensor->mRadius = MR::max(mSensorMin, mSizeUpRate * val * mSensorHeight);
}

void Sandstorm::initSandstormParam() {
    const char* arcName = nullptr;

    if (mType == Type_TwisterSan) {
        arcName = "TwisterSan.arc";
    } else if (mType == Type_SunakazeKun) {
        arcName = "SunakazeKun.arc";
    } else if (mType == Type_TwisterSanSlim) {
        arcName = "TwisterSanSlim.arc";
    }

    JMapInfo* csvParser = MR::createCsvParser(arcName, ::sParamFileName);

    MR::getCsvDataF32(&mSensorHeight, csvParser, "SensorHeight", 0);
    MR::getCsvDataF32(&mSizeUpRate, csvParser, "SizeUpRate", 0);
    MR::getCsvDataF32(&mSensorMin, csvParser, "SensorMin", 0);
    MR::getCsvDataS32(&mStepStorm, csvParser, "StepStorm", 0);
    MR::getCsvDataS32(&mStepAdjust, csvParser, "StepAdjust", 0);
    MR::getCsvDataS32(&mStepFix, csvParser, "StepFix", 0);
    MR::getCsvDataS32(&mStepEnd, csvParser, "StepEnd", 0);
    MR::getCsvDataF32(&mBindHeight, csvParser, "BindHeight", 0);
    MR::getCsvDataF32(&mSpiralRot, csvParser, "SpiralRot", 0);
    MR::getCsvDataF32(&mSpiralRadius, csvParser, "SpiralRadius", 0);
}

void Sandstorm::initTrapModel() {
    if (!isSunakazeKun()) {
        return;
    }

    mTrapModel = new SunakazeKunTrap*[2];

    for (s32 i = 0; i < 2; i++) {
        TVec3f axis = TVec3f(0.0f, 1.0f, 0.0f);
        TVec3f offset = TVec3f(0.0f, ::sTrapOffsetY, ::sTrapOffsetZ);
        MR::rotateVecDegree(&offset, axis, i * 180.0f);

        mTrapModel[i] = new SunakazeKunTrap(this, offset);
        mTrapModel[i]->initWithoutIter();
    }
}

void Sandstorm::updateBindActorMtx() {
    f32 dVar9;

    if (_C8 < 10) {
        TVec3f toPlayerCenter = *MR::getPlayerCenterPos() - mPosition;

        TVec3f up;
        MR::calcUpVec(&up, this);

        TVec3f vec;
        vec.killElement(up, toPlayerCenter);

        dVar9 = MR::getLinerValue(_C8 / 10.0f, vec.length(), _BC, 1.0f);
    } else {
        dVar9 = _BC;
    }

    TVec3f vec = TVec3f(MR::cos(_B8), _C4, MR::sin(_B8));

    if (isSunakazeKun() && isNerve(&NrvSandstorm::SandstormNrvStormFix::sInstance)) {
        vec.y += MR::sinDegree(getNerveStep() * 3.0f) * 270.0f;
    }

    PSMTXMultVec(getBaseMtx(), &vec, mBindActor->mPosition);
    MR::setBaseTRMtx(this, *reinterpret_cast< TPos3f* >(mBindActor->getBaseMtx()));
}

void Sandstorm::endBind() {
    MR::tryRumblePadStrong(this, 0);
    MR::shakeCameraNormal();

    TVec3f up;
    MR::calcUpVec(&up, this);

    MR::endBindAndPlayerJump(this, up, 0);

    if (isSunakazeKun()) {
        MR::miniTornadoJumpPlayer();
        MR::startSound(this, "SE_OJ_SANDSTORM_S_BLOW_UP");
    } else {
        MR::tornadoJumpPlayer();
        MR::startSound(this, "SE_OJ_SANDSTORM_BLOW_UP");
    }

    mBindActor = nullptr;

    MR::validateClipping(this);
    MR::endActorCamera(this, _8C, true, -1);
}

void Sandstorm::updateSpiral() {
    f32 x = _CC / mStepStorm;
    f32 value1 = MR::getLinerValue(x, 0.0f, mSpiralRadius, 1.0f);
    f32 value2 = MR::getLinerValue(x, 0.0f, mBindHeight, 1.0f);
    f32 dVar9 = MR::toRadian(-mSpiralRot * x);

    if (mStepStorm <= _C8) {
        f32 x = static_cast< f32 >(_C8) / mStepStorm;

        _BC = MR::getLinerValue(x, value1, mSpiralRadius, 1.0f);
        _C4 = MR::getLinerValue(x, value2, mBindHeight, 1.0f);
    } else {
        _BC = MR::getLinerValue((_C8 - mStepStorm) / mStepAdjust, _C0, 0.0f, 1.0f);
        _C4 = mBindHeight;
    }

    _C8++;
    _B8 = dVar9 + MR::toRadian(-mSpiralRot * _C8);
}

bool Sandstorm::tryStartBind(HitSensor* pSensor) {
    if (mBindActor != nullptr) {
        return false;
    }

    if (MR::isPlayerDamaging()) {
        return false;
    }

    if (MR::isPlayerElementModeTornado()) {
        return false;
    }

    mBindActor = pSensor->mHost;

    MR::tryRumblePadStrong(this, 0);
    MR::shakeCameraNormal();
    MR::startActorCameraTargetSelf(this, _8C, -1);
    MR::startBckPlayer("StormIn", static_cast< const char* >(nullptr));
    MR::startSound(mBindActor, "SE_PV_UPSET");
    MR::invalidateClipping(this);

    _B8 = 0.0f;
    _BC = 0.0f;
    _C0 = 0.0f;
    _C4 = 0.0f;
    _C8 = 0;

    TVec3f toBindActor = mBindActor->mPosition - mPosition;
    TVec3f up;
    MR::calcUpVec(&up, this);

    _CC = mStepStorm * toBindActor.dot(up) / mBindHeight;

    return true;
}

bool Sandstorm::tryTornadoJumpPlayer() {
    if (!MR::isPadSwing(WPAD_CHAN0)) {
        return false;
    }

    endBind();
    setNerve(&NrvSandstorm::SandstormNrvStormEnd::sInstance);

    return true;
}

bool Sandstorm::tryNonActive() {
    if (MR::isNearPlayerAnyTime(this, ::sActiveRadius)) {
        return false;
    }

    setNerve(&NrvSandstorm::SandstormNrvNonActive::sInstance);

    return true;
}

bool Sandstorm::isSunakazeKun() const {
    return mType == Type_SunakazeKun;
}
