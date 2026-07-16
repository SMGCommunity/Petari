#include "Game/Enemy/Teresa.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/SunshadeMapHolder.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/MapObj/KeySwitch.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"

namespace {
    static Color8 sPointLightColor(255, 255, 255, 255);
};  // namespace

namespace NrvTeresa {
    NEW_NERVE_ONEND(TeresaNrvAppearFromWall, Teresa, AppearFromWall, AppearFromWall);
    NEW_NERVE_ONEND(TeresaNrvAppearFromGround, Teresa, AppearFromGround, AppearFromGround);
    NEW_NERVE(TeresaNrvWait, Teresa, Wait);
    NEW_NERVE(TeresaNrvWalk, Teresa, Walk);
    NEW_NERVE(TeresaNrvRailWalk, Teresa, RailWalk);
    NEW_NERVE(TeresaNrvRailTurn, Teresa, RailTurn);
    NEW_NERVE(TeresaNrvLoveFind, Teresa, LoveFind);
    NEW_NERVE(TeresaNrvLoveChase, Teresa, LoveChase);
    NEW_NERVE(TeresaNrvLoveHit, Teresa, LoveHit);
    NEW_NERVE(TeresaNrvLoveEnd, Teresa, LoveEnd);
    NEW_NERVE(TeresaNrvChase, Teresa, Chase);
    NEW_NERVE(TeresaNrvShay, Teresa, Shay);
    NEW_NERVE(TeresaNrvAggressive, Teresa, Aggressive);
    NEW_NERVE(TeresaNrvAttackSuccess, Teresa, AttackSuccess);
    NEW_NERVE_ONEND(TeresaNrvDrift, Teresa, Drift, Drift);
    NEW_NERVE(TeresaNrvHideWall, Teresa, HideWall);
    NEW_NERVE(TeresaNrvHideWater, Teresa, HideWater);
    NEW_NERVE(TeresaNrvAscension, Teresa, Ascension);
    NEW_NERVE(TeresaNrvStop, Teresa, Stop);
};  // namespace NrvTeresa

Teresa::Teresa(const char* pName)
    : LiveActor(pName), mDisplayModel(nullptr), mKeySwitch(nullptr), _94(0.0f, 0.0f, 0.0f, 1.0f), mWallNormal(0, 0, 1), mWallHitPos(0, 0, 0),
      _BC(0, 0, 1), _C8(0, 0, 0), _D4(1, 1, 1), _E0(0, 0, 0) {
    _EC = 3000.0f;
    _F0 = 600.0f;
    _F4 = 1.0f;
    mAppearanceType = -1;
    _FC = 1;
    _FD = 0;
    _FE = 0;
}

void Teresa::init(const JMapInfoIter& rIter) {
    initFromJMapParam(rIter);
    _C8.set< f32 >(mPosition);
    mWallHitPos.set< f32 >(mPosition);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordAndTransToNearestRailPos(this);
    }

    initModelManagerWithAnm("Teresa", nullptr, false);
    MR::connectToSceneIndirectEnemy(this);
    initDummyModel(rIter);
    initSound(4, false);
    initSensor();
    MR::initShadowVolumeSphere(this, 80.0f);
    MR::onShadowFollowHostScale(this, nullptr);
    initBind();
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvTeresa::TeresaNrvWait::sInstance);
    setTransparency(1.0f);
    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }

    if (MR::isObjectName(rIter, "TeresaChief")) {
        MR::declarePowerStar(this);
        _FC = 0;
        _FD = 1;
        MR::invalidateClipping(this);
        MR::setShadowDropLength(this, nullptr, 3000.0f);
    }

    if (_FC) {
        MR::declareCoin(this, 1);
    }

    if (MR::isValidInfo(rIter) && !MR::isExistStageSwitchAppear(rIter)) {
        makeActorAppeared();

    } else {
        makeActorDead();
    }

    setStartNerve();
}

