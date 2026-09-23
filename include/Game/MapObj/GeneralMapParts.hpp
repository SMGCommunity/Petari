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
    GeneralMapParts(const char* pName);

    virtual ~GeneralMapParts() {
    }

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initMapPartsFunction(const JMapInfoIter& rIter);
    void initSensorType(const JMapInfoIter& rIter);
    void initGravity(const JMapInfoIter& rIter);
    void receiveMsgSwitchBOn();
    void receiveMsgSwitchBOff();
    void broadcastMsgToAllFunctions(u32 msg);
    bool isFixed() const;
    void startMove();
    void exeWait();
    void exeWaitForPlayerOn();
    void exeMoveStart();

    /* 0x98 */ ActorCameraInfo* mCameraInfo;
    /* 0x9C */ s32 mMoveConditionType;
    /* 0xA0 */ s32 mSignMotionType;
    /* 0xA4 */ s32 mShadowType;
    /* 0xA8 */ MR::Vector< MR::FixedArray< MapPartsFunction*, 8 > > mFunctionArray;
    /* 0xCC */ MapPartsAppearController* mAppearController;
    /* 0xD0 */ MapPartsRailRotator* mRailRotator;
    /* 0xD4 */ MapPartsRailMover* mRailMover;
    /* 0xD8 */ MapPartsRotator* mRotator;
    /* 0xDC */ MapPartsRailGuideDrawer* mGuideDrawer;
    /* 0xE0 */ MapPartsRailPosture* mRailPosture;
    /* 0xE4 */ u8 mIsCameraEnded;
    /* 0xE5 */ u8 mIsPaused;
};
