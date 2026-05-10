#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioModule.hpp"
#include "Game/Player/MarioSukekiyo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/RailUtil.hpp"

void Mario::checkEnforceMove() {
    checkEnforceMoveInner();
    updateOnimasu();
}

void Mario::checkEnforceMoveInner() {
    mMovementStates._18 = 0;
    TVec3f v15(_184);
    if (mMovementStates._1 != 0 || mMovementStates.jumping != 0) {
        _184.zero();
    }

    _8D4 = nullptr;
    bool same = moveRelativePositionWall() | moveRelativePosition(0);
    if (same) {
        if (MR::isSameMtx(*_45C->getBaseMtx(), *_45C->getPrevBaseMtx())) {
            // can't seem to get the codegen to generate for this
            if (mMovementStates.jumping) {
            }

            if (mVelocity.dot(_368) < 0.0f) {
                f32 element = MR::vecKillElement(mVelocity, _368, &mVelocity);
                f32 v8 = element;

                if (!mMovementStates._1) {
                    if (element < 0.0f) {
                        if (-element > mVerticalSpeed) {
                            v8 = -mVerticalSpeed;
                        }
                    }

                    TVec3f v13(_368);
                    v13.scale(v8);
                    mVelocity += v13;
                }
            }
        }

        mDrawStates._B = 1;

        if (!mActor->_F44) {
            addTrans(mVelocity, nullptr);
        } else {
            bool v11 = false;
            TVec3f v14(mPosition);

            if (mMovementStates._A == 0) {
                if (200.0f == _72C) {
                    v11 = true;
                }
            }

            addTrans(mVelocity, nullptr);

            if (v11) {
                if (calcDistToCeil(false) < 200.0f) {
                    mPosition = v14;
                    _1C._10 = 1;
                }
            }

            mActor->mBinder->clear();
        }

        mVelocity.zero();
        MarioActor* actor = mActor;
        actor->mPosition = mPosition;
        actor->calcCenterPos();
    }
}

// Mario::recordRelativePosition
void Mario::recordRelativePosition() {
    TVec3f strikeCenter(mActor->_2A0);
    _8CC[0]->mIdx = -1;
    _8CC[1]->mIdx = -1;

    f32 strikeRadius = 85.0f;
    if (mMovementStates._1 && _45C->isValid() && MR::isSameMtx(*_45C->getBaseMtx(), *_45C->getPrevBaseMtx())) {
        strikeRadius = 81.0f;
    }

    if (mMovementStates._A) {
        strikeRadius = 51.0f;
    }

    if (mActor->isJumpRising()) {
        invalidateRelativePosition();
        return;
    }

    const u32 strikeNum = static_cast< u32 >(Collision::checkStrikeBallToMap(strikeCenter, strikeRadius, nullptr, nullptr));
    const f32 floorDotThreshold = 0.707f;
    const f32 wallNormalDotThreshold = 0.9f;
    const Triangle* sameSensorFloor = nullptr;
    TVec3f negGravity = -*getGravityVec();

    for (u32 i = 0; i < strikeNum; i++) {
        const HitInfo* hitInfo = Collision::getStrikeInfoMap(i);
        const Triangle* tri = &hitInfo->mParentTriangle;

        if (tri->mSensor == nullptr) {
            continue;
        }

        if (MR::isSameMtx(*tri->getBaseMtx(), *tri->getPrevBaseMtx())) {
            continue;
        }

        if (tri->getNormal(0)->dot(negGravity) < floorDotThreshold) {
            if (!_8CC[0]->isValid()) {
                *_8CC[0] = *tri;
                continue;
            }

            if (tri->getNormal(0)->dot(*_8CC[0]->getNormal(0)) > wallNormalDotThreshold) {
                continue;
            }

            if (!_8CC[1]->isValid()) {
                *_8CC[1] = *tri;
                continue;
            }

            const f32 candidateDot = tri->getNormal(0)->dot(*_8CC[0]->getNormal(0));
            const f32 currentDot = _8CC[0]->getNormal(0)->dot(*_8CC[1]->getNormal(0));
            if (currentDot > candidateDot) {
                *_8CC[1] = *tri;
            }
            continue;
        }

        sameSensorFloor = tri;
        if (tri->mSensor != mHang->mWallSensor) {
            break;
        }
    }

    if (sameSensorFloor == nullptr) {
        if (isStatusActive(1)) {
            *_8C8 = *mFrontWallTriangle;
        } else if (mMovementStates._1) {
            if (_1C._13) {
                *_8C8 = *mGroundPolygon;
            } else if (isUseAnotherMovingPolygon()) {
                *_8C8 = *mGroundPolygon;
            } else {
                invalidateRelativePosition();
            }
        } else {
            invalidateRelativePosition();
        }
    } else if (_1C._13) {
        if (mGroundPolygon->mSensor == sameSensorFloor->mSensor) {
            *_8C8 = *sameSensorFloor;
        } else if (isUseAnotherMovingPolygon()) {
            *_8C8 = *sameSensorFloor;
        } else {
            invalidateRelativePosition();
        }
    } else if (isUseAnotherMovingPolygon()) {
        *_8C8 = *sameSensorFloor;
    } else {
        invalidateRelativePosition();
    }

    if (_8C8->isValid()) {
        if (mActor->selectInvalidMovingCollision(_8C8->mSensor->mHost->mName)) {
            invalidateRelativePosition();
        } else {
            PSMTXMultVec(*_8C8->getBaseInvMtx(), &mPosition, &_8A4);
        }
    }

    if (_8CC[0]->isValid()) {
        if (mActor->selectInvalidMovingCollision(_8CC[0]->mSensor->mHost->mName)) {
            _8CC[0]->mIdx = -1;
        } else {
            PSMTXMultVec(*_8CC[0]->getBaseInvMtx(), &mPosition, &_8B0);
        }
    }

    if (_8CC[1]->isValid()) {
        if (mActor->selectInvalidMovingCollision(_8CC[1]->mSensor->mHost->mName)) {
            _8CC[1]->mIdx = -1;
        } else {
            PSMTXMultVec(*_8CC[1]->getBaseInvMtx(), &mPosition, &_8BC);
        }
    }
}

