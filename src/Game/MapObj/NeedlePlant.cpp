#include "Game/MapObj/NeedlePlant.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvNeedlePlant {
    NEW_NERVE(NeedlePlantNrvWait, NeedlePlant, Wait);
    NEW_NERVE(NeedlePlantNrvShake, NeedlePlant, Shake);
}; // namespace NrvNeedlePlant

NeedlePlant::NeedlePlant(const char *pName) : MapObjActor(pName) {
    _C4 = -1; 
}
NeedlePlant::~NeedlePlant() {}

void NeedlePlant::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupAffectedScale();
    info.setupHitSensor();
    TVec3f vec;
    vec.set(0.0f, 150 * mScale.x, 0.0f);
    info.setupHitSensorParam(4, 180.0f, vec);
    info.setupShadow(nullptr);
    info.setupNerve(&NrvNeedlePlant::NeedlePlantNrvWait::sInstance);
    initialize(rIter, info);
    initEffectKeeper(1, "NeedlePlant", false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, 0);
    TVec3f vec3_2; //Second Vector3f
    vec3_2.set(0.0f, 150.0f * mScale.x, 0.0f);
    MR::initStarPointerTarget(this, 150.0f * mScale.x, vec3_2);
    MR::getJMapInfoArg0NoInit(rIter, &_C4);

    if (_C4 == -1) {
        MR::declareCoin(this, 1);
    } else if (!_C4) {
        MR::declareStarPiece(this, 3);
    }
}
void NeedlePlant::exeShake() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        MR::startSound(this, "SE_OJ_LEAVES_SWING", -1, -1);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvNeedlePlant::NeedlePlantNrvWait::sInstance);
    }
}




void NeedlePlant::kill() {
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_NEEDLE_PLANT_BREAK", -1, -1);
    if (!_C4) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        MR::appearStarPiece(this, this->mPosition, 3, 10.0f, 40.0f, false);
    } else if (_C4 == -1) {
        MR::appearCoinPop(this, this->mPosition, 1);
    }

    MapObjActor::kill();
}

void NeedlePlant::attackSensor(HitSensor *a1, HitSensor *a2) {
    // TODO: Possible inline. https://decomp.me/scratch/zwlUm
    if (MR::calcDistance(a1, a2, nullptr) > 70.0f * mScale.x + a2->mRadius) {
        return;
    }
    if (MR::isSensorPlayerOrRide(a2) && MR::sendMsgEnemyAttack(a2, a1)) {
        MR::emitEffectHitBetweenSensors(this, a1, a2, 0.0f, nullptr);
        setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);
    } else if (MR::isSensorPlayerOrRide(a2) || MR::isSensorEnemy(a2)) {
        f32 mRadius;
        mRadius = getSensor(nullptr)->mRadius;
        getSensor(nullptr)->mRadius = 70.0f * mScale.x;
        MR::sendMsgPush(a2, a1);
        getSensor(nullptr)->mRadius = mRadius;
    }
}

bool NeedlePlant::receiveMsgPush(HitSensor *a1, HitSensor *a2) {
    if (isNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance)) {
        return false;
    }
    if (!MR::isSensorEnemy(a1)) {
        return false;
    }
    setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);
    return true;
}

bool NeedlePlant::receiveMsgPlayerAttack(u32 a1, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceReflect(a1)) {
        setNerve(&NrvNeedlePlant::NeedlePlantNrvShake::sInstance);
        return true;
    } else if (MR::isMsgInvincibleAttack(a1)) {
        kill();
        return true;
    } else {
        return false;
    }
}
bool NeedlePlant::receiveMsgEnemyAttack(u32 a1, HitSensor *a2, HitSensor *a3) {
    kill();
    return true;
}

void NeedlePlant::connectToScene(const MapObjActorInitInfo &) {
    MR::connectToSceneNoShadowedMapObj(this);
}

inline void NeedlePlant::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, mObjectName); 
    }
}
