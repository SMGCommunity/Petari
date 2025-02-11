#include "Game/MapObj/Coin.hpp"
#include "Game/MapObj/CoinHolder.hpp"
#include "Game/MapObj/CoinRotater.hpp"
#include "Game/MapObj/PurpleCoinHolder.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMath.hpp"

Coin::Coin(const char *pName) : LiveActor(pName) {
    mHostInfo = nullptr;
    mFlashCtrl = nullptr;
    mAirBubble = nullptr;
    mDropPosition.x = 0.0f;
    mDropPosition.y = 0.0f;
    mDropPosition.z = 0.0f;
    mClippingRange.x = 0.0f;
    mClippingRange.y = 0.0f;
    mClippingRange.z = 0.0f;
    _B0 = 600;
    mCannotTime = 0;
    mIsInWater = false;
    _B9 = false;
    _BA = false;
    mIsPurpleCoin = false;
    mIsNeedBubble = false;
}

void Coin::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(mIsPurpleCoin ? "PurpleCoin" : "Coin", nullptr, false);
    MR::connectToSceneItemStrongLight(this);
    initHitSensor(1);
    MR::addHitSensor(this, "coin", 0x4A, 4, 55.0f, TVec3f(0.0f, 70.0f, 0.0f));
    initBinder(55.0f, 70.0f, 0);
    MR::setBinderExceptSensorType(this, &mPosition, 10.0f);
    MR::setClippingFar100m(this);
    initEffectKeeper(0, nullptr, false);
    initSound(1, false);

    if (MR::isValidInfo(rIter)) {
        setShadowAndPoseModeFromJMapIter(rIter);
    }

    if (_BA) {
        TVec3f axis;
        MR::calcActorAxisY(&axis, this);
        mGravity.set<f32>(-axis);
    }

    initShadow(rIter);

    if (mIsNeedBubble) {
        mAirBubble = MR::createPartsModelNoSilhouettedMapObj(this, "アワ", "AirBubble", nullptr);
        mAirBubble->initFixedPosition(TVec3f(0.0f, 70.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);
        mAirBubble->makeActorDead();
        MR::registerDemoSimpleCastAll(mAirBubble);
        MR::startBck(mAirBubble, "Move", nullptr);
    }

    mFlashCtrl = new FlashingCtrl(this, true);
    MR::tryCreateMirrorActor(this, mIsPurpleCoin ? "PurpleCoin" : "Coin");
    initNerve(&NrvCoin::CoinNrvFix::sInstance);

    if (MR::isValidInfo(rIter)) {
        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
            makeActorDead();
        }
        else {
            appearFixInit();
        }

        if (MR::useStageSwitchReadB(this, rIter)) {
            MR::FunctorV0M<Coin *, void (Coin::*)()> deadFunc = MR::Functor<Coin>(this, &Coin::makeActorDead);
            MR::listenStageSwitchOnB(this, deadFunc);
        }
    }
    else {
        makeActorDead();
    }

    MR::useStageSwitchSleep(this, rIter);
}

void Coin::initAfterPlacement() {
    MR::setClippingRangeIncludeShadow(this, &mClippingRange, 150.0f);
}

void Coin::initShadow(const JMapInfoIter &rIter) {
    f32 arg_5 = -1.0f;
    s32 arg_6 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg5NoInit(rIter, &arg_5);
        MR::getJMapInfoArg6NoInit(rIter, &arg_6);
    }

    switch (arg_6) {
        case 0:
            MR::initShadowSurfaceCircle(this, 50.0f);
            break;
        case 1:
            MR::initShadowVolumeCylinder(this, 50.0f);
            break;
        default:
            MR::initShadowVolumeSphere(this, 50.0f);
            break;
    }

    MR::setShadowDropPositionPtr(this, nullptr, &mDropPosition);

    if (arg_5 > 0.0f) {
        MR::setShadowDropLength(this, nullptr, arg_5);
    }
}

