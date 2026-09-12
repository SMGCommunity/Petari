#include "Game/Player/MarioTeresa.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

void Mario::startTeresaMode() {
    _418 = 0;
    _428 = 0;
    getPlayer()->mMovementStates.jumping = true;
    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates._22 = false;
    mJumpVec.zero();
    changeStatus(mTeresa);
}

void MarioTeresa::updateDropFlag() {
    _58 = 0;
    if (_34.dot(getAirGravityVec()) > 1.0f) {
        TVec3f horizontal;
        f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &horizontal);
        if (MR::isNearZero(getStickP()) && horizontal.length() < vertical) {
            _58 = 1;
        }
    }
}

bool Mario::getHitWallNorm(TVec3f* normal) {
    if (getPlayer()->mMovementStates._8) {
        if (!isThroughWall(mFrontWallTriangle)) {
            *normal = *mFrontWallTriangle->getNormal(0);
            return true;
        }
    } else if (getPlayer()->mMovementStates._19) {
        if (!isThroughWall(mBackWallTriangle)) {
            *normal = *mBackWallTriangle->getNormal(0);
            return true;
        }
    } else if (getPlayer()->mMovementStates._1A && !isThroughWall(mSideWallTriangle)) {
        *normal = *mSideWallTriangle->getNormal(0);
        return true;
    }
    return false;
}

void Mario::resetTeresaMode() {
    mActor->_F44 = true;
    _418 = 0;
    mTeresa->resetTeresaMode();
}

void MarioTeresa::resetTeresaMode() {
    _42 = 0;
    _44 = 0;
    _46 = 0;
    _48 = 0;
    _50 = 0.0f;
    _34.zero();
    _28.zero();
}

bool MarioTeresa::isTeresaAccel() const {
    return _44 != 0;
}

MarioTeresa::MarioTeresa(MarioActor* actor) : MarioState(actor, MarioStatus_Teresa) {
    _14.zero();
    _40 = 0;
    _20 = 0.0f;
    _24 = 0.0f;
    _4C = 0.0f;
    _54 = 0.0f;
    _58 = 0;
    _59 = 0;
    resetTeresaMode();
}

bool MarioTeresa::close() {
    return true;
}

bool MarioTeresa::update() {
    if (getPlayerMode() != PlayerMode_Teresa) {
        return false;
    }
    if (getPlayer()->isStatusActive(MarioStatus_Recovery)) {
        return true;
    }
    if (mActor->_EA4) {
        return true;
    }
    _50 = 0.0f;
    _14 = getJumpVec();
    if (!getPlayer()->_418) {
        _58 = 0;
    }
    getPlayer()->mMovementStates._B = false;
    _20 = checkHeight();
    _24 = mActor->getConst().getTable()->mTeresaWaitHeight;
    checkAccel();
    checkWind();
    checkGroundReflect();
    checkWallCeilReflect();
    addVelocity(_34);
    procNoControl();
    procNearGroundControl();
    procAirControl();
    procDrop();
    Mario* player = getPlayer();
    player->_3BC = 1;
    procControl();
    f32 speed = _14.length() / 10.0f;
    if (speed < 0.01f) {
        speed = 0.0f;
    }
    getPlayer()->mWalkSpeed = MR::clamp(speed, 0.0f, 1.0f);
    setJumpVec(_14);
    if (!_42) {
        getPlayer()->playSoundTeresaFlying();
    }
    return true;
}

f32 MarioTeresa::checkHeight() {
    f32 offset = 0.0f;
    f32 height = getPlayer()->mVerticalSpeed;
    f32 radius = 120.0f;
    while (offset < 2.0f * mActor->getConst().getTable()->mTeresaWaitHeight) {
        const TVec3f& position = mActor->_2A0;
        bool found = false;
        s32 count = Collision::checkStrikeBallToMapWithThickness(position + getGravityVec() * offset, radius, radius, nullptr, nullptr);
        if (count) {
            u32 i = 0;
            do {
                const HitInfo* hit = Collision::getStrikeInfoMap(i);
                TVec3f normal(*hit->mParentTriangle.getNormal(0));
                TVec3f direction(hit->mHitPos);
                direction -= position;
                f32 distance = direction.length() - 110.0f;
                if (distance < 0.0f) {
                    distance = 0.0f;
                }
                MR::normalizeOrZero(&direction);
                if (normal.dot(getGravityVec()) < -0.1f && direction.dot(getGravityVec()) > 0.1f) {
                    if (height > distance) {
                        height = distance;
                    }
                    found = true;
                }
                i++;
            } while (i < count);
        }
        if (found) {
            break;
        }
        offset += radius;
    }
    return height;
}

