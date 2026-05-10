#include "Game/MapObj/SpiderMapBlock.hpp"

namespace {
    static const f32 sCoinSpeed = 35.0f;
}

SpiderMapBlock::SpiderMapBlock(const char* pName) : LiveActor(pName), mPosZ(0.0f) {
}

bool SpiderMapBlock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        kill();
        return true;
    }
    return false;
}

bool SpiderMapBlock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    kill();
    return true;
}

SpiderMapBlock::~SpiderMapBlock() {
}

void SpiderMapBlock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mPosZ = mPosition.z;
    initModelManagerWithAnm("SpiderMapBlock", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "SpiderMapBlock", getSensor("body"), nullptr);
    initEffectKeeper(8, nullptr, false);
    initSound(4, false);
    MR::declareCoin(this, 1);
    makeActorAppeared();
}

void SpiderMapBlock::kill() {
    MR::startSound(this, "SE_OJ_ROCK_BREAK", -1, -1);
    MR::emitEffect(this, "Break");
    MtxPtr baseMtx = getBaseMtx();
    TVec3f coinVel(baseMtx[0][1], baseMtx[1][1], 0.0f);
    MR::normalizeOrZero(&coinVel);
    coinVel.mult(sCoinSpeed);
    TVec3f coinPos(mPosition.x, mPosition.y, mPosZ);
    MR::appearCoinToVelocity(this, coinPos, coinVel, 1);
    LiveActor::kill();
}
