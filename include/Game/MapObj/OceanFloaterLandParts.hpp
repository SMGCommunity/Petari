#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapPartsRailMover;

class OceanFloaterLandParts : public LiveActor {
public:
    OceanFloaterLandParts(const char*);

    virtual void init(const JMapInfoIter&);                               
    virtual void makeActorAppeared();                                     
    virtual void makeActorDead();                                         
    virtual void control();                                               
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);  

    void initMoveType(const JMapInfoIter&);
    void initRailMoveFunction(const JMapInfoIter&);
    void initLodCtrl(const JMapInfoIter&);
    void endFloatUpDemo();
    void exeMove();
    void exeWait();
    void exeDone();

    /* 0x08C */ MapPartsRailMover* mRailMover;
    /* 0x090 */ LodCtrl* mLodCtrl;
    /* 0x094 */ TVec3f mRailEndPos;
    /* 0x0A0 */ const char* mObjectName;
    /* 0x0A4 */ bool mIsMoving;
    /* 0x0A5 */ bool mIsRegisteredDemo;
    /* 0x0A8 */ s32 mAlreadyDoneFlag;
};
