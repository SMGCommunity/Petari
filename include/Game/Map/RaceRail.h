#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Map/RaceManager.h"

class ActorCameraInfo;

class PlayerRacer : public LiveActor, public AbstractRacer {
public:
    PlayerRacer(const char *);

    virtual ~PlayerRacer();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void initRacer();
    virtual void prepRacer(const RaceManager *);
    virtual void startRacer();
    virtual void updateRacer(const RaceManager *);
    virtual bool goalRacer();
    virtual void loseRacer();
    virtual void resetRacer(const RaceManager *);
    virtual void exitRacer();

    u32 _B0;
    u32 _B4;
    ActorCameraInfo* mCameraInfo;   // _B8
};

class RaceRail : public NameObj {
public:
    RaceRail(const char *);

    virtual ~RaceRail();
    virtual void init(const JMapInfoIter &);

    PlayerRacer* mPlayerRacer;  // _C
};