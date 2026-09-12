#include "Game/Player/MarioWall.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include <cstring>

bool Mario::isWalling() const {
    return getCurrentStatus() == MarioStatus_Wall;
}

void Mario::checkWallStick() {
    if (getPlayerMode() == PlayerMode_Bee) {
        checkBeeStick();
        return;
    }

    if (!checkWallJumpCode() && isEnableStickWall()) {
        if (mActor->_334) {
            TVec3f horizontal;
            f32 vertical = MR::vecKillElement(mJumpVec, mActor->_240, &horizontal);
            horizontal.setLength(-0.25f);
            mJumpVec = mActor->_240 * vertical + horizontal;
            return;
        }

        changeStatus(mWall);
        stopWalk();
        mMovementStates.jumping = false;
        mMovementStates._B = false;
        mMovementStates._6 = false;
        mMovementStates._17 = false;
        fixWallingPosition(true);
        resetTornado();
    }
}

u8 Mario::checkStickWallSide() {
    if (isStickOn()) {
        const TVec3f& direction = getWorldPadDir();
        if (direction.dot(getWallNorm()) < -0.5f) {
            return 1;
        }
        if (direction.dot(getWallNorm()) > 0.5f && !mMovementStates._1D) {
            return 2;
        }
    }
    return 0;
}

s32 Mario::checkStickFrontBack() {
    if (isStickOn()) {
        const TVec3f& direction = getWorldPadDir();
        if (direction.dot(-mFrontVec) < -0.5f) {
            return 1;
        }
        if (direction.dot(-mFrontVec) > 0.5f && !mMovementStates._1D) {
            return 2;
        }
    }
    return 0;
}

MarioWall::MarioWall(MarioActor* actor) : MarioState(actor, MarioStatus_Wall) {
    _14 = 0;
    _18 = 0;
    _1C = 0;
    _1D = 0;
    _1E = 0;
    _20 = 0.0f;
    _24.zero();
    _30.zero();
}

void MarioWall::initTriangleJump() {
    Mario* player = getPlayer();
    MR::vecKillElement(player->mJumpVec, getGravityVec(), &_30);
    MR::normalizeOrZero(&_30);
    _14 = 0;
}

u8 MarioWall::isCancel() {
    if (getPlayer()->mMovementStates._1) {
        _1C = 0;
    }
    if (_1C && getPlayer()->mMovementStates._8) {
        if (_24.dot(getPlayer()->getWallNorm()) < mActor->getConst().getTable()->mWallStickCancelAngle) {
            _1C = 0;
        }
    }
    return _1C;
}

bool Mario::fixWallingPosition(bool immediate) {
    if (!fixWallingDist()) {
        return false;
    }
    fixWallingDir(immediate);
    return true;
}

void Mario::fixWallingDir(bool immediate) {
    bool back = true;
    if (isStatusActive(MarioStatus_SideStep)) {
        back = false;
    }
    if (mMovementStates._19 && back) {
        if (immediate) {
            setFrontVecKeepUp(-*mBackWallTriangle->getNormal(0));
            return;
        }
        setFrontVecKeepUp(-*mBackWallTriangle->getNormal(0), 0.1f);
        return;
    }
    if (mMovementStates._8) {
        if (immediate) {
            setFrontVecKeepUp(-*mFrontWallTriangle->getNormal(0));
            return;
        }
        setFrontVecKeepUp(-*mFrontWallTriangle->getNormal(0), 0.1f);
    }
}

bool Mario::fixWallingTop() {
    TVec3f side;
    PSVECCrossProduct(&getWallNorm(), &getAirGravityVec(), &side);
    MR::normalizeOrZero(&side);
    if (MR::isNearZero(side)) {
        return false;
    }
    PSVECCrossProduct(&getWallNorm(), &side, &_75C);
    MR::normalizeOrZero(&_75C);
    if (MR::isNearZero(_75C)) {
        return false;
    }
    getWallNorm().dot(getAirGravityVec());
    getPlayer()->forceSetHeadVecKeepSide(_75C);
    return true;
}

bool Mario::checkWallFloorCode(u16 code) const {
    if ((mMovementStates._8 || mMovementStates._32) && _964[0] == code) {
        return true;
    }
    if (mMovementStates._19 && _964[1] == code) {
        return true;
    }
    if (mMovementStates._1A && _964[2] == code) {
        return true;
    }
    return false;
}