void MarioTeresa::checkAccel() {
    if (checkTrgA()) {
        if (!_44) {
            getPlayer()->_1C._4 = true;
        }
        _44 = mActor->getConst().getTable()->mTeresaAccelTime;
    }
    if (_44) {
        _44--;
    }
}

void MarioTeresa::checkGroundReflect() {
    if (getPlayer()->mMovementStates._1) {
        getPlayer()->mMovementStates.jumping = false;
    }
    if (!getPlayer()->mMovementStates.jumping && getPlayer()->_3CE) {
        getPlayer()->mDrawStates._C = false;
        getPlayer()->tryJump();
        cutGravityElementFromJumpVec(true);
        f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &_34);
        if (vertical < 0.0f) {
            _34 += getAirGravityVec() * vertical;
        }
    }
}

void MarioTeresa::procNoControl() {
    if (_46) {
        _46--;
        _48 = 0;
        _34.scale(mActor->getConst().getTable()->mTeresaWallReflectReduction);
    }
    if (MR::isNearZero(getStickP())) {
        _34.x *= 0.99f;
        _34.y *= 0.99f;
        _34.z *= 0.99f;
    }
}

void MarioTeresa::procNearGroundControl() {
    if (MR::isNearZero(getVelocity()) || !(calcAngleD(getPlayer()->getShadowNorm()) < 60.0f)) {
        return;
    }
    if (!(_20 < 50.0f + _24)) {
        return;
    }
    if (_46) {
        return;
    }
    if (_34.dot(getPlayer()->getShadowNorm()) >= 0.0f) {
        return;
    }
    f32 speed = _34.length();
    TVec3f direction(_34);
    if (MR::normalizeOrZero(&direction)) {
        return;
    }
    TVec3f side;
    side.cross(direction, getPlayer()->getShadowNorm());
    direction.cross(getPlayer()->getShadowNorm(), side);
    direction.setLength(speed);
    f32 blend = MR::clamp((50.0f + _24 - _20) / 50.0f, 0.0f, 1.0f);
    blend *= 0.5f;
    f32 reduction = MR::diffAngleAbs(_34, direction) / 3.1415927f;
    MR::vecBlendSphere(_34, direction, &_34, blend);
    _34.scale(1.0f - reduction);
}

void MarioTeresa::procDrop() {
    MarioActor* actor = mActor;
    bool descending = actor->getLastMove().dot(getGravityVec()) >= 0.0f;
    bool nearGround = _20 < _24 + mActor->getConst().getTable()->mTeresaDropDownHeight;
    if (descending) {
        if (nearGround) {
            f32 ratio = (_20 - _24) / mActor->getConst().getTable()->mTeresaDropDownHeight;
            if (ratio < 0.0f) {
            }
        }
    } else {
        if (nearGround) {
        }
        if (nearGround) {
        }
    }

    if (_20 > _24) {
        f32 distance = _20 - _24;
        f32 ratio = 1.0f;
        if (distance < 100.0f) {
            ratio = distance / 100.0f;
        }
        if (_58) {
            addTeresaVerticalVelocity(0.25f * ratio);
        } else {
            addTeresaVerticalVelocity(0.1f * ratio);
        }
        f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &_34);
        if (1.5f * vertical > _20 - _24) {
            vertical *= 0.75f;
        }
        _34 += getAirGravityVec() * vertical;
    } else {
        f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &_34);
        if (vertical < 0.0f) {
            _34 += getAirGravityVec() * vertical;
        }
    }
    if (_20 > 10.0f + _24) {
        TVec3f horizontal;
        MarioActor* actor = mActor;
        f32 vertical = MR::vecKillElement(actor->getLastMove(), getGravityVec(), &horizontal);
        f32 brake;
        if (vertical > -0.5f) {
            brake = 0.04f + (0.5f + vertical) / mActor->getConst().getTable()->mTeresaDropBase;
        } else {
            brake = mActor->getConst().getTable()->mTeresaRisingBrake;
        }
        MR::clamp(brake, 0.0f, 1.0f);
        getPlayer()->mDrawStates._1C = true;
    }
}

