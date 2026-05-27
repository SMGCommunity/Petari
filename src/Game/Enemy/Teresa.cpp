#include "Game/Enemy/Teresa.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/MapObj/KeySwitch.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace {
    static Color8 sPointLightColor(255, 255, 255, 255);
};

namespace NrvTeresa {
    NEW_NERVE(TeresaNrvAppearFromWall, Teresa, AppearFromWall);
    NEW_NERVE(TeresaNrvAppearFromGround, Teresa, AppearFromGround);
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
    NEW_NERVE(TeresaNrvDrift, Teresa, Drift);
    NEW_NERVE(TeresaNrvHideWall, Teresa, HideWall);
    NEW_NERVE(TeresaNrvHideWater, Teresa, HideWater);
    NEW_NERVE(TeresaNrvAscension, Teresa, Ascension);
    NEW_NERVE(TeresaNrvStop, Teresa, Stop);
};  // namespace NrvTeresa

Teresa::Teresa(const char* pName)
    : LiveActor(pName), mDisplayModel(nullptr), mKeySwitch(nullptr), _94(0.0f, 0.0f, 0.0f, 1.0f), _A4(0, 0, 1), _B0(0, 0, 0), _BC(0, 0, 1),
      _C8(0, 0, 0), _D4(1, 1, 1), _E0(0, 0, 0) {
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
    _B0.set< f32 >(mPosition);

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
        MR::makeQuatAndFrontFromRotate(&_94, &_A4, this);
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

// ...

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
        _B0.set< f32 >(mPosition);
        setNerve(&NrvTeresa::TeresaNrvAppearFromGround::sInstance);
        break;
    case 1:
        MR::invalidateClipping(this);
        MR::onBind(this);
        _B0.set< f32 >(mPosition);
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
    MR::blendQuatUpFront(&_94, -mGravity, _A4, 0.1f, 0.2f);
    _FE = 0;

    if (_F4 > 0.89f) {
        MR::requestPointLight(this, TVec3f(mPosition), sPointLightColor, 1.0f, -1);
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

// ...

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
    if (pTriangle->mSensor->isType(73)) {
        return true;
    }

    if (MR::isWallCodeGhostThrough(pTriangle)) {
        _FE = 1;
        _BC.set< f32 >(*pTriangle->getNormal(0));
        return isNerve(&NrvTeresa::TeresaNrvDrift::sInstance);
    }

    return false;
}

// ...

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

// ...

Teresa::~Teresa() {
}
