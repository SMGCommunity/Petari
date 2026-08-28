#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapPartsRailMover;

class OceanFloaterLandParts : public LiveActor {
public:
    OceanFloaterLandParts(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initMoveType(const JMapInfoIter&);
    void initRailMoveFunction(const JMapInfoIter&);
    void initLodCtrl(const JMapInfoIter&);
    void endFloatUpDemo();
    void exeWait();
    void exeMove();
    void exeDone();

    /* 0x8C */ MapPartsRailMover* mRailMover;
    /* 0x90 */ LodCtrl* mLodCtrl;
    /* 0x94 */ TVec3f mRailEndPos;
    /* 0xA0 */ const char* mObjectName;
    /* 0xA4 */ bool mIsMoving;
    /* 0xA5 */ bool mIsRegisteredDemo;
    /* 0xA8 */ s32 mAlreadyDoneFlag;
};
