#pragma once

#include "Game/MapObj/SimpleMapObj.hpp"

class GalaxyNamePlate;

class AstroMapObj : public MapObjActor {
public:
    AstroMapObj(const char*);

    virtual ~AstroMapObj() {};
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {};
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {};
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {};
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {};

    void exeOpen();
    void exeWait();
    void exeAlive();
    void exeRevival();

    void setStateAlive();
    void setStateDead();
    void selectNrvWait();
    bool isPlayMachineSE() const;
    void tryStartAllAnimAndEffect(const char*);
    bool isEndRevival() const;
    bool isAlreadyOpen(const char*, s32);
    void setStateDoorOpenOrClose();
    void startDemo();

    const JMapInfo* _C4;
    GalaxyNamePlate* _C8;
    const char* _CC;  // Uhh...
    CollisionParts* _D0;
    CollisionParts* _D4;
    s32 _D8;
};

class AstroSimpleObj : public SimpleMapObjFarMax {
public:
    AstroSimpleObj(const char*);

    virtual ~AstroSimpleObj() {};
    virtual void init(const JMapInfoIter&);
    virtual void control();

    const JMapInfo* _C4;
    GalaxyNamePlate* _C8;
};
