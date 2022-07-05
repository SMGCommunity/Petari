#pragma once

#include "Game/MapObj/MapParts.h"
#include "Game/MapObj/MapPartsRailGuideDrawer.h"
#include "Game/MapObj/MapPartsRailMover.h"
#include "Game/MapObj/MapPartsRailRotator.h"
#include "Game/MapObj/MapPartsRailPosture.h"
#include "Game/MapObj/MapPartsRotator.h"
#include "Game/MapObj/MapPartsAppearController.h"
#include "Game/Util/Array.h"

class GeneralMapParts : public MapParts {
public:

    typedef void (MapPartsFunction::*funcPtr)(void);

    virtual void kill();

    void initMapPartsFunction(const JMapInfoIter &);
    void initSensorType(const JMapInfoIter &);
    void initGravity(const JMapInfoIter &);

    ActorCameraInfo* mCameraInfo;                                       // _98
    s32 mMoveConditionType;                                             // _9C
    s32 mSignMotionType;                                                // _A0
    s32 mShadowType;                                                    // _A4
    MR::Vector<MR::FixedArray<MapPartsFunction *, 8> > mFunctionArray;  // _A8
    MapPartsAppearController* mAppearController;                        // _CC
    MapPartsRailRotator* mRailRotator;                                  // _D0
    MapPartsRailMover* mRailMover;                                      // _D4
    MapPartsRotator* mRotator;                                          // _D8
    MapPartsRailGuideDrawer* mGuideDrawer;                              // _DC
    MapPartsRailPosture* mRailPosture;                                  // _E0
    u8 _E4;
    u8 _E5;
};