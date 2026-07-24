#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/KoopaBattleMapCoinPlate.hpp"
#include "Game/Map/KoopaBattleMapDamagePlate.hpp"
#include "Game/Map/KoopaBattleMapPlate.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void FORCE_MATCH_KOOPABATTLEMAPPLANET_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)100.0f;
    (void)1180.0f;
    (void)1200.0f;
    (void)1230.0f;
    (void)0.995f;
}

namespace {
    static const f32 sRotateAccelWeak = 0.0f;    // ?
    static const f32 sRotateAccelStrong = 0.0f;  // ?
    static const f32 sRotateSpeedMax = 0.0f;     // ?
    static const f32 sRotateFriction = 0.995f;
};  // namespace

namespace NrvKoopaBattleMapPlanet {
    NEW_NERVE(KoopaBattleMapPlanetNrvWait, KoopaBattleMapPlanet, Wait);
    NEW_NERVE(KoopaBattleMapPlanetNrvDemoGrandStarAppear, KoopaBattleMapPlanet, DemoGrandStarAppear);
};  // namespace NrvKoopaBattleMapPlanet

KoopaBattleMapPlanet::~KoopaBattleMapPlanet() {
}

KoopaBattleMapPlanet::KoopaBattleMapPlanet(const char* pName, const char* pModelName, bool b1, bool b2, bool b3)
    : LiveActor(pName), mModelName(pModelName), _C0(0.0f, 0.0f, 0.0f), mDamagePlateGroup(), mCoinPlateGroup(), mPlateGroup(), mDamagePlate(), _DC(b1), _DD(b2), _DE(b3) {
    mBaseMtx.identity();
}

void KoopaBattleMapPlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::makeMtxTR(mBaseMtx, this);

    initModelManagerWithAnm(mModelName, nullptr, false);
    MR::tryStartAllAnim(this, mModelName);

    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::initCollisionParts(this, mModelName, MR::addBodyMessageSensorMapObj(this), nullptr);
    initEffectKeeper(0, nullptr, false);

    f32 boundingRadius = 0.0f;
    MR::calcModelBoundingRadius(&boundingRadius, this);
    MR::setClippingTypeSphere(this, 100.0f + boundingRadius);
    MR::setClippingFarMax(this);

    initNerve(&NrvKoopaBattleMapPlanet::KoopaBattleMapPlanetNrvWait::sInstance);

    if (!_DE) {
        createPlate();
    }

    MR::registerDemoSimpleCastAll(this);

    if (!_DD && !_DE) {
        MR::registerDemoSimpleCastAll(MR::createBloomModel(this, nullptr));
    }

    makeActorAppeared();
}

void KoopaBattleMapPlanet::startAnimPowerStarAppear() {
    setNerve(&NrvKoopaBattleMapPlanet::KoopaBattleMapPlanetNrvDemoGrandStarAppear::sInstance);
}

void KoopaBattleMapPlanet::exeWait() {
    if (!MR::isFirstStep(this)) {
        return;
    }

    MR::startBrk(this, "Wait");
}

void KoopaBattleMapPlanet::exeDemoGrandStarAppear() {
    if ((!_DD && MR::isStep(this, 90)) || (_DD && MR::isStep(this, 151))) {
        MR::startBrk(mDamagePlate, "PowerStarAppear");
        MR::emitEffect(mDamagePlate, "GrandStarAppear");
    }
}

void KoopaBattleMapPlanet::createPlate() {
    mDamagePlateGroup = new LiveActorGroup("ダメージプレートＧ", 6);
    mDamagePlateGroup->initWithoutIter();

    for (s32 idx = 0; idx < 6; idx++) {
        f32 angles[6][2] = {{0.0f, 0.0f}, {180.0f, 0.0f}, {90.0f, 0.0f}, {270.0f, 0.0f}, {0.0f, 90.0f}, {0.0f, 270.0f}};
        TVec3f vec(0.0f, 1180.0f, 0.0f);
        MR::rotateVecDegree(&vec, TVec3f(0.0f, 0.0f, 1.0f), angles[idx][0]);
        MR::rotateVecDegree(&vec, TVec3f(1.0f, 0.0f, 0.0f), angles[idx][1]);

        mBaseMtx.mult(vec, vec);

        KoopaBattleMapDamagePlate* pDamagePlate = new KoopaBattleMapDamagePlate("クッパダメージプレート");
        pDamagePlate->mPosition.set(vec);
        pDamagePlate->initWithoutIter();

        mDamagePlateGroup->registerActor(pDamagePlate);
    }

    if (_DD) {
        mDamagePlate = mDamagePlateGroup->getActor(0);
    } else {
        mDamagePlate = mDamagePlateGroup->getActor(1);
    }

    if (_DD) {
        return;
    }

    mCoinPlateGroup = new LiveActorGroup("コインプレートＧ", 4);
    mCoinPlateGroup->initWithoutIter();

    for (s32 idx = 0; idx < 4; idx++) {
        f32 angles[4][2] = {{55.0f, 45.0f}, {125.0f, 135.0f}, {55.0f, 225.0f}, {125.0f, 315.0f}};
        TVec3f vec(0.0f, 1200.0f, 0.0f);
        MR::rotateVecDegree(&vec, TVec3f(0.0f, 0.0f, 1.0f), angles[idx][0]);
        MR::rotateVecDegree(&vec, TVec3f(0.0f, 1.0f, 0.0f), angles[idx][1]);

        mBaseMtx.mult(vec, vec);

        KoopaBattleMapCoinPlate* pCoinPlate = new KoopaBattleMapCoinPlate("クッパコインプレート");
        pCoinPlate->mPosition.set(vec);
        pCoinPlate->initWithoutIter();

        mCoinPlateGroup->registerActor(pCoinPlate);
    }

    mPlateGroup = new LiveActorGroup("プレートＧ", 4);
    mPlateGroup->initWithoutIter();

    for (s32 idx = 0; idx < 4; idx++) {
        f32 angles[4][2] = {{125.0f, 45.0f}, {55.0f, 135.0f}, {125.0f, 225.0f}, {55.0f, 315.0f}};
        TVec3f vec(0.0f, 1230.0f, 0.0f);
        MR::rotateVecDegree(&vec, TVec3f(0.0f, 0.0f, 1.0f), angles[idx][0]);
        MR::rotateVecDegree(&vec, TVec3f(0.0f, 1.0f, 0.0f), angles[idx][1]);

        mBaseMtx.mult(vec, vec);

        KoopaBattleMapPlate* pCoinPlate = new KoopaBattleMapPlate("クッパプレート");
        pCoinPlate->mPosition.set(vec);
        pCoinPlate->initWithoutIter();

        mPlateGroup->registerActor(pCoinPlate);
    }
}

void KoopaBattleMapPlanet::control() {
    MR::rotateMtxMoment(&mBaseMtx, _C0);
    _C0 *= ::sRotateFriction;
}

void KoopaBattleMapPlanet::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}
