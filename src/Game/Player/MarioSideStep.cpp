#include "Game/Player/MarioSideStep.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioWall.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

bool Mario::checkWallJumpCode() {
    if (checkWallFloorCode(11) || checkWallFloorCode(6) || checkWallFloorCode(34) || checkWallFloorCode(7) || checkWallFloorCode(8)) {
        if (mMovementStates._1 || isAnimationRun(_728)) {
            stopWalk();
            push(getWallNorm() * 10.0f);
            tryPushToVelocity();
            mJumpVec.set(-getPlayer()->getWallNorm() * 10.0f);
            MR::vecKillElement(mJumpVec, getAirGravityVec(), &mJumpVec);
            mJumpVec -= getAirGravityVec() * 10.0f;
        }
        mWall->initTriangleJump();
        u32 jumpType = -1;
        if (checkWallFloorCode(7)) {
            jumpType = 1;
        }
        if (checkWallFloorCode(6)) {
            jumpType = 0;
        }
        if (checkWallFloorCode(34)) {
            jumpType = 2;
        }
        if (mWall->startBackJump(jumpType)) {
            return true;
        }
    }
    return false;
}

MarioSideStep::MarioSideStep(MarioActor* actor) : MarioState(actor, MarioStatus_SideStep) {
}

bool Mario::doSideStep() {
    if (getCurrentStatus() == MarioStatus_SideStep) {
        return false;
    }
    if (checkWallCode("NotSideStep", false)) {
        return false;
    }
    if (checkWallCode("NoAction", false)) {
        return false;
    }
    if (mActor->_468) {
        return false;
    }
    if (_1C._5) {
        return false;
    }
    if (_72C < 200.0f) {
        return false;
    }
    if (getPlayerMode() == PlayerMode_Hopper) {
        return false;
    }
    if (calcPolygonAngleD(mFrontWallTriangle) > 91.0f) {
        return false;
    }
    f32 angle = MR::diffAngleAbs(getFrontWallNorm(), _368);
    if (angle >= 1.6534699f || angle < 1.4959966f) {
        return false;
    }
    bool cannotSideStep = !isSlipFloorCode(_960);
    if (cannotSideStep) {
        return false;
    }
    if (_960 == 2) {
        return false;
    }
    if (!getPlayer()->mMovementStates._8) {
        return false;
    }
    const Triangle* wall = getPlayer()->getWallPolygon();
    if (!wall) {
        return false;
    }
    if (!MR::isSameMtx(wall->getBaseMtx()->toMtxPtr(), wall->getPrevBaseMtx()->toMtxPtr())) {
        return false;
    }
    stopWalk();
    forceStopTornado();
    changeStatus(mSideStep);
    return true;
}

bool MarioSideStep::start() {
    if (!isAnimationRun("壁押し")) {
        playSound("声壁押し");
    }
    changeAnimation("壁押し", 0UL);
    mActor->setBlendMtxTimer(10);
    return true;
}

bool MarioSideStep::close() {
    stopAnimation("壁ウエイト");
    stopAnimation("壁右歩き");
    stopAnimation("壁左歩き");
    stopAnimation("壁押し");
    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
    return true;
}

bool MarioSideStep::update() {
    if (!getPlayer()->mMovementStates._8) {
        return false;
    }
    if (getPlayer()->_1C._5) {
        return false;
    }
    if (getPlayer()->mDrawStates._A) {
        return false;
    }
    if (MR::isNormalTalking()) {
        return false;
    }
    if (mActor->_468) {
        return false;
    }
    Mario* player = getPlayer();
    bool cannotSideStep = !isSlipFloorCode(player->_960);
    if (cannotSideStep) {
        return false;
    }
    if (getStickP() < 0.1f) {
        changeAnimation("壁ウエイト", static_cast< const char* >(nullptr));
    } else {
        TVec3f direction(getWorldPadDir());
        MR::vecKillElement(direction, getPlayer()->getAirGravityVec(), &direction);
        MR::normalizeOrZero(&direction);
        TVec3f side;
        f32 forward = MR::vecKillElement(direction, getPlayer()->getWallNorm(), &side);
        if (forward < -0.866f) {
            if (!isAnimationRun("壁押し")) {
                playSound("声壁押し");
            }
            changeAnimation("壁押し", 0UL);
        } else if (forward > 0.0f) {
            if (forward > 0.707f) {
                getPlayer()->setFrontVecKeepUp(getPlayer()->getWallNorm());
            }
            getPlayer()->mMovementStates._8 = false;
            return false;
        } else {
            side *= (1.0f + 0.25f * forward) * getStickP();
            if (side.dot(getPlayer()->mSideVec) < 0.0f) {
                if (!isAnimationRun("壁右歩き")) {
                    changeAnimation("壁右歩き", static_cast< const char* >(nullptr));
                }
            } else if (!isAnimationRun("壁左歩き")) {
                changeAnimation("壁左歩き", static_cast< const char* >(nullptr));
            }
            addVelocity(side * 6.0f - getPlayer()->getWallNorm() * 6.0f);
        }
    }
    if (checkTrgA()) {
        getPlayer()->resetInline();
        getPlayer()->setFrontVecKeepUp(-getPlayer()->getWallNorm());
        getPlayer()->tryJump();
        return false;
    }
    if (mActor->isRequestRush()) {
        getPlayer()->tryWallPunch();
        return false;
    }
    if (!getPlayer()->mMovementStates._1) {
        getPlayer()->keepDistFrontWall();
        getPlayer()->tryDrop();
        return false;
    }
    return getPlayer()->fixWallingPosition(false) != false;
}