void Mario::invalidateRelativePosition() {
    _8C8->mIdx = -1;
    _8CC[0]->mIdx = -1;
    _8CC[1]->mIdx = -1;
}

TPos3f* Mario::getMoveBaseMtx() const {
    if (_8C8->isValid()) {
        return _8C8->getBaseMtx();
    }

    return nullptr;
}

// Mario::moveRelativePosition
u32 Mario::moveRelativePosition(u32) {
    Triangle* rTri = _8C8;
    if (!rTri->isValid()) {
        return 0;
    }

    if (MR::isSameMtx(*rTri->getBaseMtx(), *rTri->getPrevBaseMtx())) {
        return 0;
    }

    if (mActor->_EA4 && !isStatusActive(0x22)) {
        invalidateRelativePosition();
        return 0;
    }

    if (rTri->mSensor != nullptr && MR::isDead(rTri->mSensor->mHost)) {
        invalidateRelativePosition();
        return 0;
    }

    TVec3f movePos;
    PSMTXMultVec(*rTri->getBaseMtx(), &_8A4, &movePos);
    _184 = movePos;
    _184 -= mPosition;

    if (isSwimming()) {
        TVec3f dist(mActor->_2A0);
        dist -= movePos;

        if (dist.dot(*rTri->getNormal(0)) > 0.0f && dist.dot(_184) > 0.0f) {
            addVelocity(_184);
        }
        return 0;
    }

    addVelocity(_184);

    if (_8D4 == nullptr || rTri->mSensor != mHang->mWallSensor) {
        _8D4 = rTri->mSensor;
    }

    if (!MR::isSameMtxRot(*rTri->getBaseMtx(), *rTri->getPrevBaseMtx())) {
        mMovementStates._18 = 1;

        if (MR::isRotAxisY(*rTri->getBaseMtx(), *rTri->getPrevBaseMtx())) {
            _10._10 = 1;
        }

        TVec3f rotAxis;
        MR::calcMtxRotAxis(&rotAxis, *rTri->getBaseMtx(), *rTri->getPrevBaseMtx());
        if (MR::isSameDirection(rotAxis, getAirGravityVec(), 0.01f)) {
            _10._10 = 1;
        }
    }

    return 1;
}

// Mario::moveRelativePositionWall
u32 Mario::moveRelativePositionWall() {
    u32 moved = 0;

    for (u32 i = 0; i < 2; i++) {
        Triangle* tri = _8CC[i];
        if (!tri->isValid()) {
            continue;
        }

        if (MR::isSameMtx(*tri->getBaseMtx(), *tri->getPrevBaseMtx())) {
            continue;
        }

        if (tri->mSensor != nullptr && (!tri->mSensor->mValidByHost || !tri->mSensor->mValidBySystem)) {
            tri->mIdx = -1;
            continue;
        }

        const TVec3f* relPos = i == 0 ? &_8B0 : &_8BC;
        TVec3f movePos;
        PSMTXMultVec(*tri->getBaseMtx(), relPos, &movePos);

        TVec3f add(movePos);
        add -= mPosition;

        const f32 elem = MR::vecKillElement(add, *tri->getNormal(0), &add);
        if (elem > 0.0f) {
            TVec3f push(*tri->getNormal(0));
            push.scale(elem);
            addVelocity(push);
        }

        if (!MR::isSameMtxRot(*tri->getBaseMtx(), *tri->getPrevBaseMtx())) {
            mMovementStates._18 = 1;
        }

        if (_8D4 == nullptr || tri->mSensor != mHang->mWallSensor) {
            _8D4 = tri->mSensor;
        }

        moved = 1;
    }

    return moved;
}