void Teresa::initDummyModel(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        mDisplayModel = MR::createDummyDisplayModel(this, rIter, "SklRoot", -1);
    }

    if (mDisplayModel != nullptr) {
        mDisplayModel->kill();

        if (MR::getDummyDisplayModelId(rIter, -1) == 6) {
            mKeySwitch = new KeySwitch("鍵スイッチ");
            mKeySwitch->initKeySwitchByOwner(rIter);
            _FC = 0;
        }
    }

    if (MR::isBckExist(mDisplayModel, "InTeresa")) {
        MR::startBck(mDisplayModel, "InTeresa", nullptr);
    }
}

void Teresa::initFromJMapParam(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        mScale.scale(1.0f);
        MR::makeQuatAndFrontFromRotate(&_94, &mWallNormal, this);
        MR::getJMapInfoArg0WithInit(rIter, &mAppearanceType);
    }
}

void Teresa::initSensor() {
    f32 scale_x = mScale.x;
    initHitSensor(2);
    MR::addHitSensorEnemyAttack(this, "attack", 8, (70.0f * scale_x), TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "body", 8, (85.0f * scale_x), TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTarget(this, (85.0f * scale_x), TVec3f(0.0f, 0.0f, 0.0f));
}

void Teresa::initBind() {
    initBinder(60.0f * mScale.x, 0.0f, 0);
    MR::setBindTriangleFilter(this, TriangleFilterDelegator< Teresa >::allocateDelegator(this, &Teresa::filterBind));
}

void Teresa::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::onCalcGravity(this);
}

void Teresa::appear() {
    LiveActor::appear();
    setTransparency(0.0f);

    switch (mAppearanceType) {
    case 0:
        MR::invalidateClipping(this);
        MR::onBind(this);
        mWallHitPos.set< f32 >(mPosition);
        setNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance);
        break;
    case 1:
        MR::invalidateClipping(this);
        MR::onBind(this);
        mWallHitPos.set< f32 >(mPosition);
        setNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance);
        break;
    default:
        setStartNerve();
        break;
    }
}

void Teresa::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");

    if (mKeySwitch != nullptr) {
        mKeySwitch->appearKeySwitch(mPosition);
    }

    if (_FD) {
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this, mPosition);
    }

    if (_FC) {
        MR::appearCoinPop(this, mPosition, 1);
    }

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void Teresa::control() {
    if (!isNerve(&NrvTeresa::TeresaNrvDrift::sInstance) && !isNerve(&NrvTeresa::TeresaNrvAscension::sInstance)) {
        f32 v2 = (1.0f / (1.0f + (0.949f) * (_D4.x - 1.0f)));
        _D4.x = 1.0f + (0.949f * (_D4.x - 1.0f));
        _D4.z = v2;
    }

    tryHideWater();
    MR::blendQuatUpFront(&_94, -mGravity, mWallNormal, 0.1f, 0.2f);
    _FE = 0;

    if (_F4 > 0.89f) {
        MR::requestPointLight(this, TVec3f(mPosition), ::sPointLightColor, 1.0f, -1);
    }
}

void Teresa::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _94);

    TVec3f scale;
    // regswap
    JMathInlineVEC::PSVECMultiply(&_D4, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

void Teresa::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensorEnemy(pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
        return;
    }

    if (requestAttack(pSender, pReceiver)) {
        return;
    }
}

bool Teresa::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isSensorEnemy(pSender)) {
        TVec3f v9 = MR::getSensorPos(pSender) - MR::getSensorPos(pReceiver);
        MR::normalizeOrZero(&v9);
        mVelocity.add(v9 * 0.1f);
        return true;
    }

    return false;
}

bool Teresa::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isMsgSearchlightAttack(msg) || MR::isMsgFireBallAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
        return requestSearchLightDead();
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return isEnableStarPieceStop();
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (isEnableStarPieceStop()) {
            setNerve(&NrvTeresa::TeresaNrvStop::sInstance);
        }
    }

    return false;
}

bool Teresa::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg)) {
        return requestDrift();
    }

    if (msg == ACTMES_TERESA_PLAYER_TOUCH) {
        return requestLoveHit(pSender, pReceiver);
    }

    return false;
}