void MarioTeresa::addTeresaVerticalVelocity(f32 amount) {
    _34 += getAirGravityVec() * amount;
    TVec3f horizontal;
    f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &horizontal);
    f32 minimum = -mActor->getConst().getTable()->mTeresaRiseSpeedMax;
    f32 maximum = mActor->getConst().getTable()->mTeresaDropSpeedMax;
    maximum *= 20.0f;
    minimum *= 10.0f;
    if (_58) {
        f32 angle = 3.1415927f * (static_cast< f32 >(getPlayer()->_418) / mActor->getConst().getTable()->mTeresaWallThroughTime);
        maximum *= 1.0f + 0.5f * JMath::sSinCosTable.sinRadian(angle);
    }
    if (getPlayer()->mDrawStates._1F && _28.dot(getAirGravityVec()) > 0.707f) {
        maximum *= 2.0f;
    }
    vertical = MR::clamp(vertical, minimum, maximum);
    _34 = horizontal + getAirGravityVec() * vertical;
    _50 += amount;
}

void MarioTeresa::addTeresaHorizontalVelocity(const TVec3f& velocity) {
    TVec3f addition;
    MR::vecKillElement(velocity, getAirGravityVec(), &addition);
    _34 += addition;
    TVec3f horizontal;
    f32 vertical = MR::vecKillElement(_34, getAirGravityVec(), &horizontal);
    f32 maximum = mActor->getConst().getTable()->mTeresaHorizontalSpeedMax;
    if (getPlayer()->mDrawStates._1F && _28.dot(horizontal) > 0.707f) {
        maximum *= 2.0f;
    }
    if (horizontal.length() >= maximum) {
        horizontal.setLength(maximum);
    }
    _34 = horizontal + getAirGravityVec() * vertical;
}

void Mario::doTeresaReflection(const TVec3f& normal, bool effect) {
    mTeresa->doTeresaReflection(normal, effect);
}

void MarioTeresa::doTeresaReflection(const TVec3f& normal, bool effect) {
    TVec3f direction(normal);
    MR::normalizeOrZero(&direction);
    TVec3f tangent;
    f32 speed = MR::vecKillElement(_34, direction, &tangent);
    if (speed < 0.0f) {
        if (speed > -10.0f) {
            speed = -10.0f;
        }
        TVec3f horizontal;
        f32 vertical = MR::vecKillElement(direction * -speed, getAirGravityVec(), &horizontal);
        addTeresaHorizontalVelocity(horizontal * 2.0f);
        addTeresaVerticalVelocity(2.0f * vertical);
    } else {
        TVec3f horizontal;
        f32 vertical = MR::vecKillElement(direction, getAirGravityVec(), &horizontal);
        addTeresaHorizontalVelocity(horizontal);
        addTeresaVerticalVelocity(vertical);
    }
    _46 = mActor->getConst().getTable()->mTeresaWallReflectTime;
    _14.zero();
    if (!_42) {
        mActor->changeTeresaAnimation("hit", -1);
        playSound("声壁反射");
        if (effect) {
            playSound("テレサ壁反射");
            Mario* player = getPlayer();
            MR::emitEffectHit(mActor->_9A4, getPlayer()->_25C, player->_268, "WallHit");
        }
    }
    Mario* player = getPlayer();
    player->_418 = 0;
    mActor->_946 = 5;
    _42++;
}

XanimeGroupInfo teresaAnimeTable[] = {
    {{"基本"}, 1.0f, 16},
    {{""}},
};

XanimeBckTable2 teresaAnime2[] = {
    {{"基本"}, {{"wait", 1.0f}, {"run", 0.0f}}},
    {{""}, {{"", 0.0f}, {nullptr, 0.0f}}},
};

void MarioActor::initTeresaMarioAnimation() {
    _9B0 = 0.0f;
    _9BC = new XanimeResourceTable(MR::getResourceHolder(_9A4), teresaAnimeTable, nullptr, nullptr, nullptr, teresaAnime2, nullptr, nullptr, nullptr);
    _9B8 = new XanimePlayer(MR::getJ3DModel(_9A4), _9BC);
    _9B8->setDefaultAnimation("基本");
    _9B8->changeAnimation("基本");
    _9A4->mModelManager->mXanimePlayer = _9B8;
}

void Mario::startTeresaDisappear() {
    _418 = mActor->getConst().getTable()->mTeresaWallThroughTime;
    playSound("テレサ消える");
    playSound("声トルネード");
    MR::startCSSound("CS_TERESA", nullptr, 0);
    mTeresa->updateDropFlag();
    resetSleepTimer();
    if (_962 == 5 && mVerticalSpeed < 100.0f) {
        mTeresa->_34 += getAirGravityVec() * 10.0f;
    }
}

