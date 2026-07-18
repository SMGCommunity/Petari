#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioModule.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/MarioWait.hpp"
#include "Game/Util.hpp"

void MarioActor::settingRush() {
    rushDropThrowMemoSensor();

    switch ((s32)_924->mType) {
    case ATYPE_WATER_PRESSURE_BULLET_BIND:
        mMario->touchWater();
        break;
    }

    _934 = 1;
    resetCondition();
    XanimePlayer* p = mMario->getAnimator()->mXanimePlayer;
    p->_7E = false;
}

bool MarioActor::isFixJumpRushSensor(const HitSensor* pSensor) const {
    switch (pSensor->mType) {
    case ATYPE_TRANSFERABLE_BINDER:
    case ATYPE_SPIN_DRIVER_BIND:
    case ATYPE_SPIN_CANNON_BIND:
    case ATYPE_G_CAPTURE_BIND:
    case ATYPE_SPRING_ATTACKER_BIND:
    case ATYPE_WATER_PRESSURE_BULLET_BIND:
    case ATYPE_MARIO_LAUNCHER_BIND:
        return true;

    default:
        selectJumpRushSensor(pSensor->mHost->mName);
    }
}

bool MarioActor::isLandEffectRushSensor(const HitSensor* pSensor) const {
    switch (pSensor->mType) {
    case ATYPE_MARIO_LAUNCHER_BIND:
    case ATYPE_SPIN_CANNON_BIND:
        return true;
    default:
        return false;
    }
}

void MarioActor::resetCondition() {
    bool v2 = false;
    TVec3f v8;

    if (_EA5) {
        MR::extractMtxTrans(_EA8.mMtx, &v8);
        mMario->mPosition = mPosition;
    }

    if (mMario->mSwim->checkWaterCube(true)) {
        v2 = true;
    }

    mMario->stopAnimationUpper(nullptr, nullptr);

    if (mMario->mMovementStates._A) {
        mMario->cancelSquatMode();
    }

    XanimePlayer* p = mMario->getAnimator()->mXanimePlayer;
    p->_7E = true;
    mMario->forceStopTornado();
    updateTornado();

    // supposed to store to 0xF0, which is a vector
    // don't feel like dealing with unions right now
    mMario->mSwim->_78 = 0;
    mMario->_1C._3 = 0;

    if (mMario->isStatusActive(MarioStatus_Swim) && v2) {
        if (!_468) {
            mMario->mSwim->resetJet();
        }
    } else if (mMario->isStatusActive(MarioStatus_Teresa) && mPlayerMode == 6) {
        if (mMario->isStatusActive(MarioStatus_Wait)) {
            mMario->closeStatus(mMario->mWait);
        }

        mMario->resetTeresaMode();
    } else {
        mMario->closeStatus(nullptr);
    }

    /*
    // not sure what is going on here
    mMario->mMovementStates_LOW_WORD = 0;
    mMario->mMovementStates_HIGH_WORD = 0;
    mMario->_10_LOW_WORD = 0;
    mMario->_10_HIGH_WORD = 0;
    */

    resetPadSwing();

    if (mMario->isStatusActive(MarioStatus_Swim) && v2) {
        if (_468) {
            mMario->changeAnimation("水泳ジェット", "水泳ジェット");
        } else {
            mMario->changeAnimation(nullptr, "水泳基本");
        }
    } else {
        mMario->changeAnimation(nullptr, "基本");

        if (_468) {
            mMarioAnim->waterToGround();
        }
    }

    mMario->setFrontVecKeepUp(mMario->mFrontVec);
    updateGravityVec(true, true);

    if (!MR::isNearZero(_24C)) {
        mMario->setHeadVec(-_24C);
    }

    mMario->_1FC = mMario->mHeadVec;
    mMario->_13C.zero();
    mMario->_148.zero();
    mMario->_154.zero();
    mMario->mVerticalSpeed = 0.0f;
    mMario->_474->mIdx = -1;
    stopEffect("いい汗");
    stopEffect("ホッパー尻落ルイージ");
    stopEffect("ホッパー尻落");
    stopEffect("尻落ルイージ");
    stopEffect("尻落");

    _9C4->kill();
    MR::offCalcAnim(_9C4);

    for (u32 i = 0; i < 11; i++) {
        mMario->_984[i] = nullptr;
    }

    if (_EA5) {
        mMario->changeAnimationInterpoleFrame(0);
        mBlendMtxTimer = 0;
    }
}

// ...

bool MarioActor::takeSensor(HitSensor* pSensor) {
    if (_424) {
        return false;
    }

    if (_468) {
        return false;
    }

    changeAnimation("カブ抜き", nullptr);
    changeNullAnimation("PullOut", -1);
    _424 = pSensor;
    memorizeSensorThrow(pSensor);
    _480 = 1;
    _38C = 60;
    return true;
}
