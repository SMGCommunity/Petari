#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class ScrewSwitch : public LiveActor {
public:
    ScrewSwitch(const char*);

    ~ScrewSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeAdjust();
    void exeScrew();
    void exeEnd();
    void updateBindActorMtx();

    CollisionParts* mCollisionParts;
    LiveActor* _90;
    MapObjConnector* mMapObjConnector;
};
