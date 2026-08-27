#include "Game/MapObj/MeteorStrikeLauncher.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MeteorStrike.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace NrvMeteorStrikeLauncher {
    NEW_NERVE(MeteorStrikeLauncherNrvCreate, MeteorStrikeLauncher, Create);
    NEW_NERVE(MeteorStrikeLauncherNrvInterval, MeteorStrikeLauncher, Interval);
};  // namespace NrvMeteorStrikeLauncher

MeteorStrikeLauncher::MeteorStrikeLauncher(const char* pName)
    : LiveActor(pName), mMeteorStrikeArray(), mMeteorStrikeCount(), mIntervalStep(-1), mCreateStep(), mIsMeteorStrike(), mIsValidMultiMeteor() {
}

void MeteorStrikeLauncher::init(const JMapInfoIter& rIter) {
    const char* pName = mName;
    setName("メテオストライクランチャー");

    initMapToolInfo(rIter);

    MR::connectToSceneMapObjMovement(this);

    initRailRider(rIter);
    MR::moveCoordAndTransToRailPoint(this, 0);

    initNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvCreate));

    MR::needStageSwitchReadAppear(this, rIter);
    MR::syncStageSwitchAppear(this);

    if (mIsMeteorStrike && mIsValidMultiMeteor) {
        f32 speed = MeteorStrike::getSpeed(rIter);
        mMeteorStrikeCount = static_cast< s32 >(MR::getRailTotalLength(this) / (speed * mIntervalStep)) + 2;
    } else if (mIsMeteorStrike || mIntervalStep < 0) {
        mMeteorStrikeCount = 1;
    } else {
        mMeteorStrikeCount = 2;
    }

    mMeteorStrikeArray = new MeteorStrike*[mMeteorStrikeCount];

    for (s32 idx = 0; idx < mMeteorStrikeCount; idx++) {
        mMeteorStrikeArray[idx] = new MeteorStrike(pName);
        mMeteorStrikeArray[idx]->init(rIter);
    }

    makeActorDead();
}

void MeteorStrikeLauncher::appear() {
    LiveActor::appear();

    MR::invalidateClipping(this);

    setNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvCreate));
}

void MeteorStrikeLauncher::kill() {
    LiveActor::kill();

    if (mIntervalStep < 0) {
        return;
    }

    for (s32 idx = 0; idx < mMeteorStrikeCount; idx++) {
        if (!MR::isDead(mMeteorStrikeArray[idx])) {
            mMeteorStrikeArray[idx]->kill();
        }
    }
}

void MeteorStrikeLauncher::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::getJMapInfoArg1NoInit(rIter, &mIntervalStep)) {
        mIntervalStep *= 60;
    }

    mIsMeteorStrike = MR::isEqualObjectName(rIter, "MeteorStrike");

    MR::getJMapInfoArg2NoInit(rIter, &mIsValidMultiMeteor);
}

MeteorStrike* MeteorStrikeLauncher::getUnusedMeteorStrike() {
    for (s32 idx = 0; idx < mMeteorStrikeCount; idx++) {
        if (MR::isDead(mMeteorStrikeArray[idx])) {
            return mMeteorStrikeArray[idx];
        }
    }

    return nullptr;
}

bool isNotJudgedToClipFrustum(const TVec3f& pVec) {
    return !MR::isJudgedToClipFrustum(pVec, 200.0f);
}

bool MeteorStrikeLauncher::create() {
    MeteorStrike* pUnusedStrike = getUnusedMeteorStrike();
    if (pUnusedStrike == nullptr) {
        mCreateStep++;
        return false;
    }

    if (!mIsMeteorStrike) {
        pUnusedStrike->appear();
        return true;
    }

    if (mIsValidMultiMeteor) {
        TVec3f movedPos;
        pUnusedStrike->getMovedPos(&movedPos, 0);

        if (!isNotJudgedToClipFrustum(movedPos)) {
            pUnusedStrike->appear();
        }

        return true;
    }

    for (; mCreateStep >= 0; mCreateStep -= mIntervalStep) {
        TVec3f movedPos;
        if (!pUnusedStrike->getMovedPos(&movedPos, mCreateStep)) {
            continue;
        }

        if (isNotJudgedToClipFrustum(movedPos)) {
            continue;
        }

        pUnusedStrike->appear(mCreateStep);
        return true;
    }

    mCreateStep = 0;

    return false;
}

void MeteorStrikeLauncher::exeCreate() {
    if (MR::isFirstStep(this)) {
        mCreateStep = 0;
    }

    if (!create()) {
        return;
    }

    if (mIntervalStep < 0) {
        kill();
        return;
    }

    setNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvInterval));
}

void MeteorStrikeLauncher::exeInterval() {
    if (MR::isStep(this, mIntervalStep)) {
        setNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvCreate));
    }
}
