#pragma once

#include "Game/MapObj/MapParts.hpp"
#include "Game/Util/Array.hpp"

class ActorCameraInfo;
class MapPartsAppearController;
class MapPartsFunction;
class MapPartsRailGuideDrawer;
class MapPartsRailMover;
class MapPartsRailPosture;
class MapPartsRailRotator;
class MapPartsRotator;

class GeneralMapParts : public MapParts {
public:
    GeneralMapParts(const char*);

    virtual ~GeneralMapParts() {
    }

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initMapPartsFunction(const JMapInfoIter&);
    void initSensorType(const JMapInfoIter&);
    void initGravity(const JMapInfoIter&);
    void receiveMsgSwitchBOn();
    void receiveMsgSwitchBOff();
    void broadcastMsgToAllFunctions(u32);
    bool isFixed() const;
    void startMove();
    void exeWait();
    void exeWaitForPlayerOn();
    void exeMoveStart();

    ActorCameraInfo* mCameraInfo;                                         // 0x98
    s32 mMoveConditionType;                                               // 0x9C
    s32 mSignMotionType;                                                  // 0xA0
    s32 mShadowType;                                                      // 0xA4
    MR::Vector< MR::FixedArray< MapPartsFunction*, 8 > > mFunctionArray;  // 0xA8
    MapPartsAppearController* mAppearController;                          // 0xCC
    MapPartsRailRotator* mRailRotator;                                    // 0xD0
    MapPartsRailMover* mRailMover;                                        // 0xD4
    MapPartsRotator* mRotator;                                            // 0xD8
    MapPartsRailGuideDrawer* mGuideDrawer;                                // 0xDC
    MapPartsRailPosture* mRailPosture;                                    // 0xE0
    u8 _E4;
    u8 _E5;
};
