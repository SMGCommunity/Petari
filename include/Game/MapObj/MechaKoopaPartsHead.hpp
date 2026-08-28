#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsHead : public MapObjActor {
public:
    MechaKoopaPartsHead(const char*);

    /* 0x08 */ virtual ~MechaKoopaPartsHead();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeDemoBreak();
    void exeDemoWhiteFadeOut();
    void exeDemoWhiteWait();
    void exeDemoWhiteFadeIn();
    void exeDemoAppearStar();
};