bool MarioTeresa::start() {
    if (getPlayer()->_430 == 4) {
        TVec3f direction(-getPlayer()->_220);
        getPlayer()->setFrontVecKeepUp(direction);
    }
    Mario* player = getPlayer();
    player->_42A = 0;
    player->_430 = 0;
    getPlayer()->cancelSquatMode();
    changeAnimation("落下", "落下");
    return true;
}

void MarioTeresa::checkWind() {
    TVec3f wind;
    f32 strength;
    BigFanFunction::calcWindInfo(&wind, mActor->_2A0, &strength);
    strength = MR::clamp(strength, 0.0f, 10.0f);
    if (strength > 0.0f) {
        playSound("テレサ風に乗る", 50.0f * strength);
    }
    if (!MR::isNearZero(strength)) {
        getPlayer()->mDrawStates._1F = true;
    }
    wind.scale(strength);
    _28.x *= 0.94f;
    _28.y *= 0.94f;
    _28.z *= 0.94f;
    _28 += wind * 0.1f * 1.5f;
    _34 += wind * 0.2f;
    if (_28.length() > 0.2f) {
        getPlayer()->mDrawStates._1F = true;
    }
}

void MarioTeresa::checkWallCeilReflect() {
    TVec3f normal;
    TVec3f wallNormal;
    bool hit = false;
    if (getPlayer()->calcDistToCeil(false) < 200.0f) {
        normal = getPlayer()->_3B0;
        hit = true;
    }
    if (!hit) {
        hit = getPlayer()->getHitWallNorm(&wallNormal);
        normal = wallNormal;
    }
    if (hit) {
        f32 speed = MR::vecKillElement(_34, normal, &_34);
        if (speed < 0.0f) {
            _34 += normal * -speed;
        } else {
            _34 += normal * speed * 1.5f;
        }
        if (!_42) {
            playSound("テレサ壁反射");
            playSound("声壁反射");
            mActor->changeTeresaAnimation("hit", -1);
            MarioParts* actor = mActor->_9A4;
            const TVec3f& wallNorm = getPlayer()->getWallNorm();
            MR::emitEffectHit(actor, getPlayer()->getWallPos(), wallNorm, "WallHit");
            mActor->_946 = 10;
            _46 = mActor->getConst().getTable()->mTeresaWallReflectTime;
            _14.zero();
            Mario* player = getPlayer();
            player->_418 = 0;
        }
        _42++;
        return;
    }
    _42 = 0;
}

void MarioTeresa::procAirControl() {
    if (getPlayer()->_418 < mActor->getConst().getTable()->mTeresaWallThroughTime - 30 && ((isTeresaAccel() && !_46) || _48)) {
        if (getPlayer()->_1C._4) {
            if (_34.dot(getAirGravityVec()) > 0.0f) {
                addTeresaVerticalVelocity(-2.0f);
            } else {
                addTeresaVerticalVelocity(-0.4f);
            }
        } else {
            addTeresaVerticalVelocity(-0.25f);
        }
        if (isTeresaAccel()) {
            playSound("テレサ踏ん張り");
            if (getPlayer()->_1C._4) {
                _48 = mActor->getConst().getTable()->mTeresaTrgOnPushTime1;
            } else if (_48 < 15) {
                _48 = mActor->getConst().getTable()->mTeresaTrgOnPushTime2;
            }
        }
        if (_48) {
            _48--;
        }
    }
}

void MarioTeresa::procControl() {
    if (!_46) {
        if (!MR::isNearZero(getStickP())) {
            getPlayer()->setFrontVecKeepUp(getWorldPadDir(), mActor->getConst().getTable()->mTeresaAirWalkTurnSpd);
        }
        if (getPlayer()->_402 < (mActor->getConst().getTable()->mAirWalkTime / 2)) {
            getAnimator()->setSpeed(1.5f);
        }
        TVec3f velocity(_14);
        f32 alignment = 1.0f;
        if (!MR::isNearZero(getStickP())) {
            velocity.dot(getWorldPadDir());
        }
        if (alignment < 0.0f) {
            alignment *= 0.3f;
        }
        f32 acceleration = 0.3f;
        if (getPlayer()->_418 > (mActor->getConst().getTable()->mTeresaWallThroughTime / 2)) {
            acceleration = 2.0f;
        }
        addTeresaHorizontalVelocity(getWorldPadDir() * alignment * acceleration);
    }
    if (checkTrgZ()) {
        if (_34.dot(getAirGravityVec()) < 0.0f) {
            MR::vecKillElement(_34, getAirGravityVec(), &_34);
        }
        mActor->changeTeresaAnimation("fallquicklystart", -1);
        playSound("声壁押し");
    }
}