bool Mario::checkWallCode(const char* code, bool frontBackOnly) const {
    if (mMovementStates._19) {
        const char* wallCode = MR::getWallCodeString(mBackWallTriangle);
        if (wallCode && strcmp(wallCode, code) == 0) {
            return true;
        }
    }
    if (mMovementStates._8 || mMovementStates._32) {
        const char* wallCode = MR::getWallCodeString(mFrontWallTriangle);
        if (wallCode && strcmp(wallCode, code) == 0) {
            return true;
        }
    }
    if (frontBackOnly) {
        return false;
    }
    if (mMovementStates._1A) {
        const char* wallCode = MR::getWallCodeString(mSideWallTriangle);
        if (wallCode && strcmp(wallCode, code) == 0) {
            return true;
        }
    }
    return false;
}

bool Mario::checkWallCodeNorm(u16 code, TVec3f* normal, bool frontBackOnly) const {
    if (mMovementStates._19) {
        u16 wallCode = MR::getWallCodeIndex(mBackWallTriangle);
        if (wallCode == code) {
            if (normal) {
                *normal = *mBackWallTriangle->getNormal(0);
            }
            return true;
        }
    }
    if (mMovementStates._8 || mMovementStates._32) {
        u16 wallCode = MR::getWallCodeIndex(mFrontWallTriangle);
        if (wallCode == code) {
            if (normal) {
                *normal = *mFrontWallTriangle->getNormal(0);
            }
            return true;
        }
    }
    if (frontBackOnly) {
        return false;
    }
    if (mMovementStates._1A) {
        u16 wallCode = MR::getWallCodeIndex(mSideWallTriangle);
        if (wallCode == code) {
            if (normal) {
                *normal = *mSideWallTriangle->getNormal(0);
            }
            return true;
        }
    }
    return false;
}

void Mario::setWallCancel() {
    mWall->_1C = 1;
    mWall->_24 = getWallNorm();
}

void Mario::keepDistFrontWall() {
    if (mMovementStates._8) {
        TVec3f horizontal;
        MR::vecKillElement(_4E8 - mGroundPos, *getGravityVec(), &horizontal);
        if (horizontal.length() < 80.0f) {
            push(*mFrontWallTriangle->getNormal(0) * (80.0f - horizontal.length()));
        }
    }
}

bool Mario::isEnableStickWall() {
    switch (getPlayerMode()) {
    case PlayerMode_Teresa:
        return false;
    case PlayerMode_Foo:
        if (isStatusActive(MarioStatus_Foo)) {
            return false;
        }
        break;
    }

    if (mWall->isCancel()) {
        return false;
    }
    if (!mMovementStates.jumping || mMovementStates._1) {
        return false;
    }
    if (mMovementStates._B) {
        return false;
    }
    if (mMovementStates._F) {
        return false;
    }
    if (mDrawStates._1E) {
        return false;
    }
    if (!mMovementStates._8 && !mMovementStates._19) {
        return false;
    }
    if (mMovementStates._2 && mVerticalSpeed < mActor->getConst().getTable()->mWallStickGrHeight) {
        return false;
    }
    if (mMovementStates._15 && _4E0 < mActor->getConst().getTable()->mWallStickFrHeight && !getWallPolygon()->mSensor->isType(0x57)) {
        return false;
    }
    if (isAnimationRun("空中ひねり")) {
        if (mActor->_945 < 25) {
            return false;
        }
    } else if (mActor->isPunching() && mActor->_945 < 15) {
        return false;
    }
    if (mMovementStates._15 && mMovementStates._2 && mMovementStates._39) {
        if ((_4A4 - mShadowPos).dot(-*getGravityVec()) < mActor->getConst().getTable()->mWallStickStepHeight) {
            return false;
        }
    }

    TVec3f horizontal;
    if (mMovementStates._19) {
        MR::vecKillElement(mJumpVec, mActor->_240, &horizontal);
        if (mBackWallTriangle->getNormal(0)->dot(horizontal) > 0.0f) {
            return false;
        }
    }
    if (isInhibitWall()) {
        return false;
    }
    if (mMovementStates._8 && calcPolygonAngleD(mFrontWallTriangle) < 80.0f) {
        return false;
    }
    if (mMovementStates._19 && calcPolygonAngleD(mBackWallTriangle) < 80.0f) {
        return false;
    }
    TVec3f side;
    PSVECCrossProduct(&getWallNorm(), &getAirGravityVec(), &side);
    if (MR::normalizeOrZero(&side)) {
        return false;
    }
    if (!checkWallCode("NotWallSlip", true)) {
        mMovementStates._28 = true;
    }
    if (checkWallCode("NoAction", true)) {
        return false;
    }
    if ((getPlayerMode() != PlayerMode_Ice || !getWallPolygon()->mSensor->isType(0x57)) && isRising()) {
        return false;
    }
    if (mMovementStates._19) {
        if (!isAnimationRun("壁ジャンプ") && getPlayer()->checkStickWallSide() != 1) {
            return false;
        }
    } else if (!isAnimationRun("壁ジャンプ") && !mMovementStates._9 && getPlayer()->checkStickWallSide() != 1) {
        return false;
    }
    if (mMovementStates._30) {
        return false;
    }
    if (mActor->_468) {
        if (!checkTrgA()) {
            return false;
        }
        mWall->initTriangleJump();
        mWall->startJump();
        return false;
    }
    TVec3f position(mActor->_2AC);
    f32 distance = 120.0f;
    if (mMovementStates._19) {
        distance = -120.0f;
    }
    if (!MR::isExistMapCollision(position, mFrontVec * distance)) {
        return false;
    }
    return MR::getKarikariClingNum() == 0;
}