void Coin::setShadowAndPoseModeFromJMapIter(const JMapInfoIter &rIter) {
    bool arg_3 = false;
    MR::getJMapInfoArg3WithInit(rIter, &arg_3);
    _B9 = arg_3;
    bool arg_4 = false;
    MR::getJMapInfoArg4WithInit(rIter, &arg_4);
    _BA = arg_4;
    mIsNeedBubble = Coin::isNeedBubble(rIter);
}

void Coin::makeActorAppeared() {
    MR::offCalcGravity(this);

    if (!_BA) {
        MR::calcGravityOrZero(this);
    }

    if (!mIsPurpleCoin && MR::isGalaxyDarkCometAppearInCurrentStage()) {
        LiveActor::makeActorDead();
    }
    else {
        LiveActor::makeActorAppeared();

        if (mIsNeedBubble) {
            mAirBubble->makeActorAppeared();
            MR::setSensorRadius(this, "coin", 150.0f);
        }
        else {
            MR::setSensorRadius(this, "coin", 55.0f);
        }

        mFlashCtrl->end();
        setCalcShadowMode();
        MR::validateShadow(this, nullptr);

        if (!mIsPurpleCoin) {
            mHostInfo->_8++;
        }

        mIsInWater = MR::isInWater(mPosition);
    }
}

void Coin::makeActorDead() {
    LiveActor::makeActorDead();
    if (mAirBubble) {
        mAirBubble->makeActorDead();
    }
}

void Coin::calcAndSetBaseMtx() {
    TVec3f another_vec;
    another_vec.set<f32>(MR::isNearZero(mGravity, 0.001f) ? TVec3f(0.0f, 1.0f, 0.0f) : -mGravity);
    JMAVECScaleAdd(&another_vec, &mPosition, &mDropPosition, 70.0f);

    TPos3f pos;

    if (_BA) {
        MR::makeMtxTR(pos.toMtxPtr(), this);
    }
    else {
        MR::makeMtxUpNoSupportPos(&pos, another_vec, mPosition);
    }

    if (isNerve(&NrvCoin::CoinNrvSpinDrained::sInstance)) {
        pos.concat(pos, MR::getCoinHiSpeedRotateYMatrix());
    }
    else {
        if (mIsInWater) {
            pos.concat(pos, MR::getCoinInWaterRotateYMatrix());
        }
        else {
            pos.concat(pos, MR::getCoinRotateYMatrix());
        }
    }

    MR::setBaseTRMtx(this, pos);
}

bool Coin::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    bool result;
    
    if (MR::isMsgItemGet(msg)) {
        noticeGetCoin();
        MR::emitEffect(this, "CoinGet");
        mFlashCtrl->end();
        kill();
        result = true;
    }
    else if (MR::isMsgItemPull(msg)) {
        result = requestSpinDrain();
    }
    else if (MR::isMsgItemShow(msg)) {
        result = requestShow();
    }
    else if (MR::isMsgItemHide(msg)) {
        result = requestHide();
    }
    else if (MR::isMsgItemStartMove(msg)) {
        result = requestStartControl();
    }
    else if (MR::isMsgItemEndMove(msg)) {
        result = requestEndControl();
    }
    else if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        result = true;
    }
    else {
        result = false;
    }

    return result;
}

void Coin::exeNonActive() {
    if (MR::isFirstStep(this) || _B9) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 150.0f);
    }
}

void Coin::exeFix() {
    if (MR::isFirstStep(this) || _B9) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 150.0f);
    }
}

void Coin::exeFixTimer() {
    if (MR::isFirstStep(this)) {
        mFlashCtrl->start(_B0);
        MR::invalidateClipping(this);
        MR::offBind(this);
    }

    if (MR::isStep(this, mCannotTime)) {
        MR::validateHitSensors(this);
    }

    if (mFlashCtrl->mIsEnded) {
        kill();
    }
}

