#include "Game/MapObj/SpiderMapBlock.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

SpiderMapBlock::SpiderMapBlock(const char* pName)
    : LiveActor(pName) {
    _8C = 0.0f;
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
    _8C = mPosition.z;
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
    TVec3f v4(baseMtx[0][1], baseMtx[1][1], 0.0f);
    MR::normalizeOrZero(&v4);
    v4.x *= 35.0f;
    v4.y *= 35.0f;
    v4.z *= 35.0f;
    TVec3f v3(mPosition.x, mPosition.y, _8C);
    MR::appearCoinToVelocity(this, v3, v4, 1);
    LiveActor::kill();
}