bool MarioWall::start() {
    _18 = 0;
    _20 = 0.0f;
    changeAnimation("壁くっつき", static_cast< const char* >(nullptr));
    startPadVib(0UL);
    getPlayer()->mMovementStates._28 = false;
    getPlayer()->_20._28 = false;
    _1D = 0;
    initTriangleJump();
    _1E = 0;

    if (getPlayerMode() == PlayerMode_Ice && getPlayer()->getWallPolygon()->mSensor->isType(0x57)) {
        MarioActor* actor = mActor;
        actor->createIceWall(getPlayer()->getWallPos() - getPlayer()->getWallNorm() * 30.0f, getPlayer()->getWallNorm());
        playEffectRT("氷壁ジャンプ", getPlayer()->getWallNorm(), getPlayer()->getWallPos() - getPlayer()->getWallNorm() * 15.0f);
        playSound("スケート着地");
        _1E = 1;
    }
    return true;
}

bool MarioWall::update() {
    bool release = false;
    if (mActor->_334) {
        return false;
    }
    if (MR::getKarikariClingNum()) {
        return false;
    }
    if ((isStatusActiveID(MarioStatus_Rabbit) || mActor->_37C - getPlayer()->_558 < 6) && startJump()) {
        return false;
    }
    if (getPlayer()->mDrawStates._6) {
        if (_1D) {
            TVec3f horizontal;
            MarioActor* actor = mActor;
            if (MR::vecKillElement(actor->getLastMove(), getGravityVec(), &horizontal) < 1.0f) {
                release = true;
            }
        }
        getPlayer()->mMovementStates._1 = true;
        _1D = 1;
    } else {
        _1D = 0;
    }
    _14++;
    if (getPlayer()->mMovementStates._1 && !release) {
        if (getPlayer()->mVerticalSpeed < 80.0f) {
            if (!isOnSlipGround()) {
                getPlayer()->setFrontVecKeepUp(getPlayer()->getWallNorm());
                changeAnimation("着地", static_cast< const char* >(nullptr));
                changeAnimationInterpoleFrame(1);
                mActor->setBlendMtxTimer(4);
            }
            release = true;
        } else {
            getPlayer()->mMovementStates._1 = false;
        }
    }
    if (_14 >= mActor->getConst().getTable()->mWallReleaseTime + mActor->getConst().getTable()->mWallStickTime) {
        release = true;
    }
    if (_14 >= 3 && !getPlayer()->mMovementStates._8 && !getPlayer()->mMovementStates._32) {
        release = true;
    }
    if (release) {
        if (!getPlayer()->mMovementStates._1) {
            _1C = 1;
            TVec3f normal(getPlayer()->getWallNorm());
            _24 = normal;
            getPlayer()->setFrontVecKeepUp(-normal);
        }
        if (isOnSlipGround()) {
            addVelocityAfter(getPlayer()->getWallNorm() * 10.0f);
        } else {
            addVelocityAfter(getPlayer()->getWallNorm() * 30.0f);
        }
        return false;
    }
    if (checkTrgA() && startJump()) {
        if (_1E) {
            playSound("スケートジャンプ");
        }
        return false;
    }
    if (mActor->isRequestRush()) {
        getPlayer()->mMovementStates._2B = false;
        changeAnimation("空中ひねり", static_cast< const char* >(nullptr));
        getPlayer()->tryWallPunch();
        getPlayer()->setWallCancel();
        return false;
    }
    if (!getPlayer()->fixWallingPosition(false)) {
        getPlayer()->setWallCancel();
        return false;
    }

    f32 blend = 0.9f;
    f32 speed = mActor->getConst().getTable()->mWallDropSpeedNormal;
    u8 side = getPlayer()->checkStickWallSide();
    if (side == 1) {
        side = 0;
    }
    switch (side) {
    case 1:
        blend = 0.7f;
        if (_14 > mActor->getConst().getTable()->mWallStickTime) {
            _14 = mActor->getConst().getTable()->mWallStickTime;
        }
        _18 = 0;
        speed = mActor->getConst().getTable()->mWallDropSpeedStop;
        changeAnimation("壁くっつき", static_cast< const char* >(nullptr));
        stopEffect("共通壁手擦り");
        break;
    case 2:
        if (_1E) {
            if (_14 < mActor->getConst().getTable()->mWallStickTimeIce - 15) {
                _14 = mActor->getConst().getTable()->mWallStickTimeIce - 15;
            }
        } else if (_14 < 165) {
            _14 = 165;
        }
        if (!_18) {
            _18 = 1;
        }
        stopEffect("共通壁手擦り");
        break;
    case 0:
        if (_14 < mActor->getConst().getTable()->mWallStickTime) {
            stopEffect("共通壁手擦り");
        } else {
            if (!isAnimationRun("壁くっつき")) {
                changeAnimation("壁すべり", static_cast< const char* >(nullptr));
            }
            playSound("スリップ");
            playEffect("共通壁手擦り");
        }
        break;
    }
    if (_18) {
        _18++;
    }
    if (_14 < mActor->getConst().getTable()->mWallStickTime) {
        blend = 1.0f;
    }
    if (_1E) {
        if (_14 > mActor->getConst().getTable()->mWallStickTimeIce) {
            _1C = 1;
            _24 = getPlayer()->getWallNorm();
            return false;
        }
        return true;
    }

    _20 = _20 * blend + speed * (1.0f - blend);
    addVelocity(getPlayer()->_75C, -_20);
    if (side <= 1) {
        f32 ratio;
        if (_14 < mActor->getConst().getTable()->mWallStickTime) {
            ratio = 0.0f;
        } else {
            ratio = 1.0f - (_14 - mActor->getConst().getTable()->mWallStickTime) * mActor->getConst().getTable()->mWallSideMoveRatio;
        }
        getPlayer()->moveWallSlide(MR::clamp(ratio, 0.0f, 1.0f));
    }
    return true;
}