void Mario::recordLastGround() {
    *_474 = *mGroundPolygon;
    PSMTXMultVec(*_474->getBaseInvMtx(), &mGroundPos, &_4BC);
}

void Mario::getLastGroundPos(TVec3f* pPos) const {
    if (_474->isValid()) {
        PSMTXMultVec(*_474->getBaseMtx(), &_4BC, pPos);
    } else {
        pPos->zero();
    }
}

const TVec3f* Mario::getLastGroundEdgeNrm(u32 idx) const {
    return _474->getEdgeNormal(idx);
}

// Mario::getLastGroundEdgeIndex
u32 Mario::getLastGroundEdgeIndex(const TVec3f& rPos, const TVec3f& rDir) const {
    const TVec3f* edge0 = _474->calcAndGetEdgeNormal(0);
    const TVec3f* edge1 = _474->calcAndGetEdgeNormal(1);
    const TVec3f* edge2 = _474->calcAndGetEdgeNormal(2);

    f32 score0 = rDir.dot(*edge0);
    f32 score1 = rDir.dot(*edge1);
    f32 score2 = rDir.dot(*edge2);

    const TVec3f* pos2 = _474->calcAndGetPos(2);
    const TVec3f* pos1 = _474->calcAndGetPos(1);
    const TVec3f* pos0 = _474->calcAndGetPos(0);

    TVec3f center(*pos0);
    center += *pos1;
    TVec3f centerAvg(center);
    centerAvg += *pos2;
    centerAvg.scale(1.0f / 3.0f);

    TVec3f outDir(centerAvg);
    outDir -= rPos;
    MR::normalizeOrZero(&outDir);

    const f32 add0 = outDir.dot(*edge0);
    const f32 add1 = outDir.dot(*edge1);
    const f32 add2 = outDir.dot(*edge2);
    if (add0 > 0.0f) {
        score0 += add0;
    }
    if (add1 > 0.0f) {
        score1 += add1;
    }
    if (add2 > 0.0f) {
        score2 += add2;
    }

    if (score0 < score1 && score0 < score2) {
        return 0;
    }
    if (score1 < score0 && score1 < score2) {
        return 1;
    }
    return 2;
}

void Mario::pushedByReaction() {
    addVelocity(_928);
    _928.scale(_934);
}

void Mario::addReaction(const TVec3f& rReact) {
    _928 += rReact;
}

// Mario::tryPushToVelocity
void Mario::tryPushToVelocity() {
    _1B4 = mVelocity;
    pushedByWind();
    pushedByReaction();

    if (!MR::isNearZero(_350)) {
        if (isSwimming()) {
            TVec3f pushVec(_350);
            pushVec += _35C;
            _350.zero();
            _35C.zero();
            createAtField(true, pushVec.length());

            for (u32 i = 0; i < _578; i++) {
                TVec3f projected;
                if (MR::vecKillElement(pushVec, *_57C[i]->getNormal(0), &projected) < 0.0f) {
                    pushVec = projected;
                }
            }

            mVelocity += pushVec;
        } else if (!isStatusActive(5)) {
            if (isStatusActive(0x1B)) {
                closeStatus(mBury);
            } else {
                if (mMovementStates.jumping && !mMovementStates._1) {
                    TVec3f pushDir(_350);
                    MR::normalizeOrZero(&pushDir);

                    if (mJumpVec.dot(_350) < 0.0f) {
                        cutVecElementFromJumpVec(pushDir);
                    }
                    if (mVelocity.dot(_350) < 0.0f) {
                        MR::vecKillElement(mVelocity, pushDir, &mVelocity);
                    }
                } else if (isDamaging() && mMovementStates._1) {
                    MR::vecKillElement(_350, *getGravityVec(), &_350);
                }

                mVelocity += _350;
            }
        }

        _1C._A = 1;
    }

    mVelocity += _35C;
    _350.zero();
    _35C.zero();
}

void Mario::push(const TVec3f& rVec) {
    _350 += rVec;
    _1C._15 = 1;
}

void Mario::push2(const TVec3f& rVec) {
    _35C += rVec;
    _1C._15 = 1;
}

void Mario::powerAreaMove() {
    _190.zero();

    if (mMovementStates._1 && !mMovementStates.jumping && _960 == 28 && MR::calcAreaMoveVelocity(&_190, mPosition)) {
        if (mMovementStates._8 || mMovementStates._19 || mMovementStates._1A) {
            const TVec3f* norm = &getWallNorm();
            f32 dot = norm->dot(_190);

            if (dot < 0.0f) {
                MR::vecKillElement(_190, *norm, &_190);
                MR::vecKillElement(_190, *getGravityVec(), &_190);
            }
        }

        addVelocity(_190);
    }
}

