#include "Game/Boss/KoopaShockWave.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/PartsModel.hpp"

namespace {
    void makeShockWaveMtx(TPos3f* pDest, const TVec3f& rUp, const TVec3f& rFront, const TVec3f& r6, f32 f1, f32 f2) NO_INLINE {
        TVec3f localVec = rUp;
        localVec.scale(f1);
        TVec3f localVec2 = localVec;
        localVec2.scale(f2);
        TVec3f localVec3 = localVec2;
        localVec3.add(r6);
        MR::makeMtxUpFrontPos(pDest, rUp, rFront, localVec3);
    }
}  // namespace

namespace NrvKoopaShockWave {
    NEW_NERVE(KoopaShockWaveNrvWaveAttack, KoopaShockWave, WaveAttack);
}  // namespace NrvKoopaShockWave

KoopaShockWave::KoopaShockWave(Koopa* pKoopa)
    : LiveActor("球状衝撃波"), mKoopa(pKoopa), _90(), _C4(0.0f, 1.0f, 0.0f), _D0(0.0f, 0.0f, 1.0f), mPartsModel(), mShadow() {
    _94.identity();
    mBaseMtx.identity();
    _10C.identity();
}

KoopaShockWave::~KoopaShockWave() {
}

void KoopaShockWave::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("KoopaShockWave", nullptr, false);

    MR::startAction(this, "Spread");
    MR::setBckFrameAndStop(this, 0.0f);

    MR::connectToSceneEnemy(this);

    initHitSensor(1);
    MR::addHitSensorCallbackEnemyAttack(this, "Attack", 16, 15.0f);

    initSound(8, false);

    initNerve(&NrvKoopaShockWave::KoopaShockWaveNrvWaveAttack::sInstance);

    MR::invalidateClipping(this);

    PartsModel* pPartsModel = MR::createBloomModel(this, nullptr);
    mPartsModel = pPartsModel;
    MR::startAction(pPartsModel, "Spread");
    MR::setBckFrameAndStop(pPartsModel, 0.0f);

    mPartsModel->kill();
    MR::invalidateClipping(mPartsModel);

    ModelObj* pModelObj = MR::createModelObjNoSilhouettedMapObj("衝撃波の影", "KoopaShockWaveShadow", _10C);
    mShadow = pModelObj;
    MR::startAction(pModelObj, "Spread");
    MR::setBckFrameAndStop(pModelObj, 0.0f);

    MR::invalidateClipping(mShadow);
    mShadow->kill();

    makeActorDead();
}

void KoopaShockWave::appear() {
    LiveActor::appear();
    MR::invalidateHitSensors(this);

    _90 = 0.0f;

    setNerve(&NrvKoopaShockWave::KoopaShockWaveNrvWaveAttack::sInstance);
}

void KoopaShockWave::makeActorDead() {
    LiveActor::makeActorDead();
    mPartsModel->makeActorDead();
    mShadow->makeActorDead();
}

void KoopaShockWave::exeWaveAttack() {
    if (MR::isFirstStep(this)) {
        mPartsModel->makeActorAppeared();
        mShadow->makeActorAppeared();

        MR::startBrk(this, "Erase");
        MR::startBrk(mShadow, "Erase");
        MR::startBrk(mPartsModel, "Erase");

        MR::setBrkRate(this, 0.0f);
        MR::setBrkRate(mShadow, 0.0f);
        MR::setBrkRate(mPartsModel, 0.0f);

        MR::startAction(this, "Spread");
        MR::startAction(mShadow, "Spread");
        MR::startAction(mPartsModel, "Spread");

        TVec3f& rCenterPos = KoopaFunction::getPlanetCenterPos(mKoopa);
        _C4.sub(mKoopa->mPosition, rCenterPos);
        _D0.set(mKoopa->mFront);

        MR::makeMtxUpFrontPos(&_94, nullptr, mKoopa->mFront, nullptr);

        MR::startSound(mKoopa, "SE_BM_KOOPA_SWAVE_SHOOT", -1, -1);

        MR::validateHitSensors(this);
    }

    _90 = MR::calcNerveValue(this, 360, 0.0f, 180.0f);

    makeShockWaveMtx(&mBaseMtx, _C4, _D0, KoopaFunction::getPlanetCenterPos(mKoopa), JMath::sSinCosTable.get(0.0f), 1300.0f);

    makeShockWaveMtx(&_10C, _C4, _D0, KoopaFunction::getPlanetCenterPos(mKoopa), JMath::sSinCosTable.get(0.0f), 1260.0f);

    MR::setBckFrameAndStop(this, JMath::sSinCosTable.get(_90));
    MR::setBckFrameAndStop(mPartsModel, JMath::sSinCosTable.get(_90));
    MR::setBckFrameAndStop(mShadow, JMath::sSinCosTable.get(_90));

    MR::startLevelSound(mKoopa, "SE_BM_LV_KOOPA_SWAVE_MOVE", 90.0f, -1, -1);

    if (MR::isStep(this, 350)) {
        MR::startBrk(this, "Erase");
        MR::startBrk(mPartsModel, "Erase");
        MR::startBrk(mShadow, "Erase");
    }

    if (MR::isStep(this, 360)) {
        kill();
    }
}

void KoopaShockWave::updateHitSensor(HitSensor* pSensor) {
    TVec3f* localVec = MR::getPlayerPos();
    MR::calcLocalVec(localVec, _94);

    f32 degree = JMath::sAtanTable.atan2_(-localVec->z, localVec->x);

    TPos3f matrix = _94;
    matrix.setTrans(KoopaFunction::getPlanetCenterPos(mKoopa));

    TVec3f rotateVec;
    rotateVec.set(KoopaFunction::getPlanetRadius(mKoopa), 0.0f, 0.0f);
    MR::rotateVecDegree(&rotateVec, TVec3f(0.0f, 0.0f, 1.0f), _90);
    MR::rotateVecDegree(&rotateVec, TVec3f(0.0f, 1.0f, 0.0f), degree);

    matrix.mult(rotateVec, rotateVec);
    MR::setSensorPos(pSensor, rotateVec);
}

void KoopaShockWave::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
}

void KoopaShockWave::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::isOnGroundPlayer() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        mKoopa->receiveMessage(ACTMES_KOOPA_PLAYER_DAMAGE, pSender, MR::getMessageSensor());
    }
}
