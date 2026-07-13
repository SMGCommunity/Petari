#include "Game/Boss/DodoryuMove.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace {
    // static const f32 sRailSpeed = _;
    static const f32 sRailAccel = 0.05f;
    static const f32 sEscapeAccel = 0.05f;
};  // namespace

namespace DodoryuSub {
    MoveState::MoveState(MoveStateHolder* pHolder) : mHolder(pHolder), mHost(pHolder->mHost), mStep() {
    }

    MoveStateHolder::MoveStateHolder(Dodoryu* pHost) : mHost(pHost) {
        mRailState = new MoveStateRail(this);
        mChaseState = new MoveStateChase(this);
        mDetourState = new MoveStateDetour(this);
        mEscapeState = new MoveStateEscape(this);
        mJumpOutState = new MoveStateJumpOut(this);
        mNullState = new MoveStateNull(this);
        mCurrentState = mNullState;
        mNextState = nullptr;
        mCurrentState->mStep = 0;
    }

    void MoveStateHolder::execute() {
        if (mNextState != nullptr) {
            mNextState->mStep = 0;
            mCurrentState = mNextState;
            mNextState = nullptr;
        } else {
            mCurrentState->mStep++;
        }

        mCurrentState->execute();
    }

    void MoveStateHolder::shiftRail(f32 param1) {
        mNextState = mRailState;
        mRailState->_14 = param1;
    }

    void MoveStateHolder::shiftChase(f32 param1, f32 param2, f32 param3, f32 param4) {
        if (mCurrentState != mChaseState && mCurrentState != mDetourState) {
            mNextState = mChaseState;
        }

        MoveStateChase* chaseState = mChaseState;

        chaseState->_10 = param1;
        chaseState->_14 = param2;
        chaseState->_18 = param3;
        chaseState->_1C = param4;

        MoveStateDetour* detourState = mDetourState;

        detourState->_10 = param1;
        detourState->_14 = param2;
        detourState->_18 = param3;
        detourState->_1C = param4;
    }

    void MoveStateHolder::shiftEscape(f32 param1, s32 param2, s32 param3, f32 param4, f32 param5) {
        if (mCurrentState != mEscapeState) {
            mNextState = mEscapeState;
            mEscapeState->_10 = 0;
        }

        MoveStateEscape* escapeState = mEscapeState;

        escapeState->_24 = param1;
        escapeState->_28 = param1;
        escapeState->_14 = param2;
        escapeState->_18 = param3;
        escapeState->_1C = param4;
        escapeState->_20 = param5;
    }

    void MoveStateHolder::shiftJumpOut(f32 param1, f32 param2) {
        if (mCurrentState != mJumpOutState) {
            mNextState = mJumpOutState;
        }

        MoveStateJumpOut* jumpOutState = mJumpOutState;

        jumpOutState->_10 = param2;
        jumpOutState->_14 = param1;
    }

    void MoveStateHolder::shiftNull() {
        mNextState = mNullState;
    }

    void MoveStateHolder::brake() {
        if (mCurrentState == mEscapeState) {
            mEscapeState->_28 = 0.0f;
        } else if (mCurrentState == mChaseState) {
            mChaseState->mHost->mVelocity.zero();
        } else if (mCurrentState == mRailState) {
            mRailState->_14 = 0.0f;
        }
    }

    void MoveStateHolder::setRailSpeed(f32 speed) {
        mRailState->_14 = speed;
    }

    MoveStateRail::MoveStateRail(MoveStateHolder* pHolder) : MoveState(pHolder), _10(8.0f), _14(8.0f) {
    }

    void MoveStateRail::execute() {
        _14 += ::sRailAccel;

        if (_14 > _10) {
            _14 = _10;
        }

        MR::moveCoord(mHost, _14);
        // ...
        mHost->snapToGround();
    }