bool MarioWall::close() {
    stopAnimation("壁くっつき");
    stopAnimation("壁すべり");
    if (getPlayer()->mMovementStates._1) {
        changeAnimation(nullptr, "基本");
    }
    stopEffect("共通壁手擦り");
    getPlayer()->resetTornado();
    getPlayer()->mMovementStates._38 = false;
    return true;
}

bool MarioWall::startJump() {
    if (getPlayer()->isInhibitWall()) {
        return false;
    }
    TVec3f velocity(getPlayer()->getWallNorm());
    _24 = velocity;
    if (!getPlayer()->mDrawStates._3 && _14 < 15 && !MR::isNearZero(_30)) {
        f32 dot = velocity.dot(-_30);
        if (dot < mActor->getConst().getTable()->mWallTriJumpMargin && dot > 0.0f) {
            Mtx rotation;
            TVec3f axis;
            axis.cross(-_30, velocity);
            PSMTXRotAxisRad(rotation, &axis, MR::acos(dot));
            PSMTXMultVec(rotation, &velocity, &velocity);
        }
    }
    velocity.scale(mActor->getConst().getTable()->mWallJumpPowerXZ);
    velocity += getGravityVec() * -mActor->getConst().getTable()->mWallJumpPowerY;
    getPlayer()->tryWallJump(velocity, true);
    playEffect("共通壁ジャンプ");
    _1C = 1;
    getPlayer()->mMovementStates._2B = false;
    return true;
}