bool Teresa::filterBind(const Triangle* pTriangle) {
    if (pTriangle->mSensor->isType(ATYPE_CLIP_FIELD_MAP_PARTS)) {
        return true;
    }

    if (MR::isWallCodeGhostThrough(pTriangle)) {
        _FE = 1;
        _BC.set< f32 >(*pTriangle->getNormal(0));
        return isNerve(&NrvTeresa::TeresaNrvDrift::sInstance);
    }

    return false;
}

bool Teresa::requestAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemyAttack(pSender) || !MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (!canAttack()) {
        return false;
    }

    if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        setNerve(&NrvTeresa::TeresaNrvAttackSuccess::sInstance);
        return true;
    }

    return false;
}

bool Teresa::requestDrift() {
    if (canDrift()) {
        setNerve(&NrvTeresa::TeresaNrvDrift::sInstance);
        return true;
    }

    return false;
}

bool Teresa::requestLoveHit(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTeresa::TeresaNrvLoveChase::sInstance)) {
        MR::addVelocitySeparateHV(this, pSender, pReceiver, 20.0f, 0.0f);
        setNerve(&NrvTeresa::TeresaNrvLoveHit::sInstance);
        return true;
    }

    return false;
}

bool Teresa::requestSearchLightDead() {
    if (canSearchLightDead()) {
        setNerve(&NrvTeresa::TeresaNrvAscension::sInstance);
        return true;
    }

    return false;
}

void Teresa::setStartNerve() {
    if (MR::isExistRail(this)) {
        setNerve(&NrvTeresa::TeresaNrvRailWalk::sInstance);
        setTransparency(1.0f);
        MR::startAction(this, "Wait");
        MR::moveCoordToNearestPos(this, mPosition);
    } else {
        setNerve(&NrvTeresa::TeresaNrvWait::sInstance);
    }
}

bool Teresa::tryAppearFromWallEnd() {
    if (MR::isBckStopped(this)) {
        setStartNerve();
        return true;
    }

    return false;
}

bool Teresa::tryAppearFromGroundEnd() {
    if (MR::isBckStopped(this)) {
        setStartNerve();
        return true;
    }

    return false;
}

