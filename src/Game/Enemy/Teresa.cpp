#include "Game/Enemy/Teresa.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/MapObj/KeySwitch.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"

namespace NrvTeresa {
    static Color8 sPointLightColor(255, 255, 255, 255);
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

// ...

void Teresa::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::onCalcGravity(this);
}

Teresa::~Teresa() {
}