void MarioActor::runTeresaBaseAnimation() {
    if (!mMario->isStatusActive(MarioStatus_Wait) && !_9B8->isRun("基本")) {
        _9B8->changeAnimation("基本");
        _9B4 = MR::getRandom(60L, 180L);
        MR::startBtp(_9A4, "blink");
    }
}

void MarioActor::changeTeresaAnimation(const char* animation, s32 interpolation) {
    if (MR::isBckPlaying(_9A4, "sleep")) {
        MR::deleteEffect(_9A4, "Sleep");
    }
    if (interpolation == -1) {
        MR::startBck(_9A4, animation, nullptr);
    } else {
        MR::startBckWithInterpole(_9A4, animation, interpolation);
    }
    if (MR::isEqualString(animation, "wait") || MR::isEqualString(animation, "run")) {
        _9B4 = MR::getRandom(60L, 180L);
        MR::stopBtp(_9A4);
        return;
    }
    _9B4 = 0;
    if (MR::isExistBtp(_9A4, animation)) {
        MR::startBtp(_9A4, animation);
        return;
    }
    MR::startBtp(_9A4, "blink");
}

void MarioActor::updateTeresaAnimation() {
    bool canChange = true;
    if (MR::isBckPlaying(_9A4, "sleep")) {
        bool moving = false;
        if (mMario->mTeresa->isTeresaAccel()) {
            moving = true;
        }
        if (mMario->getStickP() != 0.0f) {
            moving = true;
        }
        if (mMario->mJumpVec.length() > 1.0f) {
            moving = true;
        }
        if (moving) {
            runTeresaBaseAnimation();
            MR::deleteEffect(_9A4, "Sleep");
        }
    }
    if (MR::isBckOneTimeAndStopped(_9A4)) {
        runTeresaBaseAnimation();
    }
    if (MR::isBckPlaying(_9A4, "hit")) {
        canChange = false;
    }
    if (MR::isBckPlaying(_9A4, "spin") && !mMario->mTeresa->isTeresaAccel()) {
        canChange = false;
    }
    if (canChange) {
        if (mMario->mTeresa->isTeresaAccel()) {
            if (!MR::isBckPlaying(_9A4, "fly") && !MR::isBckPlaying(_9A4, "spin")) {
                changeTeresaAnimation("fly", 16);
            }
        } else if (MR::isBckPlaying(_9A4, "fly") || _9B8->isRun("基本")) {
            if (getLastMove().dot(getGravityVec()) >= 1.0f) {
                changeTeresaAnimation("fall", 16);
            } else if (!mMario->mDrawStates._1C) {
                if (__fabsf(getLastMove().dot(getGravityVec())) < 1.0f) {
                    runTeresaBaseAnimation();
                }
            }
        } else if (MR::isBckPlaying(_9A4, "fall")) {
            if (getLastMove().dot(getGravityVec()) < 1.0f) {
                runTeresaBaseAnimation();
            }
        }
        if (_9B8->isRun("基本")) {
            _9B0 = mMario->mJumpVec.length() / 10.0f;
            _9B0 = MR::clamp(_9B0, 0.0f, 1.0f);
            _9B8->changeTrackWeight(0, 1.0f - _9B0);
            _9B8->changeTrackWeight(1, _9B0);
        }
    }
    if (mMario->_418) {
        if (!MR::isBckPlaying(_9A4, "spin")) {
            if (mMario->_418 > getConst().getTable()->mTeresaWallThroughTime - 3) {
                changeTeresaAnimation("spin", -1);
            }
        }
        if (_9A8 < getConst().getTable()->mTeresaAlphaLevelMax) {
            _9A8 += getConst().getTable()->mTeresaAlphaLevelInc;
        }
        mMario->_418--;
        if (!mMario->_418) {
            playSound("テレサ現れる", -1);
        }
    } else {
        if (_9A8 > 0.0f) {
            _9A8 -= getConst().getTable()->mTeresaAlphaLevelDec;
        }
        if (_9A8 > 0.0f && mMario->mTeresa->_46) {
            _9A8 -= getConst().getTable()->mTeresaAlphaLevelDec;
        }
    }
    MR::setBrkFrameAndStop(_9A4, _9A8);
    if (_9B4) {
        _9B4--;
        if (!_9B4) {
            _9B4 = MR::getRandom(90L, 240L);
            MR::startBtp(_9A4, "blink");
        }
    }
}

bool MarioTeresa::keep() {
    return update();
}

bool MarioTeresa::notice() {
    return true;
}
