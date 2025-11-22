#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/SimpleMapObj.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"

class AstroMapObj : public MapObjActor {
public:
    AstroMapObj(const char*);
    ~AstroMapObj();

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeOpen();
    void exeWait();
    void exeAlive();
    void exeRevival();

    void setStateAlive();
    void setStateDead();
    void selectNrvWait();
    bool isPlayMachineSE() const;
    void tryStartAllAnimAndEffect(const char*);  // Curiously the function name starts with "try" but it doesn't return a bool
    bool isEndRevival() const;
    bool isAlreadyOpen(const char*, s32);
    void setStateDoorOpenOrClose();
    void startDemo();

    inline bool checkStrings(const char* arg1) { return MR::isEqualString(arg1, "AstroDomeEntrance") || MR::isEqualString(arg1, "AstroLibrary"); }
    inline bool checkOtherStrings(const char* arg1) {
        return MR::isEqualString(arg1, "AstroRotateStepA") || MR::isEqualString(arg1, "AstroRotateStepB") ||
               MR::isEqualString(arg1, "AstroDecoratePartsA");
    }
    inline bool moreInlines(const char* arg1) {
        return MR::isEqualString(arg1, "AstroDomeEntrance") || MR::isEqualString(arg1, "AstroLibrary") || MR::isEqualString(arg1, "AstroChildRoom") ||
               MR::isEqualString(arg1, "AstroParking");
    }

    // Unused?
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

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
    ~AstroSimpleObj();

    virtual void init(const JMapInfoIter&);
    virtual void control();

    const JMapInfo* _C4;
    GalaxyNamePlate* _C8;
};
