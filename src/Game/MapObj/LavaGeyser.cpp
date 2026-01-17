#include "Game/MapObj/LavaGeyser.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

#define DEG_TO_RAD_0_1 0.017453292f

namespace NrvLavaGeyser {
    NEW_NERVE(LavaGeyserNrvWaitSwitch, LavaGeyser, WaitSwitch);
    NEW_NERVE(LavaGeyserNrvWait, LavaGeyser, Wait);
}  // namespace NrvLavaGeyser

LavaGeyser::LavaGeyser(const char* pName)
    : LiveActor(pName), _8c(180), _90(180), _94(0.0f, 0.0f, 0.0f), _A0(0.0f, 1.0f, 0.0f), _B0(0.0f), _AC(0.0f), _B4(0.0f), _B8(0.0f) {
}  // last floats are loaded out of order, need to figure out why

void LavaGeyser::init(const JMapInfoIter& iter) {
    MR::initDefaultPos(this, iter);
    //TODO: Fix register mismatch here
    TQuat4f quat1;
    f32 rotX = mRotation.x;
    f32 rotY = mRotation.y;
    f32 rotZ = mRotation.z;
    quat1.setEuler(DEG_TO_RAD_0_1 * rotX, DEG_TO_RAD_0_1 * rotY, DEG_TO_RAD_0_1 * rotZ);
    quat1.getYDir(_A0);
    MR::getJMapInfoArg0NoInit(iter, &_8c);
    MR::getJMapInfoArg1NoInit(iter, &_90);
    MR::useStageSwitchReadA(this, iter);
    initModelManagerWithAnm("LavaGeyser", nullptr, false);  // LavaGeyser string is stored externally in LavaGalaxyParts
    MR::startBtk(this, "LavaGeyser");
    MR::setBtkFrameAtRandom(this);
    MR::hideModel(this);
    MR::connectToSceneMapObj(this);
    initHitSensor(3);  // someday, I'd like to know what that three means
    MR::addHitSensorCallbackMapObj(this, "attack", 1, mScale.y * 100.0f);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setGroupClipping(this, iter, 16);
    if (MR::isValidSwitchA(this)) {
        initNerve(&NrvLavaGeyser::LavaGeyserNrvWaitSwitch::sInstance);
    } else {
        initNerve(&NrvLavaGeyser::LavaGeyserNrvWait::sInstance);
    }
    makeActorAppeared();
}