bool Teresa::tryRailTurn() {
    if (MR::isRailReachedGoal(this)) {
        MR::reverseRailDirection(this);
        setNerve(&NrvTeresa::TeresaNrvRailTurn::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryRailTurnEnd() {
    if (MR::isGreaterStep(this, 150)) {
        setNerve(&NrvTeresa::TeresaNrvRailWalk::sInstance);
        return true;
    }
    return false;
}

bool Teresa::tryWalk() {
    if (MR::isGreaterStep(this, 192)) {
        TVec3f v6;
        MR::getRandomVector(&v6, 1.0f);
        MR::normalizeOrZero(&v6);
        v6.orthogonalize(mGravity);
        v6 *= _F0;
        _E0 = v6 + _C8;
        setNerve(&NrvTeresa::TeresaNrvWalk::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryWalkEnd() {
    if (MR::isGreaterStep(this, 300) || _E0.distance(mPosition) <= 200.0f) {
        setNerve(&NrvTeresa::TeresaNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryChase() {
    if (MR::isPlayerElementModeTeresa()) {
        return false;
    }

    if (!isShay() && MR::isNearPlayer(this, 1800.0f)) {
        setNerve(&NrvTeresa::TeresaNrvChase::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryCheseEnd() {
    if (MR::isGreaterStep(this, 240) || MR::isPlayerElementModeTeresa() || isNotNear()) {
        setStartNerve();
        return true;
    }

    return false;
}

bool Teresa::tryShay() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvTeresa::TeresaNrvShay::sInstance);
        return true;
    }

    if (MR::isPlayerElementModeTeresa()) {
        return false;
    }

    if (isShay()) {
        setNerve(&NrvTeresa::TeresaNrvShay::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryShayEnd() {
    if (MR::isStarPointerPointing2POnPressButton(this, nullptr, false, false)) {
        return false;
    }

    if (MR::isPlayerElementModeTeresa()) {
        setStartNerve();
        return true;
    } else if (!isShay()) {
        if (MR::isExistRail(this)) {
            setStartNerve();
        } else {
            setNerve(&NrvTeresa::TeresaNrvAggressive::sInstance);
        }

        return true;
    } else {
        return false;
    }
}

bool Teresa::tryLoveFind() {
    if (MR::isPlayerElementModeTeresa() && MR::isNearPlayerAnyTime(this, 1500.0f)) {
        setNerve(&NrvTeresa::TeresaNrvLoveFind::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryLoveEnd() {
    if (MR::isPlayerElementModeTeresa() && MR::isNearPlayerAnyTime(this, 1500.0f)) {
        setNerve(&NrvTeresa::TeresaNrvLoveEnd::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryLoveFindEnd() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvTeresa::TeresaNrvLoveChase::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryLoveChaseEnd() {
    if (MR::isGreaterStep(this, 240)) {
        setStartNerve();
        return true;
    }

    return false;
}

bool Teresa::tryLoveHitEnd() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvTeresa::TeresaNrvLoveChase::sInstance);
        return true;
    }
    return false;
}

bool Teresa::tryAggressiveEnd() {
    if (MR::isGreaterStep(this, 75)) {
        setNerve(&NrvTeresa::TeresaNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryAttackSuccessEnd() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvTeresa::TeresaNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryDriftEnd() {
    if (MR::isGreaterStep(this, 50)) {
        setNerve(&NrvTeresa::TeresaNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryHideWater() {
    TVec3f v4;
    JMAVECScaleAdd(&mGravity, &mPosition, &v4, (70.0f * mScale.y));

    if (isCheckWater() && MR::isInWater(v4)) {
        TPos3f effectMtx;
        MR::makeMtxFrontNoSupportPos(&effectMtx, -mGravity, v4);
        MR::emitEffectHit(this, effectMtx, "HideWall");
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
        MR::offBind(this);
        MR::hideModel(this);
        MR::zeroVelocity(this);
        setNerve(&NrvTeresa::TeresaNrvHideWater::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryHideWall() {
    if (MR::isBindedWall(this)) {
        mWallNormal.set< f32 >(*MR::getWallNormal(this));
        mWallHitPos.set< f32 >(*MR::getWallHitPos(this));
        TPos3f frontMtx;
        MR::makeMtxFrontNoSupportPos(&frontMtx, mWallNormal, mWallHitPos);
        MR::emitEffectHit(this, frontMtx, "HideWall");
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
        MR::offBind(this);
        MR::hideModel(this);
        MR::zeroVelocity(this);
        setNerve(&NrvTeresa::TeresaNrvHideWall::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryHideWallEnd() {
    if (MR::isGreaterStep(this, 60)) {
        MR::validateHitSensors(this);
        MR::onBind(this);
        MR::showModel(this);
        setNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance);
        return true;
    }

    return false;
}

bool Teresa::tryAscension() {
    if (MR::isInAreaObj("GlaringLightArea", mPosition) || !MR::isInShadeFromTheSun(mPosition, 10000.0f)) {
        setNerve(&NrvTeresa::TeresaNrvAscension::sInstance);
        return true;
    }

    return false;
}

void Teresa::endAppear() {
    if (!_FD) {
        MR::validateClipping(this);
    }

    MR::startBrk(this, "Erase");
    setTransparency(1.0f);
}

void Teresa::endDrift() {
    MR::forceDeleteEffect(this, "SpinBlur");
    setTransparency(1.0f);
    MR::setShadowVolumeSphereRadius(this, nullptr, 80.0f);
}

void Teresa::exeAppearFromWall() {
    TVec3f v4;
    _94.getZDir(v4);

    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startAction(this, "AppearWall");
        MR::zeroVelocity(this);
        TPos3f frontMtx;
        MR::makeMtxFrontNoSupportPos(&frontMtx, v4, mWallHitPos);
        MR::emitEffectHit(this, frontMtx, "Appear");
        MR::startSound(this, "SE_EM_TERESA_APPEAR", -1, -1);
    }

    TVec3f v3(v4);
    v3 *= 0.2f;
    mVelocity.add(v3);
    MR::attenuateVelocity(this, 0.89f);
    f32 rate = MR::calcNerveRate(this, 60);
    MR::setShadowVolumeSphereRadius(this, 0, (80.0f * (1.0f - (1.0f - rate) * (1.0f - rate))));
    if (tryAppearFromWallEnd()) {
        return;
    }
}

void Teresa::exeAppearFromGround() {
    TVec3f v4;
    _94.getYDir(v4);

    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startAction(this, "AppearGround");
        MR::zeroVelocity(this);
        TPos3f frontMtx;
        MR::makeMtxFrontNoSupportPos(&frontMtx, v4, mWallHitPos);
        MR::emitEffectHit(this, frontMtx, "Appear");
        MR::startSound(this, "SE_EM_TERESA_APPEAR", -1, -1);
    }

    TVec3f v3(v4);
    v3 *= 0.2f;
    mVelocity.add(v3);
    MR::attenuateVelocity(this, 0.89f);
    f32 rate = MR::calcNerveRate(this, 60);
    MR::setShadowVolumeSphereRadius(this, 0, (80.0f * (1.0f - (1.0f - rate) * (1.0f - rate))));
    if (tryAppearFromGroundEnd()) {
        return;
    }
}

void Teresa::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::startLevelSound(this, "SE_EM_LV_TERESA_MOVE", -1, -1, -1);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryChase() && !tryWalk()) {
        if (tryLoveFind()) {
            return;
        }
    }
}

void Teresa::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBtp(this, "Normal");
    }

    MR::turnDirectionToTarget(this, &mWallNormal, _E0, 0.9994f);
    MR::addVelocityMoveToDirection(this, mWallNormal, 0.2f);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryWalkEnd()) {
        if (tryLoveFind()) {
            return;
        }
    }
}

void Teresa::exeRailWalk() {
    TVec3f v7;
    v7.rejection(MR::getRailPos(this) - mPosition, mGravity);

    if (v7.length() < 10.0f) {
        MR::moveCoord(this, 10.0f);
    }

    MR::turnDirectionToTarget(this, &mWallNormal, MR::getRailPos(this), 0.9994f);
    MR::addVelocityMoveToTarget(this, MR::getRailPos(this), 0.349f);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryRailTurn()) {
        if (tryLoveFind()) {
            return;
        }
    }
}

void Teresa::exeRailTurn() {
    if (MR::isGreaterStep(this, 60)) {
        MR::turnDirection(this, &mWallNormal, MR::getRailDirection(this), 0.9997f);
    }

    MR::addVelocityMoveToTarget(this, MR::getRailPos(this), 0.1f);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryRailTurnEnd()) {
        if (tryLoveFind()) {
            return;
        }
    }
}

void Teresa::exeLoveFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "LoveFind");
        MR::startSound(this, "SE_EM_TERESA_LOVE_FIND", -1, -1);
        MR::startSound(this, "SE_EV_TERESA_LAUGH", -1, -1);
    }

    MR::turnDirectionToTarget(this, &mWallNormal, *MR::getPlayerPos(), 0.98f);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryLoveFindEnd()) {
        if (tryLoveEnd()) {
            return;
        }
    }
}

void Teresa::exeLoveChase() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "LoveChase");
    }

    MR::turnDirectionToTarget(this, &mWallNormal, *MR::getPlayerPos(), 0.999f);
    MR::addVelocityMoveToDirection(this, mWallNormal, (0.75f * MR::calcNerveRate(this, 20)));
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay() && !tryLoveEnd()) {
        if (tryLoveChaseEnd()) {
            return;
        }
    }
}

void Teresa::exeLoveHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "LoveHit");
        MR::startSound(this, "SE_EM_TERESA_LOVE_HIT", -1, -1);
        MR::startSound(this, "SE_EV_TERESA_LOVE_HIT", -1, -1);
    }

    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay()) {
        if (tryLoveHitEnd()) {
            return;
        }
    }
}

void Teresa::exeLoveEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "LoveEnd");
    }

    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay()) {
        if (tryAggressiveEnd()) {
            return;
        }
    }
}

void Teresa::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Chase");
        MR::startSound(this, "SE_EV_TERESA_LAUGH", -1, -1);
    }

    MR::startLevelSound(this, "SE_EM_LV_TERESA_MOVE", -1, -1, -1);
    MR::turnDirectionToTarget(this, &mWallNormal, *MR::getPlayerPos(), 0.999f);
    MR::addVelocityMoveToDirection(this, mWallNormal, (0.75f * MR::calcNerveRate(this, 20)));
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay()) {
        if (tryCheseEnd()) {
            return;
        }
    }
}