bool MarioWall::startBackJump(u32 type) {
    if (getPlayer()->isInhibitWall()) {
        return false;
    }
    TVec3f velocity(getPlayer()->getWallNorm());
    if (getPlayer()->mMovementStates.jumping && velocity.dot(getPlayer()->mJumpVec) > 0.0f) {
        return false;
    }
    _24 = velocity;
    velocity.scale(mActor->getConst().getTable()->mWallBackJumpPowerXZ);
    velocity += getGravityVec() * -mActor->getConst().getTable()->mWallBackJumpPowerY;
    getPlayer()->tryWallJump(velocity, false);
    switch (type) {
    case 0: {
        playEffectRTZ("結界ヒット", _24, getPlayer()->getWallPos());
        playSound("結界ヒット");
        Mario* player = getPlayer();
        player->_402 = 0;
        player = getPlayer();
        player->_428 = 60;
        break;
    }
    case 1:
        playEffectRTZ("水壁ヒット", _24, getPlayer()->getWallPos());
        playSound("水弾かれ");
        break;
    case 2:
        playSound("トランポリンジャンプ大");
        break;
    }
    startPadVib(2);
    _1C = 1;
    getPlayer()->mMovementStates._2B = true;
    return true;
}

bool Mario::fixWallingDist() {
    if (mMovementStates._19 & mMovementStates._8) {
        return true;
    }
    if (mMovementStates._19) {
        TVec3f position(_4F4 + *mBackWallTriangle->getNormal(0) * 79.0f);
        f32 distance = MR::vecKillElement(position - mPosition, *mBackWallTriangle->getNormal(0), &position);
        mPosition += *mBackWallTriangle->getNormal(0) * distance;
        if (!fixWallingTop()) {
            return false;
        }
        mPosition = _4F4 + *mBackWallTriangle->getNormal(0) * 60.0f - _75C * 80.0f;
    } else if (mMovementStates._8) {
        if (!fixWallingTop()) {
            return false;
        }
        if (MR::isExistMapCollision(mPosition, mFrontVec * 100.0f) && mFrontVec.dot(*mFrontWallTriangle->getNormal(0)) < -0.999f) {
            TVec3f position(_4E8 + *mFrontWallTriangle->getNormal(0) * 60.0f - _75C * 80.0f);
            TVec3f correction;
            MR::vecKillElement(position - mPosition, *getGravityVec(), &correction);
            if (!MR::isNearZero(correction)) {
                TVec3f relative(correction + mActor->_288);
                if (__fabsf(relative.x) < __fabsf(correction.x)) {
                    correction.x = relative.x;
                }
                if (__fabsf(relative.y) < __fabsf(correction.y)) {
                    correction.y = relative.y;
                }
                if (__fabsf(relative.z) < __fabsf(correction.z)) {
                    correction.z = relative.z;
                }
                addTrans(correction, "壁補正");
            }
        }
    }
    return true;
}

bool Mario::isInhibitWall() const {
    if (mDrawStates._2) {
        return true;
    }
    if (checkWallCode("NotWallJump", true)) {
        return true;
    }
    return checkWallCode("NoAction", true);
}

void Mario::tryWallPunch() {
    if (!mMovementStates._2B && !checkWallCodeNorm(8, nullptr, false) && (!isStatusActive(MarioStatus_Skate) || !checkWallFloorCode(0x21))) {
        if (isSwimming()) {
            if (getPlayer()->mMovementStates._8) {
                mSwim->hitWall(*mFrontWallTriangle->getNormal(0), mFrontWallTriangle->mSensor);
            }
            if (getPlayer()->mMovementStates._19) {
                mSwim->hitWall(*mBackWallTriangle->getNormal(0), mBackWallTriangle->mSensor);
            }
            if (getPlayer()->mMovementStates._1A) {
                mSwim->hitWall(*mSideWallTriangle->getNormal(0), mSideWallTriangle->mSensor);
            }
        } else {
            stopWalk();
            TVec3f velocity(getWallNorm() * mActor->getConst().getTable()->mWallSpinFlipGround);
            if (mMovementStates.jumping) {
                velocity.scale(mActor->getConst().getTable()->mWallSpinFlipAirRatio);
            }
            velocity -= *getGravityVec() * mActor->getConst().getTable()->mWallSpinHopGround;
            tryForcePowerJump(velocity, true);
            mMovementStates._2B = true;
            startPadVib(2);
            playSound("壁反射");
            playSound("声スピンキャンセル");
            playEffectTrans("壁ヒット", getWallPos());
            if (getPlayer()->mMovementStates._8) {
                sendPunch(mFrontWallTriangle->mSensor, true);
            }
            if (getPlayer()->mMovementStates._19) {
                sendPunch(mBackWallTriangle->mSensor, true);
            }
            if (getPlayer()->mMovementStates._1A) {
                sendPunch(mSideWallTriangle->mSensor, true);
            }
            mActor->_EF6 = 30;
        }
    }
}

bool MarioWall::notice() {
    return false;
}
