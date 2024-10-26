#include "Game/Demo/DemoSimpleCastHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

DemoSimpleCastHolder::DemoSimpleCastHolder(s32 liveActorCount, s32 layoutActorCount, s32 nameObjCount) {
    mLiveActors.mMaxSize = 0;
    mMaxLiveActorCount = 0;
    mLayoutActors.mMaxSize = 0;
    mMaxLayoutActorCount = 0;
    mNameObjs.mMaxSize = 0;
    mMaxNameObjCount = 0;

    mLiveActors.init(liveActorCount);
    mLayoutActors.init(layoutActorCount);
    mNameObjs.init(nameObjCount);
}

void DemoSimpleCastHolder::registerActor(LiveActor *pActor) {
    mLiveActors.assign(pActor, mMaxLiveActorCount++);
}

void DemoSimpleCastHolder::registerActor(LayoutActor *pLayoutActor) {
    mLayoutActors.assign(pLayoutActor, mMaxLayoutActorCount++);
}

void DemoSimpleCastHolder::registerNameObj(NameObj *pNameObj) {
    mNameObjs.assign(pNameObj, mMaxNameObjCount++);
}

void DemoSimpleCastHolder::movementOnAllCasts() {
    LiveActor** curActor = mLiveActors.mArr;

    while (curActor != &mLiveActors.mArr[mMaxLiveActorCount]) {
        MR::requestMovementOn(*curActor);
        curActor++;
    }

    LayoutActor** curLayoutActor = mLayoutActors.mArr;

    while (curLayoutActor != &mLayoutActors.mArr[mMaxLayoutActorCount]) {
        MR::requestMovementOn(*curLayoutActor);
        curLayoutActor++;
    }

    NameObj** curNameObj = mNameObjs.mArr;

    while (curNameObj != &mNameObjs.mArr[mMaxNameObjCount]) {
        MR::requestMovementOn(*curNameObj);
        curNameObj++;
    }
}