void Teresa::exeShay() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shay");
        MR::startSound(this, "SE_EM_TERESA_STOP", -1, -1);
        MR::zeroVelocity(this);
    }

    if (_FD) {
        MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST", -1, -1, -1);
    }

    if (mDisplayModel != nullptr) {
        setTransparency(MR::calcNerveEaseInOutValue(this, 60, _F4, 0.5f));
    } else {
        updateNormalTransparency();
    }

    updateNormalVelocity();

    if (!tryAscension()) {
        if (tryShayEnd()) {
            return;
        }
    }
}

void Teresa::exeAggressive() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Aggressive");
    }

    MR::startLevelSound(this, "SE_EM_LV_TERESA_MOVE", -1, -1, -1);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension() && !tryShay()) {
        if (tryAggressiveEnd()) {
            return;
        }
    }
}

void Teresa::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Laugh");
        MR::setShadowVolumeSphereRadius(this, nullptr, 80.0f);
        MR::startSound(this, "SE_EV_TERESA_ATTACK_SUCCESS", -1, -1);
    }

    MR::turnDirectionToTarget(this, &mWallNormal, *MR::getPlayerPos(), 0.99f);
    updateNormalTransparency();
    updateNormalVelocity();

    if (!tryAscension()) {
        if (tryAttackSuccessEnd()) {
            return;
        }
    }
}

