#include "Game/Enemy/WalkerStateRunaway.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static WalkerStateRunawayParam sDefaultParam;
};

namespace NrvWalkerStateRunaway {
    NEW_NERVE(WalkerStateRunawayNrvWait, WalkerStateRunaway, Wait);
    NEW_NERVE(WalkerStateRunawayNrvRunaway, WalkerStateRunaway, Runaway);
    NEW_NERVE(WalkerStateRunawayNrvWallJump, WalkerStateRunaway, WallJump);
};  // namespace NrvWalkerStateRunaway

WalkerStateRunawayParam::WalkerStateRunawayParam() {
    _0 = "FollowMe";
    _4 = "Run";
    _8 = "Jump";
    _C = 0.050000001f;
    _10 = 1.0f;
    _14 = 0.89999998f;
    _18 = 0.99000001f;
    _1C = 5;
    _20 = 3.0f;
    _24 = 1100.0f;
    _28 = 1300.0f;
    _2C = 10.0f;
    _30 = 3.0f;
    _34 = 30;
    _38 = 25.0f;
    _3C = 1.0f;
    _40 = 0.89999998f;
    _44 = 1.4f;
    _48 = 6;
    _4C = 0.30000001f;
    _50 = 8.0f;
    _54 = 15.0f;
}

WalkerStateRunaway::WalkerStateRunaway(LiveActor* pActor, TVec3f* a2, WalkerStateRunawayParam* pParam)
    : ActorStateBase< LiveActor >("歩行型アクター逃げ") {
    mParentActor = pActor;
    mParam = pParam;
    _14 = a2;
    _18 = 0;
    _1C = 1.0f;

    if (pParam == nullptr) {
        mParam = &sDefaultParam;
    }

    initNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
}

void WalkerStateRunaway::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
    _18 = 0;
}

bool WalkerStateRunaway::tryRunaway() {
    if (MR::isNearPlayer(mParentActor, mParam->_24)) {
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
        return true;
    }

    return false;
}

bool WalkerStateRunaway::tryWait() {
    bool isNear = !MR::isNearPlayer(mParentActor, mParam->_28);
    if (isNear) {
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
        return true;
    }

    return false;
}

bool WalkerStateRunaway::tryWallJump() {
    if (MR::isBindedWall(mParentActor) && MR::calcHitPowerToWall(mParentActor) > 0.0f) {
        TVec3f horiz;
        MR::calcVecFromPlayerH(&horiz, mParentActor);
        TVec3f wallNorml;
        MR::calcWallNormalHorizontal(&wallNorml, mParentActor);

        if (MR::isOppositeDirection(horiz, wallNorml, 0.0099999998f)) {
            f32 dir;
            if (MR::isHalfProbability()) {
                dir = mParam->_50;
            } else {
                dir = -mParam->_50;
            }

            MR::addVelocityClockwiseToDirection(mParentActor, wallNorml, dir);
        } else {
            TVec3f dir;
            JMathInlineVEC::PSVECAdd(&horiz, &wallNorml, &dir);
            MR::addVelocityMoveToDirection(mParentActor, dir, mParam->_50);
        }

        MR::addVelocityJump(mParentActor, mParam->_54);
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWallJump::sInstance);
        return true;
    }

    return false;
}

void WalkerStateRunaway::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParentActor, mParam->_0);
    }

    MR::turnDirectionToPlayerDegree(mParentActor, _14, mParam->_20);

    f32 v1;
    if (MR::isBindedGround(mParentActor)) {
        v1 = mParam->_C;
    } else {
        v1 = mParam->_10;
    }

    MR::addVelocityToGravity(mParentActor, v1);

    f32 v2;
    if (_18 < mParam->_1C) {
        v2 = mParam->_14;
    } else {
        v2 = mParam->_18;
    }

    MR::attenuateVelocity(mParentActor, v2);

    if (tryRunaway()) {
        return;
    }
}

void WalkerStateRunaway::exeRunaway() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParentActor, mParam->_4);
    }

    TVec3f v13;
    JMAVECScaleAdd(MR::getPlayerVelocity(), MR::getPlayerPos(), &v13, mParam->_38);
    TVec3f* actorPos = &mParentActor->mPosition;
    MR::calcPerpendicFootToLineInside(&v13, *actorPos, *MR::getPlayerPos(), v13);
    MR::turnDirectionFromTargetDegree(mParentActor, _14, v13, MR::calcNerveValue(mParentActor, mParam->_34, mParam->_2C, mParam->_30));
    f32 v8 = mParam->_40;
    f32 v9 = (_1C / mParam->_3C);
    f32 _44 = mParam->_44;
    if (v9 < v8) {
        v8 = v8;
    } else {
        if (v9 > _44) {
            v8 = _44;
        } else {
            v8 = (_1C / mParam->_3C);
        }
    }

    MR::setBckRate(mParentActor, v8);

    f32 v10;
    if (_18 < mParam->_1C) {
        v10 = _1C;
    } else {
        v10 = 0.0f;
    }

    MR::addVelocityMoveToDirection(mParentActor, *_14, v10);
    f32 v11;
    if (MR::isBindedGround(mParentActor)) {
        v11 = mParam->_C;
    } else {
        v11 = mParam->_10;
    }

    MR::addVelocityToGravity(mParentActor, v11);

    f32 v12;
    if (_18 < mParam->_1C) {
        v12 = mParam->_14;
    } else {
        v12 = mParam->_18;
    }

    MR::attenuateVelocity(mParentActor, v12);
    if (!tryWait()) {
        if (tryWallJump()) {
            return;
        }
    }
}

void WalkerStateRunaway::exeWallJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParentActor, mParam->_8);
    }

    MR::turnDirectionDegree(mParentActor, _14, mParentActor->mVelocity, 45.0f);
    MR::addVelocityToGravity(mParentActor, mParam->_10);
    MR::attenuateVelocity(mParentActor, mParam->_18);
    MR::reboundVelocityFromEachCollision(mParentActor, 0.0f, mParam->_4C, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, mParam->_48)) {
        if (MR::isBindedGround(mParentActor)) {
            setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
        }
    }
}

bool WalkerStateRunaway::isRunning() const {
    return isNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
}

WalkerStateRunaway::~WalkerStateRunaway() {}
