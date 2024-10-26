#include "Game/MapObj/FloaterFloatingForce.hpp"
#include "Game/LiveActor/LiveActor.hpp"

FloaterFloatingForce::~FloaterFloatingForce() {

}

FloaterFloatingForce::FloaterFloatingForce(LiveActor *pActor, const char *pName) : MapPartsFunction(pActor, "浮力"), _18(pName), _1C(gZeroVec) {
    _28.setInlinePS(TVec3f(0.0f, 1.0f, 0.0f));
    mMoveCondition = 0.0f;
}

void FloaterFloatingForce::init(const JMapInfoIter &rIter) {
    s32 moveType = 0;
    MR::getMapPartsArgMoveConditionType(&moveType, rIter);
    mMoveCondition = moveType;
}

void FloaterFloatingForce::start() { 
    _1C.x = mHost->mPosition.x;
    _1C.y = mHost->mPosition.y;
    _1C.z = mHost->mPosition.z;
    TSMtxf matrix;
    f32 x, y, z;
    matrix.setInline_2(mHost->getBaseMtx());
    x = matrix.mMtx[0][1];
    y = matrix.mMtx[1][1];
    z = matrix.mMtx[2][1];
    _28.set(x, y, z);
    MR::normalize(&_28);
}

const TVec3f& FloaterFloatingForce::getCurrentVelocity() const {
    return TVec3f(0.0f, 0.0f, 0.0f);
}
