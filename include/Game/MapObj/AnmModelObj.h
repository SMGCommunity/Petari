#pragma once

#include "Game/MapObj/MapObjActor.h"

namespace {
    const char* cFollowJointName = "Move";
    const char* cAnimFileName = "Move";
    const char* cEndLoopEffectName = "EndLoop";
};

class AnmModelObj : public MapObjActor {
public:
    AnmModelObj(const char *);

    virtual ~AnmModelObj();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);
    virtual bool isOnStartAnmTrigger() const = 0;
    virtual bool isDone() const;
    virtual bool isKilledAtMoveDone() const;
    virtual bool isRepeat() const;
    virtual void startInner();
    virtual void moveInner();
    virtual void stopInner();

    void exeMove();
    void exeDone();

};