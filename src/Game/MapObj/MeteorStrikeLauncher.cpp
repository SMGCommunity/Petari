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
    : LiveActor(pName), mMeteorStrikeArray(), mMeteorStrikeCount(), mSpawnDelay(-1), mSpawnDistance(), mIsMeteorStrike(), mAllowMultipleMeteors() {
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

    if (mIsMeteorStrike && mAllowMultipleMeteors) {
        f32 speed = MeteorStrike::getSpeed(rIter);
        mMeteorStrikeCount = static_cast< s32 >(MR::getRailTotalLength(this) / (speed * mSpawnDelay)) + 2;
    } else if (mIsMeteorStrike || mSpawnDelay < 0) {
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

    if (mSpawnDelay < 0) {
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

    if (MR::getJMapInfoArg1NoInit(rIter, &mSpawnDelay)) {
        mSpawnDelay *= 60;
    }

    mIsMeteorStrike = MR::isEqualObjectName(rIter, "MeteorStrike");

    MR::getJMapInfoArg2NoInit(rIter, &mAllowMultipleMeteors);
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
        mSpawnDistance++;
        return false;
    }

    if (!mIsMeteorStrike) {
        pUnusedStrike->appear();
        return true;
    }

    if (mAllowMultipleMeteors) {
        TVec3f movedPos;
        pUnusedStrike->getMovedPos(&movedPos, 0);

        if (!isNotJudgedToClipFrustum(movedPos)) {
            pUnusedStrike->appear();
        }

        return true;
    }

    for (; mSpawnDistance >= 0; mSpawnDistance -= mSpawnDelay) {
        TVec3f movedPos;
        if (pUnusedStrike->getMovedPos(&movedPos, mSpawnDistance) && !isNotJudgedToClipFrustum(movedPos)) {
            pUnusedStrike->appear(mSpawnDistance);
            return true;
        }
    }

    mSpawnDistance = 0;

    return false;
}

void MeteorStrikeLauncher::exeCreate() {
    if (MR::isFirstStep(this)) {
        mSpawnDistance = 0;
    }

    if (!create()) {
        return;
    }

    if (mSpawnDelay < 0) {
        kill();
        return;
    }

    setNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvInterval));
}

void MeteorStrikeLauncher::exeInterval() {
    if (MR::isStep(this, mSpawnDelay)) {
        setNerve(GET_NERVE(MeteorStrikeLauncher, MeteorStrikeLauncherNrvCreate));
    }
}