void Coin::exeMove() { 
    if (MR::isFirstStep(this)) {
        mFlashCtrl->start(_B0);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }

    if (MR::isStep(this, mCannotTime)) {
        MR::validateHitSensors(this);
    }

    if (MR::isGreaterEqualStep(this, mCannotTime) && MR::isBindedGroundDamageFire(this)) {
        MR::emitEffect(this, "LavaFall");
        MR::startSound(this, "SE_OJ_FALL_IN_MAGMA_S", -1, -1);
        kill();
    }
    else {
        if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            kill();
        }
        else {
            MR::calcGravityOrZero(this);

            if (MR::isPressedRoofAndGround(this)) {
                MR::offBind(this);
                MR::zeroVelocity(this);
            }

            if (!MR::isNoBind(this)) {
                calcRebouond();
                mVelocity += mGravity * (mIsInWater ? 0.30000001f : 1.0f);
                attenuateVelocity();
            }

            if (mFlashCtrl->mIsEnded) {
                kill();
            }
        }
    }
}

void Coin::exeControled() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }

    MR::zeroVelocity(this);
}

void Coin::exeSpinDrained() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }

    MR::emitEffect(this, "CoinSpinBlur");
}

void Coin::exeHop() {
    if (MR::isFirstStep(this)) {
        noticeGetCoin();
    }

    mVelocity += mGravity;

    if (MR::isGreaterStep(this, 30)) {
        MR::emitEffect(this, "CoinGet");
        mFlashCtrl->end();
        kill();
    }
}

void Coin::appear() {
    appearFix();
}

void Coin::appearFixInit() {
    makeActorAppeared();
    MR::validateClipping(this);
    MR::validateHitSensors(this);
    MR::offBind(this);
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvFix::sInstance);
}

void Coin::appearFix() {
    makeActorAppeared();
    MR::validateClipping(this);
    MR::validateHitSensors(this);
    MR::offBind(this);
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvFix::sInstance);
}

void Coin::appearControlPose() {
    makeActorAppeared();
    MR::validateClipping(this);
    MR::validateHitSensors(this);
    MR::offBind(this);
    MR::calcGravityOrZero(this);
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvControled::sInstance);
}

void Coin::appearNonActive() {
    makeActorAppeared();
    MR::validateClipping(this);
    MR::invalidateHitSensors(this);
    MR::offBind(this);
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvNonActive::sInstance);
}

void Coin::appearFixTimer(s32 a1, s32 a2) {
    makeActorAppeared();
    MR::invalidateClipping(this);
    MR::invalidateHitSensors(this);
    MR::offBind(this);
    setLife(a1);
    setCannotTime(a2);
    setNerve(&NrvCoin::CoinNrvFixTimer::sInstance);
}

void Coin::appearMove(const TVec3f &a1, const TVec3f &a2, s32 a3, s32 a4) {
    mPosition.set<f32>(a1);
    makeActorAppeared();
    MR::invalidateClipping(this);
    MR::invalidateHitSensors(this);
    MR::onBind(this);
    MR::onCalcShadow(this, nullptr);
    mVelocity.set<f32>(a2);

    TVec3f stack_14;
    if (MR::normalizeOrZero(a2, &stack_14)) {
        mGravity.x = 0.0f;
        mGravity.y = -1.0f;
        mGravity.z = 0.0f;
    }
    else {
        mGravity.set<f32>(-stack_14);
    }

    MR::calcGravityOrZero(this);
    setLife(a3);
    setCannotTime(a4);
    setNerve(&NrvCoin::CoinNrvMove::sInstance);
}

void Coin::appearHop(const TVec3f &a1, const TVec3f &a2) {
    if (MR::isNearZero(a2, 0.001f)) {
        mGravity.x = 0.0f;
        mGravity.y = -1.0f;
        mGravity.z = 0.0f;
    }
    else {
        MR::normalize(-a2, &mGravity);
    }

    mPosition.set<f32>(a1);
    makeActorAppeared();
    MR::invalidateClipping(this);
    MR::invalidateHitSensors(this);
    MR::offBind(this);
    MR::invalidateShadow(this, nullptr);
    mVelocity.set<f32>(a2 * 30.0f);
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvHop::sInstance);
}

bool Coin::requestActive() {
    if (isNerve(&NrvCoin::CoinNrvNonActive::sInstance)) {
        MR::invalidateHitSensors(this);
        setNerve(&NrvCoin::CoinNrvFix::sInstance);
        return true;
    }

    return false;
}

