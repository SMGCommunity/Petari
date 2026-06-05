#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class QuestionBoxGalleryObj : public MapObjActor {
public:
    QuestionBoxGalleryObj(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void startSwitchOn();
    void startSwitchOff();

    void exeOnWait();
    void exeOnStart();
    void exeOffWait();
    void exeOffStart();
};
