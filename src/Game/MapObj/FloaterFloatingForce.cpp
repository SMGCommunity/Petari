#include "Game/MapObj/FloaterFloatingForce.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"

FloaterFloatingForce::~FloaterFloatingForce() {
}

FloaterFloatingForce::FloaterFloatingForce(LiveActor* pHost, const char* pName) : MapPartsFunction(pHost, "浮力"), _18(pName), _1C(gZeroVec) {
    _28 = TVec3f(0.0f, 1.0f, 0.0f);
    mMoveConditionType = 0.0f;
}

void FloaterFloatingForce::init(const JMapInfoIter& rIter) {
    s32 moveConditionType = 0;
    MR::getMapPartsArgMoveConditionType(&moveConditionType, rIter);
    mMoveConditionType = moveConditionType;
}

void FloaterFloatingForce::start() {
    _1C.set(mHost->mPosition);

    TPos3f mtx;
    mtx.set(mHost->getBaseMtx());
    mtx.getYDir(_28);
    MR::normalize(&_28);
}