bool Coin::requestActiveWithGravity() {
    if (isNerve(&NrvCoin::CoinNrvNonActive::sInstance)) {
        MR::invalidateClipping(this);
        MR::validateHitSensors(this);
        MR::onBind(this);
        MR::calcGravityOrZero(this);
        mVelocity.set<f32>(-mGravity * 30.0f);
        setNerve(&NrvCoin::CoinNrvMove::sInstance);
        return true;
    }

    return false;
}

bool Coin::requestDeactive() {
    if (!MR::isDead(this) && isNerve(&NrvCoin::CoinNrvFix::sInstance)) {
        MR::invalidateHitSensors(this);
        setNerve(&NrvCoin::CoinNrvNonActive::sInstance);
        return true;
    }

    return false;
}

bool Coin::requestStartControl() {
    if (MR::isDead(this)) {
        return false;
    }
    
    if (!isNerve(&NrvCoin::CoinNrvFix::sInstance) && !isNerve(&NrvCoin::CoinNrvFixHide::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCoin::CoinNrvFixHide::sInstance)) {
        MR::showModel(this);
    }

    MR::validateClipping(this);
    MR::invalidateHitSensors(this);
    MR::offBind(this);
    MR::onCalcShadow(this, nullptr);
    setNerve(&NrvCoin::CoinNrvControled::sInstance);
    mFlashCtrl->end();
    return true;
}

bool Coin::requestEndControl() {
    if (MR::isDead(this) || !isNerve(&NrvCoin::CoinNrvControled::sInstance)) {
        return false;
    }

    MR::validateClipping(this);
    MR::validateHitSensors(this);
    MR::offBind(this);
    MR::offCalcShadow(this, nullptr);
    setCalcShadowMode();
    mCannotTime = 0;
    setNerve(&NrvCoin::CoinNrvFix::sInstance);
    return true;
}

bool Coin::requestSpinDrain() {
    if (isNerve(&NrvCoin::CoinNrvSpinDrained::sInstance)) {
        return false;
    }

    if (mIsNeedBubble) {
        return false;
    }

    MR::invalidateClipping(this);
    MR::offBind(this);
    MR::validateHitSensors(this);
    setNerve(&NrvCoin::CoinNrvSpinDrained::sInstance);
    mFlashCtrl->end();
    return true;
}

bool Coin::requestShow() {
    if (isNerve(&NrvCoin::CoinNrvFixHide::sInstance) && !MR::isDead(this)) {
        MR::showModel(this);
        MR::validateHitSensors(this);
        setNerve(&NrvCoin::CoinNrvFix::sInstance);
    }

    return false;
}

bool Coin::requestHide() {
    if (isNerve(&NrvCoin::CoinNrvFix::sInstance) && !MR::isDead(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvCoin::CoinNrvFixHide::sInstance);
    }

    return false;
}

void Coin::noticeGetCoin() {
    if (mIsNeedBubble) {
        MR::emitEffect(mAirBubble, "RecoveryBubbleBreak");
        MR::incPlayerOxygen(8);
        mAirBubble->kill();
        mIsNeedBubble = false;
    }

    if (mIsPurpleCoin) {
        MR::incPurpleCoin();
        MR::startSystemSE("SE_SY_PURPLE_COIN", -1, -1);
    }
    else {
        MR::incCoin(1);
        MR::startSystemSE("SE_SY_COIN", -1, -1);
        mHostInfo->_C++;
    }
}

void Coin::setLife(s32 life) {
    if (life < 0) {
        _B0 = 0x258;
    }
    else {
        _B0 = life;
    }
}

void Coin::setHostInfo(CoinHostInfo *pInfo) {
    mHostInfo = pInfo;
}

void Coin::setCalcShadowMode() {
    if (_B9) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }
    else {
        MR::onCalcShadowOneTime(this, nullptr);
        MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
    }
}

void Coin::setCannotTime(s32 time) {
    if (time < 0) {
        mCannotTime = 0x1E;
    }
    else {
        mCannotTime = time;
    }
}