void Teresa::exeDrift() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffectAll(this);
        MR::startAction(this, "Drift");
        MR::emitEffect(this, "SpinBlur");
        _D4.x = 1.0f;
        _D4.y = 1.0f;
        _D4.z = 1.0f;
        MR::startSound(this, "SE_EM_TERESA_TORNADE", -1, -1);

        if (mDisplayModel != nullptr) {
            mDisplayModel->appear();
        }
    }

    MR::turnDirectionToTarget(this, &mWallNormal, *MR::getPlayerPos(), 0.959f);
    addDriftVelocity();
    updateNormalVelocity();
    updateNormalTransparency();
    updateDriftTransparency();

    if (!tryAscension() && !tryHideWall()) {
        if (tryDriftEnd()) {
            return;
        }
    }
}

void Teresa::exeHideWall() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        MR::setShadowVolumeSphereRadius(this, nullptr, 0.0f);
    }

    if (tryHideWallEnd()) {
        return;
    }
}

void Teresa::exeHideWater() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        MR::setShadowVolumeSphereRadius(this, nullptr, 0.0f);
        mWallHitPos.set< f32 >(_C8);
    }

    if (MR::isGreaterStep(this, 60)) {
        MR::validateHitSensors(this);
        MR::onBind(this);
        MR::showModel(this);
        setNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance);
    }
}

void Teresa::exeAscension() {
    if (MR::isFirstStep(this)) {
        _D4.x = 1.0f;
        _D4.y = 1.0f;
        _D4.z = 1.0f;
        MR::setBaseScale(this, mScale);
        MR::startSound(this, "SE_EM_TERESA_DEAD", -1, -1);
        MR::startSound(this, "SE_EV_TERESA_DEAD", -1, -1);
        kill();
    }
}

void Teresa::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(this);
        MR::startAction(this, "Wait");
        setTransparency(0.0f);
    }

    if (!tryAscension()) {
        if (MR::isGreaterStep(this, 60)) {
            setStartNerve();
        }
    }
}

void Teresa::endAppearFromGround() {
    endAppear();
}

void Teresa::endAppearFromWall() {
    endAppear();
}

