#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class QuestionBoxGalleryObj : public MapObjActor {
public:
    QuestionBoxGalleryObj(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void startSwitchOn();
    void startSwitchOff();

    void exeOnWait();
    void exeOnStart();
    void exeOffWait();
    void exeOffStart();
};
