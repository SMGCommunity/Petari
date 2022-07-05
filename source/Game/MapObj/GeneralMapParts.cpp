#include "Game/MapObj/GeneralMapParts.h"
#include "Game/LiveActor/LiveActor.h"

typedef void (MapPartsFunction::*functionPtr)();

/*
void GeneralMapParts::kill() {
    functionPtr func = &MapPartsFunction::end;
    MapPartsFunction** first = mFunctionArray.mArray.mArr;
    MapPartsFunction** last = &mFunctionArray.mArray.mArr[mFunctionArray.mCount];

    while (first != last) {
        (*first->*func)();
        first++;
    }
    LiveActor::kill();  
}
*/

void GeneralMapParts::initMapPartsFunction(const JMapInfoIter &rIter) {
    mAppearController = new MapPartsAppearController(this);
    mAppearController->init(rIter);
    mFunctionArray.push_back(mAppearController);
    f32 rotate_speed = 0.0f;
    MR::getMapPartsArgRotateSpeed(&rotate_speed, rIter); 
    bool hasRotateSpeed = 0.0f != rotate_speed;

    if (hasRotateSpeed) {
        mRotator = new MapPartsRotator(this);
        mRotator->init(rIter);
        mFunctionArray.push_back(mRotator);
    }

    if (MR::isConnectedWithRail(rIter)) {
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mFunctionArray.push_back(mRailMover);
        mRailRotator = new MapPartsRailRotator(this);
        mRailRotator->init(rIter);
        mFunctionArray.push_back(mRailRotator);
        mGuideDrawer = MR::createMapPartsRailGuideDrawer(this, "RailPoint", rIter);
        mFunctionArray.push_back(mGuideDrawer);
        mRailPosture = new MapPartsRailPosture(this);
        mRailPosture->init(rIter);
        mFunctionArray.push_back(mRailPosture);
    }
}