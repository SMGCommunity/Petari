#include "Game/MapObj/RockCreator.hpp"
#include "Game/MapObj/Rock.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvRockCreator {
    NEW_NERVE(RockCreatorNrvActive, RockCreator, Active);
    NEW_NERVE(RockCreatorNrvDeactive, RockCreator, Deactive);
}  // namespace NrvRockCreator

RockCreator::RockCreator(const char* pName)
    : LiveActor(pName), mRocks(nullptr), mRockSpeed(10.0f), mBreakModelOnRailGoal(false), mSpawnDelay(-1), mNumRocks(0), mSpawnTimer(0),
      mIsValid(true) {
}

void RockCreator::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    initMapToolInfo(rIter);
    initRailRider(rIter);
    initNerve(&NrvRockCreator::RockCreatorNrvActive::sInstance);

    MR::needStageSwitchReadAppear(this, rIter);
    MR::syncStageSwitchAppear(this);

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &RockCreator::invalidate));
    }

    Rock::Type rockType = Rock::getType(rIter);
    if (mSpawnDelay < 0) {
        mNumRocks = 1;
        MR::declareStarPiece(this, Rock::getAppearStarPieceNum(rockType));
    } else {
        mNumRocks = (static_cast< s32 >(MR::getRailTotalLength(this) / mRockSpeed) + Rock::getAppearFrame()) / mSpawnDelay + 2;
        MR::declareStarPiece(this, Rock::getAppearStarPieceNum(rockType) * 3);
    }

    mRocks = new Rock[mNumRocks];
    for (s32 idx = 0; idx < mNumRocks; idx++) {
        mRocks[idx].mCreator = this;
        mRocks[idx].mMoveSpeed = mRockSpeed;
        mRocks[idx].init(rIter);
        if (mBreakModelOnRailGoal) {
            mRocks[idx].mBreakModelOnRailGoal = true;
        }
    }

    makeActorDead();
}

void RockCreator::appear() {
    if (mIsValid) {
        LiveActor::appear();
        mSpawnTimer = 0;
        MR::invalidateClipping(this);
        setNerve(&NrvRockCreator::RockCreatorNrvActive::sInstance);
    }
}

void RockCreator::kill() {
    if (mIsValid) {
        LiveActor::kill();
        for (s32 idx = 0; idx < mNumRocks; idx++) {
            if (!MR::isDead(&mRocks[idx])) {
                mRocks[idx].kill();
            }
        }
    }
}

void RockCreator::invalidate() {
    if (mIsValid) {
        kill();
        mIsValid = false;
    }
}

void RockCreator::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mRockSpeed);

    if (MR::getJMapInfoArg1NoInit(rIter, &mSpawnDelay)) {
        mSpawnDelay *= 60;
    }

    if (Rock::getType(rIter) == Rock::NormalRock) {
        MR::getJMapInfoArg2NoInit(rIter, &mBreakModelOnRailGoal);
    } else {
        // ObjArg 5 is for brekaing ONLY SPECIFICALLY for WanWans.
        // if the object is a rock, ObjArg 5 determines if the rock should bounce on attack
        MR::getJMapInfoArg5NoInit(rIter, &mBreakModelOnRailGoal);
    }
}

void RockCreator::create() {
    for (s32 idx = 0; idx < mNumRocks; idx++) {
        if (MR::isDead(&mRocks[idx])) {
            mRocks[idx].appear();
            return;
        }
    }
}

void RockCreator::exeDeactive() {
}

void RockCreator::exeActive() {
    if (mSpawnDelay < 0 || isReadyToSpawn()) {
        create();
        if (mSpawnDelay < 0) {
            setNerve(&NrvRockCreator::RockCreatorNrvDeactive::sInstance);
            return;
        }
        mSpawnTimer = 0;
    }
    mSpawnTimer++;
}
