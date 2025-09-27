#include "Game/Map/KoopaBattleMapPlate.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

KoopaBattleMapPlate::KoopaBattleMapPlate(const char* pName) :
    LiveActor(pName)
{
    _8C.identity();
}

void KoopaBattleMapPlate::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

KoopaBattleMapPlate::~KoopaBattleMapPlate() {
    
}

void KoopaBattleMapPlate::init(const JMapInfoIter& rIter) {
    TVec3f v1;

    MR::initDefaultPos(this, rIter);
    MR::calcGravity(this);
    JMathInlineVEC::PSVECNegate(&mGravity, &v1);
    MR::makeMtxUpNoSupportPos(&_8C, v1, mPosition);
    initModelManagerWithAnm("KoopaPlate", NULL, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensor(this, "Attack", 94, 8, 250.0f, TVec3f(0.0f, -150.0f, 0.0f));
    MR::initCollisionParts(this, "KoopaPlate", getSensor("Attack"), _8C);
    initEffectKeeper(1, NULL, false);
    MR::addEffectHitNormal(this, "Hit");
    MR::setEffectBaseScale(this, "Hit", 3.0f);
    initSound(4, false);
    MR::setClippingFarMax(this);
    MR::declareStarPiece(this, 5);
    makeActorAppeared();
}

bool KoopaBattleMapPlate::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (msg != ACTMES_KOOPA_HIP_DROP_ATTACK) {
        return false;
    }

    MR::emitEffectHit(this, MR::getSensorHost(pSender)->mPosition, "Hit");
    MR::startSound(this, "SE_OJ_KOOPA_PLATE_PLN_BRK", -1, -1);
    MR::appearStarPiece(this, mPosition, 5, 30.0f, 50.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_F", -1, -1);
    MR::emitEffect(this, "Break");
    kill();

    return true;
}
