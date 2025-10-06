#include "Game/MapObj/BeamGoRoundPlanet.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

BeamGoRoundBeam::BeamGoRoundBeam(MtxPtr mtx) : LiveActor("ビームゴーラウンドビーム") {
    mModelDrawer = nullptr;
    mBloomModel = nullptr;
    mBeamJointMtx = mtx;
}

void BeamGoRoundBeam::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("BeamGoRoundBeam", nullptr, false);
    mPosition.set<f32>(mBeamJointMtx[0][3], mBeamJointMtx[1][3], mBeamJointMtx[2][3]);
    MR::connectToScene(this, 0x22, 0x5, 0x19, 0x1C);
    initHitSensor(1);
    MR::addHitSensorCallbackEnemyAttack(this, "beam", 4, 100.0f);
    initEffectKeeper(0, nullptr, false);
    mModelDrawer = new VolumeModelDrawer("ビームボリューム", "BeamGoRoundBeamVolume", getBaseMtx());
    mBloomModel = MR::createModelObjBloomModel("ビームブルーム", "BeamGoRoundBeamBloom", getBaseMtx());
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::startAllAnim(this, "BeamGoRoundBeam");
    makeActorAppeared();
}

void BeamGoRoundBeam::calcAndSetBaseMtx() {
    mPosition.set<f32>(mBeamJointMtx[0][3], mBeamJointMtx[1][3], mBeamJointMtx[2][3]);
    MR::setBaseTRMtx(this, mBeamJointMtx);
}

void BeamGoRoundBeam::draw() const {
    mModelDrawer->draw();
}

void BeamGoRoundBeam::updateHitSensor(HitSensor *pSensor) {
    f32 radius = pSensor->mRadius;
    TVec3f up;
    MR::calcUpVec(&up, this);
    TVec3f v7;
    JMAVECScaleAdd(&up, &mPosition, &v7, radius);
    TVec3f v6;
    JMAVECScaleAdd(&up, &v7, &v6, (2700.0f - radius));
    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), v7, v6);
}

void BeamGoRoundBeam::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensorEnemyAttack(pSender)) {
            MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
        }
    }
}

namespace NrvBeamGoRoundPlanet {
    NEW_NERVE(BeamGoRoundPlanetNrvWait, BeamGoRoundPlanet, Wait);
};

BeamGoRoundPlanet::BeamGoRoundPlanet(const char *pName) : MapObjActor(pName) {
    mBeams = nullptr;
    _C8 = nullptr;
    _CC = nullptr;
}

void BeamGoRoundPlanet::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupFarClipping(-1.0f);
    info.setupNerve(&NrvBeamGoRoundPlanet::BeamGoRoundPlanetNrvWait::sInstance);
    initialize(rIter, info);
    _C8 = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("body"), MR::getJointMtx(this, "move1"), (MR::CollisionScaleType)2);
    _CC = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("body"), MR::getJointMtx(this, "move2"), (MR::CollisionScaleType)2);
    MR::validateCollisionParts(_CC);
    MR::validateCollisionParts(_C8);
    initBeam();
}

void BeamGoRoundPlanet::exeWait() {
    _CC->setMtx();
    _C8->setMtx();
    MR::startLevelSound(this, "SE_OJ_LV_BEAM_GO_ROUND", -1, -1, -1);
}

void BeamGoRoundPlanet::connectToScene(const MapObjActorInitInfo &) {
    MR::connectToScenePlanet(this);
}

void BeamGoRoundPlanet::initBeam() {
    mBeams = new BeamGoRoundBeam*[16];
    for (s32 i = 0; i < 16; i++) {
        char buf[256];
        snprintf(buf, sizeof(buf), "beam%d", i + 1);
        MtxPtr jointMtx = MR::getJointMtx(this, buf);
        mBeams[i] = new BeamGoRoundBeam(jointMtx);
        mBeams[i]->initWithoutIter();
    }
}

BeamGoRoundBeam::~BeamGoRoundBeam() {

}

BeamGoRoundPlanet::~BeamGoRoundPlanet() {
    
}