void Coin::calcRebouond() {
    if (MR::isBindedRoof(this)) {
        MR::calcReboundVelocity(&mVelocity, *MR::getRoofNormal(this), 0.60000002f, 0.5f);
    }

    if (MR::isBindedWall(this)) {
        MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.60000002f, 0.5f);
    }

    if (MR::isBindedGround(this)) {
        f32 dot = mVelocity.dot(-*MR::getGroundNormal(this));

        f32 vel = 0.0f;

        if (dot >= 2.0f) {
            if (dot > 30.0f) {
                dot = 30.0f;
            }

            int what = (100.0f * dot) / 30.0f;

            if (what > 5) {
                MR::startSound(this, "SE_OJ_COIN_BOUND", what, -1);
            }

            vel = 0.75f;
        }

        MR::calcReboundVelocity(&mVelocity, -mGravity, vel, 0.5f);
    }
}

void Coin::attenuateVelocity() {
    f32 velocity;
    
    if (mIsInWater) {
        velocity = 0.80000001f;
    }
    else if (MR::isOnGround(this)) {
        velocity = 0.89999998f;
    }
    else {
        velocity = 0.995f;
    }

    MR::attenuateVelocity(this, velocity);
}

bool Coin::isNeedBubble(const JMapInfoIter &rIter) {
    if (!MR::isValidInfo(rIter)) {
        return false;
    }

    bool arg = false;
    MR::getJMapInfoArg7WithInit(rIter, &arg);
    return arg;
}

void Coin::makeArchiveList(NameObjArchiveListCollector *pList, const JMapInfoIter &rIter) {
    if (Coin::isNeedBubble(rIter)) {
        pList->addArchive("AirBubble");
    }
}

namespace MR {
    NameObj* createDirectSetCoin(const char *pName) {
        Coin* coin = new Coin(pName);
        MR::createCoinHolder();
        MR::createCoinRotater();
        MR::addToCoinHolder(coin, coin);
        return coin;
    }

    NameObj* createCoin(const NameObj *pObj, const char *pName) {
        Coin* coin = new Coin(pName);
        MR::createCoinHolder();
        MR::createCoinRotater();
        MR::addToCoinHolder(pObj, coin);
        return coin;
    }

    NameObj* createDirectSetPurpleCoin(const char *pName) {
        Coin* coin = new Coin(pName);
        coin->mIsPurpleCoin = true;
        MR::createPurpleCoinHolder();
        MR::createCoinRotater();
        MR::addToPurpleCoinHolder(coin, coin);
        return coin;
    }

    NameObj* createPurpleCoin(const NameObj *pObj, const char *pName) {
        Coin* coin = new Coin(pName);
        coin->mIsPurpleCoin = true;
        MR::createPurpleCoinHolder();
        MR::createCoinRotater();
        MR::addToPurpleCoinHolder(pObj, coin);
        return coin;
    }
};

namespace NrvCoin {
    INIT_NERVE(CoinNrvNonActive);
    INIT_NERVE(CoinNrvFix);
    INIT_NERVE(CoinNrvFixHide);
    INIT_NERVE(CoinNrvFixTimer);
    INIT_NERVE(CoinNrvControled);
    INIT_NERVE(CoinNrvSpinDrained);
    INIT_NERVE(CoinNrvMove);
    INIT_NERVE(CoinNrvHop);

    void CoinNrvHop::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeHop();
    }

    void CoinNrvMove::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeMove();
    }

    void CoinNrvSpinDrained::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeSpinDrained();
    }

    void CoinNrvControled::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeControled();
    }

    void CoinNrvFixTimer::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeFixTimer();
    }

    void CoinNrvFixHide::execute(Spine *pSpine) const {

    }

    void CoinNrvFix::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeFix();
    }

    void CoinNrvNonActive::execute(Spine *pSpine) const {
        Coin* coin = reinterpret_cast<Coin*>(pSpine->mExecutor);
        coin->exeNonActive();
    }
};

Coin::~Coin() {

}