void Teresa::updateNormalVelocity() {
    MR::addVelocityKeepHeightUseShadow(this, 150.0f + (85.0f * (mScale.y - 1.0f)), 0.40f, 40.0f, nullptr);
    MR::reboundVelocityFromCollision(this, -0.2f, 4.0f, 1.0f);
    MR::attenuateVelocity(this, 0.89f);
}

void Teresa::addDriftVelocity() {
    if (MR::isLessEqualStep(this, 35)) {
        TVec3f v5;
        MR::calcPlayerSpinPullVelocity(&v5, mPosition);
        TVec3f v4(v5);
        v4 *= 0.12f;
        mVelocity.add(v4);
    }

    MR::addVelocityAwayFromTarget(this, *MR::getPlayerPos(), -1.25f);
}

void Teresa::updateDriftAnimScale() {
    TVec3f v4;
    _94.getXDir(v4);
    f32 v2 = v4.dot(mVelocity);
    f32 v3 = (1.0f + (0.30f * MR::normalize(__fabsf(v2), 3.0f, 53.0f)));
    _D4.x = v3;
    _D4.z = 1.0f / v3;
}

void Teresa::updateDriftTransparency() {
    f32 v2 = MR::calcNerveRate(this, 50);
    f32 v3 = ((4.0f * v2) * (v2 - 1.0f));
    f32 v4 = (1.0f - (v3 * v3));
    f32 v5 = MR::calcNerveValue(this, 25, _F4, v4);
    setTransparency(v5);
    MR::setShadowVolumeSphereRadius(this, nullptr, (80.0f * v4));
}

void Teresa::updateNormalTransparency() {
    if (_F4 < 1.0f) {
        f32 v1 = _F4 + 0.02f;
        _F4 += 0.02f;

        if (v1 > 1.0f) {
            _F4 = 1.0f;
        }
    }

    setTransparency(_F4);
}

void Teresa::setTransparency(f32 val) {
    MR::setBrkFrameAndStop(this, (9.0f * (1.0f - val)));

    if (val < 1.0f) {
        if (mDisplayModel != nullptr) {
            mDisplayModel->appear();
        }
    } else {
        if (mDisplayModel != nullptr) {
            mDisplayModel->kill();
        }
    }

    _F4 = val;
}

bool Teresa::canAttack() const {
    if (isNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance) || isNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvAttackSuccess::sInstance) || isNerve(&NrvTeresa::TeresaNrvDrift::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvAscension::sInstance)) {
        return false;
    }

    return true;
}

bool Teresa::canDrift() const {
    if (isNerve(&NrvTeresa::TeresaNrvDrift::sInstance) || isNerve(&NrvTeresa::TeresaNrvAscension::sInstance)) {
        return false;
    }

    return true;
}

bool Teresa::canSearchLightDead() const {
    if (isNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance) || isNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvHideWater::sInstance) || isNerve(&NrvTeresa::TeresaNrvHideWall::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvAscension::sInstance)) {
        return false;
    }

    return true;
}

bool Teresa::isEnableStarPieceStop() const {
    if (isNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance) || isNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvDrift::sInstance) || isNerve(&NrvTeresa::TeresaNrvHideWater::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvHideWall::sInstance) || isNerve(&NrvTeresa::TeresaNrvAscension::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvStop::sInstance)) {
        return false;
    }

    return true;
}

bool Teresa::isCheckWater() const {
    if (isNerve(&NrvTeresa::TeresaNrvAppearFromWall::sInstance) || isNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvHideWater::sInstance) || isNerve(&NrvTeresa::TeresaNrvHideWall::sInstance) ||
        isNerve(&NrvTeresa::TeresaNrvAscension::sInstance)) {
        return false;
    }

    return true;
}

bool Teresa::isShay() const {
    bool isNotSquat = MR::isPlayerSquat() == false;

    if (isNotSquat) {
        TVec3f playerFront;
        MR::getPlayerFrontVec(&playerFront);
        isNotSquat = playerFront.dot(mPosition - *MR::getPlayerPos()) > 0.0f;
    }

    if (isNotSquat) {
        return MR::isNearPlayer(this, 1500.0f);
    }

    return isNotSquat;
}

Teresa::~Teresa() {
}