// Mario::powerRailMove
void Mario::powerRailMove() {
    _19C.zero();

    if (!mMovementStates._1 || mMovementStates.jumping || _960 != 0x1B) {
        return;
    }

    const LiveActor* host = getGroundPolygon()->mSensor->mHost;
    if (!host) {
        return;
    }

    TVec3f railDir;
    MR::calcNearestRailDirection(&railDir, host, mPosition);

    s32 arg = -1;
    MR::getRailArg3NoInit(host, &arg);
    if (arg == -1) {
        arg = 10;
    }

    MR::normalizeOrZero(&railDir);

    if (mMovementStates._8 || mMovementStates._19 || mMovementStates._1A) {
        const TVec3f* wallNorm = &getWallNorm();
        if (wallNorm->dot(railDir) < 0.0f) {
            MR::vecKillElement(railDir, *wallNorm, &railDir);
            MR::vecKillElement(railDir, *getGravityVec(), &railDir);
        }
    }

    TVec3f railMove(railDir);
    railMove.scale(static_cast< f32 >(arg));
    _19C = railMove;
    addVelocity(_19C);
}

void Mario::recordJumpEnforceMove() {
    _10._17 = 1;
    _8D8 = _8D4;
    _8DC = _184;
    *_8E8 = *_8C8;
}

// Mario::doEnforceJump
void Mario::doEnforceJump(f32 param) {
    if (mJumpVec.dot(_184) < 0.0f) {
        mDrawStates._4 = 1;
        initActiveJumpVec();
    }

    TVec3f enforce(_184);
    enforce.scale(param);
    _8DC = enforce;

    f32 element = MR::vecKillElement(_8DC, getAirGravityVec(), &_8DC);

    TVec3f jump(_8DC);
    jump += mJumpVec;
    mJumpVec = jump;

    if (element < 0.0f) {
        TVec3f grav(getAirGravityVec());
        grav.scale(element);
        mJumpVec += grav;
    }

    mVelocity -= _184;
    invalidateRelativePosition();
}

// Mario::pushedByWind
void Mario::pushedByWind() {
    f32 windMag = _91C.length();
    const char* noAnim = static_cast< const char* >(nullptr);

    TVec3f windDir(_91C);
    MR::normalizeOrZero(&windDir);

    if (MR::isNearZero(windDir)) {
        if (isAnimationRun("向かい風ふんばり")) {
            stopAnimation("向かい風ふんばり", noAnim);
        }

        if (isAnimationRun("向かい風走り")) {
            stopAnimation("向かい風走り", noAnim);
        }

        if (_10.debugMode) {
            getPlayerMode();
        }

        _10.debugMode = 0;
        return;
    }

    if (mMovementStates.jumping) {
        const MarioConstTable* table = mActor->getConst().getTable();
        TVec3f add(_91C);
        add.scale(table->mWindJumpingFriction);
        mJumpVec += add;
        return;
    }

    if (_71C == 0) {
        const MarioConstTable* table = mActor->getConst().getTable();
        if (windMag <= table->mWindSlideLimit) {
            return;
        }

        windMag = (windMag - table->mWindSlideLimit) * table->mWindSlideFriction;
        TVec3f add(windDir);
        add.scale(windMag);
        _350 += add;
        _1C._A = 1;
        changeAnimation("向かい風ふんばり", noAnim);
        return;
    }

    TVec3f scaled(windDir);
    scaled.scale(windMag);

    if (mVelocity.dot(windDir) > 0.0f) {
        const MarioConstTable* table = mActor->getConst().getTable();
        TVec3f add(scaled);
        add.scale(table->mWindForwardFriction);
        _350 += add;
        _1C._A = 1;
        stopAnimation("向かい風ふんばり", noAnim);
        stopAnimation("向かい風走り", noAnim);
        return;
    }

    TVec3f combined(scaled);
    combined += mVelocity;

    TVec3f killed;
    f32 element = MR::vecKillElement(combined, windDir, &killed);

    const MarioConstTable* table = mActor->getConst().getTable();
    if (element > table->mWindSlideLimit) {
        element = (element - table->mWindSlideLimit) * table->mWindSlideFrictionAgainst;
    } else if (element > 0.0f) {
        element = 0.0f;
    }

    clearVelocity();

    TVec3f add(windDir);
    add.scale(element);

    TVec3f final(killed);
    final += add;

    changeAnimation("向かい風走り", noAnim);
    _350 += final;
    _1C._A = 1;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
