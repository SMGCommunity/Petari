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
    GeneralMapParts(const char *);

    virtual ~GeneralMapParts();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initMapPartsFunction(const JMapInfoIter &);
    void initSensorType(const JMapInfoIter &);
    void initGravity(const JMapInfoIter &);
    void receiveMsgSwitchBOn();
    void receiveMsgSwitchBOff();
    void broadcastMsgToAllFunctions(u32);
    bool isFixed() const;
    void startMove();
    void exeWait();
    void exeMoveStart();

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

namespace NrvGeneralMapParts {
    NERVE(HostTypeWait);
    NERVE(HostTypeWaitForPlayerOn);
    NERVE(HostTypeMoveStart);
    NERVE(HostTypeMove);
};
