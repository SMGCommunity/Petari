#include "Game/Player/MarioFrontStep.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Map/HitInfo.hpp"

MarioFrontStep::MarioFrontStep(MarioActor *pActor) : MarioState(pActor, 0x20) {}

bool Mario::doFrontStep() {
    if (getCurrentStatus() == 0x20) {
        return false;
    }
    if (getCurrentStatus() == 0x15) {
        return false;
    }
    if (checkWallCode("NotSideStep", false)) {
        return false;
    }
    if (checkWallCode("NoAction", false)) {
        return false;
    }
    if (mActor->_468.x != 0) {
        return false;
    }
    if (_1C._5) {
        return false;
    }
    if (mMovementStates._0) {
        return false;
    }
    if (getPlayerMode() == 5) {
        return false;
    }
    if (_960 == 2) {
        return false;
    }
    if (calcAngleD(_368) >= 30.0f) {
        return false;
    }
    if (mAirGravityVec.dot(_374) >= 0.25f) {
        return false;
    }
    Triangle stack_20 = Triangle();
    TVec3f* var = &mActor->_2A0;
    TVec3f stack_14 = mFrontVec * 200.0f;
    if (!MR::getFirstPolyOnLineToMap(nullptr, &stack_20, *var, stack_14)) {
        return false;
    }
    TVec3f stack_8 = -*MR::getNormal(&stack_20);
    setFrontVecKeepUp(stack_8);
    stopWalk();
    forceStopTornado();
    changeStatus(mFrontStep);
    return true;
}

bool MarioFrontStep::start() {
    changeAnimation("前壁ウエイト", (char*)nullptr);
    mActor->setBlendMtxTimer(10);
    getPlayer()->lockGroundCheck(this, true);
    return true;
}

bool MarioFrontStep::update() {
    getPlayer()->stopWalk();
    if (getPlayer()->_1C._5) {
        return false;
    }
    TVec3f* var = &mActor->_2A0;
    if (!MR::isExistMapCollision(*var, getFrontVec() * 200.0f)) {
        return false;
    }
    if (!(getStickP() < 0.1f)) {
        TVec3f padHorizontal(getWorldPadDir());
        MR::vecKillElement(padHorizontal, getPlayer()->getAirGravityVec(), &padHorizontal);
        MR::normalizeOrZero(&padHorizontal);
        TVec3f padSideways;
        float f1 = MR::vecKillElement(padHorizontal, -getPlayer()->mFrontVec, &padSideways);
        if (!(f1 < -0.866f) && f1 > 0.0f) {
            return false;
        }
    }
    if (checkTrgA()) {
        Mario* pMario = getPlayer();
        pMario->_74C = 0.0f;
        pMario->_750 = 0;
        pMario->_754 = 0;
        getPlayer()->setFrontVecKeepUp(-getPlayer()->getWallNorm());
        getPlayer()->tryJump();
        return false;
    }
    if (mActor->isRequestRush()) {
        getPlayer()->tryWallPunch();
        return false;
    }
    if (!getPlayer()->mMovementStates._1) {
        getPlayer()->tryDrop();
        return false;
    }
    return true;
}

bool MarioFrontStep::close() {
    stopAnimation("前壁ウエイト", (char*)nullptr);
    getPlayer()->lockGroundCheck(this, false);
    return true;
}

bool MarioFrontStep::postureCtrl(MtxPtr mtx) {
    TVec3f stack_2c;
    if (MR::vecBlendSphere(-getPlayer()->getAirGravityVec(), -getPlayer()->mFrontVec, &stack_2c, 0.25f) != 0) {
        MR::normalizeOrZero(&stack_2c);
    }
    else {
        stack_2c.setPS2(-getPlayer()->getAirGravityVec());
    }
    MR::makeMtxUpFront((TPos3f*)mtx, stack_2c, getPlayer()->mFrontVec);
    return true;
}
