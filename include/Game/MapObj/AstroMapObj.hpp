#pragma once

#include "Game/MapObj/SimpleMapObj.hpp"

class GalaxyNamePlate;

class AstroMapObj : public MapObjActor {
public:
    AstroMapObj(const char*);

    /* 0x08 */ virtual ~AstroMapObj() {};
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {};
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {};
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {};
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {};

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

    /* 0x08 */ virtual ~AstroSimpleObj() {};
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    const JMapInfo* _C4;
    GalaxyNamePlate* _C8;
};
