#include "Game/LiveActor/Binder.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioModule.hpp"

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

    _8D4 = 0;
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
// Mario::moveRelativePositionWall

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

void Mario::pushedByReaction() {
    addVelocity(_928);
    _928.scale(_934);
}

void Mario::addReaction(const TVec3f& rReact) {
    _928 += rReact;
}

// Mario::tryPushToVelocity

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

void Mario::recordJumpEnforceMove() {
    _10._17 = 1;
    _8D8 = _8D4;
    _8DC = _184;
    *_8E8 = *_8C8;
}

// Mario::doEnforceJump

// Mario::pushedByWind
