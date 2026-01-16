#include "Game/MapObj/LavaGeyser.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

#define FLOAT_1 0.017453292f

LavaGeyser::LavaGeyser(const char* pName) : LiveActor(pName), _8c(180), _90(180), _94(0.0f, 0.0f, 0.0f), _A0(0.0f, 1.0f, 0.0f), _B0(0.0f), 
_AC(0.0f), _B4(0.0f), _B8(0.0f) {} //last floats are loaded out of order, need to figure out why

void LavaGeyser::init(const JMapInfoIter& iter) {
    MR::initDefaultPos(this, iter);
    TQuat4f quat1;
    quat1.setEuler(mRotation.x * FLOAT_1, mRotation.y * FLOAT_1, mRotation.z * FLOAT_1);
}
