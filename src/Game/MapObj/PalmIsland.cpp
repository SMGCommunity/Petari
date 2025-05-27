#include "Game/MapObj/PalmIsland.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "revolution/types.h"

namespace NrvPalmIsland {
    NEW_NERVE(PalmIslandNrvWait, PalmIsland, Wait);
    NEW_NERVE(PalmIslandNrvFloat, PalmIsland, Float);
}; // namespace NrvPalmIsland

PalmIsland::PalmIsland(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90.x = 0.0;
    _90.y = 0.0;
    _90.z = 0.0;
    _9C = 0;
}

PalmIsland::~PalmIsland() {}

void PalmIsland::init(const JMapInfoIter &rIter) {
    HitSensor* sensor;
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PalmIsland", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    sensor = getSensor("body");
    MR::initCollisionParts(this, "PalmIsland", sensor, nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    _8C = MR::getRandom(0l, 60);
    initNerve(&NrvPalmIsland::PalmIslandNrvWait::sInstance);
    makeActorAppeared();
}

inline void PalmIsland::exeWait() {
    if (MR::isStep(this, _8C)) {
        setNerve(&NrvPalmIsland::PalmIslandNrvFloat::sInstance);
    }
}

void PalmIsland::exeFloat() {
    if (MR::isFirstStep(this)) {
        _90.setPS2(mPosition);
        MR::emitEffect(this, "Ripple");
        MR::setEffectHostSRT(this, "Ripple", &_90, nullptr, nullptr);
    }
    f32 value = JMath::sSinCosTable.sinLapRad(90.0f + 1.44f * getNerveStep()) * 1.5f;
    bool temp = false;
    if (value > 0) {
        temp = true;
    }
    if (temp != _9C) {
        MR::startSound(this, "SE_OJ_PALM_ISLAND_WAVE", -1, -1);
    }
    _9C = temp;

    mVelocity.scale(value, mGravity);
}