    void MoveStateChase::execute() {
        TVec3f toPlayer = *MR::getPlayerPos() - mHost->mPosition;

        if (DodoryuUtil::calcVerticalizedDir(mHost, &toPlayer, toPlayer)) {
            DodoryuUtil::turnToward(mHost, toPlayer, _1C);

            TVec3f accelDir;
            mHost->mBaseMtx.getZDir(accelDir);

            DodoryuUtil::accelerate(mHost, accelDir, _10, _14, _18);
        }

        if (mHost->checkWallWithVelocity()) {
            mHolder->mNextState = mHolder->mDetourState;

            DodoryuUtil::rotateVelocityByWall(mHost);
        } else {
            DodoryuUtil::addVelocity(mHost, true);
        }
    }

    MoveStateDetour::MoveStateDetour(MoveStateHolder* pHolder) : MoveState(pHolder) {
    }

    void MoveStateDetour::execute() {
        TVec3f accelDir = mHost->mVelocity;

        if (!MR::isNearZero(accelDir)) {
            MR::normalize(&accelDir);
            DodoryuUtil::turnToward(mHost, accelDir, 1.8f);
        } else {
            TVec3f yDir;
            mHost->mBaseMtx.getYDir(yDir);

            accelDir.cross(yDir, mHost->_134);
        }

        DodoryuUtil::accelerate(mHost, accelDir, _10, _14, _18);
        DodoryuUtil::addVelocity(mHost, false);

        if (!mHost->snapToWall()) {
            mHolder->mNextState = mHolder->mChaseState;
            mHost->snapToGround();
        } else {
            mHost->snapToGround();
            DodoryuUtil::rotateVelocityByWall(mHost);

            if (isPlayerOppositeFromWall()) {
                mHolder->mNextState = mHolder->mChaseState;
            }
        }
    }

    bool MoveStateDetour::isPlayerOppositeFromWall() const {
        TVec3f toPlayer = *MR::getPlayerPos() - mHost->mPosition;

        return mHost->_134.dot(toPlayer) > 0.0f;
    }

    MoveStateEscape::MoveStateEscape(MoveStateHolder* pHolder)
        : MoveState(pHolder), _10(), _14(60), _18(60), _1C(), _20(), _28(), _2C(0.0f, 0.0f, 0.0f) {
    }

    void MoveStateEscape::execute() {
        if (_10 == 0) {
            _10 = MR::getRandom(_14, _18 + 1);

            DodoryuUtil::calcEscapeDir(mHost, &_2C, _1C);
        }

        _28 += ::sEscapeAccel;

        if (_28 > _24) {
            _28 = _24;
        }

        DodoryuUtil::turnToward(mHost, _2C, _20);

        TVec3f newVelocity;
        mHost->mBaseMtx.getZDir(newVelocity);
        newVelocity.setLength(_28);

        mHost->mVelocity.set(newVelocity);

        if (mHost->checkWallWithVelocity()) {
            DodoryuUtil::rotateVelocityByWall(mHost);
        }

        DodoryuUtil::addVelocity(mHost, true);
        mHost->mVelocity.zero();

        _10--;

        if (_10 < 0) {
            _10 = 0;
        }
    }

    MoveStateJumpOut::MoveStateJumpOut(MoveStateHolder* pHolder) : MoveState(pHolder), _10(3.0f), _14(1.0f) {
    }

    void MoveStateJumpOut::execute() {
        TVec3f toPlayer = *MR::getPlayerPos() - mHost->mPosition;

        DodoryuUtil::calcVerticalizedDir(mHost, &toPlayer, toPlayer);
        DodoryuUtil::turnToward(mHost, toPlayer, _10);

        mHost->mVelocity.set(toPlayer * _14);

        if (mHost->checkWallWithVelocity()) {
            DodoryuUtil::rotateVelocityByWall(mHost);
        }

        DodoryuUtil::addVelocity(mHost, true);
        mHost->mVelocity.zero();
    }

    MoveStateNull::MoveStateNull(MoveStateHolder* pHolder) : MoveState(pHolder) {
    }

    void MoveStateNull::execute() {
    }
};  // namespace DodoryuSub
